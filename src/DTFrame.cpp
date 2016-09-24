#include "DTFrame.h"

DTFrame::DTFrame(int iPixelQuantity)
{
	this->pixelQuantity=iPixelQuantity;
    this->pixels = new vector<DTPixel*>;
    this->clientID=0;
    this->clientName="Server";
    this->timestamp=0;
    this->sequence=0;
}

DTFrame::DTFrame(long iTimeStamp, vector<DTPixel*>* iVectorPixels, int iPixelQuantity ,  unsigned short iSequence)
{
	this->clientID=0;
	this->clientName="Server";
	this->timestamp=iTimeStamp;
	this->pixels=iVectorPixels;
    this->sequence=iSequence;
    this->pixelQuantity=iPixelQuantity;
}

DTFrame::DTFrame(string sourceXML)
{
	vector <DTPixel*> * newVector= new vector<DTPixel*>;
	this->pixels=newVector;

	ofxXmlSettings XML;
	XML.loadFromBuffer(sourceXML);
	int protocolVersion = 1;
	//if( XML.bDocLoaded ){
		TiXmlElement* myFrame=XML.doc.RootElement();
        int numPixelTags = XML.getNumTags("Pixel");
		if (myFrame){
			string timestampName = "timestamp";
			if (myFrame->Attribute(timestampName.c_str())){
				this->timestamp=ofToInt(myFrame->Attribute("timestamp"));
			}
			else{
				this->timestamp = 0;
			}
			string clientIDName = "clientID";
			if (myFrame->Attribute(clientIDName.c_str())){
				this->clientID=ofToInt(myFrame->Attribute("clientID"));
			}
			else{
				clientID = 0;
			}
			string clientNameName = "clientName";
			if (myFrame->Attribute(clientNameName.c_str())){
				this->clientName=myFrame->Attribute("clientName");
			}
			else{
				this->clientName="Unknown";
			}
			string protocolVersionName = "protocolVersion";
			if (myFrame->Attribute(protocolVersionName.c_str())){
				protocolVersion=ofToInt(myFrame->Attribute("protocolVersion"));
			}
			else{
				protocolVersion = 1;
			}
            if (protocolVersion==1){
                TiXmlElement* myPixel= myFrame->FirstChildElement();
                if (myPixel){
					int error = 0;
					int cont = 0;
					for( myPixel; myPixel; myPixel=myPixel->NextSiblingElement())
					{
						cont ++;
						string idName = "id";
						int id=0;
						if (ofToInt(myPixel->Attribute(idName.c_str()))){
							id = ofToInt(myPixel->Attribute("id"));
						}
						
						float r=0;
						string rName = "r";
						if (ofToInt(myPixel->Attribute(rName.c_str()))){
							r = ofToFloat(myPixel->Attribute("r"));
						}
						else{
							r=0;
						}

						float g=0;
						string gName = "g";
						if (ofToInt(myPixel->Attribute(gName.c_str()))){
							g = ofToFloat(myPixel->Attribute("g"));
						}
						else{
							g=0;
						}

						float b=0;
						string bName = "b";
						if (ofToInt(myPixel->Attribute(bName.c_str()))){
							b = ofToFloat(myPixel->Attribute("b"));
						}
						else{
							b=0;
						}

						float a=0;
						string aName = "a";
						if (ofToInt(myPixel->Attribute(aName.c_str()))){
							a = ofToFloat(myPixel->Attribute("a"));
						}
						else{
							a=255;
						}
                        
                        TiXmlElement* render=myPixel->FirstChildElement();
                        if (!render){
                            return;
                        }
                        
                        string meshName= "mesh";
                        string linkedMesh="";
                        if(render->Attribute(meshName.c_str())){
                            linkedMesh=render->Attribute("mesh");
                        }
                        else{
                            return;
                        }
                        
                        ofVec3f front;
                        ofVec3f up;
                        ofVec3f positionPixel;
                        
                        TiXmlElement* frontElement=render->FirstChildElement();
                        if (!frontElement){
                            return;
                        }
                        
                        TiXmlElement* upElement=frontElement->NextSiblingElement();
                        if (!upElement){
                            return;
                        }
                        
                        TiXmlElement* positionElement=upElement->NextSiblingElement();
                        if (!positionElement){
                            return;
                        }
                        
                        string xName = "x";
                        string yName = "y";
                        string zName = "z";
                        
                        float xFront=0.0f;
                        if (frontElement->Attribute(xName.c_str())){
                            string test = frontElement->Attribute("x");
                            xFront=ofToFloat(frontElement->Attribute("x"));
                            //cout << xFront << endl;
                        }
                        else{
                            xFront=0.0f;
                        }
                        
                        float yFront=0.0f;
                        if (frontElement->Attribute(yName.c_str())){
                            yFront=ofToFloat(frontElement->Attribute("y"));
                        }
                        else{
                            yFront=0.0f;
                        }
                        
                        float zFront=0.0f;
                        if (frontElement->Attribute(zName.c_str())){
                            zFront=ofToFloat(frontElement->Attribute("z"));
                        }
                        else{
                            zFront=0.0f;
                        }
                        
                        float xUp=0.0f;
                        if (upElement->Attribute(xName.c_str())){
                            xUp=ofToFloat(upElement->Attribute("x"));
                        }
                        else{
                            xUp=0.0f;
                        }
                        
                        float yUp=0.0f;
                        if (upElement->Attribute(yName.c_str())){
                            yUp=ofToFloat(upElement->Attribute("y"));
                        }
                        else{
                            yUp=0.0f;
                        }
                        
                        float zUp=0.0f;
                        if (upElement->Attribute(zName.c_str())){
                            zUp=ofToFloat(upElement->Attribute("z"));
                        }
                        else{
                            zUp=0.0f;
                        }
                        
                        float xPos=0.0f;
                        if (positionElement->Attribute(xName.c_str())){
                            string test = positionElement->Attribute("x");
                            xPos=ofToFloat(test);
                            //cout << xPos << endl;
                        }
                        else{
                            xPos=0.0f;
                        }
                        
                        float yPos=0.0f;
                        if (positionElement->Attribute(yName.c_str())){
                            yPos=ofToFloat(positionElement->Attribute("y"));
                        }
                        else{
                            yPos=0.0f;
                        }
                        
                        float zPos=0.0f;
                        if (positionElement->Attribute(zName.c_str())){
                            zPos=ofToFloat(positionElement->Attribute("z"));
                        }
                        else{
                            zPos=0.0f;
                        }
                        
                        positionPixel.x = xPos;
                        positionPixel.y = yPos;
                        positionPixel.z = zPos;
                        
                        front.x = xFront;
                        front.y = yFront;
                        front.z= zFront;
                        
                        up.x = xUp;
                        up.y = yUp;
                        up.z = zUp;

						//DTPixel* newPixel = new DTPixel(id,r,g,b,a,x,y,z);
						DTPixel* newPixel = new DTPixel(id,r,g,b,a,positionPixel.x,positionPixel.y,positionPixel.z,front,up,linkedMesh);

						newVector->push_back(newPixel);
                        
 					}
					if (error==1){
						//error
						this->clientID = -1;
					}
                }
				else{
                    
                    //error
                    this->clientID = -1;
					                    
                    
                    
				}
            }
			
			else{
				//el hijo es un nodo CData
                //busqué en internet... y hay algunos problemas al guardar binario en cdata..,
                //hay problemas con: /0, y algún binario que forme ]]> sin querer.
                
                string dataToParse = myFrame->GetText();
                vector<string> parserPixels=ofSplitString(dataToParse, "|");
                for (int i=0; i<parserPixels.size(); i++){
                    string pixelData=parserPixels[i];
                    vector<string> splittedData= ofSplitString(pixelData,";");
                    int parsedId =  ofToFloat(ofSplitString(splittedData[0],":")[1]);
                    float parsedR = ofToFloat(ofSplitString(splittedData[1],":")[1]);
                    float parsedG = ofToFloat(ofSplitString(splittedData[2],":")[1]);
                    float parsedB = ofToFloat(ofSplitString(splittedData[3],":")[1]);
                    float parsedA = ofToFloat(ofSplitString(splittedData[4],":")[1]);
                    ofVec3f dummyFront(0,0,0);
                    ofVec3f dummyUp(0,0,0);
                    
                    DTPixel* newPixel = new DTPixel(parsedId,parsedR,parsedG,parsedB,parsedA,0,0,0,dummyFront,dummyUp,"dummy");
                    
                    newVector->push_back(newPixel);
                    
                }

			}
		}


	//}else{
		//error
	//	this->clientID = -1;
	//}
}

