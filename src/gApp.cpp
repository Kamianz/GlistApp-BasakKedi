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
	soundenabled = true;
	musicenabled = true;
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
        	if(!soundenabled) {
            	if(sound < MAX_ACTIVE_SOUNDS) {
    				handleSound(sound, volume);
    			} else {
    				replaceSound(sound);
    			}
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

void gApp::stopAllSounds() {
    if(sounds[SOUND_MENU].isPlaying()) {
        sounds[SOUND_MENU].setPaused(true);
    }

    if(sounds[SOUND_GAME].isPlaying()) {
        sounds[SOUND_GAME].setPaused(true);
    }
}

void gApp::onSceneChange() {
    stopAllSounds();
}

void gApp::toggleSound() {
	soundenabled = !soundenabled;
}
void gApp::toggleMusic() {
    musicenabled = !musicenabled;

    if(!musicenabled) {
        if(sounds[SOUND_MENU].isPlaying()) {
            sounds[SOUND_MENU].setPaused(true);
        }

        if(sounds[SOUND_GAME].isPlaying()) {
            sounds[SOUND_GAME].setPaused(true);
        }
    }
    else {
        if(sounds[SOUND_MENU].isLoaded()) {
            sounds[SOUND_MENU].setPaused(false);
        }

        if(sounds[SOUND_GAME].isLoaded()) {
            sounds[SOUND_GAME].setPaused(false);
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

std::vector<int> gApp::getSavedData() {
	saveddatas.clear();

	std::string statement = "SELECT * FROM Save WHERE isdead = 0;";
	std::vector<std::string> selectedData;

	database.execute(statement);
	if(database.hasSelectData()) {
		std::string row = database.getSelectData();

		setPipes(row);

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
		saveddatas.push_back(level);
	}

	return saveddatas;
}

std::vector<std::pair<int, std::pair<int, int>>> gApp::getTopFiveScores() {
    std::vector<std::pair<int, std::pair<int, int>>> topFiveScores;

    std::string statement = "SELECT id, score, gold FROM Players ORDER BY score DESC LIMIT 5";
    database.loadDatabase("database.db");
    database.execute(statement);

    while (database.hasSelectData()) {
        std::string row = database.getSelectData();
        setPipes(row);

        int id = gToInt(row.substr(firstpipe + 1, secondpipe - firstpipe - 1));
        int score = gToInt(row.substr(secondpipe + 1, thirdpipe - secondpipe - 1));
        int gold = gToInt(row.substr(thirdpipe + 1));

        topFiveScores.emplace_back(id, std::make_pair(score, gold));
    }

    return topFiveScores;
}

void gApp::saveDatas(std::vector<int> datas) {
	int healthlevel = datas[0];
	int damagelevel = datas[1];
	int attackspeedlevel = datas[2];
	int goldmultiplierlevel = datas[3];
	int buffmultiplierlevel = datas[4];
	int score = datas[5];
	int gold = datas[6];
	int isdead = datas[7];
	int enemylevel = datas[8];
	int level = datas[9];

	std::string statement = "Inster * FROM Players ORDER BY score DESC LIMIT 5;";
	database.execute(statement);
}

bool gApp::saveControl() {
	std::string statement = "SELECT * FROM Save WHERE isdead = 0;";
	std::vector<std::string> selectedData;

	database.loadDatabase("database.db");
	database.execute(statement);

	while(database.hasSelectData()) {
	    std::string rowData = database.getSelectData();
	    selectedData.push_back(rowData);
	}

	for(const auto& row : selectedData) {
		setPipes(row);

		id = gToInt(row.substr(firstpipe + 1, secondpipe - firstpipe - 1));
		healthlevel = gToInt(row.substr(secondpipe + 1, thirdpipe - secondpipe - 1));
		damagelevel = gToInt(row.substr(thirdpipe + 1, fourthpipe - thirdpipe - 1));
		attackspeedlevel = gToInt(row.substr(fourthpipe + 1, fifthpipe - fourthpipe - 1));
		goldmultiplierlevel = gToInt(row.substr(fifthpipe + 1, sixthpipe - fifthpipe - 1));
		buffmultiplierlevel = gToInt(row.substr(sixthpipe + 1, seventhpipe - sixthpipe - 1));
		score = gToInt(row.substr(seventhpipe + 1, eighthpipe - seventhpipe - 1));
		gold = gToInt(row.substr(eighthpipe + 1, ninthpipe - eighthpipe - 1));
		isdead = gToInt(row.substr(ninthpipe + 1));

		if(isdead == 0) return true;
	}

	return false;
}

void gApp::resetSave() {
	std::string statement = std::string("UPDATE Save SET healthlevel = 1, ")
	        + "damagelevel = 1, "
	        + "attackspeedlevel = 1, "
	        + "goldmultiplierlevel = 1, "
	        + "buffmultiplierlevel = 1, "
	        + "score = 0, "
	        + "gold = 0, "
	        + "enemylevel = 1, "
	        + "level = 1, "
	        + "isdead = 1 "
	        + "WHERE Id = 1;";

	database.execute(statement);
}

void gApp::setPipes(std::string row) {
    firstpipe = row.find("|", 1);
    secondpipe = row.find("|", firstpipe + 1);
    thirdpipe = row.find("|", secondpipe + 1);
    fourthpipe = row.find("|", thirdpipe + 1);
    fifthpipe = row.find("|", fourthpipe + 1);
    sixthpipe = row.find("|", fifthpipe + 1);
    seventhpipe = row.find("|", sixthpipe + 1);
    eighthpipe = row.find("|", seventhpipe + 1);
    ninthpipe = row.find("|", eighthpipe + 1);
    tenthpipe = row.find("|", ninthpipe + 1);
    eleventhpipe = row.find("|", tenthpipe + 1);
}

void gApp::updateLowestScores(const std::pair<int, int>& newScoreGold) {
    std::string statement = "SELECT id, score, gold FROM Players ORDER BY score ASC LIMIT 5";
    database.loadDatabase("database.db");
    database.execute(statement);

    std::vector<std::tuple<int, int, int>> lowestScores;

    while(database.hasSelectData()) {
        std::string row = database.getSelectData();
        setPipes(row);

        int id = gToInt(row.substr(firstpipe + 1, secondpipe - firstpipe - 1));
        int score = gToInt(row.substr(seventhpipe + 1, eighthpipe - seventhpipe - 1));
        int gold = gToInt(row.substr(eighthpipe + 1, ninthpipe - eighthpipe - 1));

        lowestScores.emplace_back(id, score, gold);
    }

    for(const auto& lowestScore : lowestScores) {
        int id = std::get<0>(lowestScore);
        int oldScore = std::get<1>(lowestScore);
        int oldGold = std::get<2>(lowestScore);

        if(newScoreGold.first > oldScore || newScoreGold.second > oldGold) {
            std::string updateStatement = "UPDATE Players SET score = " + std::to_string(newScoreGold.first) +
                                           ", gold = " + std::to_string(newScoreGold.second) +
                                           " WHERE id = " + std::to_string(id);
            database.execute(updateStatement);
            break;
        }
    }
}
