 /*
 * gCanvas.cpp
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan, Mehmet Furkan Utlu, Onur Imre
 */


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
	root->insertDatabase(createInsertStatement(player));
}

void gCanvas::setup() {
	saveddata = root->getIsDead();
	loadGame(saveddata);

	setupGame();
	setupBackground();
	setupPlayer();
	setupDrops();
	setupExplosion();
	setupBullet();
	setupEnemy();
	setupLevel();

	setupPanel();
	setupGameButtons();
	setupPausePanel();
	setupMarket();
	setupWarning();
	setupEndPanel();
}

void gCanvas::update() {
	calculateFPS();
	//
	if(gamestate == GAMESTATE_PLAY){
        updateBackground();
        updateDrops();
        updatePlayer();
        updateExplosion();
        updateBullet();
        generateEnemy();
        updateEnemy();
        updateSpecialAbility();
    	updateDifficultyMessage();
	}
}

void gCanvas::draw() {
    drawBackground();
    drawPlayer();
    drawDrops();
    drawExplosion();
    drawBullet();
    drawEnemy();

    drawGameButtons();
    drawSpecialAbility();
    if(gamestate == GAMESTATE_PLAY){
    	drawDifficultyMessage();
//        drawGameButtons();
    }
    drawPanel();
    if(gamestate == GAMESTATE_MARKET) drawMarket();
	if(gamestate == GAMESTATE_PAUSE) drawPausePanel();
    if(gamestate == GAMESTATE_WARNING || gamestate == GAMESTATE_EXIT) drawWarning();
    if(gamestate == GAMESTATE_END_GAME) drawEndPanel();
	//
    previousframetime = currentframetime;
}

void gCanvas::keyPressed(int key) {
	//	if(key == 32) {
	//		if(player.canshoot){
	//			generateBullet(player.x + (player.w / 2), player.y + (player.h / 1.5f), player.w, player.h / 4, OWNER_PLAYER, PLAYER, player.damage);
	//			player.canshoot = false;
	//		}
	//	}
    if(key == 32) {
        if (player.energy >= player.maxenergy / 2) {
            activateSpecialAbility();
            player.energy -= player.maxenergy / 2;
        }
    }
	if(key == 87) {
		player.upkey = true;
		root->soundManager(root->SOUND_BUTTON, 100, root->SOUND_TYPE_ONHIT);
	}
	if(key == 83) {
		player.downkey = true;
		root->soundManager(root->SOUND_BUTTON, 100, root->SOUND_TYPE_ONHIT);
	}
	if(key == 65) {
		player.leftkey = true;
		root->soundManager(root->SOUND_BUTTON, 100, root->SOUND_TYPE_ONHIT);
	}
	if(key == 68) {
		player.rightkey = true;
		root->soundManager(root->SOUND_BUTTON, 100, root->SOUND_TYPE_ONHIT);
	}

	if(key == 256){
        if (gamestate == GAMESTATE_PLAY) {
            gamestate = GAMESTATE_PAUSE;
            //std::cout << "Game paused." << gamestate << std::endl;
        } else {
            changeGameState(GAMESTATE_PLAY);
           // std::cout << "Game resumed." << std::endl;
        }
	}
}

void gCanvas::keyReleased(int key) {
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
}

void gCanvas::mouseMoved(int x, int y) {
}

void gCanvas::mouseDragged(int x, int y, int button) {
}

void gCanvas::mousePressed(int x, int y, int button) {
	generateDrop(player.x + 1000, player.y + 200, player.w, player.h, DROP_POWER_BUFF);
	generateDrop(player.x + 1000, player.y, player.w, player.h, DROP_GOLD);
	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(x > gamebutton[i].x && x < (gamebutton[i].x + gamebutton[i].w) &&
		   y > gamebutton[i].y && y < (gamebutton[i].y + gamebutton[i].h)) {
			if(!gamebutton[i].pressed) {
				root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
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
//						if(player.canshoot){
//							generateBullet(player.x + (player.w / 2), player.y + (player.h / 1.5f), player.w, player.h / 4, OWNER_PLAYER, PLAYER , player.damage);
//							player.canshoot = false;
//						}
						break;
				}
				gamebutton[i].pressed = true;
			}
		}
	}

	if(gamestate == GAMESTATE_MARKET) {
		for(int i = 0; i < MARKET_SLOTS; i++) {
			if(x > marketbutton[i].x && x < (marketbutton[i].x + marketbutton[i].w) && y > marketbutton[i].y && y < (marketbutton[i].y + marketbutton[i].h)) {
				marketbutton[i].pressed = true;
				root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
			}
		}
		if (pow(x - marketclosebutton.centerx, 2) + pow(y - marketclosebutton.centery, 2) < pow(marketclosebutton.radius, 2)) {
			marketclosebutton.pressed = true;
			root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
		}
	}

	if(gamestate == GAMESTATE_WARNING || gamestate == GAMESTATE_EXIT) {
		for(int i = 0; i < 2; i++) {
			if (pow(x - xcenter[i], 2) + pow(y - ycenter[i], 2) < pow(radius[i], 2)) {
				warningbutton[i].pressed = true;
				root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
			}
		}
	}

	if(gamestate == GAMESTATE_END_GAME) {
		for(int i = 0; i < 2; i++) {
			if (pow(x - endgamebutton[i].centerx, 2) + pow(y - endgamebutton[i].centery, 2) < pow(endgamebutton[i].radius, 2)) {
				endgamebutton[i].pressed = true;
				root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
			}
		}
	}

	//
	if(gamestate == GAMESTATE_PAUSE){
		if (gamestate == GAMESTATE_PAUSE) {
			if (x > settingsbuttonx && x < settingsbuttonx + settingsbuttonw &&
				y > settingsbuttony[BUTTON_UNCLICK] && y < settingsbuttony[BUTTON_UNCLICK] + settingsbuttonh[BUTTON_UNCLICK]) {

				settingsbuttonstate = !settingsbuttonstate;
			}

			if (settingsbuttonstate == BUTTON_CLICK) {
				if (x > soundButtonX && x < soundButtonX + soundButtonW &&
					y > soundButtonY && y < soundButtonY + soundButtonH) {
					root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
					root->toggleSound();
				}

				if (x > musicButtonX && x < musicButtonX + musicButtonW &&
					y > musicButtonY && y < musicButtonY + musicButtonH) {
					root->soundManager(root->SOUND_BUTTON2, 100, root->SOUND_TYPE_ONHIT);
					root->toggleMusic();
				}
			}
		}
	}

	// Pause panel.
    if (gamestate == GAMESTATE_PAUSE) {
        if (pow(x - (pausepanelbuttonx[BUTTON_PLAY] + pausepanelbuttonw / 2), 2) + pow(y - (pausepanelbuttony + pausepanelbuttonh / 2), 2) <= pow(pausepanelbuttonw / 2, 2)) {
            changeGameState(GAMESTATE_PLAY);
        }

        if (pow(x - (pausepanelbuttonx[BUTTON_RESTART] + pausepanelbuttonw / 2), 2) + pow(y - (pausepanelbuttony + pausepanelbuttonh / 2), 2) <= pow(pausepanelbuttonw / 2, 2)) {
			root->onSceneChange();
    		gCanvas* cnv = new gCanvas(root);
    		appmanager->setCurrentCanvas(cnv);
    		endgamebutton[END_GAME_BUTTON_RESTART].pressed = false;
        }
        if (pow(x - (pausepanelbuttonx[BUTTON_MENU] + pausepanelbuttonw / 2), 2) + pow(y - (pausepanelbuttony + pausepanelbuttonh / 2), 2) <= pow(pausepanelbuttonw / 2, 2)) {
			root->onSceneChange();
    		menuCanvas* menu = new menuCanvas(root);
    		appmanager->setCurrentCanvas(menu);
    		endgamebutton[END_GAME_BUTTON_MENU].pressed = false;
        }
    }
}

