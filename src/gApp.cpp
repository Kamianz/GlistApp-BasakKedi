/*
 * gApp.cpp
 *
 *  Created on: May 6, 2020
 *      Author: noyan
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
//	gCanvas* cnv = new gCanvas(this);
//	appmanager->setCurrentCanvas(cnv);

	menuCanvas* cnv = new menuCanvas(this);
	appmanager->setCurrentCanvas(cnv);

	setupSounds();

}

void gApp::update() {
}

void gApp::setupSounds() {
	 buttonclick[0].loadSound("buttonclick.wav");
	 buttonclick[1].loadSound("buttonclick2.wav");
	 collectcoin.loadSound("collectcoin.wav");
	 enemyfire.loadSound("dusmanates.wav");
	 catfire.loadSound("kediates.ogg");
	 losemusic.loadSound("lose.wav");
	 menumusic.loadSound("menumuzigi.mp3");
	 gamemusic.loadSound("oyunmuzigi.mp3");
	 powerup.loadSound("powerup.wav");
	 powerdown.loadSound("powerdown.wav");
	 explosion.loadSound("se2.wav");
}

void gApp::soundManager(int sound, int musicvalue, int type) {
	float volume = musicvalue / 100;
	bool musicstate = musicvalue <= 0 ? true : false;

	if(type == SOUND_TYPE_STARTING) {
		if(sound == SOUND_MENU) {
			menumusic.setVolume(volume);
			menumusic.play();
		}
		if(sound == SOUND_GAME) {
			gamemusic.setVolume(volume);
			gamemusic.play();
		}
	}
	if(type == SOUND_TYPE_ENDING) {
		if(sound == SOUND_MENU) {
			menumusic.stop();
		}
		if(sound == SOUND_GAME) {
			gamemusic.stop();
		}
	}
	if(type == SOUND_TYPE_ONHIT) {
		if(sound == SOUND_BUTTON) {
			buttonclick[0].setVolume(volume);
			buttonclick[0].play();
		}
		if(sound == SOUND_BUTTON2) {
			buttonclick[1].setVolume(volume);
			buttonclick[1].play();
		}
		if(sound == SOUND_COIN) {
			collectcoin.setVolume(volume);
			collectcoin.play();
		}
		if(sound == SOUND_ENEMY_FIRE) {
			enemyfire.setVolume(volume);
			enemyfire.play();
		}
		if(sound == SOUND_CAT_FIRE) {
			catfire.setVolume(volume);
			catfire.play();
		}
		if(sound == SOUND_LOSE) {
			losemusic.setVolume(volume);
			losemusic.play();
		}
		if(sound == SOUND_POWERUP) {
			powerup.setVolume(volume);
			powerup.play();
		}
		if(sound == SOUND_POWERDOWN) {
			powerdown.setVolume(volume);
			powerdown.play();
		}
		if(sound == SOUND_EXPLOSION) {
			explosion.setVolume(volume);
			explosion.play();
		}
	}
}
