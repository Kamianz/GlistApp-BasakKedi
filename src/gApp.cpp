/*
 * gApp.cpp
 *
 *  Created on: September 17, 2024
 *      Author: Kadir Semih Arslan
 */

#include "gApp.h"
#include "gCanvas.h"
#include "menuCanvas.h"


gApp::gApp() {
}

gApp::gApp(int argc, char **argv) : gBaseApp(argc, argv) {
}

gApp::~gApp() {
	database.close();
	closeAllSounds();
}

void gApp::setup() {
	setupSounds();
	setupDatabase();

//	gCanvas* cnv = new gCanvas(this);
//	appmanager->setCurrentCanvas(cnv);

	menuCanvas* menu = new menuCanvas(this);
	appmanager->setCurrentCanvas(menu);
}

void gApp::update() {
}

bool gApp::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void gApp::setupSounds() {
    soundpaths.clear();
    soundpaths.push_back("menumuzigi.mp3");
    soundpaths.push_back("oyunmuzigi.mp3");
    soundpaths.push_back("buttonclick.wav");
    soundpaths.push_back("buttonclick2.wav");
    soundpaths.push_back("collectcoin.wav");
    soundpaths.push_back("dusmanates.wav");
    soundpaths.push_back("kediates.ogg");
    soundpaths.push_back("lose.wav");
    soundpaths.push_back("powerup.wav");
    soundpaths.push_back("powerdown.wav");
    soundpaths.push_back("se2.wav");

//    for(const auto& path : soundpaths) {
//        if (!fileexists(path)) {
//            gLogi("Error: Sound file not found: " + path);
//        }
//    }
}

void gApp::soundManager(int sound, int musicvalue, int type) {
    float volume = musicvalue / 100.0f;

    switch(type) {
        case SOUND_TYPE_STARTING:
            if(sound == SOUND_MENU || sound == SOUND_GAME) {
                setVolumeAndPlay(sound, volume);
            }
            break;

        case SOUND_TYPE_ENDING:
            if(sound == SOUND_MENU || sound == SOUND_GAME) {
                stopSound(sound);
            }
            break;

        case SOUND_TYPE_ONHIT:
        	if(sound < MAX_ACTIVE_SOUNDS) {
				handleSound(sound, volume);
			} else {
				replaceSound(sound);
			}
            break;

        default:
//            gLogi("Error");
            break;
    }
}

void gApp::setVolumeAndPlay(int sound, float volume) {
    if(sound < SOUND_COUNT) {
        if(!sounds[sound].isLoaded()) {
            sounds[sound].loadSound(soundpaths[sound]);
        }
        sounds[sound].setVolume(volume);
        sounds[sound].play();
    }
//  else {
//    gLogi("Error: Invalid sound index in setVolumeAndPlay: " + std::to_string(sound));
//  }
}

void gApp::stopSound(int sound) {
    std::vector<int> prioritizedsounds = {SOUND_GAME, SOUND_BUTTON, SOUND_CAT_FIRE, SOUND_ENEMY_FIRE};

    if(sound < SOUND_COUNT) {
        if(std::find(prioritizedsounds.begin(), prioritizedsounds.end(), sound) == prioritizedsounds.end()) {
            sounds[sound].stop();
        }
    }
//  else {
//      gLogi("Error: Invalid sound index in stopSound: " + std::to_string(sound));
//  }
}

void gApp::handleSound(int sound, float volume) {
    if(sound < SOUND_COUNT) {
        if(!sounds[sound].isLoaded()) {
            sounds[sound].loadSound(soundpaths[sound]);
        }
        sounds[sound].setVolume(volume);
        sounds[sound].play();
    }
//  else {
//      gLogi("Error: Invalid sound index in handleSound: " + std::to_string(sound));
//  }
}