void gCanvas::mouseReleased(int x, int y, int button) {
	// For test purpose.
//	if(button == 0) targetfps -= 10;
//	if(button == 1) targetfps += 10;
//	player.powerup = true;
	// For test purpose.

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

	if(gamestate == GAMESTATE_MARKET) {
		for(int i = 0; i < MARKET_SLOTS; i++) {
			if(marketbutton[i].pressed) {
				marketBuy(i, player.gold);
				marketbutton[i].pressed = false;
			}
		}
		if(marketclosebutton.pressed == true) {
			changeGameState(GAMESTATE_PLAY);
			marketclosebutton.pressed = false;
		}
	}

	if(gamestate == GAMESTATE_WARNING) {
		if (warningbutton[WARNING_NO].pressed == true) {
			if(lastgamestate == GAMESTATE_PAUSE) {
				changeGameState(GAMESTATE_PAUSE);
			}
			if(lastgamestate == GAMESTATE_MARKET)  {
				changeGameState(GAMESTATE_MARKET);
			}

			warningbutton[WARNING_NO].pressed = false;
		}
		if(warningbutton[WARNING_YES].pressed == true){
			if(lastgamestate == GAMESTATE_PAUSE) {
				menuCanvas* menu = new menuCanvas(root);
				appmanager->setCurrentCanvas(menu);
			}
			if(lastgamestate == GAMESTATE_MARKET) {
				changeGameState(GAMESTATE_MARKET);
			}
			warningbutton[WARNING_YES].pressed = false;
		}
	}

	if(gamestate == GAMESTATE_END_GAME) {
		if(endgamebutton[END_GAME_BUTTON_RESTART].pressed) {
			gCanvas* cnv = new gCanvas(root);
			appmanager->setCurrentCanvas(cnv);
			endgamebutton[END_GAME_BUTTON_RESTART].pressed = false;
		}
		if(endgamebutton[END_GAME_BUTTON_MENU].pressed) {
			menuCanvas* menu = new menuCanvas(root);
			appmanager->setCurrentCanvas(menu);
			endgamebutton[END_GAME_BUTTON_MENU].pressed = false;
		}
	}
}

void gCanvas::mouseScrolled(int x, int y) {
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
	changeGameState(GAMESTATE_PLAY);
//	changeGameState(GAMESTATE_END_GAME);
//	changeGameState(50);
	waittimer = 0;
	fps = 0;
	targetfps = 60;
	showdifficultyincreasemessage = false;
	difficultymessageframes = 0;
	endgamestar = 3;

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

    soundButtonW = settingsbuttonw * 0.6;
    soundButtonH = settingsbuttonh[BUTTON_CLICK] * 0.12;
    soundButtonX = settingsbuttonx + settingsbuttonw * 0.2;
    soundButtonY = settingsbuttony[BUTTON_CLICK] + settingsbuttonh[BUTTON_CLICK] * 0.09;

    musicButtonW = settingsbuttonw * 0.6;
    musicButtonH = settingsbuttonh[BUTTON_CLICK] * 0.15;
    musicButtonX = settingsbuttonx + settingsbuttonw * 0.2;
    musicButtonY = settingsbuttony[BUTTON_CLICK] + settingsbuttonh[BUTTON_CLICK] * 0.3;

    pausepanelbuttonx[BUTTON_PLAY] = pausepanelx + 120;
    pausepanelbuttonx[BUTTON_RESTART] = pausepanelx + 322;
    pausepanelbuttonx[BUTTON_MENU] = pausepanelx + 530;
    pausepanelbuttony = pausepanely + 148;
    pausepanelbuttonw = 134;
    pausepanelbuttonh = 134;
}

void gCanvas::setupBackground() {
	backgroundimage[CITY].loadImage("city_1.png");
	backgroundimage[SKY].loadImage("space_1.png");

	background[CITY].w = getWidth();
	background[CITY].h = getHeight();
	background[CITY].x = 0;
	background[CITY].y = 0;

    background[SKY].w = getWidth();
    background[SKY].h = getHeight();
    background[SKY].x = 0;
    background[SKY].y = -background[SKY].h;

    backgroundspeed = 4.5f;
    mapyvelocity = 4.0f;
}
void gCanvas::setupPlayer() {
	poweruptimes = 3;
	poweruptimer = 0;
	player.powerup = false;

	playerimg[0].loadImage("yellow_idle_1.png");
	playerimg[1].loadImage("yellow_idle_2.png");
	playerimg[2].loadImage("yellow_dead_1.png");
	playerimg[3].loadImage("yellow_dead_2.png");
	playerimg[4].loadImage("yellow_dead_3.png");

	player.w = playerimg[0].getWidth();
	player.h = playerimg[0].getHeight();
	player.x = 50;
	player.y = (getHeight() - player.h) / 2;
	player.speed = 12;
	player.upkey = false;
	player.downkey = false;
	player.leftkey = false;
	player.rightkey = false;
	player.animcounter = 0;
	player.animframeno = 0;
	player.ishit = false;
	player.deadanimplayed = false;
	player.health = player.maxhealth = gamedatas.health;
	player.maxenergy = 400;
	player.canshoot = true;
	player.cooldown = 1;
	player.cooldowntimer = 0;
	player.cooldownholder = player.cooldown;
	player.buffcooldown = 5;
	player.buffcooldowntimer = 0;
	player.buffcooldownholder = player.buffcooldown;
	player.hurtcooldown = 2;
	player.hurtcooldowntimer = 0;
	player.hurtcooldownholder = player.cooldown;
	player.damage = gamedatas.damage;

	player.isdead = gamedatas.isdead;
	player.gold = gamedatas.gold;
	player.score = gamedatas.score;
	player.energy = 0;
	player.level = gamedatas.level;

	player.goldmultiplier = 1;
	player.buffmultiplier = 1;
}

