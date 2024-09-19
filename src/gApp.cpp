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
	gCanvas* cnv = new gCanvas(this);
	appmanager->setCurrentCanvas(cnv);

//	menuCanvas* cnv = new menuCanvas(this);
//	appmanager->setCurrentCanvas(cnv);
}

void gApp::update() {
}
