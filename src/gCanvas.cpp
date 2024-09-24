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
	setupDrops();
	setupExplosion();
	setupPanel();
	setupBullet();
	setupEnemy();
	setupLevel();

	setupGameButtons();
	setupPausePanel();
}

void gCanvas::update() {
	if(gamestate == GAMESTATE_PLAY){
		updateBackground();
		updateDrops();
		updatePlayer();
		updateExplosion();
		updateBullet();
		generateEnemy();
		updateEnemy();
	}
	if(gamestate == GAMESTATE_WAITFORNEXTLEVEL) {
		player.x = 50;
		player.y = (getHeight() - player.h) / 2;
		activebullets.clear();
		if(waitTimer > 0) {
			waitTimer--;
		} else {
			showNextLevelMessage = false;
			currentenemylevel++;
			setupLevel();
			gamestate = GAMESTATE_PLAY;
		}
	}
}

void gCanvas::draw() {
	calculateFPS();

	//
    drawBackground();
    drawPlayer();
    drawDrops();
    drawExplosion();
    drawPanel();
    drawBullet();
    drawEnemy();
    drawGameButtons();

	if(gamestate == GAMESTATE_PAUSE){
		drawPausePanel();
	}

    if(showNextLevelMessage) {
        std::string message = "Daha zor dusmanlar geliyor!";
        float textWidth = panelfont.getStringWidth(message);
        float textX = (getWidth() - textWidth) / 2;
        float textY = getHeight() / 2;

        panelfont.drawText(message, textX, textY);
    }
	//

    previousFrameTime = currentFrameTime;
}

