// File   : scene.cpp
// Program: CS4465, Final Project
// Author : Keith T. Fancher
// Date   : 04/13/2006
// Purpose: Controls the animation of the scene


#include <GL/glut.h>
#include <time.h>
#include "md2.h"
#include "scene.h"
#include "animator.h"


int sceneState     = SCENE_STATE_INTRO_TEXT;

// positions
GLfloat alienX     = 80.0;
GLfloat alienY     = -50.0;
GLfloat sephirothX = -100.0;
GLfloat sephirothY = -50.0;

GLfloat zoomFactor = 0.1f; // scale used to zoom in beginning
GLfloat rotFactor  = 0.0f; // rotate whole scene while zooming

GLfloat alienAnimSpeed     = 0.005f; // seconds
GLfloat sephirothAnimSpeed = 0.005f; // seconds

// for moving text in the beginning
unsigned long introTextTime = 0l;
GLfloat alienTextX     = -140.0f;
GLfloat sephirothTextX = 140.0f;
GLfloat vsTextY        = -40.0f;

unsigned long fightTextTime = 0l;
bool   showFightText = true; // used for blinking text
char   fightText1[]  = " READY";
char   fightText2[]  = "  SET";
char   fightText3[]  = "DESTROY!";
char * fightText     = fightText1;


void drawString(float x, float y, void * font, char * string)
{
  char * c;

  glRasterPos2f(x, y);
  
  for(c = string; *c != '\0'; c++) 
  {
    glutBitmapCharacter(font, *c);
  }
}


