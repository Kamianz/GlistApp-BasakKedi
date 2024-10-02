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

	static const int BACKGROUNDLEVELLIMIT = 5;
	static const int MAXENEMYTYPENUM = 6;
	static const int PAUSEPANELMAXBUTTONNUM = 3;
	static const int UFO_RED = 0, UFO_BLACK = 1, UFO_GREEN = 2, SUIT_BLACK = 3, SUIT_ORANGE = 4, SUIT_PURPLE = 5;
	static const int PLAYER = 0, SUIT_ALIEN = 1, UFO_ALIEN = 2;
	static const int CITY = 0, SKY = 1;
	static const int BUTTON_COUNT = 5;
	static const int BUTTON_LEFT = 0, BUTTON_RIGHT = 1, BUTTON_UP = 2, BUTTON_DOWN = 3, BUTTON_FIRE = 4;
	static const int BUTTON_PLAY = 0, BUTTON_RESTART = 1, BUTTON_MENU = 2;
	static const int GOLD_FRAME_COUNT = 10, PLAYER_FRAME_COUNT = 5, POWER_BUFF_FRAME_COUNT = 4;
	static const int PLAYER_IDLE = 0, PLAYER_HURT = 1;
	static const int DROP_GOLD = 0, DROP_POWER_BUFF = 1;
	static const int BACKGROUND_COUNT = 2;
	static const int EXPLOSION_COLUMN = 4, EXPLOSION_ROW = 4;
	static const int OWNER_PLAYER = 0, OWNER_ENEMY = 1;
	static const int COL_PB = 0, COL_EB = 1, COL_PE = 2, COL_BB = 3, COL_D = 4;
	static const int GAMESTATE_PAUSE = 0, GAMESTATE_PLAY = 1, GAMESTATE_WAITFORNEXTLEVEL = 2, GAMESTATE_MARKET = 3, GAMESTATE_WARNING = 4, GAMESTATE_EXIT = 5, GAMESTATE_END_GAME = 6;
    static const int MARKET_HEALTH = 0, MARKET_DAMAGE = 1, MARKET_ATTACK_SPEED = 2, MARKET_GOLD_MULTIPLIER = 3, MARKET_BUFF_MULTIPLIER = 4;
	static const int BUTTON_UNCLICK = 0, BUTTON_CLICK = 1;
    static const int WARNING_NOT_SELECTED = -1, WARNING_NO = 0, WARNING_YES = 1;
    static const int POWER_UP_TIME = 3;
    static const int TRANSITION_STEPS = 255;
    static const int MARKET_SLOTS = 5;
    static const int END_GAME_PANEL_COUNT = 4, END_GAME_BUTTON_RESTART = 0, END_GAME_BUTTON_MENU = 1;
    static const int END_GAME_LIST_NUMBER = 6;

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

		int level;
		float gold, totalgold, score;
		float goldmultiplier, buffmultiplier;
		int energy, maxenergy;
		bool isdead;
	};

	struct Enemy {
		float x, y, w, h;
		float bulletx, bullety;
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
	    int id; // Drop türü
	    int x, y; // X ve Y koordinatlarý
	    int w, h; // Geniþlik ve yükseklik
	    float speed; // Hýz (float kullanmak daha pürüzsüz hareket saðlar)
	    int animcounter, animframeno; // Animasyon sayacý ve çerçevesi
	    bool iscollide; // Çarpýþma durumu
	    float offsetY; // Dikey dalgalanma için ek offset
	};

	struct Background {
		float x, y, w, h;
	};

	struct Buttons {
		int x, y, w, h;
		bool pressed;
		int centerx, centery, radius;
	};

	struct Panel {
		int x, y, w, h;
	};

	struct SpecialAbility {
	    float radius;
	    float maxradius;
	    float centerx;
	    float centery;
	    bool active;
	    float r, g, b, alpha;
	};

	struct Boss{
		int x, y, w, h;
	};

	struct Datas {
		int id, health, damage, level, healthlevel, damagelevel, attackspeedlevel, goldmultiplierlevel, buffmultiplierlevel, score, gold, isdead, enemylevel;
	};

	struct Multiplier {
		float healthmultiplier, damagemultiplier, attackspeedmultiplier, goldmultipliermultiplier, buffmultipliermultiplier;
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
    void setupEndPanel();

	void updateBackground();
	void updatePlayer();
    void updateExplosion();
    void updateBullet();
    void updateEnemy();
	void updateDrops();
	void updateDifficultyMessage();
	void updateSpecialAbility();
	void updateGoldAnimation(Drop &drop);

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
    void drawEndPanel();
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
	void marketBuy(int slot, float &gold);
    void changeGameState(int gamestate);
    void activateSpecialAbility();

	bool checkCollision(int x, int y, int w, int h, int type, int power = 0, int id = 0);
	int getRandomDrop();
	int clamp(int value, int min, int max);

	void calculateFPS();
	float getFPS();
	void increaseGold(int quantity);
	void increaseScore(int quantity);
	void decreaseGold(int quantity);
	void decreaseScore(int quantity);
	void toggleSound();
	void toggleMusic();

	void calculateStar();
	std::string createInsertStatement(const Player& player);
	void loadGame(std::vector<int> data);

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
	gImage enemyimage[MAXENEMYTYPENUM];
	gImage healthbarimage;
	gImage healthfillimage;
	gImage energyfillimage;
	gImage pausepanelimage;
	gImage settingsbutton[2];
    gImage warningimage;
    gImage marketpanelimage;
    gImage marketslotimage;
    gImage marketclosebuttonimage;
    gImage endgamepanelimage[END_GAME_PANEL_COUNT];

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
    Panel endgamepanel;
    Panel endgamelistpanel;
    Buttons marketbutton[MARKET_SLOTS];
    Buttons marketclosebutton;
	Buttons gamebutton[BUTTON_COUNT];
    Buttons warningbutton[2];
    SpecialAbility special;
    Buttons endgamebutton[2];
    Datas gamedatas;
    Multiplier multiplier;

	std::vector<Drop> activedrops;
	std::vector<Bullet> activebullets;
	std::vector<SpecialAbility> specials;
	std::vector<int> saveddata;

    bool powerup;
    int testcounter;
    float poweruptimes, poweruptimer;
	float backgroundspeed;
	int mapleft, mapright, maptop, mapbottom;
	int buttongap;
	int gamestate;
	int pausepanelx, pausepanely, pausepanelw, pausepanelh;
	int settingsbuttonstate;
	int settingsbuttonx, settingsbuttony[2], settingsbuttonw, settingsbuttonh[2];
	int soundButtonX, soundButtonY, soundButtonW, soundButtonH;
	int musicButtonX, musicButtonY, musicButtonW, musicButtonH;
	int pausepanelbuttonx[PAUSEPANELMAXBUTTONNUM], pausepanelbuttony, pausepanelbuttonw, pausepanelbuttonh;
    int marketcost[MARKET_SLOTS];
    int xcenter[2], ycenter[2], radius[2];
    int lastgamestate;
    int endgamestar;
    int onhitsoundnum = 2;

	gFont panelfont;
    gFont marketfont;
    gFont marketcostfont;
    gFont warningfont;
    gFont endgamefont;

	std::string goldtext, puantext;
    std::string healthText = "100%";
    std::string energyText = "0%";
    std::string markettexts[MARKET_SLOTS];
    std::string endgamelist[END_GAME_LIST_NUMBER];

    gColor powercolor;

	// Fps things
    std::chrono::high_resolution_clock::time_point previousframetime;
    std::chrono::high_resolution_clock::time_point currentframetime;
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
    float enemyspeeds[MAXENEMYTYPENUM];
    float enemydamages[MAXENEMYTYPENUM];
    float enemyhealths[MAXENEMYTYPENUM];
    float enemycooldown[MAXENEMYTYPENUM];
    float enemycooldowntimer[MAXENEMYTYPENUM];
    int currentenemylevel;
    int enemiesToSpawn;
    int remainingEnemies;

    int spawnctr = 0;
    int spawnctrlimit = 120;
    bool shownextlevelmessage;
    int waittimer;
    float mapyvelocity;

	bool showdifficultyincreasemessage;
	int difficultymessageframes;
};

#endif /* GCANVAS_H_ */