DTFrame::~DTFrame(void)
{
	vector<DTPixel*>::iterator it = this->pixels->begin();
	while (it != this->pixels->end())
	{	
		DTPixel* px = *it;
		delete px;
		it++;
	}
	delete this->pixels;
}

void DTFrame::addPixel(DTPixel *newPixel){
    this->pixels->push_back(newPixel);
}

long DTFrame::getTimestamp()
{
	return this->timestamp;
}

int DTFrame::getSequence()
{
	return this->sequence;
}

string DTFrame::toXML()
{
	DTPixel* auxPixel;
	string result="";
	for (int i = 0 ; i<this->pixels->size(); i++)
	{
		auxPixel= (DTPixel*) this->pixels->at(i);
		result= result + auxPixel->toXML();
	}
	
	return "<Frame timeStamp='" + ofToString(this->timestamp) + "' clientID='-1' clientName=''>" + result + "</Frame>";
}

string DTFrame::toXMLReduced(){
    DTPixel* auxPixel;
	string result="";
	for (int i = 0 ; i<this->pixels->size(); i++)
	{
        auxPixel= (DTPixel*) this->pixels->at(i);
        if (i==0){
            result= result + auxPixel->toXMLReduced();
        }
        else{
            result= result + "|" + auxPixel->toXMLReduced();
        }
		
		
	}
	
	return "<Frame timeStamp='" + ofToString(this->timestamp) + "' clientID='-1' clientName=''><![CDATA[" + result + "]]></Frame>";
}


