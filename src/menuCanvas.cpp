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
}

void menuCanvas::update() {
	updateMenu();
}

void menuCanvas::draw() {
	drawMenu();
	drawButton();
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
	if(x > buttongroup[0].x && x < (buttongroup[0].x + buttongroup[0].w) && y > buttongroup[0].y && y < (buttongroup[0].y + buttongroup[0].h)) {
		buttongroup[0].pressed = true;
	}
}

void menuCanvas::mouseReleased(int x, int y, int button) {
	if(buttongroup[0].pressed && x > buttongroup[0].x && x < (buttongroup[0].x + buttongroup[0].w) && y > buttongroup[0].y && y < (buttongroup[0].y + buttongroup[0].h)) {
		buttongroup[0].pressed = false;
		gCanvas* main = new gCanvas(root);
		appmanager->setCurrentCanvas(main);
	}
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

}

void menuCanvas::updateMenu() {
	for(int i = 0; i < 2; i++) {
		background[i].x -= 1;
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
