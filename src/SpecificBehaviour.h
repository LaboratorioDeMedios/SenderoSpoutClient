//
//  SpecificBehaviour.h
//  GenericClient
//
//  Created by Christian Clark on 3/26/13.
//  Copyright (c) 2013 chachi.clark@gmail.com. All rights reserved.
//

#ifndef GenericClient_SpecificBehaviour_h
#define GenericClient_SpecificBehaviour_h

#include "ofMain.h"
#include "Pixel.h"
#include "DTFrame.h"
#include "DTPixel.h"
#include <map>
#include <vector>


class SpecificBehaviour {

public:

	SpecificBehaviour();
	~SpecificBehaviour(void);

	void setup(map<int, Pixel*>* pixels, vector<Pixel*>* pixelsFast);

	virtual void customSetup(map<int, Pixel*>* pixels, vector<Pixel*>* pixelsFast);
	virtual void update();
	virtual void draw();
	virtual void drawPre();
	virtual void drawPost();
	virtual void keyPressed(int key);
	virtual void exit();

protected:

	map<int, Pixel*>* pixels;
	vector<Pixel*>* pixelsFast;

};

#endif