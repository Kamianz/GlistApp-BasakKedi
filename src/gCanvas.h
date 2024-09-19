 /*
 * gCanvas.h
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "vector"
#include "thread"


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

	static const int BUTTON_COUNT = 4;
	static const int BUTTON_LEFT = 0, BUTTON_RIGHT = 1, BUTTON_UP = 2, BUTTON_DOWN = 3;
	static const int GOLD_FRAME_COUNT = 10, PLAYER_FRAME_COUNT = 5, POWER_FRAME_COUNT = 4;
	static const int PLAYER_IDLE = 0, PLAYER_HURT = 1;
	static const int BACKGROUND_COUNT = 2;
	static const int EXPLOSION_COLUMN = 4, EXPLOSION_ROW = 4;

	struct Buttons {
		int x, y, w, h;
		bool pressed;
	};

	struct Gold {
		int x, y, w, h;
		int animcounter, animframeno;
	};

	struct Player {
		float x, y, w, h;
		float speed;
		float health;
		bool upkey, downkey, leftkey, rightkey;
		int animcounter, animframeno;
		bool ishit;
		bool deadanimplayed;
	};

	struct Background {
		int x, y, w, h;
	};

	struct Panel {
		int x, y, w, h;
	};

	void setupBackground();
	void setupGameButtons();
	void setupGold();
	void setupPlayer();
	void setupExplosion();
	void setupPanel();

	void updateBackground();
	void updateGold();
	void updatePlayer();
    void updateExplosion();

	void drawBackground();
	void drawGameButtons();
	void drawGold();
	void drawPlayer();
    void drawExplosion();
    void drawPanel();

	void generateGold(int x, int y, int w, int h);
	void generateExplosion(int explosionx, int explosiony, int explosionw, int explosionh);

	void goldAnimator(Gold &gold, int maxanimframe);
	void playerAnimator(Player &player, int startanimframe, int maxanimframe, int animtype);

	gImage backgroundimage;
	gImage playerimg[PLAYER_FRAME_COUNT];
	gImage gamebuttonimage[BUTTON_COUNT];
	gImage goldimage[GOLD_FRAME_COUNT];
	gImage explosionImage;
	gImage puanpanelimage;
	gImage goldpanelimage;


	Background background[BACKGROUND_COUNT];
	Buttons gamebutton[BUTTON_COUNT];
	Player player;
	Panel puanpanel, goldpanel, text[2];

	std::vector<Gold> golds;

	int mapleft, mapright, maptop, mapbottom;
	int buttongap;

	gFont panelfont;
	std::string goldtext, puantext;

    //explosion
    int explosionframew, explosionframeh, explosionmaxframe;

    std::vector<int> newexplosion;
    std::vector<std::vector<int>> activeExplosion;

    enum explosionEnum {
    	EX_X, EX_Y, EX_W, EX_H, EX_SX, EX_SY, EX_FRAMENO, EX_COUNTER
    };
};

#endif /* GCANVAS_H_ */