void gApp::replaceSound(int newSound) {
    if(newSound >= SOUND_COUNT) {
//        gLogi("Error: Invalid sound index in replaceSound: " + std::to_string(newSound));
        return;
    }

    std::vector<int> prioritizedsounds = {SOUND_GAME, SOUND_BUTTON, SOUND_CAT_FIRE, SOUND_ENEMY_FIRE};

    bool soundexists = false;

    for(int i = 0; i < MAX_ACTIVE_SOUNDS; ++i) {
        if(sounds[i].isPlaying() && sounds[i].getPath() == soundpaths[newSound]) {
        	soundexists = true;
            break;
        }
    }

    if(!soundexists) {
        int soundToStop = -1;

        for(int i = 0; i < MAX_ACTIVE_SOUNDS; ++i) {
            if(!sounds[i].isPlaying()) {
                soundToStop = i;
                break;
            }
        }

        if(soundToStop == -1) {
            for(int i = 0; i < MAX_ACTIVE_SOUNDS; ++i) {
                if(std::find(prioritizedsounds.begin(), prioritizedsounds.end(), i) == prioritizedsounds.end()) {
                    soundToStop = i;
                    sounds[soundToStop].stop();
                    break;
                }
            }
        }

        if(soundToStop == -1) {
//            gLogi("Error: No available sound slots to replace.");
            return;
        }

        sounds[soundToStop].close();
        sounds[soundToStop].loadSound(soundpaths[newSound]);
        sounds[soundToStop].setVolume(1.0f);
        sounds[soundToStop].play();
//        gLogi("Replaced sound at index " + std::to_string(soundToStop) + " with " + soundpaths[newSound]);
    }
}

void gApp::closeAllSounds() {
    for(int i = 0; i < MAX_ACTIVE_SOUNDS; ++i) {
        if(sounds[i].isLoaded()) {
            sounds[i].close();
//            gLogi("Closed sound at index " + std::to_string(i));
        }
    }
}

void gApp::setupDatabase() {
	datas.clear();

	numberofdata = 0;
	std::string statement = "SELECT * FROM Players ORDER BY score DESC LIMIT 5;";
	std::vector<std::string> selectedData;

	database.loadDatabase("database.db");
	database.execute(statement);

	while(database.hasSelectData()) {
	    std::string rowData = database.getSelectData();
	    selectedData.push_back(rowData);
	}

	for(const auto& row : selectedData) {
	    int firstpipe = row.find("|", 1);
	    int secondpipe = row.find("|", firstpipe + 1);
	    int thirdpipe = row.find("|", secondpipe + 1);
	    int fourthpipe = row.find("|", thirdpipe + 1);
	    int fifthpipe = row.find("|", fourthpipe + 1);
	    int sixthpipe = row.find("|", fifthpipe + 1);
	    int seventhpipe = row.find("|", sixthpipe + 1);
	    int eighthpipe = row.find("|", seventhpipe + 1);
	    int ninthpipe = row.find("|", eighthpipe + 1);

	    id = gToInt(row.substr(firstpipe + 1, secondpipe - firstpipe - 1));
	    healthlevel = gToInt(row.substr(secondpipe + 1, thirdpipe - secondpipe - 1));
	    damagelevel = gToInt(row.substr(thirdpipe + 1, fourthpipe - thirdpipe - 1));
	    attackspeedlevel = gToInt(row.substr(fourthpipe + 1, fifthpipe - fourthpipe - 1));
	    goldmultiplierlevel = gToInt(row.substr(fifthpipe + 1, sixthpipe - fifthpipe - 1));
	    buffmultiplierlevel = gToInt(row.substr(sixthpipe + 1, seventhpipe - sixthpipe - 1));
	    score = gToInt(row.substr(seventhpipe + 1, eighthpipe - seventhpipe - 1));
	    gold = gToInt(row.substr(eighthpipe + 1, ninthpipe - eighthpipe - 1));
	    isdead = gToInt(row.substr(ninthpipe + 1));

	    std::ostringstream oss;
	    oss << "ID: " << id
	        << " Health Level: " << healthlevel
	        << " Damage Level: " << damagelevel
	        << " Attack Speed Level: " << attackspeedlevel
	        << " Gold Multiplier Level: " << goldmultiplierlevel
	        << " Buff Multiplier Level: " << buffmultiplierlevel
	        << " Score: " << score
	        << " Gold: " << gold
	        << " Is Dead: " << (isdead ? "Yes" : "No");
	    datas.push_back(oss.str());


	    // Get data number, i made like this because idk why just i want it like this.
	    numberofdata++;

	    // Total score and total gold for calculate star limits.
	    totalscore += score;
	    totalgold += gold;

	    // Add top 5 score and gold values.
	    topfivescore.push_back(gToStr(score));
	    topfivegold.push_back(std::to_string(gold));

	    // Get saved data
	    if(!isdead) {
	    	saveddatas.push_back(id);
	    	saveddatas.push_back(isdead);
	    }
	}

//	for (int i = 0; i < datas.size(); i++) {
//	    std::cout << "Data " << i + 1 << ": " << datas[i] << std::endl;
//	    std::cout << "Score " << i + 1 << ": " << topfivescore[i] << std::endl;
//	    std::cout << "Gold " << i + 1 << ": " << topfivegold[i] << std::endl;
//	}
}

