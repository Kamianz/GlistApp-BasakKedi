 /*
 * gCanvas.h
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan, Mehmet Furkan Utlu, Onur Imre
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "menuCanvas.h"
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

	static const int backgroundlevellimit = 5;
	static const int maxenemytypenum = 6;
	static const int UFO_RED = 0, UFO_BLACK = 1, UFO_GREEN = 2, SUIT_BLACK = 3, SUIT_ORANGE = 4, SUIT_PURPLE = 5;
	static const int PLAYER = 0, SUIT_ALIEN = 1, UFO_ALIEN = 2;
	static const int CITY = 0, SKY = 1;
	static const int BUTTON_COUNT = 5;
	static const int BUTTON_LEFT = 0, BUTTON_RIGHT = 1, BUTTON_UP = 2, BUTTON_DOWN = 3, BUTTON_FIRE = 4;
	static const int GOLD_FRAME_COUNT = 10, PLAYER_FRAME_COUNT = 5, POWER_BUFF_FRAME_COUNT = 4;
	static const int PLAYER_IDLE = 0, PLAYER_HURT = 1;
	static const int DROP_GOLD = 0, DROP_POWER_BUFF = 1;
	static const int BACKGROUND_COUNT = 2;
	static const int EXPLOSION_COLUMN = 4, EXPLOSION_ROW = 4;
	static const int OWNER_PLAYER = 0, OWNER_ENEMY = 1;
	static const int COL_PB = 0, COL_EB = 1, COL_PE = 2, COL_BB = 3, COL_D = 4;
	static const int GAMESTATE_PAUSE = 0, GAMESTATE_PLAY = 1, GAMESTATE_WAITFORNEXTLEVEL = 2, GAMESTATE_MARKET = 3, GAMESTATE_WARNING = 4, GAMESTATE_EXIT = 5;
    static const int MARKET_HEALTH = 0, MARKET_DAMAGE = 1, MARKET_ATTACK_SPEED = 2, MARKET_GOLD_MULTIPLIER = 3, MARKET_BUFF_MULTIPLIER = 4;
	static const int BUTTON_UNCLICK = 0, BUTTON_CLICK = 1;
    static const int WARNING_NOT_SELECTED = -1, WARNING_NO = 0, WARNING_YES = 1;
    static const int POWER_UP_TIME = 3;
    static const int TRANSITION_STEPS = 255;
    static const int MARKET_SLOTS = 5;

	struct Player {
		float x, y, w, h;
		float speed;
		int health, maxhealth;
		float damage;
		bool upkey, downkey, leftkey, rightkey;
		int animcounter, animframeno;
		bool ishit;
		bool canshoot;
		bool deadanimplayed;
		bool powerup;
		float cooldown, cooldowntimer, cooldownholder;
		float buffcooldown, buffcooldowntimer, buffcooldownholder;
		float hurtcooldown, hurtcooldowntimer, hurtcooldownholder;
		int maxenergy;
		int level;
		float gold, score;
		float goldmultiplier, buffmultiplier;
		int energy;
	};

	struct Enemy {
		float x, y, w, h;
		float speed;
		float health;
		float damage;
		bool isalive;
		bool canshoot;
		float cooldown, cooldowntimer, cooldownholder;
		int type;
		int level;
		bool hitBySpecial;
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
	struct SpecialAbility {
	    float radius;
	    float maxRadius;
	    float centerX;
	    float centerY;
	    bool active;
	    float r, g, b, alpha;
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
    void setupWarning();
    void setupMarket();

	void updateBackground();
	void updatePlayer();
    void updateExplosion();
    void updateBullet();
    void updateEnemy();
	void updateDrops();
	void updateDifficultyMessage();
	void updateSpecialAbility();

	void drawBackground();
	void drawGameButtons();
	void drawPlayer();
    void drawExplosion();
    void drawPanel();
    void drawBullet();
    void drawEnemy();
	void drawDrops();
    void drawPausePanel();
    void drawMarket();
    void drawWarning();
    void drawDifficultyMessage();
    void drawSpecialAbility();

	void generateDrop(int x, int y, int w, int h, int id);
	void generateExplosion(int explosionx, int explosiony, int explosionw, int explosionh);
	void generateBullet(int x, int y, int w, int h, int owner, int type, int damage);
	void generateEnemy();

	void animator(int &animcounter, int &animframeno, int startframe, int framecount, float targetfps);
	bool cooldown(float &time, float &timer, float savedtime = 0);
	void spawnEnemy(int type);
    void enablePowerUp(float &time);
    void enablePlayerHit(float &time);
	void marketBuy(int slot, int money);
    void changeGameState(int gamestate);
    void activateSpecialAbility();

	bool checkCollision(int x, int y, int w, int h, int type, int power = 0, int id = 0);
	int getRandomDrop();
	int clamp(int value, int min, int max);

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
    gImage warningimage;
    gImage marketpanelimage;
    gImage marketslotimage;

	Background background[BACKGROUND_COUNT];
	Player player;
	Panel puanpanel, goldpanel, text[2];
	Bullet playerbullet;
	Panel healthbar;
	Panel healthfill;
	Panel healthtext;
	Panel energyfill;
	Panel energytext;
	Panel energybar;
    Panel marketpanel;
    Panel marketslot[MARKET_SLOTS];
    Panel markettext[MARKET_SLOTS];
    Panel warning;
    Buttons marketbutton[MARKET_SLOTS];
    Buttons marketclosebutton;
	Buttons gamebutton[BUTTON_COUNT];
    Buttons warningbutton[2];
    SpecialAbility special;

	std::vector<Drop> activedrops;
	std::vector<Bullet> activebullets;
	std::vector<SpecialAbility> specials;

    bool powerup;
    float poweruptimes, poweruptimer;
	float backgroundspeed;
	int mapleft, mapright, maptop, mapbottom;
	int buttongap;
	int gamestate;
	int pausepanelx, pausepanely, pausepanelw, pausepanelh;
	int settingsbuttonstate;
	int settingsbuttonx, settingsbuttony[2], settingsbuttonw, settingsbuttonh[2];
    int marketcost[MARKET_SLOTS];
    int xcenter[2], ycenter[2], radius[2];
    int lastgamestate;

	gFont panelfont;
    gFont marketfont;
    gFont marketcostfont;
    gFont warningfont;
	std::string goldtext, puantext;
    std::string healthText = "100%";
    std::string energyText = "0%";
    std::string markettexts[MARKET_SLOTS];

    gColor powercolor;

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

	bool showDifficultyIncreaseMessage;
	int difficultyMessageFrames;
};

#endif /* GCANVAS_H_ */