void animateScene(unsigned long time, CMD2Instance * sephirothInstance, CMD2Instance * alienInstance)
{
  unsigned long elapsedTime  = time - fightTextTime;
  unsigned long elapsedTime2 = time - introTextTime;
 
  // controls the flow of evenets, more or less
  switch(sceneState)
  {
    case SCENE_STATE_INTRO_TEXT:
      alienTextX += 1.0f;
      sephirothTextX -= 1.0f;
      vsTextY += 1.0f;

      if(alienTextX >= 15.0f)
      {
        alienTextX = 15.0f;
      }
      if(sephirothTextX <= -40.0f)
      {
        sephirothTextX = -40.0f;
      }
      if(vsTextY >= 0.0f)
      {
        vsTextY = 0.0f;
      }

      glColor3f(1.0, 0.0, 0.0);
      drawString(alienTextX, 0.0f, GLUT_BITMAP_HELVETICA_18, "ALIEN");
      drawString(sephirothTextX, 0.0f, GLUT_BITMAP_HELVETICA_18, "SEPHIROTH");
      drawString(0.0f, vsTextY, GLUT_BITMAP_HELVETICA_18, "vs");

      // done animating text, pause then move on
      if(alienTextX == 15.0f && sephirothTextX == -40.0f && vsTextY == 0.0f)
      {
        if(elapsedTime2 >= 3)
        {
          sceneState = SCENE_STATE_INTRO_ZOOM;
        }
      }
      break;

    case SCENE_STATE_INTRO_ZOOM:
      zoomFactor += 0.003f; // zoom
      if(zoomFactor >= 1.0)
        zoomFactor = 1.0;
      
      rotFactor += 1.0; // rotate
      if(rotFactor >= 360.0)
      {
        rotFactor = 360.0;
        sceneState = SCENE_STATE_BEGIN;
      }

      glScalef(zoomFactor, zoomFactor, zoomFactor);
      glRotatef(rotFactor, 0.0, 1.0, 0.0);
      break;

    case SCENE_STATE_BEGIN:
      sceneState = SCENE_STATE_ALIEN_TAUNT;
      alienInstance->SetAnimation(alienInstance->TAUNT, alienInstance->IDLE);
      sephirothInstance->SetAnimation(sephirothInstance->IDLE, sephirothInstance->_REPEAT);
      break;

    case SCENE_STATE_ALIEN_TAUNT:
      if(alienInstance->GetAnimation() == alienInstance->IDLE)
      {
        sceneState = SCENE_STATE_SEPH_TAUNT;
        alienInstance->SetAnimation(alienInstance->IDLE, alienInstance->_REPEAT);
        sephirothInstance->SetAnimation(sephirothInstance->TAUNT, sephirothInstance->IDLE);
      }
      break;

    case SCENE_STATE_SEPH_TAUNT:
      if(sephirothInstance->GetAnimation() == sephirothInstance->IDLE)
      {
        sceneState = SCENE_STATE_FIGHT_TEXT;
        fightTextTime = time;
        //sceneState = SCENE_STATE_ALIEN_RUN;
        //alienInstance->SetAnimation(alienInstance->RUN, alienInstance->_REPEAT);
      }
      break;

    case SCENE_STATE_FIGHT_TEXT:
      // blink the text
      if(elapsedTime <= 1)
      {
        glColor3f(1.0, 0.0, 0.0);
        showFightText = TRUE;
        fightText = fightText1;
      }
      if(elapsedTime >= 1 && elapsedTime <= 2)
        showFightText = FALSE;
      if(elapsedTime >= 2 && elapsedTime <= 3)
      {
        glColor3f(1.0, 1.0, 0.0);
        showFightText = TRUE;
        fightText = fightText2;
      }
      if(elapsedTime >= 3 && elapsedTime <= 4)
        showFightText = FALSE;
      if(elapsedTime >= 4 && elapsedTime <= 5)
      {
        glColor3f(0.0, 1.0, 0.0);
        showFightText = TRUE;
        fightText = fightText3;
      }

      // draw the text
      if(showFightText)
        drawString(-10.0f, 0.0f, GLUT_BITMAP_HELVETICA_18, fightText);
      
      // show text for 5 seconds
      if(elapsedTime >= 5) 
      {
        sceneState = SCENE_STATE_ALIEN_RUN;
        alienInstance->SetAnimation(alienInstance->RUN, alienInstance->_REPEAT);
      }
      break;

    case SCENE_STATE_ALIEN_RUN:
      alienX -= 1.0;
      if(alienX <= 20.0) // time for seph to start blocking
      {
        sephirothInstance->SetAnimation(sephirothInstance->FLIPOFF, sephirothInstance->IDLE);
        sephirothAnimSpeed = 0.009f;
      }
      if(alienX <= -60.0) // gets pushed back
      {
        sceneState = SCENE_STATE_SEPH_BLOCK;
        alienInstance->SetAnimation(alienInstance->PAIN1, alienInstance->JUMP);
        sephirothAnimSpeed = 0.005f;
        //sephirothInstance->SetAnimation(sephirothInstance->FLIPOFF, sephirothInstance->IDLE);
      }
      break;

    case SCENE_STATE_SEPH_BLOCK:
      alienX += 2.0;
      if(alienX >= 50.0)
      {
        alienInstance->SetAnimation(alienInstance->IDLE, alienInstance->_REPEAT);
        sceneState = SCENE_STATE_SEPH_JUMPING;
        sephirothInstance->SetAnimation(sephirothInstance->JUMP, sephirothInstance->IDLE);
      }
      break;

    case SCENE_STATE_SEPH_JUMPING:
      sephirothX -= 2.5f;
      if(sephirothX <= -250.0)
      {
        loadSephirothModelArmed();
        sceneState = SCENE_STATE_SEPH_RETURNING;
        sephirothInstance->SetAnimation(sephirothInstance->RUN, sephirothInstance->_REPEAT);
      }
      break;

    case SCENE_STATE_SEPH_RETURNING:
      sephirothX += 1.0f;
      if(sephirothX >= -90)
      {
        sceneState = SCENE_STATE_SEPH_POINTING;
        sephirothInstance->SetAnimation(sephirothInstance->POINT, sephirothInstance->IDLE);
      }
      break;

    case SCENE_STATE_SEPH_POINTING:
      if(sephirothInstance->GetAnimation() == sephirothInstance->IDLE)
      {
        sceneState = SCENE_STATE_SEPH_SHOOTING;
        //sephirothAnimSpeed = 0.002f;
        sephirothInstance->SetAnimation(sephirothInstance->ATTACK, sephirothInstance->IDLE);
        alienInstance->SetAnimation(alienInstance->DEATH1, alienInstance->_STATIC);
      }
      break;

    case SCENE_STATE_SEPH_SHOOTING:
      if(sephirothInstance->GetAnimation() == sephirothInstance->IDLE)
      {
        sceneState = SCENE_STATE_SEPH_VICTORY;
        //sephirothAnimSpeed = 0.005f;
      }
      break;

    case SCENE_STATE_SEPH_VICTORY:
      //if(sephirothInstance->GetAnimation() == sephirothInstance->IDLE)
      //{
        sephirothInstance->SetAnimation(sephirothInstance->TAUNT, sephirothInstance->IDLE);
        sceneState = SCENE_STATE_END;
      //}
      break;

    // why isn't this working?
    case SCENE_STATE_END:
      //gluLookAt(0.0, -50.0, 0.0, -100.0, -50.0, 0.0, 0, 1, 0);
      //glColor3f(1.0, 1.0, 0.0);
      //drawString(0.0f, 0.0f, GLUT_BITMAP_HELVETICA_18, "VICTORY!");
      break;
  }


  // don't draw them while the intro text is there
  if(sceneState != SCENE_STATE_INTRO_TEXT)
  {
    alienInstance->Move(alienX, alienY, 0.0);
    alienInstance->Rotate(180);
    alienInstance->Animate(alienAnimSpeed);
    alienInstance->Render();

    sephirothInstance->Move(sephirothX, sephirothY, 0.0);
    sephirothInstance->Animate(sephirothAnimSpeed);
    sephirothInstance->Render();
  }
}