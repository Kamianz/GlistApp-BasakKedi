/*
 * gApp.h
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan
 */

#ifndef GAPP_H_
#define GAPP_H_

#include "gBaseApp.h"
#include "gFmodSound.h"
#include "gDatabase.h"
#include <fstream>


class gApp : public gBaseApp {
public:
	gApp();
	gApp(int argc, char **argv);
	~gApp();

	void setup();
	void update();

	// Sound const values.
	static const int SOUND_TYPE_ONHIT = 0;
	static const int SOUND_TYPE_STARTING = 1;
	static const int SOUND_TYPE_ENDING = 2;

	static const int SOUND_MENU = 0;
	static const int SOUND_GAME = 1;
	static const int SOUND_BUTTON = 2;
	static const int SOUND_BUTTON2 = 3;
	static const int SOUND_COIN = 4;
	static const int SOUND_ENEMY_FIRE = 5;
	static const int SOUND_CAT_FIRE = 6;
	static const int SOUND_LOSE = 7;
	static const int SOUND_POWERUP = 8;
	static const int SOUND_POWERDOWN = 9;
	static const int SOUND_EXPLOSION = 10;

	static const int SOUND_COUNT = 11;
	static const int MAX_ACTIVE_SOUNDS = 8;

	void soundManager(int sound, int musicvalue, int type);

	void toggleSound();
	void toggleMusic();
	void onSceneChange();

	// Database.
	void insertDatabase(std::string insert);
	void setupDatabase();

	bool saveControl();
	void resetSave();
	void endSave();
	void saveDatas(std::vector<int> datas);
	std::vector<std::pair<int, std::pair<int, int>>> getTopFiveScores();
	std::vector<int> getSavedData();
	void updateLowestScores(const std::pair<int, int>& newScoreGold);
	float getTotalScore();
	float getTotalGold();
	float getPlayerDataNumber();

private:
	// Sound control.
	void setupSounds();
    void setVolumeAndPlay(int sound, float volume);
    void stopSound(int sound);
    void handleSound(int sound, float volume);
    void replaceSound(int newSound);
    bool fileExists(const std::string& filename);
    void closeAllSounds();
    void stopAllSounds();

    gFmodSound sounds[MAX_ACTIVE_SOUNDS];
    gFmodSound soundt;
    std::vector<std::string> soundpaths;

    bool musicenabled;
    bool soundenabled;

	// Database control.

	gDatabase database;

	// Values
	int id, healthlevel, damagelevel, attackspeedlevel, goldmultiplierlevel, buffmultiplierlevel, score, gold, isdead, enemylevel, level;
	int numberofdata, totalscore, totalgold;
	std::vector<std::string> topfivescore;
	std::vector<std::string> topfivegold;
	std::vector<std::string> datas;
	std::vector<std::string> selectedData;
	std::vector<int> saveddatas;

	// New
	void setPipes(std::string row);
	int firstpipe, secondpipe, thirdpipe, fourthpipe, fifthpipe, sixthpipe, seventhpipe, eighthpipe, ninthpipe, tenthpipe, eleventhpipe;
};

#endif /* GAPP_H_ */