void gCanvas::setupEnemy() {
	enemyimage[UFO_RED].loadImage("red_ufo_idle_1.png");
	enemyimage[UFO_BLACK].loadImage("black_ufo_idle_1.png");
	enemyimage[UFO_GREEN].loadImage("green_ufo_idle_1.png");
	enemyimage[SUIT_BLACK].loadImage("black_suit_idle_1.png");
	enemyimage[SUIT_PURPLE].loadImage("purple_suit_idle_1.png");
	enemyimage[SUIT_ORANGE].loadImage("orange_suit_idle_1.png");

	enemyspeeds[UFO_RED] = 3.0f;
	enemyspeeds[UFO_BLACK] = 2.5f;
	enemyspeeds[UFO_GREEN] = 2.0f;
	enemyspeeds[SUIT_BLACK] = 1.5f;
	enemyspeeds[SUIT_PURPLE] = 1.0f;
	enemyspeeds[SUIT_ORANGE] = 1.2f;

	enemydamages[UFO_RED] = 25.0f;
	enemydamages[UFO_BLACK] = 20.0f;
	enemydamages[UFO_GREEN] = 22.0f;
	enemydamages[SUIT_BLACK] = 15.0f;
	enemydamages[SUIT_PURPLE] = 18.0f;
	enemydamages[SUIT_ORANGE] = 20.0f;

	enemyhealths[UFO_RED] = 150.0f;
	enemyhealths[UFO_BLACK] = 125.0f;
	enemyhealths[UFO_GREEN] = 140.0f;
	enemyhealths[SUIT_BLACK] = 75.0f;
	enemyhealths[SUIT_PURPLE] = 100.0f;
	enemyhealths[SUIT_ORANGE] = 110.0f;

	enemycooldown[UFO_RED] = 3.0f;
	enemycooldown[UFO_BLACK] = 2.0f;
	enemycooldown[UFO_GREEN] = 2.5f;
	enemycooldown[SUIT_BLACK] = 2.0f;
	enemycooldown[SUIT_PURPLE] = 2.5f;
	enemycooldown[SUIT_ORANGE] = 2.7f;

	enemycooldowntimer[UFO_RED] = 0;
	enemycooldowntimer[UFO_BLACK] = 0;
	enemycooldowntimer[UFO_GREEN] = 0;
	enemycooldowntimer[SUIT_BLACK] = 0;
	enemycooldowntimer[SUIT_PURPLE] = 0;
	enemycooldowntimer[SUIT_ORANGE] = 0;

	currentenemylevel = gamedatas.enemylevel;
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
		dropimage[DROP_GOLD][i].loadImage("golds/" + gToStr(i + 1) + ".png");
	}
	for(int i = 0; i < POWER_BUFF_FRAME_COUNT; i++) {
		dropimage[DROP_POWER_BUFF][i].loadImage("power_" + gToStr(i + 1) + ".png");
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
    enemiesToSpawn = 10 + (currentenemylevel - 1) * 2;
    remainingEnemies = enemiesToSpawn;
    spawnctr = 0;
    float enemymaxSpeed = 5.0f;
    float enemyminSpeed = 1.0f;
    float maxAttackCooldown = 2.0f;
    float minAttackCooldown = 0.5f;

    for(int type = 0; type < MAXENEMYTYPENUM; type++) {
    	enemycooldown[type] = std::max(minAttackCooldown, std::min(maxAttackCooldown, maxAttackCooldown - (currentenemylevel * 0.05f)));
        enemycooldown[type] = std::max(1.0f, enemymaxSpeed - enemyspeeds[type]);
        enemyhealths[type] *= (1.0f + currentenemylevel * 0.05f);
        enemydamages[type] *= (1.0f + currentenemylevel * 0.05f);
    }
}

void gCanvas::updateBackground() {
    if(currentenemylevel % (BACKGROUNDLEVELLIMIT * 2) >= BACKGROUNDLEVELLIMIT) {
        if(background[SKY].y < 0) {
            background[SKY].y += mapyvelocity;
            background[CITY].y += mapyvelocity;
        }
    }
    if(currentenemylevel % (BACKGROUNDLEVELLIMIT * 2) < BACKGROUNDLEVELLIMIT) {
        if(background[CITY].y > 0) {
            background[CITY].y -= mapyvelocity;
            background[SKY].y -= mapyvelocity;

        }
    }
	background[CITY].x -= backgroundspeed;
	background[SKY].x -= backgroundspeed;
	if (background[CITY].x + background[CITY].w < 0) {
		background[CITY].x = 0;
		background[SKY].x = 0;
	}
}


void gCanvas::updatePlayer() {
    if(player.upkey && player.y > maptop) player.y -= 1 * player.speed;
    if(player.downkey && (player.y + player.h) < mapbottom) player.y += 1 * player.speed;
    if(player.leftkey && player.x > mapleft) player.x -= 1 * player.speed;
    if(player.rightkey && (player.x + player.w) < mapright) player.x += 1 * player.speed;

    if(player.ishit && !player.powerup) {
        animator(player.animcounter, player.animframeno, 2, 3, 30);

        if (player.animframeno == 3) {
            player.animframeno = 0;
            player.ishit = false;
        }
    }
    else {
        animator(player.animcounter, player.animframeno, 0, 1, 30);
        player.ishit = checkCollision(player.x, player.y, player.w, player.h, COL_PE);
    }

	if(player.canshoot){
		generateBullet(player.x + (player.w / 2), player.y + (player.h / 1.5f), player.w, player.h / 4, OWNER_PLAYER, PLAYER, player.damage);
		player.canshoot = false;
	}
	else {
    	player.canshoot = cooldown(player.cooldown, player.cooldowntimer, player.cooldownholder);
    }

    if(player.powerup) {
        bool powerfinish = cooldown(player.buffcooldown, player.buffcooldowntimer, player.buffcooldownholder);
        if(powerfinish) {
        	player.powerup = false;
			root->soundManager(root->SOUND_POWERDOWN, 100, root->SOUND_TYPE_ONHIT);
        }
    }

    if(player.health <= 0) {
    	player.isdead = true;
    	calculateStar();
    	root->soundManager(root->SOUND_LOSE, 100, root->SOUND_TYPE_ONHIT);
    	changeGameState(GAMESTATE_END_GAME);
    }
}


