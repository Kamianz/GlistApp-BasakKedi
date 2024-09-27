/*
 * gApp.h
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan
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

	void soundManager(int sound, int musicvalue, int type);

	static const int SOUND_TYPE_ONHIT = 0, SOUND_TYPE_STARTING = 1, SOUND_TYPE_ENDING = 2;
	static const int SOUND_MENU = 0, SOUND_GAME = 1, SOUND_BUTTON = 2, SOUND_BUTTON2 = 3, SOUND_COIN = 4, SOUND_ENEMY_FIRE = 5, SOUND_CAT_FIRE = 6, SOUND_LOSE = 7, SOUND_POWERUP = 8, SOUND_POWERDOWN = 9, SOUND_EXPLOSION = 10;

private:
	gFmodSound sounds[11];
	bool issoundloaded[11];
	bool soundsloaded;

	void setupSounds();
	void controlSounds();
};

#endif /* GAPP_H_ */
