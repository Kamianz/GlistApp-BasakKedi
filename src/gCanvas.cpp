 /*
 * gCanvas.cpp
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan
 */


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
}

void gCanvas::setup() {
	setupBackground();
	setupPlayer();
	setupGold();
	setupExplosion();
	setupPanel();

	setupGameButtons();
}

void gCanvas::update() {
	updateBackground();
	updateGold();
	updatePlayer();
	updateExplosion();
}

void gCanvas::draw() {
	drawBackground();
	drawPlayer();
	drawGold();
	drawExplosion();
	drawPanel();

	drawGameButtons();
}

void gCanvas::keyPressed(int key) {
//	gLogi("gCanvas") << "keyPressed:" << key;
	if(key == 71) {
		generateGold(50, 50, 50, 50);
	}
	if(key == 32) {
//		player.ishit = !player.ishit;
		generateExplosion(player.x, player.y, 128, 128);
	}
	if(key == 87) {
		player.upkey = true;
	}
	if(key == 83) {
		player.downkey = true;
	}
	if(key == 65) {
		player.leftkey = true;
	}
	if(key == 68) {
		player.rightkey = true;
	}
}

void gCanvas::keyReleased(int key) {
//	gLogi("gCanvas") << "keyReleased:" << key;
	if(key == 87) {
		player.upkey = false;
	}
	if(key == 83) {
		player.downkey = false;
	}
	if(key == 65) {
		player.leftkey = false;
	}
	if(key == 68) {
		player.rightkey = false;
	}
}

void gCanvas::charPressed(unsigned int codepoint) {
//	gLogi("gCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void gCanvas::mouseMoved(int x, int y) {
//	gLogi("gCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseDragged(int x, int y, int button) {
}

void gCanvas::mousePressed(int x, int y, int button) {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		// Eðer mouse butona týklanýrsa
		if(x > gamebutton[i].x && x < (gamebutton[i].x + gamebutton[i].w) &&
		   y > gamebutton[i].y && y < (gamebutton[i].y + gamebutton[i].h)) {
			if(!gamebutton[i].pressed) {
				// Ýlgili tuþa basýlýyor ve pressed durumu güncelleniyor
				switch(i) {
					case BUTTON_LEFT:
						player.leftkey = true;
						break;
					case BUTTON_RIGHT:
						player.rightkey = true;
						break;
					case BUTTON_UP:
						player.upkey = true;
						break;
					case BUTTON_DOWN:
						player.downkey = true;
						break;
				}
				gamebutton[i].pressed = true;
			}
		}
	}
}

void gCanvas::mouseReleased(int x, int y, int button) {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		// Yalnýzca basýlý tuþu serbest býrakýrken koordinatlar kontrol ediliyor
		if(gamebutton[i].pressed &&
		   x > gamebutton[i].x && x < (gamebutton[i].x + gamebutton[i].w) &&
		   y > gamebutton[i].y && y < (gamebutton[i].y + gamebutton[i].h)) {
			// Ýlgili tuþ býrakýlýyor ve pressed durumu güncelleniyor
			switch(i) {
				case BUTTON_LEFT:
					player.leftkey = false;
					break;
				case BUTTON_RIGHT:
					player.rightkey = false;
					break;
				case BUTTON_UP:
					player.upkey = false;
					break;
				case BUTTON_DOWN:
					player.downkey = false;
					break;
			}
			gamebutton[i].pressed = false;
		}
	}
}

void gCanvas::mouseScrolled(int x, int y) {
//	gLogi("gCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseEntered() {

}

void gCanvas::mouseExited() {

}

void gCanvas::windowResized(int w, int h) {

}

void gCanvas::showNotify() {

}

void gCanvas::hideNotify() {

}