float gApp::getTotalScore() {
	return totalscore;
}

float gApp::getTotalGold() {
	return totalgold;
}

float gApp::getPlayerDataNumber() {
	return numberofdata;
}

void gApp::insertDatabase(std::string insert) {
	database.execute(insert);
}

std::vector<std::string> gApp::getScore() {
	return topfivescore;
}

std::vector<std::string> gApp::getGold() {
	return topfivegold;
}

std::vector<int> gApp::getIsDead() {
	saveddatas.clear();

	std::string statement = "SELECT * FROM Players WHERE isdead = 0;";
	std::vector<std::string> selectedData;

	database.execute(statement);
	if(database.hasSelectData()) {
		std::string row = database.getSelectData();

        int firstpipe = row.find("|", 1);
        int secondpipe = row.find("|", firstpipe + 1);
        int thirdpipe = row.find("|", secondpipe + 1);
        int fourthpipe = row.find("|", thirdpipe + 1);
        int fifthpipe = row.find("|", fourthpipe + 1);
        int sixthpipe = row.find("|", fifthpipe + 1);
        int seventhpipe = row.find("|", sixthpipe + 1);
        int eighthpipe = row.find("|", seventhpipe + 1);
        int ninthpipe = row.find("|", eighthpipe + 1);
        int tenthpipe = row.find("|", ninthpipe + 1);
        int eleventhpipe = row.find("|", tenthpipe + 1);

        id = gToInt(row.substr(firstpipe + 1, secondpipe - firstpipe - 1));
        healthlevel = gToInt(row.substr(secondpipe + 1, thirdpipe - secondpipe - 1));
        damagelevel = gToInt(row.substr(thirdpipe + 1, fourthpipe - thirdpipe - 1));
        attackspeedlevel = gToInt(row.substr(fourthpipe + 1, fifthpipe - fourthpipe - 1));
        goldmultiplierlevel = gToInt(row.substr(fifthpipe + 1, sixthpipe - fifthpipe - 1));
        buffmultiplierlevel = gToInt(row.substr(sixthpipe + 1, seventhpipe - sixthpipe - 1));
        score = gToInt(row.substr(seventhpipe + 1, eighthpipe - seventhpipe - 1));
        gold = gToInt(row.substr(eighthpipe + 1, ninthpipe - eighthpipe - 1));
        isdead = gToInt(row.substr(ninthpipe + 1));
        enemylevel = gToInt(row.substr(tenthpipe + 1, eleventhpipe - tenthpipe - 1));
        level = gToInt(row.substr(eleventhpipe + 1));

		saveddatas.push_back(id);
		saveddatas.push_back(healthlevel);
		saveddatas.push_back(damagelevel);
		saveddatas.push_back(attackspeedlevel);
		saveddatas.push_back(goldmultiplierlevel);
		saveddatas.push_back(buffmultiplierlevel);
		saveddatas.push_back(score);
		saveddatas.push_back(gold);
		saveddatas.push_back(isdead);
		saveddatas.push_back(enemylevel);
	}

	return saveddatas;
}

void gApp::getTopFiveScores() {
    std::vector<std::pair<int, int>> scoreGoldPairs;

    std::string statement = "SELECT * FROM Players";
    std::vector<std::string> selectedData;

    database.loadDatabase("database.db");
    database.execute(statement);

    while(database.hasSelectData()) {
        std::string rowData = database.getSelectData();
        selectedData.push_back(rowData);
    }

    for(const auto& row : selectedData) {
        int seventhpipe = row.find("|", row.rfind("|", row.rfind("|") - 1) + 1);
        int eighthpipe = row.find("|", seventhpipe + 1);
        int ninthpipe = row.find("|", eighthpipe + 1);

        int score = gToInt(row.substr(seventhpipe + 1, eighthpipe - seventhpipe - 1));
        int gold = gToInt(row.substr(eighthpipe + 1, ninthpipe - eighthpipe - 1));

        // Add score and gold as a pair
        scoreGoldPairs.emplace_back(score, gold);
    }

    // Sort the scores from largest to smallest
    std::sort(scoreGoldPairs.begin(), scoreGoldPairs.end(), std::greater<std::pair<int, int>>());

    // Get the top 5 scores and their respective gold values
    for (int i = 0; i < std::min(5, (int)scoreGoldPairs.size()); i++) {
        topfivescore.push_back(gToStr(scoreGoldPairs[i].first));
        topfivegold.push_back(std::to_string(scoreGoldPairs[i].second));
    }
}
