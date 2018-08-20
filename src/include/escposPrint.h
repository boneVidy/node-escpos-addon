#pragma once

#include <windows.h>
#include <string>
#include <IOSTREAM>
#include <winioctl.h>
#include <setupapi.h>
#include <tchar.h>
#include <stdio.h>
#include <winnt.h>
#pragma comment(lib, "setupapi.lib")

using namespace std;

class PrintDevice
{
  public:
    string Port;  //串口端口号
	  int BawdRate;//波特率
	  int DataBits;  //数据位
	  char Parity;  //校验位
	  int ReceiveBuffer;  //缓冲区
	  int StopBits;//停止位
    //HANDLE DHandler;
};

// SetupDiGetInterfaceDeviceDetail所需要的输出长度，定义足够大
#define INTERFACE_DETAIL_SIZE    (1024)

//设备数量上限，假设16台上限
#define MAX_DEVICE 16
__declspec(dllexport)  int PrintRawData(string devicePath, const string & meg);
int WriteRawData(const char * str, HANDLE hPort);
HANDLE InitPort(PrintDevice &device);
void InitializeDevicePar(PrintDevice &device);