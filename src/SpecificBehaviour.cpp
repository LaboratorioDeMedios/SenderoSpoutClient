//
//  SpecificBehaviour.cpp
//  GenericClient
//
//  Created by Christian Clark on 3/26/13.
//  Copyright (c) 2013 chachi.clark@gmail.com. All rights reserved.
//

#include <iostream>
#include "SpecificBehaviour.h"

SpecificBehaviour::SpecificBehaviour() {

}

SpecificBehaviour::~SpecificBehaviour(void) {

}

void SpecificBehaviour::setup(map<int, Pixel*>* iPixels, vector<Pixel*>* iPixelsFast) {
	//this class shares the same collections as the generic client manager. BE CAREFUL

	this->pixels = iPixels;
	this->pixelsFast = iPixelsFast;

	customSetup(iPixels, iPixelsFast);

}


//custom setup for the specific behaviours that will inherit from SpeficifBehaviour
void SpecificBehaviour::customSetup(map<int, Pixel*>* iPixels, vector<Pixel*>* iPixelsFast) {
}


void SpecificBehaviour::update() {
	//new behaviour here
	//sample -> paint every pixel with red at key pressed and blend with black at update
	vector<Pixel*>::iterator it = this->pixelsFast->begin();

	while (it != this->pixelsFast->end())
	{
		Pixel* px = *it;
		px->blendRGBA(0, 0, 0, 255, 0.2f);
		it++;
	}
	//end of sample
}

void SpecificBehaviour::draw() {
	//custom draw here.
}

void SpecificBehaviour::drawPre() {
	//custom draw here.
}

void SpecificBehaviour::drawPost() {
	//custom draw here.
}

void SpecificBehaviour::keyPressed(int key) {
	//sample -> paint every pixel with red at key pressed and blend with black at update
	vector<Pixel*>::iterator it = this->pixelsFast->begin();

	while (it != this->pixelsFast->end())
	{
		Pixel* px = *it;
		px->blendRGBA(255, 0, 0, 255, 1.0f);
		it++;
	}
	//end of sample

}


void SpecificBehaviour::exit() {

}