// File   : scene.h
// Program: CS4465, Final Project
// Author : Keith T. Fancher
// Date   : 04/13/2006
// Purpose: Controls the animation of the scene


#ifndef __SCENE_H__
#define __SCENE_H__


#define SCENE_STATE_BEGIN           0
#define SCENE_STATE_INTRO_TEXT      1
#define SCENE_STATE_INTRO_ZOOM      2
#define SCENE_STATE_SEPH_TAUNT      3
#define SCENE_STATE_ALIEN_TAUNT     4
#define SCENE_STATE_FIGHT_TEXT      5
#define SCENE_STATE_ALIEN_RUN       6
#define SCENE_STATE_SEPH_BLOCK      7
#define SCENE_STATE_SEPH_JUMPING    8
#define SCENE_STATE_SEPH_RETURNING  9
#define SCENE_STATE_SEPH_POINTING   10
#define SCENE_STATE_SEPH_SHOOTING   11
#define SCENE_STATE_SEPH_VICTORY    12
#define SCENE_STATE_END             13


void drawString(float, float, void *, char *);
void animateScene(unsigned long, CMD2Instance *, CMD2Instance *);


#endif