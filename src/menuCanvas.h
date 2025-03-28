 /*
 * gCanvas.h
 *
 *  Created on: September 18, 2024
 *      Author: Kadir Semih Arslan
 */

#ifndef MENU_CANVAS_H_
#define MENU_CANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "gCanvas.h"


class menuCanvas : public gBaseCanvas {
public:
	menuCanvas(gApp* root);
	virtual ~menuCanvas();

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

	int gamestate;
	static const int GAMESTATE_START = 0, GAMESTATE_WARNING = 1;
    static const int WARNING_NOT_SELECTED = -1, WARNING_NO = 0, WARNING_YES = 1;
	const int NUM_BACKGROUNDS = 2;

	struct ImageObject {
		int x, y, w, h;
	};

	struct ButtonObject {
		int x, y, w, h;
		int pressed;
		int centerx, centery, radius;
	};

	struct Panel {
		int x, y, w, h;
	};

	ImageObject background[2];
	ImageObject logo;
	ImageObject player;
    Panel warning;
	ButtonObject buttongroup[1];
	ButtonObject warningbutton[2];

	gImage backgroundimage;
	gImage logoimage;
	gImage playerimage;
	gImage playbutton;
	gImage warningimage;

    gFont warningfont;
	std::vector<int> saveddata;

	float parallaxspeed;
	bool hasSave;

	void setupMenu();
	void setupButton();
    void setupContinuePanel();

    void updateMenu();
	void updateButton();

	void drawMenu();
	void drawButton();
    void drawContinuePanel();

    void startGame();
};

#endif /* MENU_CANVAS_H_ */
