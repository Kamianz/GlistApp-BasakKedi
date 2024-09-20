/*
 * gApp.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef GAPP_H_
#define GAPP_H_

#include "gBaseApp.h"
#include "gFmodSound.h"


class gApp : public gBaseApp {
public:
	gApp();
	gApp(int argc, char **argv);
	~gApp();

	void setup();
	void update();

	void setupSounds();
	void soundManager(int sound, int musicvalue, int type);

	gFmodSound buttonclick[2];
	gFmodSound collectcoin;
	gFmodSound enemyfire;
	gFmodSound catfire;
	gFmodSound losemusic;
	gFmodSound menumusic;
	gFmodSound gamemusic;
	gFmodSound powerup;
	gFmodSound powerdown;
	gFmodSound explosion;

	static const int SOUND_TYPE_ONHIT = 0, SOUND_TYPE_STARTING = 1, SOUND_TYPE_ENDING = 2;
	static const int SOUND_MENU = 0, SOUND_GAME = 1, SOUND_BUTTON = 2, SOUND_BUTTON2 = 3, SOUND_COIN = 4, SOUND_ENEMY_FIRE = 5, SOUND_CAT_FIRE = 6, SOUND_LOSE = 7, SOUND_POWERUP = 8, SOUND_POWERDOWN = 9, SOUND_EXPLOSION = 10;
};

#endif /* GAPP_H_ */