void gCanvas::setupBackground() {
	backgroundimage.loadImage("city_1.png");
	background[0].w = backgroundimage.getWidth();
	background[0].h = backgroundimage.getHeight();
	background[0].x = 0;
	background[0].y = 0;

	background[1].w = backgroundimage.getWidth();
	background[1].h = backgroundimage.getHeight();
	background[1].x = getWidth();
	background[1].y = 0;
}
void gCanvas::setupPlayer() {
	playerimg[0].loadImage("yellow_idle_1.png");
	playerimg[1].loadImage("yellow_idle_2.png");
	playerimg[2].loadImage("yellow_dead_1.png");
	playerimg[3].loadImage("yellow_dead_2.png");
	playerimg[4].loadImage("yellow_dead_3.png");

	player.w = playerimg[0].getWidth();
	player.h = playerimg[0].getHeight();
	player.x = 50;
	player.y = (getHeight() - player.h) / 2;
	player.speed = 2;
	player.upkey = false;
	player.downkey = false;
	player.leftkey = false;
	player.rightkey = false;
	player.animcounter = 0;
	player.animframeno = 0;
	player.ishit = false;
	player.deadanimplayed = false;
	player.health = 3;
}

void gCanvas::setupExplosion() {
	explosionImage.loadImage("explosion2.png");

	explosionframew = explosionImage.getWidth() / EXPLOSION_ROW;
	explosionframeh = explosionImage.getHeight() / EXPLOSION_COLUMN;
	explosionmaxframe = EXPLOSION_ROW * EXPLOSION_COLUMN;
}

void gCanvas::setupGameButtons() {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		gamebuttonimage[i].loadImage("buttons/shadedDark" + gToStr(24 + i) + ".png");
		gamebutton[i].w = gamebuttonimage[i].getWidth() * 2;
		gamebutton[i].h = gamebuttonimage[i].getHeight() * 2;
		gamebutton[i].y = getHeight() - (gamebutton[i].h * 1.25f);
		gamebutton[i].pressed = false;
	}

	buttongap = gamebutton[0].w / 2;

	// Sol taraftaki düðmeler
	for (int i = 0; i < 2; ++i) {
	    gamebutton[i].x = i * (gamebutton[i].w + buttongap) + gamebutton[i].w / 2;
	}

	// Sað taraftaki düðmeler
	for (int i = 2; i < BUTTON_COUNT; ++i) {
	    gamebutton[i].x = getWidth() - (BUTTON_COUNT - i) * (gamebutton[i].w + buttongap) - gamebutton[i].w / 2;
	}
}

void gCanvas::setupGold() {
	for(int i = 0; i < GOLD_FRAME_COUNT; i++) {
		goldimage[i].loadImage("golds/" + gToStr(i + 1) + ".png");
	}
}

void gCanvas::updateBackground() {
	for(int i = 0; i < BACKGROUND_COUNT; i++) {
		background[i].x -= 1;
		if((background[i].x + background[i].w) < 0) {
			background[i].x = getWidth();
		}
	}
}

void gCanvas::updateGold() {
	for(auto& gold : golds) {
		goldAnimator(gold, GOLD_FRAME_COUNT);
	}
}

void gCanvas::updatePlayer() {
	// Movement
	if(player.upkey) player.y -= 1 * player.speed;
	if(player.downkey) player.y += 1 * player.speed;
	if(player.leftkey) player.x -= 1 * player.speed;
	if(player.rightkey) player.x += 1 * player.speed;

	// Animation
	if(player.ishit) {
		playerAnimator(player, 2, 3, PLAYER_HURT);
	}
	else {
		playerAnimator(player, 0, 2, PLAYER_IDLE);
	}
}

void gCanvas::updateExplosion() {
	for(int i = 0; i < activeExplosion.size(); i++) {
		if(activeExplosion[i][EX_FRAMENO] < explosionmaxframe - 1) {
			activeExplosion[i][EX_COUNTER]++;
			if(activeExplosion[i][EX_COUNTER] % 3 == 0) {
				activeExplosion[i][EX_COUNTER] = 0;
				activeExplosion[i][EX_FRAMENO]++;
			}
			activeExplosion[i][EX_SX] = (activeExplosion[i][EX_FRAMENO] % EXPLOSION_ROW) * explosionframew;
			activeExplosion[i][EX_SY] = (activeExplosion[i][EX_FRAMENO] / EXPLOSION_COLUMN) * explosionframeh;
		} else activeExplosion.erase(activeExplosion.begin() + i);
	}
}

void gCanvas::drawPlayer() {
	playerimg[player.animframeno].draw(player.x, player.y, player.w, player.h);
}

void gCanvas::drawBackground() {
	for(int i = 0; i < BACKGROUND_COUNT; i++) {
		backgroundimage.draw(background[i].x, background[i].y, background[i].w, background[i].h);
	}
}

