//
//  GenericClientManager.h
//  GenericClient
//
//  Created by Christian Clark on 3/26/13.
//  Copyright (c) 2013 chachi.clark@gmail.com. All rights reserved.
//

#ifndef GenericClient_GenericClientManager_h
#define GenericClient_GenericClientManager_h

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxXmlSettings.h"
#include "Pixel.h"
#include "DTPixel.h"
#include "DTFrame.h"
#include "SpecificBehaviour.h"
#include "SpoutBehaviour.h"
#include <map>
#include <vector>

class GenericClientManager : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	ofxTCPClient tcpClient;
	ofxUDPManager udpManager;

	map<int, Pixel*> *pixels;
	vector<Pixel*> *pixelsFast;

	map<string, ofxAssimpModelLoader *>* models;

	unsigned short sequenceNumber;

	string serverIP;
	int serverTCPPort;
	int UDPPort;

private:

	int loadFromXML();

	int configureFromServer();

	void transmitFrame();

	void checkServerMessages();

	ofEasyCam cam;

	float minX;
	float maxX;
	float minY;
	float maxY;
	float maxZ;
	float minZ;

	int appFps;
	int newFps;
	ofTrueTypeFont myfont;

	int waitForReceiver;

	bool useServer;
	bool transmitEnabled;
	bool showAxis;
	bool enableDraw;

	SpecificBehaviour* specific;

};


#endif