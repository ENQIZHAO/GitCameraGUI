#include "DLPController.h"
#include "dlpc350_usb.h"
#include "dlpc350_api.h"
#include <time.h>
#include <hidapi.h>
#include <Windows.h>
#include <iostream>
#include <string>

inline void delay_msec(int msec)
{
	clock_t now = clock();
	while (clock() - now < msec);
}

DLPController::DLPController()
{
	DLPC350_USB_Init();
;
	//throw gcnew System::NotImplementedException();
}

DLPController::~DLPController()
{
	//throw gcnew System::NotImplementedException();
}

bool DLPController::USBconnect()
{
	//DLPC350_USB_Close();
	DLPC350_USB_Open();
	if (DLPC350_USB_IsConnected())
	{
		bool mode = false;
		uint patMode = 0;
		DLPC350_GetMode(&mode);
		if (mode == false)
		{
			//如果不是单张放映模式切换为单张放映模式
			DLPC350_SetMode(true);
			delay_msec(100);
			for(int i=0;i<5;i++)
			{
				//循环确认模式是否切换成功
				DLPC350_GetMode(&mode);
				if (mode)
					break;
				delay_msec(100);
				if (i > 5)
					break;
			}
		}
		//如果目前就是单张放映模式，检查是否DLP正在动作，如果正在动作，给出命令阻止DLP继续执行指令
		else
		{
			DLPC350_GetPatternDisplay(&patMode);
			if (patMode != 0)
			{
				int i = 0;
				unsigned int patMode;

				DLPC350_PatternDisplay(0);
				delay_msec(100);
				while (1)
				{
					DLPC350_GetPatternDisplay(&patMode);
					if (patMode == 0)
						break;
					else
						DLPC350_PatternDisplay(0);
					delay_msec(100);
					if (i++ > 5)
						break;
				}
			}
		}
		
		//切换放映触发器为内部触发
		DLPC350_SetPatternTriggerMode(0x01); //Trigger Mode - 1
		//切换图片数据源为内部闪存
		DLPC350_SetPatternDisplayMode(false);
		return true;
	}
	else
	{
		return false;

	}
}

bool DLPController::USBdisConnect()
{
	DLPC350_USB_Close();
	DLPC350_USB_Exit();
	return false;
}

bool DLPController::addCommond(int TrigType, int imageNum, int PatNum, int LEDSelect, bool trigOutPrev)
{
	if (commondCount >= 12)
	{
		//OutputDebugString("can't add more commond!The max count of commond is 12\n");
		return false;
	}

	//压入新命令
	//DLPC350_LoadImageIndex(imageNum);
	if (DLPC350_AddToPatLut(TrigType, PatNum, 1, LEDSelect, 0, 1, 1, trigOutPrev)<0)
	{
		//OutputDebugString( "Add commond failed!\n");
		return false;
	}
	else
	{
		PICIndex[commondCount] = imageNum;
		/*int a =imageNumList[commondCount];*/
		commondCount++;
		return true;
	}
	

	
}

bool DLPController::sendCommond(bool isRepeat, uint expTime, uint PatPeiod)
{
	bool ready = false;
	uint status;
	//设置从闪存读取图片
	if (DLPC350_SetPatternDisplayMode(false)<0)
	{
		//OutputDebugString("Display config failed!\n");
		return false;

	}

	//设置动作
	if (DLPC350_SetPatternConfig(commondCount, isRepeat, 1, commondCount)<0)
	{
		//OutputDebugString("Pattren config failed!\n");
		return false;
	}
	//设置图片标号计数


	//设置曝光时间
	if (DLPC350_SetExposure_FramePeriod(expTime, PatPeiod)<0)
	{
		//OutputDebugString("ExpTime config failed!\n");
		return false;
	}
	//设置触发模式
	if (DLPC350_SetPatternTriggerMode(1)<0)
	{
		//OutputDebugString("Trigger config failed!\n");
		return false;
	}
	//发送命令
	if (DLPC350_SendPatLut()<0)
	{
		//OutputDebugString("send commond failed!\n");
		return false;
	}
	if (DLPC350_SendImageLut(&PICIndex[0],commondCount)<0)
	{
		//OutputDebugString("Pattren config failed!\n");
		return false;
	}
	
	//开始验证命令序列
	if (DLPC350_StartPatLutValidate()<0)
	{
		//OutputDebugString("Start validate failed!\n");
		return false;
	}
	for(int i=0;i<5;i++)
	{
		if (DLPC350_CheckPatLutValidate(&ready, &status) < 0)
		{
			if (i < 4)
			{
				//OutputDebugString(std::string("Validating luadata is not ready,retry..").append(std::to_string(i)+"\n").c_str());

			}
			else
			{
				//OutputDebugString("Can't validate luadata\n");
				return false;
			}
			delay_msec(100);
		}
		if (ready)
		{
			break;
		}
	}





	return true;
	;
}

bool DLPController::startplay()
{
	if (DLPC350_PatternDisplay(2)<0)
	{
		return false;

	}
	else
	{
		return true;
	}
}

bool DLPController::pauseplay()
{
	if (DLPC350_PatternDisplay(1) < 0)
	{
		return false;

	}
	else
	{
		return true;
	}
}

bool DLPController::stopplay()
{

	if (DLPC350_PatternDisplay(0) < 0)
	{
		return false;

	}
	else
	{
		return true;
	}

}

bool DLPController::clearCommond()
{
	//清除DLP中的旧命令
	if (DLPC350_ClearPatLut() < 0)
	{
		//OutputDebugString("Can't clear Patlut!");
		return false;
	}
	else
	{
		commondCount = 0;
		return true;
	}
}

bool DLPController::getPicNum(unsigned int &num)
{
	if (DLPC350_GetNumImagesInFlash(&num))
	{

		return false;
	}
	else
	{
		return true;
	}
}

int DLPController::getImageIndex()
{
	uint imageIndex;
	DLPC350_GetImageIndex(&imageIndex);


	return imageIndex;
}

bool DLPController::setLEDbrightness(unsigned int RedCurrent, unsigned int GreenCurrent, unsigned int BlueCurrent)
{
	//光亮度设置0-255
	DLPC350_SetLedEnables(1, 1, 0, 0);
	DLPC350_SetLEDPWMInvert(1);
	return DLPC350_SetLedCurrents(RedCurrent, GreenCurrent, BlueCurrent)==0;
}
