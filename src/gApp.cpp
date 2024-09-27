/*
 * gApp.cpp
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan
 */

#include "gApp.h"
#include "gCanvas.h"
#include "menuCanvas.h"


gApp::gApp() {
}

gApp::gApp(int argc, char **argv) : gBaseApp(argc, argv) {
}

gApp::~gApp() {
}

void gApp::setup() {
	setupSounds();
	controlSounds();

//	gCanvas* cnv = new gCanvas(this);
//	appmanager->setCurrentCanvas(cnv);

	menuCanvas* menu = new menuCanvas(this);
	appmanager->setCurrentCanvas(menu);
}

void gApp::update() {
}

void gApp::setupSounds() {
	 sounds[SOUND_MENU].loadSound("menumuzigi.mp3");
	 sounds[SOUND_GAME].loadSound("oyunmuzigi.mp3");
	 sounds[SOUND_BUTTON].loadSound("buttonclick.wav");
	 sounds[SOUND_BUTTON2].loadSound("buttonclick2.wav");
	 sounds[SOUND_COIN].loadSound("collectcoin.wav");
	 sounds[SOUND_ENEMY_FIRE].loadSound("dusmanates.wav");
	 sounds[SOUND_CAT_FIRE].loadSound("kediates.ogg");
	 sounds[SOUND_LOSE].loadSound("lose.wav");
	 sounds[SOUND_POWERUP].loadSound("powerup.wav");
	 sounds[SOUND_POWERDOWN].loadSound("powerdown.wav");
	 sounds[SOUND_EXPLOSION].loadSound("se2.wav");

	 issoundloaded[SOUND_MENU] = sounds[SOUND_MENU].isLoaded();
	 issoundloaded[SOUND_GAME] = sounds[SOUND_GAME].isLoaded();
	 issoundloaded[SOUND_BUTTON] = sounds[SOUND_BUTTON].isLoaded();
	 issoundloaded[SOUND_BUTTON2] = sounds[SOUND_BUTTON2].isLoaded();
	 issoundloaded[SOUND_COIN] = sounds[SOUND_COIN].isLoaded();
	 issoundloaded[SOUND_ENEMY_FIRE] = sounds[SOUND_ENEMY_FIRE].isLoaded();
	 issoundloaded[SOUND_CAT_FIRE] = sounds[SOUND_CAT_FIRE].isLoaded();
	 issoundloaded[SOUND_LOSE] = sounds[SOUND_LOSE].isLoaded();
	 issoundloaded[SOUND_POWERUP] = sounds[SOUND_POWERUP].isLoaded();
	 issoundloaded[SOUND_POWERDOWN] = sounds[SOUND_POWERDOWN].isLoaded();
	 issoundloaded[SOUND_EXPLOSION] = sounds[SOUND_EXPLOSION].isLoaded();
}

void gApp::controlSounds() {
//	for(int i = 0; i < 11; i++) {
//		gLogi(std::to_string(i)) << issoundloaded[i];
//	}
}

void gApp::soundManager(int sound, int musicvalue, int type) {
	float volume = musicvalue / 100;
	bool musicstate = musicvalue <= 0 ? true : false;

	if(type == SOUND_TYPE_STARTING) {
		if(sound == SOUND_MENU) {
			sounds[SOUND_MENU].setVolume(volume);
			sounds[SOUND_MENU].play();
		}
		if(sound == SOUND_GAME) {
			sounds[SOUND_GAME].setVolume(volume);
			sounds[SOUND_GAME].play();
		}
	}
	if(type == SOUND_TYPE_ENDING) {
		if(sound == SOUND_MENU) {
			sounds[SOUND_MENU].stop();
		}
		if(sound == SOUND_GAME) {
			sounds[SOUND_GAME].stop();
		}
	}
	if(type == SOUND_TYPE_ONHIT) {
		if(sound == SOUND_BUTTON) {
			sounds[SOUND_BUTTON].setVolume(volume);
			sounds[SOUND_BUTTON].play();
		}
		if(sound == SOUND_BUTTON2) {
			sounds[SOUND_BUTTON2].setVolume(volume);
			sounds[SOUND_BUTTON2].play();
		}
		if(sound == SOUND_COIN) {
			sounds[SOUND_COIN].setVolume(volume);
			sounds[SOUND_COIN].play();
		}
		if(sound == SOUND_ENEMY_FIRE) {
			sounds[SOUND_ENEMY_FIRE].setVolume(volume);
			sounds[SOUND_ENEMY_FIRE].play();
		}
		if(sound == SOUND_CAT_FIRE) {
			sounds[SOUND_CAT_FIRE].setVolume(volume);
			sounds[SOUND_CAT_FIRE].play();
		}
		if(sound == SOUND_LOSE) {
			sounds[SOUND_LOSE].setVolume(volume);
			sounds[SOUND_LOSE].play();
		}
		if(sound == SOUND_POWERUP) {
			sounds[SOUND_POWERUP].setVolume(volume);
			sounds[SOUND_POWERUP].play();
		}
		if(sound == SOUND_POWERDOWN) {
			sounds[SOUND_POWERDOWN].setVolume(volume);
			sounds[SOUND_POWERDOWN].play();
		}
		if(sound == SOUND_EXPLOSION) {
			sounds[SOUND_EXPLOSION].setVolume(volume);
			sounds[SOUND_EXPLOSION].play();
		}
	}
}