void gCanvas::updateEnemy() {
	for(int i = 0; i < enemies.size(); i++){
		Enemy& enemy = enemies[i];
		// Delete an enemy if their health is below 0 or they are outside the map limits, then generate drops.
		if(enemy.health <= 0 ){
			// Increase score
			player.score++;
			puantext = gToStr(player.score);
			// Generate drop
			int choosedrop = getRandomDrop();
			generateDrop(enemy.x, enemy.y, enemy.w, enemy.h, choosedrop);

			remainingEnemies--;
			// Generate explosion
			generateExplosion(enemy.x, enemy.y, enemy.w, enemy.h);
			// Delete
			enemies.erase(enemies.begin() + i);
		}

		if (enemy.x > getWidth() * 3 / 4) {
			enemy.bulletx = enemy.x -= enemy.speed * (targetfps / getFPS());
		}

		if (!enemy.canshoot) {
			enemy.canshoot = cooldown(enemy.cooldown, enemy.cooldowntimer, enemy.cooldownholder);
		} else {
			int bullettype = enemy.type < 3 ? UFO_ALIEN : SUIT_ALIEN;
			generateBullet(enemy.bulletx, enemy.bullety, enemy.w, enemy.h, OWNER_ENEMY, bullettype, enemy.damage);
			enemy.canshoot = false;
		}
	}
	if(remainingEnemies <= 0) {
		changeGameState(GAMESTATE_MARKET);
		currentenemylevel++;
		setupLevel();
	}
}

void gCanvas::updateDrops() {
    for (int i = 0; i < activedrops.size(); i++) {
        Drop& drop = activedrops[i];

        // Update drops position.
        drop.x -= drop.speed; // Hýzla sola hareket etmeye devam et.

        // Update drops width and height.
        drop.h = dropimage[drop.id][drop.animframeno].getHeight();
        drop.w = dropimage[drop.id][drop.animframeno].getWidth();

        // Geniþlik deðerine göre X konumunu ayarla.
        int offsetX = drop.w / 2; // Geniþliðin yarýsýný al.
        int drawX = drop.x + offsetX; // Görsel konumunu hesapla.

        // Y konumunu hafif bir dalgalanma ile güncelle.
        drop.offsetY = sin(drop.animcounter * 0.1f) * 5; // Dalgalanma (5 birim yükseklik)
        int drawY = drop.y + drop.offsetY; // Güncellenmiþ Y konumu

        // Animation.
        if (drop.id == DROP_GOLD)
            animator(drop.animcounter, drop.animframeno, 0, GOLD_FRAME_COUNT - 1, 5);
        if (drop.id == DROP_POWER_BUFF)
            animator(drop.animcounter, drop.animframeno, 0, POWER_BUFF_FRAME_COUNT - 1, 5);

        // Collision control.
        drop.iscollide = checkCollision(drop.x, drawY, drop.w, drop.h, COL_D, 0, drop.id);

        if (drop.iscollide || (drop.x + drop.w) < mapleft) {
            activedrops.erase(activedrops.begin() + i);
            i--;
        }
    }
}

