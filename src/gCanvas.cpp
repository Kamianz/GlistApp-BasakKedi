 /*
 * gCanvas.cpp
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan, Mehmet Furkan Utlu
 */


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
}

void gCanvas::setup() {
	setupGame();
	setupBackground();
	setupPlayer();
	setupGold();
	setupExplosion();
	setupPanel();
	setupBullet();
	setupEnemy();

	setupGameButtons();
	setupPausePanel();
}

void gCanvas::update() {
	if(gamestate == GAMESTATE_PLAY){
		updateBackground();
		updateGold();
		updatePlayer();
		updateExplosion();
		updateBullet();
		generateEnemy();
		updateEnemy();
	}
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

	if(gamestate == GAMESTATE_PAUSE){
		drawPausePanel();
	}
}

void gCanvas::keyPressed(int key) {
//	gLogi("gCanvas") << "keyPressed:" << key;
//	gLogi("Key ") << std::to_string(key);
	if(key == 256){
        if (gamestate == GAMESTATE_PLAY) {
            gamestate = GAMESTATE_PAUSE;
            //std::cout << "Game paused." << gamestate << std::endl;
        } else {
            gamestate = GAMESTATE_PLAY;
           // std::cout << "Game resumed." << std::endl;
        }
	}
	if(key == 32) {
//		player.ishit = !player.ishit;
//		generateExplosion(player.x, player.y, 128, 128);
		if(player.canshoot){
			generateBullet(player.x + (player.w / 2), player.y + (player.h / 1.5f), player.w, player.h / 4, OWNER_PLAYER, PLAYER, player.damage);
			player.canshoot = false;
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
					case BUTTON_FIRE:
						if(player.canshoot){
							generateBullet(player.x + (player.w / 2), player.y + (player.h / 1.5f), player.w, player.h / 4, OWNER_PLAYER, PLAYER , player.damage);
							player.canshoot = false;
						}
						break;
				}
				gamebutton[i].pressed = true;
			}
		}
	}
	if(gamestate == GAMESTATE_PAUSE){
		if(x > settingsbuttonx && x < settingsbuttonx + settingsbuttonw && y > settingsbuttony[settingsbuttonstate] && y < settingsbuttony[settingsbuttonstate] + settingsbuttonh[settingsbuttonstate]){
			settingsbuttonstate = (!settingsbuttonstate);
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

void gCanvas::setupGame() {
	mapleft = 0;
	mapright = getWidth();
	maptop = 0;
	mapbottom = getHeight();
	gamestate = GAMESTATE_PLAY;
}
void gCanvas::setupPausePanel() {
	pausepanelimage.loadImage("gui/pausemenu.png");
	pausepanelw = pausepanelimage.getWidth() * 2;
	pausepanelh = pausepanelimage.getHeight() * 2;
	pausepanelx = getWidth() / 2 - pausepanelw / 2;
	pausepanely = getHeight() / 2 - pausepanelh / 2;

	settingsbuttonstate = 0;

	settingsbutton[BUTTON_CLICK].loadImage("gui/ayarlar.png");
	settingsbutton[BUTTON_UNCLICK].loadImage("gui/ayarlarbutton.png");

	settingsbuttonw = settingsbutton[BUTTON_UNCLICK].getWidth();
	settingsbuttonh[BUTTON_UNCLICK] = settingsbutton[BUTTON_UNCLICK].getHeight();
	settingsbuttonh[BUTTON_CLICK] = settingsbutton[BUTTON_CLICK].getHeight();
	settingsbuttonx = getWidth() - settingsbuttonw - 30;
	settingsbuttony[BUTTON_UNCLICK] = getHeight() - settingsbuttonh[BUTTON_UNCLICK] - 30;
	settingsbuttony[BUTTON_CLICK] = getHeight() - settingsbuttonh[BUTTON_CLICK] - 30;

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
	player.speed = 10;
	player.upkey = false;
	player.downkey = false;
	player.leftkey = false;
	player.rightkey = false;
	player.animcounter = 0;
	player.animframeno = 0;
	player.ishit = false;
	player.deadanimplayed = false;
	player.health = 1000;
	player.canshoot = true;
	player.cooldown = 2;
	player.cooldowntimer = 0;
	player.damage = 120;
	player.energy = 0;

	player.gold = 0;
	player.score = 0;
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

void gCanvas::setupBullet() {
	bulletimage[0].loadImage("cat_shot_1.png");
	bulletimage[1].loadImage("suit_alien_shot_1.png");
	bulletimage[2].loadImage("ufo_alien_shot_1.png");
}

void gCanvas::setupPanel() {
	puanpanelimage.loadImage("gui/puanpanel.png");
	goldpanelimage.loadImage("gui/altinpanel.png");
    healthbarimage.loadImage("gui/life_energy_bar.png");
    healthfillimage.loadImage("gui/life_fill.png");
    energyfillimage.loadImage("gui/energy_fill.png");
	panelfont.loadFont("action_man.ttf", 24);

	puanpanel.w = puanpanelimage.getWidth();
	puanpanel.h = puanpanelimage.getHeight();
	puanpanel.x = 0;
	puanpanel.y = 0;

	goldpanel.w = goldpanelimage.getWidth();
	goldpanel.h = goldpanelimage.getHeight();
	goldpanel.x = 0;
	goldpanel.y = puanpanel.h;

	float healthbarsizeratio = 0.5f;
    healthbar.w = healthbarimage.getWidth() * healthbarsizeratio;
    healthbar.h = healthbarimage.getHeight() * healthbarsizeratio;
    healthbar.x = getWidth() - healthbar.w - 10;
    healthbar.y = 10;

    healthfill.w = healthbar.w - 40 * healthbarsizeratio;
	healthfill.h = healthbar.h - 32 * healthbarsizeratio;
	healthfill.x = healthbar.x + 20 * healthbarsizeratio;
	healthfill.y = healthbar.y + 16 * healthbarsizeratio;

    healthtext.w = panelfont.getStringWidth(healthText);
    healthtext.h = panelfont.getStringHeight(healthText);
    healthtext.x = healthbar.x + (healthbar.w / 2) - (healthtext.w / 2);
    healthtext.y = healthbar.y + (healthbar.h / 2) + (healthtext.h / 4);

    energybar.w = healthbarimage.getWidth() * healthbarsizeratio;
    energybar.h = healthbarimage.getHeight() * healthbarsizeratio;
    energybar.x = getWidth() - healthbar.w - 10;
    energybar.y = 10 + healthbar.h;

    energyfill.w = healthbar.w - 40 * healthbarsizeratio;
    energyfill.h = healthbar.h - 32 * healthbarsizeratio;
    energyfill.x = healthbar.x + 20 * healthbarsizeratio;
    energyfill.y = healthbar.y + 16 * healthbarsizeratio + healthbar.h;

    energytext.w = panelfont.getStringWidth(energyText);
    energytext.h = panelfont.getStringHeight(energyText);
    energytext.x = energybar.x + (energybar.w / 2) - (energytext.w / 2);
    energytext.y = energybar.y + (energybar.h / 2) + (energytext.h / 4);

	puantext = "0";
	goldtext = "0";

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
	for(int i = 0; i < activegolds.size(); i++) {
		Gold& gold = activegolds[i];

		gold.x -= gold.speed;
		goldAnimator(gold, GOLD_FRAME_COUNT);

		gold.iscollide = checkCollision(gold.x, gold.y, gold.w, gold.h, COL_GP);
		if(gold.iscollide || (gold.x + gold.w) < mapleft) activegolds.erase(activegolds.begin() + i);
	}
}

void gCanvas::updatePlayer() {
	// Movement
	if(player.upkey && player.y > maptop) player.y -= 1 * player.speed;
	if(player.downkey && (player.y + player.h) < mapbottom) player.y += 1 * player.speed;
	if(player.leftkey && player.x > mapleft) player.x -= 1 * player.speed;
	if(player.rightkey && (player.x + player.w) < mapright) player.x += 1 * player.speed;

	// Control the collision between the player and the enemy.
	player.ishit = checkCollision(player.x, player.y, player.w, player.h, COL_PE);
	playerAnimControl();

	if(!player.ishit) playerAnimator(player, 0, 2, PLAYER_IDLE);
	if(!player.canshoot) {
		player.canshoot = cooldown(player.cooldown, player.cooldowntimer);
	}

//	checkCollision(player.x, player.y, player.w, player.h, COL_PB);
}

void gCanvas::updateEnemy() {
	for(int i = 0; i < enemies.size(); i++){
		Enemy& enemy = enemies[i];
		// Delete an enemy if their health is below 0 or they are outside the map limits, then generate drops.
		if(enemy.health <= 0 || (enemy.x + enemy.w) < mapleft){
			// Increase score
			player.score++;
			// Generate drop
			generateGold(enemy.x, enemy.y, enemy.w, enemy.h);
			// Generate explosion
			generateExplosion(enemy.x, enemy.y, enemy.w, enemy.h);
			// Delete
			enemies.erase(enemies.begin() + i);
		}
		// Enemy movement.
		enemy.x -= enemy.speed;
		// Enemy shoot control.
		if(!enemy.canshoot) {
			enemy.canshoot = cooldown(enemy.cooldown, enemy.cooldowntimer);
		}
		else {
			int bullettype = enemy.type < 3 ? UFO_ALIEN : SUIT_ALIEN;
			generateBullet(enemy.x, enemy.y + (enemy.h / 4), enemy.w, enemy.h, OWNER_ENEMY, bullettype, enemy.damage);
			enemy.canshoot = false;
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
        Bullet& bullet = activebullets[i];
        bullet.x += bullet.speed;
        bullet.ishit = false;
        if(bullet.owner == OWNER_ENEMY) {
            bullet.ishit = checkCollision(bullet.x, bullet.y, bullet.w, bullet.h, COL_PB, bullet.damage);
            if(bullet.ishit){
                playerAnimControl();
                activebullets.erase(activebullets.begin() + i);
                i--;
                continue;
            }

        }
        if(bullet.owner == OWNER_PLAYER) {
            bullet.ishit = checkCollision(bullet.x, bullet.y, bullet.w, bullet.h, COL_EB, bullet.damage);
            if(bullet.ishit){
                activebullets.erase(activebullets.begin() + i);
                i--;
                continue;
            }
        }

        bullet.ishit = checkCollision(bullet.x, bullet.y, bullet.w, bullet.h, COL_BB, 0, bullet.id);

        if (bullet.ishit || bullet.x > getWidth() || (bullet.x + bullet.w) < mapleft) {
            activebullets.erase(activebullets.begin() + i);
            i--;
        }
    }
}

void gCanvas::drawPlayer() {
	playerimg[player.animframeno].draw(player.x, player.y, player.w, player.h);
}

void gCanvas::drawEnemy() {
	for(int i = 0; i < enemies.size(); i++){
		Enemy& enemy = enemies[i];
		enemyimage[enemy.type].draw(enemy.x, enemy.y, enemy.w, enemy.h);
	}

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
	for(const auto& gold : activegolds) {
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
		if(bullet.owner == OWNER_PLAYER) bulletimage[bullet.type].draw(bullet.x, bullet.y, bullet.w, bullet.h);
		if(bullet.owner == OWNER_ENEMY) bulletimage[bullet.type].draw(bullet.x, bullet.y, bullet.w, bullet.h);
	}
}

void gCanvas::drawPanel() {
	puanpanelimage.draw(puanpanel.x, puanpanel.y, puanpanel.w, puanpanel.h);
	goldpanelimage.draw(goldpanel.x, goldpanel.y, goldpanel.w, goldpanel.h);

	panelfont.drawText(puantext, text[0].x, text[0].y);
	panelfont.drawText(goldtext, text[1].x, text[1].y);

//health
    healthbarimage.draw(healthbar.x, healthbar.y, healthbar.w, healthbar.h);
    float healthPercentage = std::max(0.0f, std::min(static_cast<float>(player.health) / 1000.0f, 1.0f));
    float fillWidthHealth = healthfill.w * healthPercentage;
    healthText = gToStr(int(healthPercentage * 100)) + "%";
// Energy
    healthbarimage.draw(energybar.x, energybar.y, energybar.w, energybar.h);
    float energyPercentage = std::max(0.0f, std::min(static_cast<float>(player.energy) / 500.0f, 1.0f));
    float fillWidthEnergy = energyfill.w * energyPercentage;
    energyText = gToStr(int(energyPercentage * 100)) + "%";


    healthfillimage.draw(healthfill.x, healthfill.y, fillWidthHealth, healthfill.h);
    panelfont.drawText(healthText, healthtext.x, healthtext.y);
    energyfillimage.draw(energyfill.x, energyfill.y, fillWidthEnergy, energyfill.h);
    panelfont.drawText(energyText, energytext.x, energytext.y);

}
void gCanvas::drawPausePanel() {
	pausepanelimage.draw(pausepanelx, pausepanely, pausepanelw, pausepanelh);
	settingsbutton[settingsbuttonstate].draw(settingsbuttonx, settingsbuttony[settingsbuttonstate], settingsbuttonw, settingsbuttonh[settingsbuttonstate]);

}

void gCanvas::generateGold(int x, int y, int w, int h) {
	Gold newgold;

	newgold.w = goldimage[0].getWidth();
	newgold.h = goldimage[0].getHeight();
	newgold.x = x + ((w - newgold.w) / 2);
	newgold.y = y + ((h - newgold.h) / 2);
	newgold.animcounter = 0;
	newgold.animframeno = 0;
	newgold.iscollide = false;
	newgold.speed = 5;

	activegolds.push_back(newgold);
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

void gCanvas::generateBullet(int x, int y, int w, int h, int owner, int type, int damage) {
	Bullet newbullet;
	if(activebullets.size() > 0) newbullet.id = activebullets[activebullets.size() - 1].id + 1;
	else newbullet.id = 0;

	if(owner == OWNER_ENEMY) {
		newbullet.w = w / 2;
		newbullet.h = h / 4;
	}
	else {
		newbullet.w = w;
		newbullet.h = h;
	}
	newbullet.x = x;
	newbullet.y = y;
	newbullet.damage = damage;
	newbullet.speed = owner == OWNER_PLAYER ? 10 : -10;
	newbullet.owner = owner;
	newbullet.ishit = false;
	newbullet.type = type;

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

void gCanvas::spawnEnemy(int type) {
	Enemy newenemy;
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

bool gCanvas::checkCollision(int x, int y, int w, int h, int type, int power, int id) {
	if(type == COL_PB) {
		checkcol = gCheckCollision(x, y, x + w, y + h, player.x, player.y, player.x + player.w, player.y + player.h);
		if(checkcol) {
			player.ishit = true;
			player.health -= power;
			return checkcol;
		}
	}

	if(type == COL_PE) {
        for(auto& enemy : enemies) {
            checkcol = gCheckCollision(x, y, x + w, y + h, enemy.x, enemy.y, enemy.x + enemy.w, enemy.y + enemy.h);
            if(checkcol) {
                player.ishit = true;
                player.health -= enemy.damage;
	            player.energy += 25;
	            player.energy = std::min(player.energy, 500);
                if(player.health > 0) {
                    enemy.health = 0;
                }
                return checkcol;
            }
        }
	}

	if(type == COL_EB) {
		for(int i = 0; i < enemies.size(); i++) {
			Enemy& enemy = enemies[i];

			checkcol = gCheckCollision(x, y, x + w, y + h, enemy.x, enemy.y, enemy.x + enemy.w, enemy.y + enemy.h);
			if(checkcol) {
				enemy.health -= power;
	            player.energy += 25;
	            player.energy = std::min(player.energy, 500);
				return checkcol;


			}
		}
	}

	if(type == COL_BB) {
		for(int i = 0; i < activebullets.size(); i++) {
			Bullet& bullet = activebullets[i];
			if(id != bullet.id) {
				checkcol = gCheckCollision(x, y, x + w, y + h, bullet.x, bullet.y, bullet.x + bullet.w, bullet.y + bullet.h);
				return checkcol;
			}
		}
	}

	if(type == COL_GP) {
		checkcol = gCheckCollision(x, y, x + w, y + h, player.x, player.y, player.x + player.w, player.y + player.h);
		player.gold++;
		return checkcol;
	}

	return false;
}

void gCanvas::playerAnimControl() {
	// Animation
	if(player.ishit) {
		playerAnimator(player, 2, 3, PLAYER_HURT);
		player.ishit = cooldown(player.cooldown, player.cooldowntimer);
	}
}
