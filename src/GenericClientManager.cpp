//
//  GenericClientManager.cpp
//  GenericClient
//
//  Created by Christian Clark on 3/26/13.
//  Copyright (c) 2013 chachi.clark@gmail.com. All rights reserved.
//

#include <iostream>
#include "GenericClientManager.h"

void GenericClientManager::setup() {

	this->minX = 99999.0f;
	this->maxX = 0.0f;
	this->minY = 99999.0f;
	this->maxY = 0.0f;

	this->pixels = new map<int, Pixel*>;
	this->pixelsFast = new vector<Pixel*>;
	this->models = new map<string, ofxAssimpModelLoader *>;

	this->sequenceNumber = 0;

	this->loadFromXML();

	if (useServer) {
		this->tcpClient.setup(this->serverIP, this->serverTCPPort);
		this->configureFromServer();
		this->udpManager.Create();
		this->udpManager.Connect(this->serverIP.data(), this->UDPPort);
		this->udpManager.SetNonBlocking(true);
	}

	this->transmitEnabled = true;

	ofSetFrameRate(this->appFps);
	this->newFps = this->appFps;
	this->waitForReceiver = 0;

	ofBackground(0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	ofSetupScreen();

	myfont.loadFont("Arial.ttf", 12);

	this->showAxis = true;
	this->enableDraw = true;

	//here we call the specific setup
	// TODO: choose from XML which behaviour we are goinge to instantiate
	//this->specific = new SpecificBehaviour();

	this->specific = new SpoutBehaviour();

	specific->setup(this->pixels, this->pixelsFast);

}

//--------------------------------------------------------------
void GenericClientManager::update() {

	//check for serverMessages
	if (useServer) {
		this->checkServerMessages();
	}

	if (this->newFps != this->appFps) {
		this->appFps = this->newFps;
		ofSetFrameRate(this->appFps);
	}

	this->specific->update();

	if (transmitEnabled && useServer) {
		transmitFrame();
	}

}

void GenericClientManager::checkServerMessages() {
	string response = "";
	bool received = false;

	// <message fps="18" messageError="true" lastSequenceNumber="330">

	response = this->tcpClient.receive();
	if (response.length() > 0) {
		received = true;
	}

	if (received) {
		ofxXmlSettings XML;
		XML.loadFromBuffer(response);

		TiXmlElement* serverMessage = XML.doc.RootElement();
		if (serverMessage) {
			string fpsName = "fps";
			if (serverMessage->Attribute(fpsName.c_str())) {
				int messageFps = ofToInt(serverMessage->Attribute("fps"));
				this->newFps = messageFps;
			}

			string messageErrorName = "messageError";
			if (serverMessage->Attribute(messageErrorName.c_str())) {
				bool error = ofToBool(serverMessage->Attribute("messageError"));
				if (error) {
					this->waitForReceiver = 15; // wait 15 frames until the server cleans his buffer
					string lastSequenceNumberName = "lastSequenceNumber";
					if (serverMessage->Attribute(lastSequenceNumberName.c_str())) {
						this->sequenceNumber = ofToInt(serverMessage->Attribute("lastSequenceNumber")) + 1;
						string responseMessage = "<?xml version=\"1.0\" ?><ErrorACK sequence='" + ofToString(this->sequenceNumber) + "'></ErrorACK>";
						this->tcpClient.send(responseMessage);
					}
				}
			}
		}
	}
}

void GenericClientManager::transmitFrame() {
	//construyo DTFrame en base a los pixels mezclados

	if (this->waitForReceiver == 0) { // envio solo si el server no me pidio esperarpor error

		vector<DTPixel*>* newVector = new vector<DTPixel*>;
		map<int, Pixel*>::iterator it = this->pixels->begin();
		while (it != this->pixels->end())
		{
			Pixel* px = it->second;
			ofVec3f position = px->getPosition();
			ofVec3f front = px->getFront();
			ofVec3f up = px->getUp();
			string modelName = px->getModelName();
			DTPixel* dtPix = new DTPixel(px->getId(), px->getR(), px->getG(), px->getB(), px->getA(), position.x, position.y, position.z, front, up, modelName);

			newVector->push_back(dtPix);

			it++;
		}
		DTFrame * newFrame = new DTFrame(0, newVector, this->pixels->size(), this->sequenceNumber);

		// slice the DTFrame and transmit by piece.

		int pixelQuantity = newVector->size();
		int minId = 0;
		int maxId = pixelQuantity - 1;

		while (pixelQuantity>0) {
			celebra_packet_t toTransmit;
			int headerSize = 18;
			if (pixelQuantity <= 341) {
				toTransmit = newFrame->getBinaryPacketFromFrame(minId, maxId, this->sequenceNumber, 1);
				int sent = this->udpManager.SendAll((char *)&toTransmit, headerSize + ((maxId - minId + 1) * 3));
				pixelQuantity = 0;
			}
			else {

				toTransmit = newFrame->getBinaryPacketFromFrame(minId, minId + 340, this->sequenceNumber, 0);
				int sent = this->udpManager.SendAll((char *)&toTransmit, headerSize + (341 * 3));
				pixelQuantity = pixelQuantity - 341;
				minId = minId + 341;
			}

			// must increment sequence number.
			if (this->sequenceNumber == 65535) {
				this->sequenceNumber = 0;
			}
			else {
				this->sequenceNumber++;
			}

		}

		delete newFrame;
	}
	else {
		this->waitForReceiver -= 1;
	}

}

void GenericClientManager::keyPressed(int key) {
	specific->keyPressed(key);
}


//--------------------------------------------------------------
void GenericClientManager::keyReleased(int key) {

}

//--------------------------------------------------------------
void GenericClientManager::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void GenericClientManager::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void GenericClientManager::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void GenericClientManager::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void GenericClientManager::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void GenericClientManager::dragEvent(ofDragInfo dragInfo) {

}

int GenericClientManager::configureFromServer() {

	string configurationRequest = "<?xml version=\"1.0\" ?><ConfigurationRequest installationName='Celebra1' clientName='CelebraClient'></ConfigurationRequest>";
	this->tcpClient.send(configurationRequest);
	bool received = false;
	string response = "";
	while (!received) {
		response = this->tcpClient.receive();
		if (response.length() > 0) {
			received = true;
		}

	}

	if (received) {
		ofxXmlSettings XML;
		XML.loadFromBuffer(response);

		TiXmlElement* serverInfo = XML.doc.RootElement();
		if (serverInfo) {
			string udpPortName = "UDPPort";
			if (serverInfo->Attribute(udpPortName.c_str())) {
				this->UDPPort = ofToInt(serverInfo->Attribute("UDPPort"));
			}
			else {
				this->UDPPort = 0;
			}
			TiXmlElement* frame = serverInfo->FirstChildElement();
			if (frame) {
				TiXmlElement* pixel = frame->FirstChildElement();
				for (pixel; pixel; pixel = pixel->NextSiblingElement())
				{

					string idName = "id";
					int id = 0;
					if (ofToInt(pixel->Attribute(idName.c_str()))) {
						id = ofToInt(pixel->Attribute("id"));
					}


					int r = 0;
					string rName = "r";
					if (ofToInt(pixel->Attribute(rName.c_str()))) {
						r = ofToInt(pixel->Attribute("r"));
					}
					else {
						r = 0;
					}

					int g = 0;
					string gName = "g";
					if (ofToInt(pixel->Attribute(gName.c_str()))) {
						g = ofToInt(pixel->Attribute("g"));
					}
					else {
						g = 0;
					}

					int b = 0;
					string bName = "b";
					if (ofToInt(pixel->Attribute(bName.c_str()))) {
						b = ofToInt(pixel->Attribute("b"));
					}
					else {
						b = 0;
					}

					int a = 0;
					string aName = "a";
					if (ofToInt(pixel->Attribute(aName.c_str()))) {
						a = ofToInt(pixel->Attribute("a"));
					}
					else {
						a = 255;
					}

					TiXmlElement* render = pixel->FirstChildElement();
					if (!render) {
						return -1;
					}

					string meshName = "mesh";
					string linkedMesh = "";
					if (render->Attribute(meshName.c_str())) {
						linkedMesh = render->Attribute("mesh");
					}
					else {
						return -1;
					}

					ofVec3f front;
					ofVec3f up;
					ofVec3f positionPixel;

					TiXmlElement* frontElement = render->FirstChildElement();
					if (!frontElement) {
						return -1;
					}

					TiXmlElement* upElement = frontElement->NextSiblingElement();
					if (!upElement) {
						return -1;
					}

					TiXmlElement* positionElement = upElement->NextSiblingElement();
					if (!positionElement) {
						return -1;
					}

					string xName = "x";
					string yName = "y";
					string zName = "z";

					float xFront = 0.0f;
					if (frontElement->Attribute(xName.c_str())) {
						string test = frontElement->Attribute("x");
						xFront = ofToFloat(frontElement->Attribute("x"));
					}
					else {
						xFront = 0.0f;
					}

					float yFront = 0.0f;
					if (frontElement->Attribute(yName.c_str())) {
						yFront = ofToFloat(frontElement->Attribute("y"));
					}
					else {
						yFront = 0.0f;
					}

					float zFront = 0.0f;
					if (frontElement->Attribute(zName.c_str())) {
						zFront = ofToFloat(frontElement->Attribute("z"));
					}
					else {
						zFront = 0.0f;
					}

					float xUp = 0.0f;
					if (upElement->Attribute(xName.c_str())) {
						xUp = ofToFloat(upElement->Attribute("x"));
					}
					else {
						xUp = 0.0f;
					}

					float yUp = 0.0f;
					if (upElement->Attribute(yName.c_str())) {
						yUp = ofToFloat(upElement->Attribute("y"));
					}
					else {
						yUp = 0.0f;
					}

					float zUp = 0.0f;
					if (upElement->Attribute(zName.c_str())) {
						zUp = ofToFloat(upElement->Attribute("z"));
					}
					else {
						zUp = 0.0f;
					}

					float xPos = 0.0f;
					if (positionElement->Attribute(xName.c_str())) {
						string test = positionElement->Attribute("x");
						xPos = ofToFloat(test);
						//cout << xPos << endl;
					}
					else {
						xPos = 0.0f;
					}

					float yPos = 0.0f;
					if (positionElement->Attribute(yName.c_str())) {
						yPos = ofToFloat(positionElement->Attribute("y"));
					}
					else {
						yPos = 0.0f;
					}

					float zPos = 0.0f;
					if (positionElement->Attribute(zName.c_str())) {
						zPos = ofToFloat(positionElement->Attribute("z"));
					}
					else {
						zPos = 0.0f;
					}

					positionPixel.x = xPos;
					positionPixel.y = yPos;
					positionPixel.z = zPos;

					front.x = xFront;
					front.y = yFront;
					front.z = zFront;

					up.x = xUp;
					up.y = yUp;
					up.z = zUp;

					Pixel* newPixel = new Pixel(id, r, g, b, a, positionPixel);

					front.normalize();
					up.normalize();

					ofMatrix4x4 rotFront;

					ofVec3f sourceFront(1, 0, 0); //already normalized
					ofVec3f sourceUp(0, 0, 1); //already normalized

					ofQuaternion qFront;

					qFront.makeRotate(sourceFront, front);

					ofVec3f newSourceUp = rotFront * sourceUp;

					//cout << "New Up x:" << newUp.x << " y:" << newUp.y << " z:" << newUp.z << endl;

					ofQuaternion qUp;

					qUp.makeRotate(newSourceUp, up);

					ofQuaternion qTot = qFront; // * qUp esto anda mal. hay que multiplicar por la rotación up para que todo esté correcto

												// we save the rotation in the pixel

					newPixel->setRotQ(qTot);

					//now we link the mesh

					map<string, ofxAssimpModelLoader*>::iterator itModel = this->models->find(linkedMesh);

					if (itModel != this->models->end()) {
						ofxAssimpModelLoader* model = itModel->second;
						newPixel->setModel(model);
					}

					newPixel->setModelName(linkedMesh);

					newPixel->setFront(front);
					newPixel->setUp(up);

					this->pixels->insert(pair<int, Pixel*>(id, newPixel));
					this->pixelsFast->push_back(newPixel);
				}
			}

		}

	}

}

int GenericClientManager::loadFromXML() {

	ofxXmlSettings XML;

	if (XML.loadFile("configuration.xml")) {


		TiXmlElement* config = XML.doc.RootElement();
		if (!config) {
			return -1;
		}

		TiXmlElement* myMeshes = config->FirstChildElement();
		if (!myMeshes) {
			return -1;
		}

		TiXmlElement* mesh = myMeshes->FirstChildElement();
		if (!mesh) {
			return -1;
		}

		string addressName = "address";
		if (config->Attribute(addressName.c_str())) {
			this->serverIP = config->Attribute("address");
		}
		else {
			return -1;
		}

		string portName = "port";
		if (config->Attribute(portName.c_str())) {
			this->serverTCPPort = ofToInt(config->Attribute("port"));
		}
		else {
			return -1;
		}

		string fpsName = "fps";
		if (config->Attribute(fpsName.c_str())) {
			this->appFps = ofToInt(config->Attribute("fps"));
		}
		else {
			this->appFps = 24;
		}

		string useServerName = "useServer";
		if (config->Attribute(useServerName.c_str())) {
			this->useServer = (ofToInt(config->Attribute("useServer")) == 1);
		}
		else {
			this->useServer = false;
		}

		//meshes
		for (mesh; mesh; mesh = mesh->NextSiblingElement())
		{	//asuming all devices are mBeds

			string meshNameName = "name";
			string nameMesh = "";
			if (mesh->Attribute(meshNameName.c_str())) {
				nameMesh = mesh->Attribute("name");
			}
			else {
				return -1;
			}

			string pathName = "path";
			string meshPath = "";
			if (mesh->Attribute(pathName.c_str())) {
				meshPath = mesh->Attribute("path");
			}
			else {
				return -1;
			}

			ofxAssimpModelLoader* model = new ofxAssimpModelLoader();

			model->loadModel(meshPath);

			model->setScaleNormalization(false);

			model->setScale(1.0f, 1.0f, 1.0f);

			this->models->insert(pair<string, ofxAssimpModelLoader*>(nameMesh, model));
		}

		if (!this->useServer) {

			TiXmlElement* myPixels = myMeshes->NextSiblingElement();
			if (!myPixels) {
				return -1;
			}
			TiXmlElement* pixel = myPixels->FirstChildElement();
			if (!pixel) {
				return -1;
			}

			int pixelCounter = 0;
			//configuracion de pixels
			for (pixel; pixel; pixel = pixel->NextSiblingElement())
			{
				string idName = "id";
				string rName = "r";
				string gName = "g";
				string bName = "b";
				string aName = "a";

				int id = 0;
				if (pixel->Attribute(idName.c_str())) {
					id = ofToInt(pixel->Attribute("id"));
				}
				else {
					return -1;
				}
				float r = 0;
				if (pixel->Attribute(rName.c_str())) {
					r = ofToFloat(pixel->Attribute("r"));
				}
				else {
					r = 0;
				}
				float g = 0;
				if (pixel->Attribute(gName.c_str())) {
					g = ofToFloat(pixel->Attribute("g"));
				}
				else {
					g = 0;
				}
				float b = 0;
				if (pixel->Attribute(bName.c_str())) {
					b = ofToFloat(pixel->Attribute("b"));
				}
				else {
					b = 0;
				}
				float a = 0;
				if (pixel->Attribute(aName.c_str())) {
					a = ofToFloat(pixel->Attribute("a"));
				}
				else {
					a = 255.0f;
				}

				TiXmlElement* render = pixel->FirstChildElement();
				if (!render) {
					return -1;
				}

				string meshName = "mesh";
				string linkedMesh = "";
				if (render->Attribute(meshName.c_str())) {
					linkedMesh = render->Attribute("mesh");
				}
				else {
					return -1;
				}

				ofVec3f front;
				ofVec3f up;
				ofVec3f positionPixel;

				TiXmlElement* frontElement = render->FirstChildElement();
				if (!frontElement) {
					return -1;
				}

				TiXmlElement* upElement = frontElement->NextSiblingElement();
				if (!upElement) {
					return -1;
				}

				TiXmlElement* positionElement = upElement->NextSiblingElement();
				if (!positionElement) {
					return -1;
				}

				string xName = "x";
				string yName = "y";
				string zName = "z";

				float xFront = 0.0f;
				if (frontElement->Attribute(xName.c_str())) {
					string test = frontElement->Attribute("x");
					xFront = ofToFloat(frontElement->Attribute("x"));
					//cout << xFront << endl;
				}
				else {
					xFront = 0.0f;
				}

				float yFront = 0.0f;
				if (frontElement->Attribute(yName.c_str())) {
					yFront = ofToFloat(frontElement->Attribute("y"));
				}
				else {
					yFront = 0.0f;
				}

				float zFront = 0.0f;
				if (frontElement->Attribute(zName.c_str())) {
					zFront = ofToFloat(frontElement->Attribute("z"));
				}
				else {
					zFront = 0.0f;
				}

				float xUp = 0.0f;
				if (upElement->Attribute(xName.c_str())) {
					xUp = ofToFloat(upElement->Attribute("x"));
				}
				else {
					xUp = 0.0f;
				}

				float yUp = 0.0f;
				if (upElement->Attribute(yName.c_str())) {
					yUp = ofToFloat(upElement->Attribute("y"));
				}
				else {
					yUp = 0.0f;
				}

				float zUp = 0.0f;
				if (upElement->Attribute(zName.c_str())) {
					zUp = ofToFloat(upElement->Attribute("z"));
				}
				else {
					zUp = 0.0f;
				}

				float xPos = 0.0f;
				if (positionElement->Attribute(xName.c_str())) {
					string test = positionElement->Attribute("x");
					xPos = ofToFloat(test);
					//cout << xPos << endl;
				}
				else {
					xPos = 0.0f;
				}

				float yPos = 0.0f;
				if (positionElement->Attribute(yName.c_str())) {
					yPos = ofToFloat(positionElement->Attribute("y"));
				}
				else {
					yPos = 0.0f;
				}

				float zPos = 0.0f;
				if (positionElement->Attribute(zName.c_str())) {
					zPos = ofToFloat(positionElement->Attribute("z"));
				}
				else {
					zPos = 0.0f;
				}

				positionPixel.x = xPos;
				positionPixel.y = yPos;
				positionPixel.z = zPos;

				front.x = xFront;
				front.y = yFront;
				front.z = zFront;

				up.x = xUp;
				up.y = yUp;
				up.z = zUp;

				Pixel* newPixel = new Pixel(id, r, g, b, a, positionPixel);

				front.normalize();
				up.normalize();

				ofMatrix4x4 rotFront;

				ofVec3f sourceFront(1, 0, 0); //already normalized
				ofVec3f sourceUp(0, 0, 1); //already normalized

				ofQuaternion qFront;

				qFront.makeRotate(sourceFront, front);

				ofVec3f newSourceUp = rotFront * sourceUp;

				//cout << "New Up x:" << newUp.x << " y:" << newUp.y << " z:" << newUp.z << endl;

				ofQuaternion qUp;

				qUp.makeRotate(newSourceUp, up);

				ofQuaternion qTot = qFront; // * qUp esto anda mal. hay que multiplicar por la rotación up para que todo esté correcto

											// we save the rotation in the pixel

				newPixel->setRotQ(qTot);

				//now we link the mesh

				map<string, ofxAssimpModelLoader*>::iterator itModel = this->models->find(linkedMesh);

				if (itModel != this->models->end()) {
					ofxAssimpModelLoader* model = itModel->second;
					newPixel->setModel(model);
				}

				newPixel->setModelName(linkedMesh);

				newPixel->setFront(front);
				newPixel->setUp(up);

				this->pixels->insert(pair<int, Pixel*>(id, newPixel));
				this->pixelsFast->push_back(newPixel);

				if (pixelCounter == 0) {
					maxX = positionPixel.x;
					minX = positionPixel.x;
					maxY = positionPixel.y;
					minY = positionPixel.y;
					maxZ = positionPixel.z;
					minZ = positionPixel.z;
				}
				else {
					if (positionPixel.x>maxX) {
						maxX = positionPixel.x;
					}

					if (positionPixel.x<minX) {
						minX = positionPixel.x;
					}

					if (positionPixel.y>maxY) {
						maxY = positionPixel.y;
					}

					if (positionPixel.y<minY) {
						minY = positionPixel.y;
					}

					if (positionPixel.z>maxZ) {
						maxZ = positionPixel.z;
					}

					if (positionPixel.z<minZ) {
						minZ = positionPixel.z;
					}
				}

				pixelCounter++;
			}
		}
	}
	else {
		//error
		return -1;
	}
	return 1;

}


void GenericClientManager::draw() {
	ofBackground(50);
	ofSetColor(255, 255, 255);
	if (enableDraw) {

		specific->drawPre();

		cam.begin();

		if (showAxis) {
			//axis
			ofSetColor(255, 0, 0);
			ofSetLineWidth(3);
			ofLine(0, 0, 0, 100, 0, 0);
			ofSetColor(0, 255, 0);
			ofLine(0, 0, 0, 0, 100, 0);
			ofSetColor(0, 0, 255);
			ofLine(0, 0, 0, 0, 0, 100);
			//axis
		}

		glPushMatrix();

		float width = (float)ofGetWidth();
		float height = (float)ofGetHeight();

		vector<Pixel*>::iterator it = this->pixelsFast->begin();
		while (it != this->pixelsFast->end())
		{
			Pixel* px = *it;
			px->draw();
			it++;
		}
		glPopMatrix();

		specific->draw();

		cam.end();

		specific->drawPost();

	}
}


void GenericClientManager::exit() {
	if (this->tcpClient.isConnected()) {
		this->tcpClient.close();
	}
	this->specific->exit();
}
