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
	setupBullet();
	setupEnemy();
	setupGameButtons();

	// For control purpose.
//	enemyx = 1000;
//	enemyy = player.y;
//	enemyw = player.w;
//	enemyh = player.h;
//	canenemyshoot = true;
//	enemycd = 2;
//	enemytimer = 0;
}

void gCanvas::update() {
	updateBackground();
	updateGold();
	updatePlayer();
	updateExplosion();
	updateBullet();
	generateEnemy();
	updateEnemy();
	// For control purpose.
//	if(!canenemyshoot) {
//		canenemyshoot = cooldown(enemycd, enemytimer);
//	}
//	else {
//		generateBullet(enemyx, enemyy, enemyw, enemyh, OWNER_ENEMY);
//		canenemyshoot = false;
//	}
}

void gCanvas::draw() {
	drawBackground();
	drawPlayer();
	drawGold();
	drawExplosion();
	drawPanel();
	drawBullet();
	drawEnemy();
	drawGameButtons();

	// For control purpose.
//	gDrawRectangle(enemyx, enemyy, enemyw, enemyh);
}

void gCanvas::keyPressed(int key) {
//	gLogi("gCanvas") << "keyPressed:" << key;
//	gLogi("Key ") << std::to_string(key);
	if(key == 71) {
		generateGold(50, 50, 50, 50);
	}
	if(key == 70) {
	}
	if(key == 32) {
//		player.ishit = !player.ishit;
//		generateExplosion(player.x, player.y, 128, 128);
		if(player.shootkey){
			generateBullet(player.x + (player.w / 2), player.y + (player.h / 1.5f), player.w, player.h / 4, OWNER_PLAYER);
			player.shootkey = false;
		}
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
		// E�er mouse butona t�klan�rsa
		if(x > gamebutton[i].x && x < (gamebutton[i].x + gamebutton[i].w) &&
		   y > gamebutton[i].y && y < (gamebutton[i].y + gamebutton[i].h)) {
			if(!gamebutton[i].pressed) {
				// �lgili tu�a bas�l�yor ve pressed durumu g�ncelleniyor
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
		// Yaln�zca bas�l� tu�u serbest b�rak�rken koordinatlar kontrol ediliyor
		if(gamebutton[i].pressed &&
		   x > gamebutton[i].x && x < (gamebutton[i].x + gamebutton[i].w) &&
		   y > gamebutton[i].y && y < (gamebutton[i].y + gamebutton[i].h)) {
			// �lgili tu� b�rak�l�yor ve pressed durumu g�ncelleniyor
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
	player.shootkey = true;
	player.cooldown = 2;
	player.cooldowntimer = 0;
}

void gCanvas::setupExplosion() {
	explosionImage.loadImage("explosion2.png");

	explosionframew = explosionImage.getWidth() / EXPLOSION_ROW;
	explosionframeh = explosionImage.getHeight() / EXPLOSION_COLUMN;
	explosionmaxframe = EXPLOSION_ROW * EXPLOSION_COLUMN;
}

void gCanvas::setupGameButtons() {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(i >= BUTTON_COUNT - 1) {
			gamebuttonimage[i].loadImage("buttons/shadedDark48.png");
		}
		else gamebuttonimage[i].loadImage("buttons/shadedDark" + gToStr(24 + i) + ".png");
		gamebutton[i].w = gamebuttonimage[i].getWidth() * 2;
		gamebutton[i].h = gamebuttonimage[i].getHeight() * 2;
		gamebutton[i].y = getHeight() - (gamebutton[i].h * 1.25f);
		gamebutton[i].pressed = false;
	}

	buttongap = gamebutton[0].w / 2;

	// Sol taraftaki d��meler
	for (int i = 0; i < 2; ++i) {
	    gamebutton[i].x = i * (gamebutton[i].w + buttongap) + gamebutton[i].w / 2;
	}

	// Sa� taraftaki d��meler
	for (int i = 2; i < BUTTON_COUNT; ++i) {
	    gamebutton[i].x = getWidth() - (BUTTON_COUNT - i) * (gamebutton[i].w + buttongap) - gamebutton[i].w / 2;
	}
}

void gCanvas::setupGold() {
	for(int i = 0; i < GOLD_FRAME_COUNT; i++) {
		goldimage[i].loadImage("golds/" + gToStr(i + 1) + ".png");
	}
}

void gCanvas::setupBullet() {
	bulletimage[0].loadImage("cat_shot_1.png");
	bulletimage[1].loadImage("suit_alien_shot_1.png");
	bulletimage[2].loadImage("ufo_alien_shot_1.png");
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

	if(!player.shootkey) {
		player.shootkey = cooldown(player.cooldown, player.cooldowntimer);
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

void gCanvas::updateBullet() {
	for(int i = 0; i < activebullets.size(); i++) {
		activebullets[i].x += activebullets[i].speed * activebullets[i].owner;
		if(activebullets[i].x > getWidth() || activebullets[i].x + activebullets[i].w < 0) activebullets.erase(activebullets.begin() + i);
	}
}

void gCanvas::drawPlayer() {
	playerimg[player.animframeno].draw(player.x, player.y, player.w, player.h);
}

void gCanvas::drawBackground() {
    for(int i = 0; i < BACKGROUND_COUNT; i++) {
        if (background[i].x + background[i].w > 0 && background[i].x < getWidth()) {
            backgroundimage.draw(background[i].x, background[i].y, background[i].w, background[i].h);
        }
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

void gCanvas::drawBullet() {
	for(auto& bullet : activebullets) {
		if(bullet.owner == OWNER_PLAYER) bulletimage[bullet.owner - 1].draw(bullet.x, bullet.y, bullet.w, bullet.h);
		if(bullet.owner == OWNER_ENEMY) bulletimage[1].draw(bullet.x, bullet.y, bullet.w, bullet.h);
	}
}

void gCanvas::drawPanel() {
	puanpanelimage.draw(puanpanel.x, puanpanel.y, puanpanel.w, puanpanel.h);
	goldpanelimage.draw(goldpanel.x, goldpanel.y, goldpanel.w, goldpanel.h);

	panelfont.drawText(puantext, text[0].x, text[0].y);
	panelfont.drawText(goldtext, text[1].x, text[1].y);
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

void gCanvas::generateBullet(int x, int y, int w, int h, int owner) {
	Bullet newbullet;

	newbullet.w = w;
	newbullet.h = h;
	newbullet.x = x;
	newbullet.y = y;
	newbullet.damage = 100;
	newbullet.speed = 10;
	newbullet.owner = owner;

	activebullets.push_back(newbullet);
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

bool gCanvas::cooldown(int &time, int &timer) {
	timer++;
	if(timer % 60 == 0) {
		time--;
		if(time <= 0) {
			return true;
		}
	}
	return false;
}
void gCanvas::drawEnemy() {
	for(int i = 0; i < enemies.size(); i++){
		enemy& enemy = enemies[i];
		enemyimage[enemy.type].draw(enemy.x, enemy.y, enemy.w, enemy.h);
	}

}
void gCanvas::updateEnemy() {
	for(int i = 0; i < enemies.size(); i++){
		enemy& enemy = enemies[i];
		enemy.x -= enemy.speed;
//cani ve x degeri 0 dan kucukse sil
		if(enemy.health <= 0 && enemy.x < 0){
			enemy.isalive = false;
			// sil
			enemies.erase(enemies.begin() + i);
		}
	}
}
void gCanvas::generateEnemy() {
	spawnctr++;
	if(spawnctr > spawnctrlimit){
		spawnctr = 0;
		int type = int(gRandom(float(maxenemytypenum)));
		spawnEnemy(type);
	}

}
void gCanvas::spawnEnemy(int type) {
	enemy newenemy;
	newenemy.w = enemyimage[type].getWidth();
	newenemy.h = enemyimage[type].getHeight();
	newenemy.x = getWidth();
	newenemy.y = int(gRandom(float(getHeight() - newenemy.h)));
	newenemy.type = type;
	newenemy.speed = enemyspeeds[type];
	newenemy.health = enemyhealths[type];
	newenemy.damage = enemydamages[type];
	newenemy.isalive = true;
	newenemy.cooldown = enemycooldown[type];
	newenemy.cooldowntimer = enemycooldowntimer[type];

	enemies.push_back(newenemy);
}

void gCanvas::setupEnemy() {
	enemyimage[UFO_RED].loadImage("red_ufo_idle_1.png");
	enemyimage[UFO_BLACK].loadImage("black_ufo_idle_1.png");
	enemyimage[UFO_GREEN].loadImage("green_ufo_idle_1.png");
	enemyimage[SUIT_BLACK].loadImage("black_suit_idle_1.png");
	enemyimage[SUIT_PURPLE].loadImage("purple_suit_idle_1.png");
	enemyimage[SUIT_ORANGE].loadImage("orange_suit_idle_1.png");

	enemyspeeds[UFO_RED] = 5.0f;
	enemyspeeds[UFO_BLACK] = 5.0f;
	enemyspeeds[UFO_GREEN] = 5.0f;
	enemyspeeds[SUIT_BLACK] = 5.0f;
	enemyspeeds[SUIT_PURPLE] = 5.0f;
	enemyspeeds[SUIT_ORANGE] = 5.0f;

	enemydamages[UFO_RED] = 50.0f;
	enemydamages[UFO_BLACK] = 30.0f;
	enemydamages[UFO_GREEN] = 40.0f;
	enemydamages[SUIT_BLACK] = 25.0f;
	enemydamages[SUIT_PURPLE] = 35.0f;
	enemydamages[SUIT_ORANGE] = 45.0f;

	enemyhealths[UFO_RED] = 500.0f;
	enemyhealths[UFO_BLACK] = 300.0f;
	enemyhealths[UFO_GREEN] = 400.0f;
	enemyhealths[SUIT_BLACK] = 250.0f;
	enemyhealths[SUIT_PURPLE] = 350.0f;
	enemyhealths[SUIT_ORANGE] = 450.0f;

	enemycooldown[UFO_RED] = 5.0f;
	enemycooldown[UFO_BLACK] = 3.0f;
	enemycooldown[UFO_GREEN] = 4.0f;
	enemycooldown[SUIT_BLACK] = 2.0f;
	enemycooldown[SUIT_PURPLE] = 3.0f;
	enemycooldown[SUIT_ORANGE] = 4.0f;

	enemycooldowntimer[UFO_RED] = 0;
	enemycooldowntimer[UFO_BLACK] = 0;
	enemycooldowntimer[UFO_GREEN] = 0;
	enemycooldowntimer[SUIT_BLACK] = 0;
	enemycooldowntimer[SUIT_PURPLE] = 0;
	enemycooldowntimer[SUIT_ORANGE] = 0;

}


