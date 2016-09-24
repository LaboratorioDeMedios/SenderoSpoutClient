#include "SpoutBehaviour.h"



SpoutBehaviour::SpoutBehaviour()
{
	
}


SpoutBehaviour::~SpoutBehaviour()
{
}

void SpoutBehaviour::customSetup(map<int, Pixel*>* pixels, vector<Pixel*>* pixelsFast) {

	this->settings = new ofxXmlSettings();

	if (settings->loadFile("spout_conf.xml")) {
		loadConfig();
	}

	testModeEnabled = false;
	pixelIdToTest = 0;

	spoutReceiver.setup();

}


void SpoutBehaviour::loadConfig() {
	settings->pushTag("Configuration");
	// load states
	settings->pushTag("FrameMap");
	int pixelCount = settings->getNumTags("Pixel");

	if (pixelCount>0) {
		for (int i = 0; i < pixelCount; i++) {
			int pixId = ofToInt(settings->getAttribute("Pixel", "id", "0", i));
			int x = ofToInt(settings->getAttribute("Pixel", "x", "0", i));
			int y = ofToInt(settings->getAttribute("Pixel", "y", "0", i));

			this->pixelMap.insert(pair<int, ofVec2f>(pixId, ofVec2f(x, y)));
		}
	}
	settings->popTag();
	settings->popTag();
}

void SpoutBehaviour::update() {


	if (testModeEnabled) {

		vector<Pixel*>::iterator it = this->pixelsFast->begin();

		while (it != this->pixelsFast->end()) {
			Pixel* px = *it;

			if (px->getId() == this->pixelIdToTest) {
				px->blendRGBA(255, 255, 255, 255, 1.0f);
			}
			else {
				px->blendRGBA(0, 0, 0, 0, 1.0f);
			}
			it++;
		}

	}
	else {

		spoutReceiver.updateTexture();

		if (this->spoutReceiver.getMbInitialized()) {
			ofPixels pxs;
			spoutReceiver.getTexture().readToPixels(pxs);
		
			vector<Pixel*>::iterator it = this->pixelsFast->begin();

			while (it != this->pixelsFast->end()) {
				Pixel* px = *it;
			
				map<int, ofVec2f>::iterator itMap;
				itMap = this->pixelMap.find(px->getId());
				if (itMap != pixelMap.end()) {
					//we have a map for the pixel
					ofVec2f mappedCoordinates = itMap->second;

					int pixelSize = 3;
					bool pixelFormatRecognized = false;
					int rOffset = 0;
					int gOffset = 0;
					int bOffset = 0;

					switch (pxs.getPixelFormat())
					{
					case OF_PIXELS_GRAY:
					{
						pixelSize = 1;

						rOffset = 0;
						gOffset = 0;
						bOffset = 0;

						pixelFormatRecognized = true;
						break;
					}
					case OF_PIXELS_GRAY_ALPHA:
					{
						pixelSize = 2;

						rOffset = 0;
						gOffset = 0;
						bOffset = 0;

						pixelFormatRecognized = true;
						break;
					}
					case OF_PIXELS_RGB:
					{
						pixelSize = 3;

						rOffset = 0;
						gOffset = 1;
						bOffset = 2;

						pixelFormatRecognized = true;
						break;
					}
					case OF_PIXELS_BGR:
					{
						pixelSize = 3;

						rOffset = 2;
						gOffset = 1;
						bOffset = 0;

						pixelFormatRecognized = true;
						break;
					}
					case OF_PIXELS_RGBA:
					{
						pixelSize = 4;

						rOffset = 0;
						gOffset = 1;
						bOffset = 2;

						pixelFormatRecognized = true;
						break;
					}
					case OF_PIXELS_BGRA:
					{
						pixelSize = 4;

						rOffset = 2;
						gOffset = 1;
						bOffset = 0;

						pixelFormatRecognized = true;
						break;
					}
					default:
					{
						ofLogNotice("Pixel Format from Spout not recognized.");
					}
					}

					if (pixelFormatRecognized) {
						int rIndex = ((pxs.getWidth() * mappedCoordinates.y) * pixelSize + mappedCoordinates.x * pixelSize + rOffset);
						int gIndex = ((pxs.getWidth() * mappedCoordinates.y) * pixelSize + mappedCoordinates.x * pixelSize + gOffset);
						int bIndex = ((pxs.getWidth() * mappedCoordinates.y) * pixelSize + mappedCoordinates.x * pixelSize + bOffset);

						if (rIndex < pxs.size() && gIndex < pxs.size() && bIndex < pxs.size()) {

							int r = pxs[rIndex];
							int g = pxs[gIndex];
							int b = pxs[bIndex];

							px->blendRGBA(r, g, b, 255, 1.0f);
						}
						else {
							ofLogError("Pixel index outside of boundaries. Check pixel format.");
						}

					}

				}
				
				it++;
			}

		}
	}
}

void SpoutBehaviour::draw() {
	
	
}

void SpoutBehaviour::drawPre() {
	ofPushMatrix();

	ofDisableDepthTest();

	ofTranslate(0.0f, 0.0f, 0.0f);
	ofRotate(0.0f);
	
	if (this->spoutReceiver.getMbInitialized()) {
		spoutReceiver.getTexture().draw(10, 10);
	}

	ofSetColor(255, 50, 50);
	ofDrawBitmapString("// Sendero Spout Client v0.01 //", 10, 560);
	ofSetColor(255);
	ofDrawBitmapString("Test Mode: " + ofToString(this->testModeEnabled), 10, 600);
	ofDrawBitmapString("Pixel ID to test: " + ofToString(this->pixelIdToTest), 10, 620);
	ofDrawBitmapString("Press 's' to select Spout Input. ", 10, 640);
	ofDrawBitmapString("Press 't' to enable/disable test mode. ", 10, 660);
	ofDrawBitmapString("Press 'up' or 'down' to change pixel to test. ", 10, 680);

	ofEnableDepthTest();
	ofPopMatrix();
	
}

void SpoutBehaviour::drawPost() {
	
}

void SpoutBehaviour::keyPressed(int key) {

	switch (key) {

	case 's':
	{
		spoutReceiver.showSenders();
		break;
	}
	case 't':
	{
		this->testModeEnabled = !this->testModeEnabled;
		break;
	}	
	case OF_KEY_UP:
	{

		this->pixelIdToTest += 1;
		if (this->pixelIdToTest >= this->pixelsFast->size()) {
			this->pixelIdToTest = this->pixelsFast->size() - 1;
		}
		break;
	}
	case OF_KEY_DOWN:
	{
		this->pixelIdToTest -= 1;

		if (this->pixelIdToTest < 0) {
			this->pixelIdToTest = 0;
		}

		break;
	}
	default: {
		break;
	}
	}
	
}

void SpoutBehaviour::exit() {
	spoutReceiver.exit();
}
