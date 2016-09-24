#pragma once
#include "ofMain.h"
#include <string.h>


using namespace std;

class DTPixel
{
public:
	DTPixel(void);
	DTPixel(int iId,float iR, float iG, float iB,float iA, float iX, float iY, float iZ, ofVec3f iFront, ofVec3f iUp, string iMeshName);
	~DTPixel(void);
	float getR();
	float getG();
	float getB();
	float getA();
    ofVec3f getFront();
    ofVec3f getUp();
    string getMeshName();
	void setR(float iR);
	void setG(float iG);
	void setB(float iB);
	void setA(float iA);
    void setFront(ofVec3f iFront);
    void setUp(ofVec3f iUp);
    void setMeshName(string iName);
	int getId();
	float getPosX();
	float getPosY();
	float getPosZ();
	string toXML();
    string toXMLReduced();
private:
	int id;
	float R;
	float G;
	float B;
	float A;
	float X;
	float Y;
	float Z;
    ofVec3f front;
    ofVec3f up;
    string meshName;
};
