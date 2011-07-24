// File   : animator.cpp
// Program: CS4465, Final Project
// Author : Keith T. Fancher
// Date   : 04/13/2006
// Purpose: A fight scene between an Alien and Sephiroth, utilizing
//          the .md2 model file format.


#include <GL/glut.h>
#include <time.h>
#include "animator.h"
#include "md2.h"
#include "scene.h"


// time variables
unsigned long initTime    = 0;
unsigned long timeElapsed = 0;

// model data
CMD2Data *     alienModel        = NULL;
CMD2Data *     sephirothModel    = NULL;
CMD2Instance * alienInstance     = NULL;
CMD2Instance * sephirothInstance = NULL;

GLfloat sceneRotation = 0.0f;


void loadAlienModel()
{
  if(alienModel)
  {
    delete alienModel;
    alienModel = NULL;
  }

  alienModel = new CMD2Data;
  alienModel->Load("alien\\tris.md2", "alien\\alien.tga", NULL, NULL, 1.5);

  if(alienInstance)
  {
    delete alienInstance;
    alienInstance = NULL;
  }
  alienInstance = alienModel->GetInstance();
}

void loadAlienModelArmed()
{
  if(alienModel)
  {
    delete alienModel;
    alienModel = NULL;
  }

  alienModel = new CMD2Data;
  alienModel->Load("alien\\tris.md2", "alien\\alien.tga", "alien\\weapon.md2", "alien\\weapon.tga", 1.5);

  if(alienInstance)
  {
    delete alienInstance;
    alienInstance = NULL;
  }
  alienInstance = alienModel->GetInstance();
}

void loadSephirothModel()
{
  if(sephirothModel)
  {
    delete sephirothModel;
    sephirothModel = NULL;
  }

 	sephirothModel = new CMD2Data;
  sephirothModel->Load("sephiroth\\tris.md2", "sephiroth\\sephiroth.tga", NULL, NULL, 1.5);

  if(sephirothInstance)
  {
    delete sephirothInstance;
    sephirothInstance = NULL;
  }
  sephirothInstance = sephirothModel->GetInstance();
}

void loadSephirothModelArmed()
{
  if(sephirothModel)
  {
    delete sephirothModel;
    sephirothModel = NULL;
  }

  sephirothModel = new CMD2Data;
  sephirothModel->Load("sephiroth\\tris.md2", "sephiroth\\sephiroth.tga", "sephiroth\\weapon.md2", "sephiroth\\weapon.tga", 1.5);

  if(sephirothInstance)
  {
    delete sephirothInstance;
    sephirothInstance = NULL;
  }
  sephirothInstance = sephirothModel->GetInstance();
}


void init()
{
  // set up window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-140, 140, -70, 70, -250, 250);
  //glFrustum(-500, 500, -500, 500, -500, 500);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);  
  
  // load models
  glEnable(GL_TEXTURE_2D);   	
  loadAlienModel();
  loadSephirothModel();
  
  // set timer
  initTime = time(NULL);
}


void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  
  animateScene(timeElapsed, sephirothInstance, alienInstance);

  glFlush();
	glutSwapBuffers();
}


void idle()
{
  timeElapsed = time(NULL) - initTime;

  sceneRotation += 0.1f;
  if(sceneRotation >= 360.0f)
    sceneRotation = 0.0f;

  glutPostRedisplay();
}


int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1000, 500);
  glutCreateWindow("Alien vs. Sephiroth");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  init();
  glutMainLoop();

  return 0;
}