string DTFrame::getClientName()
{
	return this->clientName;
}

vector<DTPixel*>* DTFrame::getPixels()
{
	return this->pixels;
}

int DTFrame::getClientID(){
	return this->clientID;
}

void DTFrame::setSequence(int newSequence){
    this->sequence=newSequence;
}

celebra_packet_t DTFrame::getBinaryPacketFromFrame(unsigned short minId, unsigned short maxId, unsigned short sequence,unsigned short finishFrame){
    celebra_packet_t result;
    
    result.minIdHi = short_get_high_byte(minId);
    result.minId = short_get_low_byte(minId);
    
    result.maxIdHi = short_get_high_byte(maxId);
    result.maxId = short_get_low_byte(maxId);
    
    result.sequenceHi = short_get_high_byte(sequence);
    result.sequence = short_get_low_byte(sequence);
    
    result.finishFrame = short_get_low_byte(finishFrame);
    memcpy(&result.id, "Celebra1", 8);
    result.ver = 1;
    
    unsigned short dataIndex = 0;
    bool hasData = false;
    
    for (int q=minId; q<=maxId; q++){
        //busco el pixel y convierto sus datos
        bool find=false;
        int i=0;
        DTPixel* myPixel=0;
        while (!find && i<this->pixels->size()){
            myPixel = this->pixels->operator[](i);
            if (myPixel->getId()==q){
                find=true;
            }
            i++;
        }
        if (find){
            unsigned short R = (unsigned short) myPixel->getR();
            unsigned short G = (unsigned short) myPixel->getG();
            unsigned short B = (unsigned short) myPixel->getB();
            
            result.data[dataIndex] = short_get_low_byte(R);
            dataIndex++;
            result.data[dataIndex] = short_get_low_byte(G);
            dataIndex++;
            result.data[dataIndex] = short_get_low_byte(B);
            hasData=true;
            dataIndex++;
        }
    }
    
    result.lengthHi = short_get_high_byte(dataIndex);
    result.length = short_get_low_byte(dataIndex);
    
    return result;
    
}