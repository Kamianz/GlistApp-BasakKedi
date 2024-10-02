/*
* menuCanvas.cpp
*
*  Created on: September 18, 2024
*      Author: Kadir Semih Arslan
*/


#include "menuCanvas.h"
#include "gCanvas.h"


menuCanvas::menuCanvas(gApp *root) : gBaseCanvas(root) {
	this->root = root;
}

menuCanvas::~menuCanvas() {
}

void menuCanvas::setup() {
	setupMenu();
	setupButton();
	setupContinuePanel();
	isdead = root->getIsDead();

	root->soundManager(root->SOUND_MENU, 100, root->SOUND_TYPE_STARTING);
}

void menuCanvas::update() {
	updateMenu();
}

void menuCanvas::draw() {
	drawMenu();
	drawButton();
	if(gamestate == GAMESTATE_WARNING) drawContinuePanel();
}

void menuCanvas::keyPressed(int key) {
//	gLogi("menuCanvas") << "keyPressed:" << key;
}

void menuCanvas::keyReleased(int key) {
//	gLogi("menuCanvas") << "keyReleased:" << key;
}

void menuCanvas::charPressed(unsigned int codepoint) {
//	gLogi("menuCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void menuCanvas::mouseMoved(int x, int y) {
//	gLogi("menuCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void menuCanvas::mouseDragged(int x, int y, int button) {
//	gLogi("menuCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void menuCanvas::mousePressed(int x, int y, int button) {
	if(gamestate == GAMESTATE_START && x > buttongroup[0].x && x < (buttongroup[0].x + buttongroup[0].w) && y > buttongroup[0].y && y < (buttongroup[0].y + buttongroup[0].h)) {
		buttongroup[0].pressed = true;
	}

	if(gamestate == GAMESTATE_WARNING) {
		for(int i = 0; i < 2; i++) {
			if (pow(x - warningbutton[i].centerx, 2) + pow(y - warningbutton[i].centery, 2) < pow(warningbutton[i].radius, 2)) {
				warningbutton[i].pressed = true;
				root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
			}
		}
	}
}

void menuCanvas::mouseReleased(int x, int y, int button) {
	if(buttongroup[0].pressed) {
		buttongroup[0].pressed = false;
		root->soundManager(root->SOUND_BUTTON, 100, root->SOUND_TYPE_ONHIT);
		if(isdead.size() > 0) gamestate = GAMESTATE_WARNING;
		else startGame();
	}

	if(gamestate == GAMESTATE_WARNING) {
		if (warningbutton[WARNING_NO].pressed == true) {
			deleteSave(isdead[0]);
			warningbutton[WARNING_NO].pressed = false;
			startGame();
		}
		if(warningbutton[WARNING_YES].pressed == true){
			warningbutton[WARNING_YES].pressed = false;
			startGame();
		}
	}

//	if(onhitsoundnum > 10) onhitsoundnum = 2;
//	soundTest(onhitsoundnum);
//	onhitsoundnum++;
}

void menuCanvas::mouseScrolled(int x, int y) {
//	gLogi("menuCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void menuCanvas::mouseEntered() {

}

void menuCanvas::mouseExited() {

}

void menuCanvas::windowResized(int w, int h) {

}

void menuCanvas::showNotify() {

}

void menuCanvas::hideNotify() {

}

void menuCanvas::setupMenu() {
	backgroundimage.loadImage("space_1.png");
	playerimage.loadImage("menu_basakkedi_qhd.png");
	logoimage.loadImage("menu_oyunismi.png");

	background[0].w = getWidth();
	background[0].h = getHeight();
	background[0].x = 0;
	background[0].y = 0;

	background[1].w = getWidth();
	background[1].h = getHeight();
	background[1].x = background[1].w;
	background[1].y = 0;

	logo.w = logoimage.getWidth();
	logo.h = logoimage.getHeight();
	logo.x = 0;
	logo.y = 0;

	player.w = playerimage.getWidth();
	player.h = playerimage.getHeight();
	player.x = 0;
	player.y = (getHeight() - player.h) / 2;

	isdead = root->getIsDead();
	gamestate = GAMESTATE_START;

	parallaxspeed = 5;
}

void menuCanvas::updateMenu() {
	for (int i = 0; i < NUM_BACKGROUNDS; i++) {
	    background[i].x -= parallaxspeed;

	    if (background[i].x + background[i].w < 0) {
	        background[i].x = getWidth();
	    }
	}
}

