#pragma once
#include <vector>
#include "DTPixel.h"
#include <string.h>
#include "ofxXmlSettings.h"

#define LOW_BYTE 0x00FF
#define HIGH_BYTE 0xFF00

// convert from shorts to bytes and back again
#define short_get_high_byte(x) ((HIGH_BYTE & x) >> 8)
#define short_get_low_byte(x)  (LOW_BYTE & x)

#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) ) ;

using namespace std;


#ifdef TARGET_OSX
//paquete binario de celebra
//el maximo de pixels por paquete es de 341
struct celebra_packet_s {
	uint8_t  id[8]; //8 bytes
	uint8_t  ver; // 9 bytes
	uint8_t  sequenceHi; //10bytes
	uint8_t  sequence; //11 bytes
	uint8_t  minIdHi; // 12 bytes
	uint8_t  minId; // 13 bytes
	uint8_t  maxIdHi; // 14bytes
	uint8_t  maxId; // 15 bytes
	uint8_t  lengthHi; //16 bytes
	uint8_t  length; //17 bytes
	uint8_t finishFrame; //18bytes
	uint8_t  data[1024];
}  __attribute__((packed));
#endif

#ifdef _WIN32
//paquete binario de celebra
//el maximo de pixels por paquete es de 341
#pragma pack(1)
struct celebra_packet_s {
	uint8_t  id[8]; //8 bytes
	uint8_t  ver; // 9 bytes
	uint8_t  sequenceHi; //10bytes
	uint8_t  sequence; //11 bytes
	uint8_t  minIdHi; // 12 bytes
	uint8_t  minId; // 13 bytes
	uint8_t  maxIdHi; // 14bytes
	uint8_t  maxId; // 15 bytes
	uint8_t  lengthHi; //16 bytes
	uint8_t  length; //17 bytes
	uint8_t finishFrame; //18bytes
	uint8_t  data[1024];
};
#endif


typedef struct celebra_packet_s celebra_packet_t;

class DTFrame
{
public:
	DTFrame(int iPixelQuantity);
	DTFrame(string sourceXML);
	DTFrame(long iTimeStamp, vector<DTPixel*>* iVectorPixels, int iPixelQuantity , unsigned short iSequence);
	~DTFrame(void);
	long getTimestamp();
	string getClientName();
	int getClientID();
    int getSequence();
	vector<DTPixel*>* getPixels();
	string toXML();
    string toXMLReduced();
    void addPixel(DTPixel* newPixel);
    void setSequence(int newSequence);
    celebra_packet_t getBinaryPacketFromFrame(unsigned short minId, unsigned short maxId, unsigned short sequence,unsigned short finishFrame);
private:
	long timestamp;
	int clientID;
    unsigned short sequence;
	string clientName;
	vector <DTPixel*>* pixels;
    int pixelQuantity;
	
};
