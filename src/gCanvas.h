 /*
 * gCanvas.h
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan, Mehmet Furkan Utlu
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "vector"
#include "thread"

#include "iostream"
#include "chrono"

class gCanvas : public gBaseCanvas {
public:
	gCanvas(gApp* root);
	virtual ~gCanvas();

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void charPressed(unsigned int codepoint);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y);
	void mouseEntered();
	void mouseExited();
	void windowResized(int w, int h);

	void showNotify();
	void hideNotify();

private:
	gApp* root;

	static const int maxenemytypenum = 6;
	static const int UFO_RED = 0, UFO_BLACK = 1, UFO_GREEN = 2, SUIT_BLACK = 3, SUIT_ORANGE = 4, SUIT_PURPLE = 5;
	static const int PLAYER = 0, SUIT_ALIEN = 1, UFO_ALIEN = 2;
	static const int BUTTON_COUNT = 5;
	static const int BUTTON_LEFT = 0, BUTTON_RIGHT = 1, BUTTON_UP = 2, BUTTON_DOWN = 3, BUTTON_FIRE = 4;
	static const int GOLD_FRAME_COUNT = 10, PLAYER_FRAME_COUNT = 5, POWER_BUFF_FRAME_COUNT = 4;
	static const int PLAYER_IDLE = 0, PLAYER_HURT = 1;
	static const int DROP_GOLD = 0, DROP_POWER_BUFF = 1;
	static const int BACKGROUND_COUNT = 2;
	static const int EXPLOSION_COLUMN = 4, EXPLOSION_ROW = 4;
	static const int OWNER_PLAYER = 0, OWNER_ENEMY = 1;
	static const int COL_PB = 0, COL_EB = 1, COL_PE = 2, COL_BB = 3, COL_D = 4;
	static const int GAMESTATE_PAUSE = 0, GAMESTATE_PLAY = 1, GAMESTATE_WAITFORNEXTLEVEL = 2;
	static const int BUTTON_UNCLICK = 0, BUTTON_CLICK = 1;
	static const int CITY = 0, SKY = 1;

	struct Player {
		float x, y, w, h;
		float speed;
		int health;
		float damage;
		bool upkey, downkey, leftkey, rightkey;
		int animcounter, animframeno;
		bool ishit;
		bool canshoot;
		bool deadanimplayed;
		float cooldown, cooldowntimer;

		int level;
		int gold, score;
		int energy;
	};

	struct Enemy {
		float x, y, w, h;
		float speed;
		float health;
		float damage;
		bool isalive;
		bool canshoot;
		float cooldown, cooldowntimer;
		int type;
		int level;
	};

	struct Bullet {
		int id;
		int x, y, w, h;
		int damage, speed;
		int owner;
		bool ishit;
		int type;
	};

	struct Drop {
		int id;
		int x, y, w, h;
		int speed;
		int animcounter, animframeno;
		bool iscollide;
	};

	struct Background {
		float x, y, w, h;
	};

	struct Buttons {
		int x, y, w, h;
		bool pressed;
	};

	struct Panel {
		int x, y, w, h;
	};

	void setupGame();
	void setupBackground();
	void setupGameButtons();
	void setupPlayer();
	void setupExplosion();
	void setupPanel();
	void setupBullet();
	void setupEnemy();
	void setupDrops();
	void setupLevel();
	void setupPausePanel();

	void updateBackground();
	void updatePlayer();
    void updateExplosion();
    void updateBullet();
    void updateEnemy();
	void updateDrops();

	void drawBackground();
	void drawGameButtons();
	void drawPlayer();
    void drawExplosion();
    void drawPanel();
    void drawBullet();
    void drawEnemy();
	void drawDrops();
    void drawPausePanel();

	void generateDrop(int x, int y, int w, int h, int id);
	void generateExplosion(int explosionx, int explosiony, int explosionw, int explosionh);
	void generateBullet(int x, int y, int w, int h, int owner, int type, int damage);
	void generateEnemy();

	void animator(int &animcounter, int &animframeno, int startframe, int framecount, float targetfps);
	bool cooldown(float &time, float &timer);
	void spawnEnemy(int type);

	bool checkCollision(int x, int y, int w, int h, int type, int power = 0, int id = 0);

	void calculateFPS();
	float getFPS();

	gImage backgroundimage[2];
	gImage playerimg[PLAYER_FRAME_COUNT];
	gImage gamebuttonimage[BUTTON_COUNT];
	gImage goldimage[GOLD_FRAME_COUNT];
	gImage powerbuffimage[POWER_BUFF_FRAME_COUNT];
	gImage dropimage[2][10];
	gImage explosionImage;
	gImage puanpanelimage;
	gImage goldpanelimage;
	gImage bulletimage[3];
	gImage enemyimage[maxenemytypenum];
	gImage healthbarimage;
	gImage healthfillimage;
	gImage energyfillimage;
	gImage pausepanelimage;
	gImage settingsbutton[2];


	Background background[BACKGROUND_COUNT];
	Buttons gamebutton[BUTTON_COUNT];
	Player player;
	Panel puanpanel, goldpanel, text[2];
	Bullet playerbullet;
	Panel healthbar;
	Panel healthfill;
	Panel healthtext;
	Panel energyfill;
	Panel energytext;
	Panel energybar;

	std::vector<Drop> activedrops;
	std::vector<Bullet> activebullets;

	float backgroundspeed;
	int mapleft, mapright, maptop, mapbottom;
	int buttongap;
	int gamestate;
	int pausepanelx, pausepanely, pausepanelw, pausepanelh;
	int settingsbuttonstate;
	int settingsbuttonx, settingsbuttony[2], settingsbuttonw, settingsbuttonh[2];

	gFont panelfont;
	std::string goldtext, puantext;
    std::string healthText = "100%";
    std::string energyText = "0%";

	// Fps things
    std::chrono::high_resolution_clock::time_point previousFrameTime;
    std::chrono::high_resolution_clock::time_point currentFrameTime;
	float deltatime;
	float fps;
	float targetfps;

    // Explosion
    int explosionframew, explosionframeh, explosionmaxframe;

    std::vector<int> newexplosion;
    std::vector<std::vector<int>> activeExplosion;

    enum explosionEnum {
    	EX_X, EX_Y, EX_W, EX_H, EX_SX, EX_SY, EX_FRAMENO, EX_COUNTER
    };

    bool checkcol;

    // Enemy
    std::vector<Enemy> enemies;
    float enemyspeeds[maxenemytypenum];
    float enemydamages[maxenemytypenum];
    float enemyhealths[maxenemytypenum];
    float enemycooldown[maxenemytypenum];
    float enemycooldowntimer[maxenemytypenum];
    int currentenemylevel;
    int enemiesToSpawn;
    int remainingEnemies;

    int spawnctr = 0;
    int spawnctrlimit = 120;
    bool showNextLevelMessage;
    int waitTimer;
    float mapyvelocity;
};

#endif /* GCANVAS_H_ */
