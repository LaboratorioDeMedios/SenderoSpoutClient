#include "DTPixel.h"

DTPixel::DTPixel(void)
{
	this->id=-1;
	this->R=0;
	this->G=0;
	this->B=0;
	this->A=0;
	this->X=0;
	this->Y=0;
	this->Z=0;
}

DTPixel::~DTPixel(void)
{
}

DTPixel::DTPixel(int iId,float iR, float iG, float iB,float iA, float iX, float iY, float iZ, ofVec3f iFront, ofVec3f iUp, string iMeshName){
	this->id=iId;
	this->R=iR;
	this->G=iG;
	this->B=iB;
	this->A=iA;
	this->X=iX;
	this->Y=iY;
	this->Z=iZ;
    this->front=iFront;
    this->up=iUp;
    this->meshName=iMeshName;
}

ofVec3f DTPixel::getFront(){
    return this->front;
}
ofVec3f DTPixel::getUp(){
    return this->up;
}

string DTPixel::getMeshName(){
    return this->meshName;
}

void DTPixel::setFront(ofVec3f iFront){
    this->front=iFront;
}

void DTPixel::setUp(ofVec3f iUp){
    this->up=iUp;
}

void DTPixel::setMeshName(string iName){
    this->meshName=iName;
}
	
float DTPixel::getR(){
	return this->R;
}
float DTPixel::getG(){
	return this->G;
}
float DTPixel::getB(){
	return this->B;
}
float DTPixel::getA(){
	return this->A;
}
int DTPixel::getId(){
	return this->id;
}
float DTPixel::getPosX(){
	return this->X;
}
float DTPixel::getPosY(){
	return this->Y;
}
float DTPixel::getPosZ(){
	return this->Z;
}

string DTPixel::toXML(){
	string res= "<Pixel id='" + ofToString(this->id) + "' r='" + ofToString(this->R) + "' g='" + ofToString(this->G) +  "' b='" + ofToString(this->B) +  "' a='" + ofToString(this->A) +  "' >";
    res += "<Render mesh='" + this->meshName + "'>";
    res += "<Front x='" + ofToString(this->front.x) +  "' y='" + ofToString(this->front.y) +  "' z='" + ofToString(this->front.z) +  "'/>";
    res += "<Up x='" + ofToString(this->up.x) +  "' y='" + ofToString(this->up.y) +  "' z='" + ofToString(this->up.z) +  "'/>";
    res += "<Position x='" + ofToString(this->X) +  "' y='" + ofToString(this->Y) +  "' z='" + ofToString(this->Z) +  "'/>";
    res += "</Render>";
    res += "</Pixel>" ;
    return res;
    
}

//not used for configuration purposes
string DTPixel::toXMLReduced(){
    return "id:" + ofToString(this->id) + ",r:" + ofToString(this->R) + ",g:" + ofToString(this->G) +  ",b:" + ofToString(this->B) +  ",a:" + ofToString(this->A) +  ",x:" + ofToString(this->X) +  ",y:" + ofToString(this->Y) +  ",z:" + ofToString(this->Z);
}


void DTPixel::setR(float iR){
	this->R = iR;
}
void DTPixel::setG(float iG){
	this->G=iG;
}
void DTPixel::setB(float iB){
	this->B=iB;
}
void DTPixel::setA(float iA){
	this->A = iA;
}