void gCanvas::drawGameButtons() {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		gamebuttonimage[i].draw(gamebutton[i].x, gamebutton[i].y, gamebutton[i].w, gamebutton[i].h);
	}
}

void gCanvas::drawGold() {
	for(const auto& gold : golds) {
		goldimage[gold.animframeno].draw(gold.x, gold.y, gold.w, gold.h);
	}
}

void gCanvas::drawExplosion() {
	for(int i = 0; i < activeExplosion.size(); i++) {
		explosionImage.drawSub(activeExplosion[i][EX_X],
				activeExplosion[i][EX_Y],
				activeExplosion[i][EX_W],
				activeExplosion[i][EX_H],
				activeExplosion[i][EX_SX],
				activeExplosion[i][EX_SY], explosionframew, explosionframeh);
	}
}

void gCanvas::generateGold(int x, int y, int w, int h) {
	Gold newgold;
	newgold.x = x;
	newgold.y = y;
	newgold.w = w;
	newgold.h = h;
	newgold.animcounter = 0;
	newgold.animframeno = 0;

	golds.push_back(newgold);
}

void gCanvas::generateExplosion(int explosionx, int explosiony, int explosionw,
		int explosionh) {
	newexplosion.push_back(explosionx);
	newexplosion.push_back(explosiony);
	newexplosion.push_back(explosionw);
	newexplosion.push_back(explosionh);
	newexplosion.push_back(0);
	newexplosion.push_back(0);
	newexplosion.push_back(0);
	newexplosion.push_back(0);

	activeExplosion.push_back(newexplosion);
	newexplosion.clear();
}

void gCanvas::goldAnimator(Gold &gold, int maxanimframe) {
	gold.animcounter++;

	if(gold.animcounter % 4 == 0) {
		gold.animframeno++;
		if(gold.animframeno >= maxanimframe) gold.animframeno = 0;
		gold.animcounter = 0;
	}
}

void gCanvas::playerAnimator(Player &player, int startanimframe, int maxanimframe, int animtype) {
	if(animtype == PLAYER_IDLE) {
		player.animcounter++;

		if(player.animcounter % 30 == 0) {
			player.animframeno++;
			if(player.animframeno >= maxanimframe) player.animframeno = startanimframe;
			player.animcounter = 0;
		}
	}
	if(animtype == PLAYER_HURT) {
		player.animframeno = startanimframe;
		if(!player.deadanimplayed) {
			player.animcounter++;

			if(player.animcounter % 30 == 0) {
				player.animframeno++;
				if(player.animframeno >= maxanimframe) {
					if(player.health > 0) {
						player.ishit = false;
						player.animframeno = startanimframe;
					}
					else {
						player.animframeno = maxanimframe - 1;
						player.deadanimplayed = true;
					}
				}
			}
		}
	}
}

void gCanvas::setupPanel() {
	puanpanelimage.loadImage("gui/puanpanel.png");
	goldpanelimage.loadImage("gui/altinpanel.png");

	puanpanel.w = puanpanelimage.getWidth();
	puanpanel.h = puanpanelimage.getHeight();
	puanpanel.x = 0;
	puanpanel.y = 0;

	goldpanel.w = goldpanelimage.getWidth();
	goldpanel.h = goldpanelimage.getHeight();
	goldpanel.x = 0;
	goldpanel.y = puanpanel.h;

	puantext = "0";
	goldtext = "0";

	panelfont.loadFont("action_man.ttf", 24);

	text[0].x = puanpanel.x + (puanpanel.w / 2.75f);
	text[0].y = puanpanel.y + (puanpanel.h - (panelfont.getStringHeight(puantext) / 1.25f));

	text[1].x = goldpanel.x + (goldpanel.w / 3.30f);
	text[1].y = goldpanel.y + (goldpanel.h - (panelfont.getStringHeight(puantext) / 1.25f));
}

void gCanvas::drawPanel() {
	puanpanelimage.draw(puanpanel.x, puanpanel.y, puanpanel.w, puanpanel.h);
	goldpanelimage.draw(goldpanel.x, goldpanel.y, goldpanel.w, goldpanel.h);

	panelfont.drawText(puantext, text[0].x, text[0].y);
	panelfont.drawText(goldtext, text[1].x, text[1].y);
}