void gCanvas::generateEnemy() {
    if (remainingEnemies > 0 && enemiesToSpawn > 0) {
        spawnctr++;
        if (spawnctr > spawnctrlimit) {
            spawnctr = 0;
            std::vector<int> allowedEnemies;
            if (currentenemylevel % 10 >= 5) {
                allowedEnemies = {UFO_RED, UFO_BLACK, UFO_GREEN};
            } else {
                allowedEnemies = {SUIT_BLACK, SUIT_PURPLE, SUIT_ORANGE};
            }

            int randomIndex = int(gRandom(float(allowedEnemies.size())));
            int type = allowedEnemies[randomIndex];

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
	static float colorchangetime = 0.0f; // Effect timer.
	if(player.powerup) enablePowerUp(colorchangetime);
	if(player.ishit) enablePlayerHit(colorchangetime);

    playerimg[player.animframeno].draw(player.x, player.y, player.w, player.h);

	if(player.powerup || player.ishit) {
		colorchangetime += 0.1f;
		renderer->setColor(255, 255, 255);
	}
}

void gCanvas::drawEnemy() {
    for (int i = 0; i < enemies.size(); i++) {
        Enemy& enemy = enemies[i];
        enemyimage[enemy.type].draw(enemy.x, enemy.y, enemy.w, enemy.h);

        float healthPercentage = std::max(0.0f, std::min(enemy.health / enemyhealths[enemy.type], 1.0f));

        if (healthPercentage < 1.0f) {
            float barWidth = enemy.w * 0.6f;
            float barHeight = 8.0f;
            float barX = enemy.x + (enemy.w - barWidth) / 2;
            float barY = enemy.y - 15;

            healthbarimage.draw(barX, barY, barWidth, barHeight * 2);

            healthfillimage.draw(barX + 5, barY + 5, (barWidth - 10) * healthPercentage, barHeight);
        }
    }
}

void gCanvas::drawBackground() {
    backgroundimage[CITY].draw(background[CITY].x, background[CITY].y, background[CITY].w, background[CITY].h);
    backgroundimage[SKY].draw(background[SKY].x, background[SKY].y, background[SKY].w, background[SKY].h);
    if (background[CITY].x + background[CITY].w < getWidth()) {
        backgroundimage[CITY].draw(background[CITY].x + background[CITY].w, background[CITY].y, background[CITY].w, background[CITY].h);
        backgroundimage[SKY].draw(background[SKY].x + background[SKY].w, background[SKY].y, background[SKY].w, background[SKY].h);
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
		Drop &drop = activedrops[i];

	    int offsetX = drop.w / 2;
	    int drawX = drop.x + offsetX;

	    int drawY = drop.y + drop.offsetY;
		dropimage[drop.id][drop.animframeno].draw(drop.x + drawX, drop.y, drop.w, drop.h);
	}
}

void gCanvas::drawPanel() {
	puanpanelimage.draw(puanpanel.x, puanpanel.y, puanpanel.w, puanpanel.h);
	goldpanelimage.draw(goldpanel.x, goldpanel.y, goldpanel.w, goldpanel.h);

	panelfont.drawText(puantext, text[0].x, text[0].y);
	panelfont.drawText(goldtext, text[1].x, text[1].y);
	// Health
    healthbarimage.draw(healthbar.x, healthbar.y, healthbar.w, healthbar.h);
    float healthPercentage = std::max(0, std::min(player.health / player.maxhealth, 1));
	float fillWidthHealth = healthfill.w * healthPercentage;
    healthText = gToStr(player.health) + " / " + gToStr(player.maxhealth);

	// Energy
	healthbarimage.draw(energybar.x, energybar.y, energybar.w, energybar.h);
	float energyPercentage = std::max(0, std::min(player.energy / player.maxenergy, 1));
	float fillWidthEnergy = energyfill.w * energyPercentage;
    energyText = gToStr(player.energy) + " / " + gToStr(player.maxenergy);

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

    newdrop.w = dropimage[id][0].getWidth();
    newdrop.h = dropimage[id][0].getHeight();
    newdrop.x = x + ((w - newdrop.w) / 2);
    newdrop.y = y + ((h - newdrop.h) / 2);
    newdrop.animcounter = 0;
    newdrop.animframeno = 0;
    newdrop.iscollide = false;
    newdrop.speed = 1;
    newdrop.id = id;

//    newdrop.startx = newdrop.x;

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

bool gCanvas::cooldown(float &time, float &timer, float savedtime) {
    timer += (targetfps / getFPS());
    if (timer >= getFPS()) {
        time--;
        timer = 0.0f;
        if (time <= 0) {
            time = savedtime;
            return true;
        }
    }
    return false;
}

void gCanvas::spawnEnemy(int type) {
	Enemy newenemy;
	newenemy.level = currentenemylevel;
	newenemy.w = enemyimage[type].getWidth() * 0.8f;
	newenemy.h = enemyimage[type].getHeight() * 0.8f;
	newenemy.x = getWidth();
	newenemy.y = int(gRandom(float(getHeight() - newenemy.h)));
	newenemy.type = type;
	newenemy.speed = enemyspeeds[type];
	newenemy.health = enemyhealths[type];
	newenemy.damage = enemydamages[type];
	newenemy.isalive = true;
	newenemy.cooldown = enemycooldown[type];
	newenemy.cooldowntimer = enemycooldowntimer[type];
	newenemy.cooldownholder = newenemy.cooldown;
	newenemy.canshoot = true;

	int bullettype = newenemy.type < 3 ? UFO_ALIEN : SUIT_ALIEN;
	if(bullettype == SUIT_ALIEN) {
		newenemy.bulletx = newenemy.x - (newenemy.w / 6);
		newenemy.bullety = newenemy.y + (newenemy.h / 2.15f);
	}
	else {
		newenemy.bulletx = newenemy.x;
		newenemy.bullety = newenemy.y + (newenemy.h / 1.25f);
	}

	newenemy.hitBySpecial = false;
	enemies.push_back(newenemy);
}

bool gCanvas::checkCollision(int x, int y, int w, int h, int type, int power, int id) {
	if(type == COL_PB) {
		checkcol = gCheckCollision(x, y, x + w, y + h, player.x, player.y, player.x + player.w, player.y + player.h);
		if(checkcol) {
			if(!player.powerup) {
				player.health -= power;
				player.health = std::max(player.health, 0);
				player.ishit = true;
			}
			return checkcol;
		}
	}

	if(type == COL_PE) {
        for(auto& enemy : enemies) {
            checkcol = gCheckCollision(x, y, x + w, y + h, enemy.x, enemy.y, enemy.x + enemy.w, enemy.y + enemy.h);
            if(checkcol) {
            	if(!player.powerup) {
                    player.health -= enemy.damage;
                    player.health = std::max(player.health, 0);
            	}
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
				player.energy += power / 6;
	            player.energy = std::min(player.energy, 400);
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
			increaseGold(1);
			root->soundManager(root->SOUND_COIN, 100, root->SOUND_TYPE_ONHIT);
		}
		if(id == DROP_POWER_BUFF && checkcol) {
			player.powerup = true;
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
    if (previousframetime.time_since_epoch().count() == 0) {
        previousframetime = std::chrono::high_resolution_clock::now();
    }

    currentframetime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> elapsedTime = currentframetime - previousframetime;

    // Get delta time in seconds.
    deltatime = elapsedTime.count();

    // Calculate fps.
    fps = 1.0f / deltatime;
}

float gCanvas::getFPS() {
	return fps;
}

void gCanvas::enablePowerUp(float &time) {
    // Color transition.
    int red = static_cast<int>((std::sin(time * 1.5f) + 1) * 127.5f + 127.5f);
    int green = static_cast<int>((std::sin(time * 1.5f + 2) + 1) * 127.5f + 127.5f);
    int blue = static_cast<int>((std::sin(time * 1.5f + 4) + 1) * 127.5f + 127.5f);

    red = clamp(red, 0, 255);
    green = clamp(green, 0, 255);
    blue = clamp(blue, 0, 255);

    powercolor = gColor(red, green, blue);
    renderer->setColor(red, green, blue);
}

void gCanvas::enablePlayerHit(float &time) {
    float transitionValue = (std::sin(time * 1.5f) + 1) * 0.5f;

    int red = static_cast<int>(200 - (transitionValue * 10));
    int green = static_cast<int>(200 - (transitionValue * 10));
    int blue = static_cast<int>(transitionValue * 25);

    red = clamp(red, 0, 200);
    green = clamp(green, 0, 200);
    blue = clamp(blue, 0, 255);

    powercolor = gColor(red, green, blue);
    renderer->setColor(red, green, blue);
}

int gCanvas::clamp(int value, int min, int max) {
    return (value < min) ? min : (value > max) ? max : value;
}

int gCanvas::getRandomDrop() {
	int randomvalue = rand() % 100;
	if(randomvalue < 90) return DROP_GOLD;
	else return DROP_POWER_BUFF;
}

void gCanvas::setupMarket() {
	marketpanelimage.loadImage("gui/marketpanel.png");
	marketslotimage.loadImage("gui/marketslot2.png");
	marketclosebuttonimage.loadImage("gui/closebutton.png");

	marketpanel.w = marketpanelimage.getWidth() * 1.5f;
	marketpanel.h = marketpanelimage.getHeight() * 1.5f;
	marketpanel.x = (getWidth() - marketpanel.w) / 2;
	marketpanel.y = (getHeight() - marketpanel.h) / 2;

	marketfont.loadFont("action_man.ttf", 18);
	marketcostfont.loadFont("action_man.ttf", 18);

	markettexts[0] = "Health " + gToStr(player.maxhealth) + " -> " + gToStr(player.maxhealth * 1.15f);
	markettexts[1] = "Damage " + gToStr(player.damage) + " -> " + gToStr(player.damage * 1.15f);
	markettexts[2] = "Attack Speed " + gToStr(player.cooldownholder) + " -> " + gToStr(player.cooldownholder / 1.15f);
	markettexts[3] = "Gold Multiplier " + gToStr(player.goldmultiplier) + " -> " + gToStr(player.goldmultiplier * 1.15f);
	markettexts[4] = "Buff Multiplier " + gToStr(player.buffmultiplier) + " -> " + gToStr(player.buffmultiplier * 1.15f);

	for(int i = 0; i < MARKET_SLOTS; i++) {
		// Slot position.
		marketslot[i].w = marketslotimage.getWidth() * 1.25f;
		marketslot[i].h = marketslotimage.getHeight() / 1.5f;
		marketslot[i].x = marketpanel.x + ((marketpanel.w - marketslot[i].w) / 2);
		marketslot[i].y = marketpanel.y + ((marketpanel.h - marketslot[i].h) / 4) + (marketslot[i].h * i) + (10 * i);

		// Text position.
		markettext[i].w = marketfont.getStringWidth(markettexts[i]);
		markettext[i].h = marketfont.getStringHeight(markettexts[i]);
		markettext[i].x = marketslot[i].x + (marketslot[i].w / 14);
		markettext[i].y = marketslot[i].y + ((marketslot[i].h - markettext[i].h) / 2) + markettext[i].h;

		// Button position.
		marketbutton[i].w = marketslot[i].w / 4;
		marketbutton[i].h = marketslot[i].h / 2.5f;
		marketbutton[i].x = marketslot[i].x + (marketslot[i].w - marketbutton[i].w) - (marketbutton[i].w / 4);
		marketbutton[i].y = marketslot[i].y + ((marketslot[i].h - marketbutton[i].h) / 2);

		// Button pressed.
		marketbutton[i].pressed = false;
	}

	marketcost[MARKET_HEALTH] = 10 * gamedatas.healthlevel;
	marketcost[MARKET_DAMAGE] = 10 * gamedatas.damagelevel;
	marketcost[MARKET_ATTACK_SPEED] = 10 * gamedatas.attackspeedlevel;
	marketcost[MARKET_GOLD_MULTIPLIER] = 10 * gamedatas.goldmultiplierlevel;
	marketcost[MARKET_BUFF_MULTIPLIER] = 10 * gamedatas.buffmultiplierlevel;

	marketclosebutton.w = 100;
	marketclosebutton.h = 100;
	marketclosebutton.x = marketpanel.x + ((marketpanel.w - marketclosebutton.w) / 2);
	marketclosebutton.y = marketpanel.y + ((marketpanel.h - marketclosebutton.h));
	marketclosebutton.centerx = marketclosebutton.x + marketclosebutton.w / 2;
	marketclosebutton.centery = marketclosebutton.y + marketclosebutton.h / 2;
	marketclosebutton.radius = marketclosebutton.w / 2;
	marketclosebutton.pressed = false;
}

void gCanvas::drawMarket() {
	marketpanelimage.draw(marketpanel.x, marketpanel.y, marketpanel.w, marketpanel.h);
	for(int i = 0; i < MARKET_SLOTS; i++) {
		marketslotimage.draw(marketslot[i].x, marketslot[i].y, marketslot[i].w, marketslot[i].h);
		marketfont.drawText(markettexts[i], markettext[i].x, markettext[i].y);
		if(marketbutton[i].pressed) renderer->setColor(255, 0, 0);
		marketslotimage.draw(marketbutton[i].x, marketbutton[i].y, marketbutton[i].w, marketbutton[i].h);
		marketcostfont.drawText(gToStr(marketcost[i]),
				marketbutton[i].x + ((marketbutton[i].w - marketcostfont.getStringWidth(gToStr(marketcost[i]))) / 2),
				marketbutton[i].y + ((marketbutton[i].h - marketcostfont.getStringHeight(gToStr(marketcost[i])))));
		if(marketbutton[i].pressed) renderer->setColor(255, 255, 255);
	}


	marketclosebuttonimage.draw(marketclosebutton.x, marketclosebutton.y, marketclosebutton.w, marketclosebutton.h);
}

void gCanvas::marketBuy(int slot, float &gold) {
	if(gold < marketcost[slot]) {
		changeGameState(GAMESTATE_WARNING);
		return;
	}
	decreaseGold(marketcost[slot]);
	marketcost[slot] = static_cast<int>(marketcost[slot] * 1.15f);
	float temp = player.maxhealth;

	switch(slot) {
		case MARKET_HEALTH:
			player.maxhealth *= multiplier.healthmultiplier;
			player.health += player.maxhealth - temp;
			markettexts[0] = "Health " + gToStr(player.maxhealth) + " -> " + gToStr(player.maxhealth * multiplier.healthmultiplier);
			break;
		case MARKET_DAMAGE:
			player.damage *= multiplier.damagemultiplier;
			markettexts[1] = "Damage " + gToStr(player.damage) + " -> " + gToStr(player.damage * multiplier.damagemultiplier);
			break;
		case MARKET_ATTACK_SPEED:
			player.speed /= multiplier.attackspeedmultiplier;
			markettexts[2] = "Attack Speed " + gToStr(player.cooldownholder) + " -> " + gToStr(player.cooldownholder / multiplier.attackspeedmultiplier);
			break;
		case MARKET_GOLD_MULTIPLIER:
			player.goldmultiplier *= multiplier.goldmultipliermultiplier;
			markettexts[3] = "Gold Multiplier " + gToStr(player.goldmultiplier) + " -> " + gToStr(player.goldmultiplier * multiplier.goldmultipliermultiplier);
			break;
		case MARKET_BUFF_MULTIPLIER:
			player.buffmultiplier *= multiplier.buffmultipliermultiplier;
			markettexts[4] = "Buff Multiplier " + gToStr(player.buffmultiplier) + " -> " + gToStr(player.buffmultiplier * multiplier.buffmultipliermultiplier);
			break;
	}
}

void gCanvas::setupWarning() {
    warningimage.loadImage("gui/uyarimarket.png");

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

    xcenter[WARNING_NO] = warningbutton[WARNING_NO].x + warningbutton[0].w / 2;
    ycenter[WARNING_NO] = warningbutton[WARNING_NO].y + warningbutton[0].h / 2;
    radius[WARNING_NO] = warningbutton[WARNING_NO].w / 1.6f;

    xcenter[WARNING_YES] = warningbutton[WARNING_YES].x + warningbutton[1].w / 2;
    ycenter[WARNING_YES] = warningbutton[WARNING_YES].y + warningbutton[1].h / 2;
    radius[WARNING_YES] = warningbutton[WARNING_YES].w / 1.6f;

    warningbutton[WARNING_NO].pressed = false;
    warningbutton[WARNING_YES].pressed = false;

    warningfont.loadFont("action_man.ttf", 18);
}



void gCanvas::drawWarning() {
    if(gamestate == GAMESTATE_WARNING) warningimage.draw(warning.x, warning.y, warning.w, warning.h);
    if(gamestate == GAMESTATE_EXIT) warningimage.draw(warning.x, warning.y, warning.w, warning.h); // deðiþtir.

//    for(int i = 0; i < 2; i++) {
//    	gDrawCircle(xcenter[i], ycenter[i], radius[i], true, 100);
//    }
}

void gCanvas::changeGameState(int gamestate) {
	lastgamestate = this->gamestate;
	this->gamestate = gamestate;
}

void gCanvas::setupEndPanel() {
	for(int i = 0; i < END_GAME_PANEL_COUNT; i++) {
		endgamepanelimage[i].loadImage("gui/oyunsonu" + gToStr(i) + ".png");
	}
	endgamepanel.w = endgamepanelimage[0].getWidth() * 1.5f;
	endgamepanel.h = endgamepanelimage[0].getHeight() * 1.5f;
	endgamepanel.x = (getWidth() - endgamepanel.w) / 2;
	endgamepanel.y = (getHeight() - endgamepanel.h) / 2;

	endgamebutton[END_GAME_BUTTON_MENU].w = 101;
	endgamebutton[END_GAME_BUTTON_MENU].h = 101;
	endgamebutton[END_GAME_BUTTON_MENU].x = endgamepanel.x + ((endgamepanel.w - endgamebutton[END_GAME_BUTTON_MENU].w) / 2) + endgamebutton[END_GAME_BUTTON_MENU].w;
	endgamebutton[END_GAME_BUTTON_MENU].y = endgamepanel.y + ((endgamepanel.h - endgamebutton[END_GAME_BUTTON_MENU].h));

	endgamebutton[END_GAME_BUTTON_MENU].centerx = endgamebutton[END_GAME_BUTTON_MENU].x + endgamebutton[END_GAME_BUTTON_MENU].w / 2;
	endgamebutton[END_GAME_BUTTON_MENU].centery = endgamebutton[END_GAME_BUTTON_MENU].y + endgamebutton[END_GAME_BUTTON_MENU].h / 2;
	endgamebutton[END_GAME_BUTTON_MENU].radius = endgamebutton[END_GAME_BUTTON_MENU].w / 2;

	endgamebutton[END_GAME_BUTTON_RESTART].w = 101;
	endgamebutton[END_GAME_BUTTON_RESTART].h = 101;
	endgamebutton[END_GAME_BUTTON_RESTART].x = endgamepanel.x + ((endgamepanel.w - endgamebutton[END_GAME_BUTTON_RESTART].w) / 2) - (endgamebutton[END_GAME_BUTTON_RESTART].w / 1.25f);
	endgamebutton[END_GAME_BUTTON_RESTART].y = endgamepanel.y + ((endgamepanel.h - endgamebutton[END_GAME_BUTTON_RESTART].h));

	endgamebutton[END_GAME_BUTTON_RESTART].centerx = endgamebutton[END_GAME_BUTTON_RESTART].x + endgamebutton[END_GAME_BUTTON_RESTART].w / 2;
	endgamebutton[END_GAME_BUTTON_RESTART].centery = endgamebutton[END_GAME_BUTTON_RESTART].y + endgamebutton[END_GAME_BUTTON_RESTART].h / 2;
	endgamebutton[END_GAME_BUTTON_RESTART].radius = endgamebutton[END_GAME_BUTTON_RESTART].w / 2;

	endgamebutton[END_GAME_BUTTON_MENU].pressed = false;
	endgamebutton[END_GAME_BUTTON_RESTART].pressed = false;

	// List panel.
	endgamelistpanel.w = (endgamepanel.w / 1.3f);
	endgamelistpanel.h = endgamepanel.h / 4;
	endgamelistpanel.x = endgamepanel.x + ((endgamepanel.w - endgamelistpanel.w) / 2);
	endgamelistpanel.y = endgamepanel.y + (endgamepanel.h / 2);

	// Load font.
	endgamefont.loadFont("action_man.ttf", 24);

	// Get database values.
	root->getTopFiveScores();
	std::vector<std::string> topfivescore = root->getScore();
	std::vector<std::string> topfivegold = root->getGold();

	// Edit texts.
	endgamelist[0] = "Top 5";

	for(int i = 0; i < END_GAME_LIST_NUMBER; i++) {
		if(topfivescore.size() <= i) {
			endgamelist[i + 1] = "0 / 0";
		}
		else {
			endgamelist[i + 1] = "Score: " + gToStr(topfivescore[i]) + " / Total Gold: " + gToStr(topfivegold[i]);
		}
	}
}

void gCanvas::drawEndPanel() {
	endgamepanelimage[endgamestar].draw(endgamepanel.x, endgamepanel.y, endgamepanel.w, endgamepanel.h);

	for(int i = 0; i < END_GAME_LIST_NUMBER; i++) {
		endgamefont.drawText(endgamelist[i], endgamelistpanel.x + ((endgamelistpanel.w - endgamefont.getStringWidth(endgamelist[i])) / 2), endgamelistpanel.y + (endgamefont.getStringHeight(endgamelist[i]) * (i * 1.75f)));
	}

//	gDrawRectangle(endgamelistpanel.x, endgamelistpanel.y, endgamelistpanel.w, endgamelistpanel.h);
//	gDrawRectangle(endgamebutton[END_GAME_BUTTON_RESTART].x, endgamebutton[0].y, endgamebutton[0].w, endgamebutton[0].h);
//	gDrawRectangle(endgamebutton[END_GAME_BUTTON_MENU].x, endgamebutton[END_GAME_BUTTON_MENU].y, endgamebutton[END_GAME_BUTTON_MENU].w, endgamebutton[END_GAME_BUTTON_MENU].h);
}

void gCanvas::increaseGold(int quantity) {
	player.gold += quantity;
	player.totalgold += quantity;
	goldtext = gToStr(player.gold);
}

void gCanvas::increaseScore(int quantity) {
	player.score += quantity;
	puantext = gToStr(player.score);
}

void gCanvas::decreaseGold(int quantity) {
	player.gold -= quantity;
	goldtext = gToStr(player.gold);
}

void gCanvas::decreaseScore(int quantity) {
	player.score -= quantity;
	puantext = gToStr(player.score);
}

void gCanvas::calculateStar() {
	if(root->getPlayerDataNumber() > 0) {
		float avgscore = root->getTotalScore() / root->getPlayerDataNumber();
		float avggold = root->getTotalGold() / root->getPlayerDataNumber();

		float playerscore = player.score;
		float playergold = player.totalgold;

		float result = (playerscore / avgscore) + (playergold / avggold);

		if(result < 0.5f) endgamestar = 0;
		else if(result >= 0.5f && result < 1) endgamestar = 1;
		else if(result >= 1 && result < 1.5f) endgamestar = 2;
		else if(result >= 1.5f) endgamestar = 3;

		std::string updatestatement = "UPDATE Players SET isdead = 1 WHERE id = " + gToStr(gamedatas.id) + ";";
		root->insertDatabase(updatestatement);
	}
	else {
		if(player.score < 10) endgamestar = 0;
		else if(player.score >= 10 && player.score < 20) endgamestar = 2;
		else if(player.score >= 20) endgamestar = 3;
	}
}

std::string gCanvas::createInsertStatement(const Player &player) {
    return "INSERT INTO Players (healthlevel, damagelevel, attackspeedlevel, goldmultiplierlevel, buffmultiplierlevel, score, gold, enemylevel, level, isdead) VALUES ("
        + std::to_string(gamedatas.healthlevel) + ", "
        + std::to_string(gamedatas.damagelevel) + ", "
        + std::to_string(gamedatas.attackspeedlevel) + ", "
        + std::to_string(gamedatas.goldmultiplierlevel) + ", "
        + std::to_string(gamedatas.buffmultiplierlevel) + ", "
        + std::to_string(player.score) + ", "
        + std::to_string(player.totalgold) + ", "
        + std::to_string(currentenemylevel) + ", "
        + std::to_string(player.level) + ", "
        + (player.isdead ? "1" : "0") + ");";
}

void gCanvas::loadGame(std::vector<int> data) {
	multiplier.healthmultiplier = 1.15f;
	multiplier.goldmultipliermultiplier = 1.15f;
	multiplier.damagemultiplier = 1.15f;
	multiplier.buffmultipliermultiplier = 1.15f;
	multiplier.attackspeedmultiplier = 1.15f;

	if(saveddata.size() > 0) {
		gamedatas.id = data[0];
		gamedatas.healthlevel = data[1];
		gamedatas.damagelevel = data[2];
		gamedatas.attackspeedlevel = data[3];
		gamedatas.goldmultiplierlevel = data[4];
		gamedatas.buffmultiplierlevel = data[5];
		gamedatas.score = data[6];
		gamedatas.gold = data[7];
		gamedatas.isdead = data[8];
		gamedatas.enemylevel = data[9];
		gamedatas.level = data[10];

		for(int i = 0; i < gamedatas.healthlevel; i++) {
			gamedatas.health = 1000 * multiplier.healthmultiplier;
		}
		for(int i = 0; i < gamedatas.damagelevel; i++) {
			gamedatas.damage = 120 * multiplier.damagemultiplier;
		}
		increaseScore(gamedatas.score);
		increaseGold(gamedatas.gold);
	}
	else {
		gamedatas.id = 0;
		gamedatas.health = 1000;
		gamedatas.damage = 120;
		gamedatas.healthlevel = gamedatas.damagelevel = gamedatas.attackspeedlevel = gamedatas.goldmultiplierlevel = gamedatas.buffmultiplierlevel = gamedatas.enemylevel = gamedatas.level = 1;
		gamedatas.score = gamedatas.gold = gamedatas.isdead = 0;
	}
}

void gCanvas::updateDifficultyMessage() {
    if (showdifficultyincreasemessage) {
        difficultymessageframes--;
        if (difficultymessageframes <= 0) {
            showdifficultyincreasemessage = false;
        }
    }
}

void gCanvas::drawDifficultyMessage() {
    if (showdifficultyincreasemessage) {
        std::string difficultyMessage = "More difficult enemies are coming!";
        float textWidth = panelfont.getStringWidth(difficultyMessage);
        float textX = (getWidth() - textWidth) / 2;
        float textY = getHeight() / 2;
        panelfont.drawText(difficultyMessage, textX, textY);
    }
}
void gCanvas::activateSpecialAbility() {
    special.radius = player.w / 2;
    special.maxradius = 2000;
    special.centerx = player.x + (player.w / 2);
    special.centery = player.y + (player.h / 2);
    special.active = true;
    specials.push_back(special);
}

void gCanvas::updateSpecialAbility() {
    for (int i = 0; i < specials.size(); i++) {
        SpecialAbility& special = specials[i];

        if (special.radius < special.maxradius) {
            special.radius += 10;
        }

        for (int j = 0; j < activebullets.size(); j++) {
            Bullet& bullet = activebullets[j];
            if (bullet.owner == OWNER_ENEMY) {
                float distance = sqrt(pow(bullet.x - special.centerx, 2) + pow(bullet.y - special.centery, 2));
                if(fabs(distance - special.radius) <= 10.0f) {
                    activebullets.erase(activebullets.begin() + j);
                    j--;
                }
            }

        }

        for (int k = 0; k < enemies.size(); k++) {
            Enemy& enemy = enemies[k];
            float distance = sqrt(pow(enemy.x - special.centerx, 2) + pow(enemy.y - special.centery, 2));

            if(fabs(distance - special.radius) <= 10.0f && !enemy.hitBySpecial) {
                enemy.health -= player.damage;
                enemy.hitBySpecial = true;
            }
        }

        if(special.radius >= special.maxradius) {
            specials.erase(specials.begin() + i);
            i--;
        }
    }
}

void gCanvas::drawSpecialAbility() {
    for (auto& special : specials) {
        gDrawCircle(special.centerx, special.centery, special.radius, false, 255);
    }
}
