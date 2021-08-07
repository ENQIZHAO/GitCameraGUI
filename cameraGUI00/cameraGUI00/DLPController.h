#pragma once
typedef unsigned int uint;
typedef unsigned char uchar;


#pragma execution_character_set("utf-8")
namespace DLPC {

#define RED		1
#define GREEN	2
#define BLUE	4

}
class DLPController
{
public:
	DLPController();
	~DLPController();
	bool USBconnect();
	bool USBdisConnect();
	bool addCommond(int TrigType, int imageNum, int PatNum, int LEDSelect, bool trigOutPrev);
	bool sendCommond(bool isRepeat, uint expTime, uint PatPeiod);
	bool startplay();
	bool pauseplay();
	bool stopplay();
	bool clearCommond();
	bool getPicNum(unsigned int &num);
	int getImageIndex();
	bool setLEDbrightness(unsigned int RedCurrent, unsigned int GreenCurrent, unsigned int BlueCurrent);
	uchar  PICIndex[12] ;
private:
	uint commondCount = 0;
 	

};