void gCanvas::keyPressed(int key) {
//	gLogi("gCanvas") << "keyPressed:" << key;
//	gLogi("Key ") << std::to_string(key);
	if(key == 32) {
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

	if(key == 256){
        if (gamestate == GAMESTATE_PLAY) {
            gamestate = GAMESTATE_PAUSE;
            //std::cout << "Game paused." << gamestate << std::endl;
        } else {
            gamestate = GAMESTATE_PLAY;
           // std::cout << "Game resumed." << std::endl;
        }
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
				// Play button sound
				root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
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
	if(button == 0) targetfps -= 10;
	if(button == 1) targetfps += 10;

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
	waitTimer = 0;
	showNextLevelMessage = false;
	fps = 0;
	targetfps = 60;

	root->soundManager(root->SOUND_GAME, 100, root->SOUND_TYPE_STARTING);
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
	backgroundimage[0].loadImage("city_1.png");
	backgroundimage[1].loadImage("city_1.png");

	background[0].w = backgroundimage[0].getWidth();
	background[0].h = backgroundimage[0].getHeight();
	background[0].x = 0;
	background[0].y = 0;

	background[1].w = backgroundimage[1].getWidth();
	background[1].h = backgroundimage[1].getHeight();
	background[1].x = getWidth();
	background[1].y = 0;

	backgroundspeed = 4.5f;
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
	player.cooldown = 60;
	player.cooldowntimer = 0;
	player.damage = 120;

	player.gold = 0;
	player.score = 0;
	player.energy = 0;
	player.level = 1;
}

void gCanvas::setupEnemy() {
	enemyimage[UFO_RED].loadImage("red_ufo_idle_1.png");
	enemyimage[UFO_BLACK].loadImage("black_ufo_idle_1.png");
	enemyimage[UFO_GREEN].loadImage("green_ufo_idle_1.png");
	enemyimage[SUIT_BLACK].loadImage("black_suit_idle_1.png");
	enemyimage[SUIT_PURPLE].loadImage("purple_suit_idle_1.png");
	enemyimage[SUIT_ORANGE].loadImage("orange_suit_idle_1.png");

	enemyspeeds[UFO_RED] = 10.0f;
	enemyspeeds[UFO_BLACK] = 10.0f;
	enemyspeeds[UFO_GREEN] = 10.0f;
	enemyspeeds[SUIT_BLACK] = 10.0f;
	enemyspeeds[SUIT_PURPLE] = 10.0f;
	enemyspeeds[SUIT_ORANGE] = 10.0f;

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

	currentenemylevel = 1;

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

void gCanvas::setupBullet() {
	bulletimage[0].loadImage("cat_shot_1.png");
	bulletimage[1].loadImage("suit_alien_shot_1.png");
	bulletimage[2].loadImage("ufo_alien_shot_1.png");
}

void gCanvas::setupDrops() {
	for(int i = 0; i < GOLD_FRAME_COUNT; i++) {
		goldimage[i].loadImage("golds/" + gToStr(i + 1) + ".png");
		dropimage[0][i].loadImage("golds/" + gToStr(i + 1) + ".png");
	}
	for(int i = 0; i < POWER_BUFF_FRAME_COUNT; i++) {
		powerbuffimage[i].loadImage("power_" + gToStr(i + 1) + ".png");
		dropimage[1][i].loadImage("power_" + gToStr(i + 1) + ".png");
	}
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
	healthtext.x = healthbar.x + 15;
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
	energytext.x = energybar.x + 15;
	energytext.y = energybar.y + (energybar.h / 2) + (energytext.h / 4);

	puantext = gToStr(player.score);
	goldtext = gToStr(player.gold);

	panelfont.loadFont("action_man.ttf", 24);

	text[0].x = puanpanel.x + (puanpanel.w / 2.75f);
	text[0].y = puanpanel.y + (puanpanel.h - (panelfont.getStringHeight(puantext) / 1.25f));

	text[1].x = goldpanel.x + (goldpanel.w / 3.30f);
	text[1].y = goldpanel.y + (goldpanel.h - (panelfont.getStringHeight(puantext) / 1.25f));
}

void gCanvas::setupLevel() {
	enemiesToSpawn = 10 + (currentenemylevel - 1) * 5;
	remainingEnemies = enemiesToSpawn;

    spawnctr = 0;
}

void gCanvas::updateBackground() {
	for(int i = 0; i < BACKGROUND_COUNT; i++) {
		background[i].x -= backgroundspeed;

		if ((background[i].x + background[i].w) < 0) {
			background[i].x = getWidth();
		}
	}
}


void gCanvas::updatePlayer() {
    if (player.upkey && player.y > maptop) player.y -= 1 * player.speed;
    if (player.downkey && (player.y + player.h) < mapbottom) player.y += 1 * player.speed;
    if (player.leftkey && player.x > mapleft) player.x -= 1 * player.speed;
    if (player.rightkey && (player.x + player.w) < mapright) player.x += 1 * player.speed;

    if (player.ishit) {
        animator(player.animcounter, player.animframeno, 2, 3, 30);

        if (player.animframeno == 3) {
            player.animframeno = 0;
            player.ishit = false;
        }
    } else {
        animator(player.animcounter, player.animframeno, 0, 1, 30);
        player.ishit = checkCollision(player.x, player.y, player.w, player.h, COL_PE);
    }

    if (!player.canshoot) {
        player.canshoot = checkCollision(player.x, player.y, player.w, player.h, COL_PB);
    }
}


void gCanvas::updateEnemy() {
	for(int i = 0; i < enemies.size(); i++){
		Enemy& enemy = enemies[i];
		// Delete an enemy if their health is below 0 or they are outside the map limits, then generate drops.
		if(enemy.health <= 0 || (enemy.x + enemy.w) < mapleft){
			// Increase score
			player.score++;
			puantext = gToStr(player.score);
			// Generate drop
			int choosedrop = (int)gRandom(2);
			generateDrop(enemy.x, enemy.y, enemy.w, enemy.h, choosedrop);

			remainingEnemies--;
			// Generate explosion
			generateExplosion(enemy.x, enemy.y, enemy.w, enemy.h);
			// Delete
			enemies.erase(enemies.begin() + i);
		}
        if(remainingEnemies <= 0) {
        	gamestate = GAMESTATE_WAITFORNEXTLEVEL;
        	waitTimer = 180;
        	showNextLevelMessage = true;

        }
		// Enemy movement.
		enemy.x -= enemy.speed * (targetfps / getFPS());
		// Enemy shoot control.
		if (!enemy.canshoot) {
		    enemy.canshoot = cooldown(enemy.cooldown, enemy.cooldowntimer);
		} else {
		    int bullettype = enemy.type < 3 ? UFO_ALIEN : SUIT_ALIEN;
		    generateBullet(enemy.x, enemy.y + (enemy.h / 4), enemy.w, enemy.h, OWNER_ENEMY, bullettype, enemy.damage);
		    enemy.canshoot = false;
		}
	}
}

void gCanvas::updateDrops() {
	for(int i = 0; i < activedrops.size(); i++) {
		Drop& drop = activedrops[i];

		drop.x -= drop.speed;
		if(drop.id == DROP_GOLD) animator(drop.animcounter, drop.animframeno, 0, GOLD_FRAME_COUNT - 1, 5);
		if(drop.id == DROP_POWER_BUFF) animator(drop.animcounter, drop.animframeno, 0, POWER_BUFF_FRAME_COUNT - 1, 5);

		drop.iscollide = checkCollision(drop.x, drop.y, drop.w, drop.h, COL_D, 0, drop.id);
		if(drop.iscollide || (drop.x + drop.w) < mapleft) activedrops.erase(activedrops.begin() + i);
	}
}

void gCanvas::generateEnemy() {
    if (remainingEnemies > 0 && enemiesToSpawn > 0) {
        spawnctr++;
        if (spawnctr > spawnctrlimit) {
            spawnctr = 0;
            int type = int(gRandom(float(maxenemytypenum)));
            spawnEnemy(type);
            enemiesToSpawn--;
        }
    }
}

void gCanvas::updateExplosion() {
    for (int i = 0; i < activeExplosion.size(); i++) {
        animator(activeExplosion[i][EX_COUNTER], activeExplosion[i][EX_FRAMENO], 0, explosionmaxframe - 1, 2);

        activeExplosion[i][EX_SX] = (activeExplosion[i][EX_FRAMENO] % EXPLOSION_ROW) * explosionframew;
        activeExplosion[i][EX_SY] = (activeExplosion[i][EX_FRAMENO] / EXPLOSION_COLUMN) * explosionframeh;

        // If animation complated delete explosion from array.
        if (activeExplosion[i][EX_FRAMENO] >= explosionmaxframe - 1) {
            activeExplosion.erase(activeExplosion.begin() + i);
        }
    }
}

void gCanvas::updateBullet() {
    for(int i = 0; i < activebullets.size(); i++) {
        Bullet& bullet = activebullets[i];
        bullet.x += bullet.speed * (targetfps / getFPS());
        bullet.ishit = false;
        if(bullet.owner == OWNER_ENEMY) {
            bullet.ishit = checkCollision(bullet.x, bullet.y, bullet.w, bullet.h, COL_PB, bullet.damage);
            if(bullet.ishit){
                activebullets.erase(activebullets.begin() + i);
                i--;
                player.ishit = true;
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
        backgroundimage[i].draw(background[i].x, background[i].y, background[i].w, background[i].h);
    }
}

void gCanvas::drawGameButtons() {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		gamebuttonimage[i].draw(gamebutton[i].x, gamebutton[i].y, gamebutton[i].w, gamebutton[i].h);
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

void gCanvas::drawDrops() {
	for(int i = 0; i < activedrops.size(); i++) {
		Drop drop = activedrops[i];
		dropimage[drop.id][drop.animframeno].draw(drop.x, drop.y, drop.w, drop.h);
	}
}

void gCanvas::drawPanel() {
	puanpanelimage.draw(puanpanel.x, puanpanel.y, puanpanel.w, puanpanel.h);
	goldpanelimage.draw(goldpanel.x, goldpanel.y, goldpanel.w, goldpanel.h);

	panelfont.drawText(puantext, text[0].x, text[0].y);
	panelfont.drawText(goldtext, text[1].x, text[1].y);
	// Health
    healthbarimage.draw(healthbar.x, healthbar.y, healthbar.w, healthbar.h);
    float healthPercentage = std::max(0.0f, std::min(static_cast<float>(player.health) / 1000.0f, 1.0f));
	float fillWidthHealth = healthfill.w * healthPercentage;
    healthText = gToStr(player.health) + " / 1000";

	// Energy
	healthbarimage.draw(energybar.x, energybar.y, energybar.w, energybar.h);
	float energyPercentage = std::max(0.0f, std::min(static_cast<float>(player.energy) / 500.0f, 1.0f));
	float fillWidthEnergy = energyfill.w * energyPercentage;
    energyText = gToStr(player.energy) + " / 500";

    std::string remainingText = "Remaining Enemies: " + std::to_string(remainingEnemies);
    float textWidth = panelfont.getStringWidth(remainingText);
    float textX = (getWidth() - textWidth) / 2;
    float textY = 50;

    panelfont.drawText(remainingText, textX, textY);

	healthfillimage.draw(healthfill.x, healthfill.y, fillWidthHealth, healthfill.h);
	panelfont.drawText(healthText, healthtext.x, healthtext.y);
	energyfillimage.draw(energyfill.x, energyfill.y, fillWidthEnergy, energyfill.h);
	panelfont.drawText(energyText, energytext.x, energytext.y);
}

void gCanvas::drawPausePanel() {
	pausepanelimage.draw(pausepanelx, pausepanely, pausepanelw, pausepanelh);
	settingsbutton[settingsbuttonstate].draw(settingsbuttonx, settingsbuttony[settingsbuttonstate], settingsbuttonw, settingsbuttonh[settingsbuttonstate]);
}

void gCanvas::generateDrop(int x, int y, int w, int h, int id) {
	Drop newdrop;

	newdrop.w = goldimage[0].getWidth();
	newdrop.h = goldimage[0].getHeight();
	newdrop.x = x + ((w - newdrop.w) / 2);
	newdrop.y = y + ((h - newdrop.h) / 2);
	newdrop.animcounter = 0;
	newdrop.animframeno = 0;
	newdrop.iscollide = false;
	newdrop.speed = 5;
	newdrop.id = id;

	activedrops.push_back(newdrop);
}

void gCanvas::generateExplosion(int explosionx, int explosiony, int explosionw,
		int explosionh) {

	// Play explosion sound
	root->soundManager(root->SOUND_EXPLOSION, 100, root->SOUND_TYPE_ONHIT);
	// Generate explosion.
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
	newbullet.speed = owner == OWNER_PLAYER ? 20 : -20;
	newbullet.owner = owner;
	newbullet.ishit = false;
	newbullet.type = type;

	activebullets.push_back(newbullet);

	// Play bullet sound.
	if(owner == OWNER_ENEMY) root->soundManager(root->SOUND_ENEMY_FIRE, 100, root->SOUND_TYPE_ONHIT);
	if(owner == OWNER_PLAYER) root->soundManager(root->SOUND_CAT_FIRE, 100, root->SOUND_TYPE_ONHIT);
}

bool gCanvas::cooldown(float &time, float &timer) {
    timer += (targetfps / getFPS());
    if (timer >= getFPS()) {
        time--;
        timer = 0.0f;
        if (time <= 0) {
            time = 0;
            return true;
        }
    }
    return false;
}

void gCanvas::spawnEnemy(int type) {
	Enemy newenemy;
	newenemy.level = currentenemylevel;
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
	newenemy.canshoot = true;

	enemies.push_back(newenemy);
}

bool gCanvas::checkCollision(int x, int y, int w, int h, int type, int power, int id) {
	if(type == COL_PB) {
		checkcol = gCheckCollision(x, y, x + w, y + h, player.x, player.y, player.x + player.w, player.y + player.h);
		if(checkcol) {
			player.health -= power;
			player.health = std::max(player.health, 0);
			return checkcol;
		}
	}

	if(type == COL_PE) {
        for(auto& enemy : enemies) {
            checkcol = gCheckCollision(x, y, x + w, y + h, enemy.x, enemy.y, enemy.x + enemy.w, enemy.y + enemy.h);
            if(checkcol) {
                player.health -= enemy.damage;
                if(player.health > 0) {
                    enemy.health = 0;
                }
                player.health = std::max(player.health, 0);
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

	if(type == COL_D) {
		checkcol = gCheckCollision(x, y, x + w, y + h, player.x, player.y, player.x + player.w, player.y + player.h);
		if(id == DROP_GOLD && checkcol) {
			player.gold++;
			goldtext = gToStr(player.gold);
			root->soundManager(root->SOUND_COIN, 100, root->SOUND_TYPE_ONHIT);
		}
		if(id == DROP_POWER_BUFF && checkcol) {
			root->soundManager(root->SOUND_POWERUP, 100, root->SOUND_TYPE_ONHIT);
		}
		return checkcol;
	}

	return false;
}

void gCanvas::animator(int &animcounter, int &animframeno, int startframe, int framecount, float targetfps) {
    if (animframeno < startframe) {
        animframeno = startframe;
    }

    animcounter++;

    int framespeed = static_cast<int>(60.0f * (targetfps / getFPS()));

    if (animcounter >= framespeed) {
        animframeno++;
        animcounter = 0;
    }

    if (animframeno > framecount) {
        animframeno = startframe;
    }
}

void gCanvas::calculateFPS() {
	// Calculte delta time.
    if (previousFrameTime.time_since_epoch().count() == 0) {
        previousFrameTime = std::chrono::high_resolution_clock::now();
    }

    currentFrameTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> elapsedTime = currentFrameTime - previousFrameTime;

    // Get delta time in seconds.
    deltatime = elapsedTime.count();

    // Calculate fps.
    fps = 1.0f / deltatime;

//    gLogi("Delta Time: ") << deltatime << " seconds, FPS: " << fps;
}

float gCanvas::getFPS() {
	return fps;
}

