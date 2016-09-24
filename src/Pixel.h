#pragma once
#include "ofMain.h"
#include "DTPixel.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"

class Pixel
{
private:
	int id;
	ofVec3f position;
    ofxAssimpModelLoader* model;
    string modelName;
    ofQuaternion rot;
    ofVec3f front;
    ofVec3f up;
    
public:
    float r;
	float g;
	float b;
	float a;
	Pixel(void);
	Pixel(int iId, float iR, float iG, float iB, float iA, ofVec3f iPosition);
	~Pixel(void);
	float getId();
	float getR();
	float getG();
	float getB();
	float getA();
    string getModelName();
    ofVec3f getFront();
    ofVec3f getUp();
    void setFront(ofVec3f newFront);
    void setUp(ofVec3f newUp);
    ofQuaternion getRotQ();
    void setRotQ(ofQuaternion newQ);
    void setModel(ofxAssimpModelLoader* newModel);
    void setModelName(string newName);
	ofVec3f getPosition();
	void blendRGBA(float iR, float iG, float iB, float iA, float blendFactor);
	DTPixel* getDTPixel();
    void draw();
    
};
