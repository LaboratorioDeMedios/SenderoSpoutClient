#pragma once
#include "SpecificBehaviour.h"
#include "ofxXmlSettings.h"
#include "ofxSpout2Receiver.h"
#include <map>

class SpoutBehaviour : public SpecificBehaviour
{
public:
	bool testModeEnabled;
	int pixelIdToTest;

	ofxSpout2::Receiver spoutReceiver;

	SpoutBehaviour();
	~SpoutBehaviour();

	void customSetup(map<int, Pixel*>* pixels, vector<Pixel*>* pixelsFast);
	void update();
	void draw();
	void drawPre();
	void drawPost();
	void keyPressed(int key);
	void exit();

	void loadConfig();

	ofxXmlSettings* settings;

	map<int, ofVec2f> pixelMap;
	ofImage currentImage;


};