void menuCanvas::drawMenu() {
	for(int i = 0; i < 2; i++) {
		backgroundimage.draw(background[i].x, background[i].y, background[i].w, background[i].h);
	}
	logoimage.draw(logo.x, logo.y, logo.w, logo.h);
	playerimage.draw(player.x, player.y, player.w, player.h);
}

void menuCanvas::setupButton() {
	playbutton.loadImage("gui/playbutton.png");
	buttongroup[0].w = playbutton.getWidth() * 2;
	buttongroup[0].h = playbutton.getHeight() * 2;
	buttongroup[0].x = getWidth() - ((getWidth() - buttongroup[0].w) / 3);
	buttongroup[0].y = (getHeight() - buttongroup[0].h) / 2;
	buttongroup[0].pressed = false;
}

void menuCanvas::updateButton() {
}

void menuCanvas::drawButton() {
	if(!buttongroup[0].pressed) {
	    playbutton.draw(buttongroup[0].x, buttongroup[0].y, buttongroup[0].w, buttongroup[0].h);
	}
	else {
	    float scaleFactor = 1.5f;
	    float newWidth = buttongroup[0].w / scaleFactor;
	    float newHeight = buttongroup[0].h / scaleFactor;

	    float newX = buttongroup[0].x + (buttongroup[0].w - newWidth) / 2;
	    float newY = buttongroup[0].y + (buttongroup[0].h - newHeight) / 2;

	    playbutton.draw(newX, newY, newWidth, newHeight);
	}
}

void menuCanvas::soundTest(int soundnum) {
	gLogi("Sound ") << soundnum;
	root->soundManager(soundnum, 100, root->SOUND_TYPE_ONHIT);
}

void menuCanvas::setupContinuePanel() {
    warningimage.loadImage("gui/uyaricontinue.png");

    float original_w = warningimage.getWidth();
    float original_h = warningimage.getHeight();

    warning.w = original_w * 1.5f;
    warning.h = original_h * 1.5f;
    warning.x = (getWidth() - warning.w) / 2;
    warning.y = (getHeight() - warning.h) / 2;

    float original_button0_x = 100.0f;
    float original_button0_y = 140.0f;
    float original_button0_w = 50.0f;
    float original_button0_h = 60.0f;
    float original_button1_x = 207.0f;

    warningbutton[WARNING_NO].w = original_button0_w * 1.5f;
    warningbutton[WARNING_NO].h = original_button0_h * 1.5f;
    warningbutton[WARNING_NO].x = warning.x + (original_button0_x * 1.5f);
    warningbutton[WARNING_NO].y = warning.y + (original_button0_y * 1.48f);

    warningbutton[WARNING_YES].w = original_button0_w * 1.5f;
    warningbutton[WARNING_YES].h = original_button0_h * 1.5f;
    warningbutton[WARNING_YES].x = warning.x + (original_button1_x * 1.5f);
    warningbutton[WARNING_YES].y = warning.y + (original_button0_y * 1.48f);

    warningbutton[WARNING_NO].centerx = warningbutton[WARNING_NO].x + warningbutton[0].w / 2;
    warningbutton[WARNING_NO].centery = warningbutton[WARNING_NO].y + warningbutton[0].h / 2;
    warningbutton[WARNING_NO].radius = warningbutton[WARNING_NO].w / 1.6f;

    warningbutton[WARNING_YES].centerx = warningbutton[WARNING_YES].x + warningbutton[1].w / 2;
    warningbutton[WARNING_YES].centery = warningbutton[WARNING_YES].y + warningbutton[1].h / 2;
    warningbutton[WARNING_YES].radius = warningbutton[WARNING_YES].w / 1.6f;

    warningbutton[WARNING_NO].pressed = false;
    warningbutton[WARNING_YES].pressed = false;

    warningfont.loadFont("action_man.ttf", 18);
}

void menuCanvas::drawContinuePanel() {
	 if(gamestate == GAMESTATE_WARNING) warningimage.draw(warning.x, warning.y, warning.w, warning.h);
}

bool menuCanvas::saveControl() {
	return true;
}

void menuCanvas::deleteSave(int id) {
	std::string statement = "UPDATE Players SET isdead = 1 WHERE id = " + gToStr(id) + ";";
	root->insertDatabase(statement);
}

void menuCanvas::startGame() {
	root->onSceneChange();
	gCanvas* game = new gCanvas(root);
	appmanager->setCurrentCanvas(game);
}
