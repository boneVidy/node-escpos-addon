#include <windows.h>
#include <string>
#include <winioctl.h>
#include <setupapi.h>
#include <tchar.h>
#include <stdio.h>
#include <list>
#include <setupapi.h>
#include <initguid.h>
#include <devpropdef.h> 
#include <devpkey.h>
#pragma comment(lib, "setupapi.lib")
using namespace std;
// #include "escp
// SetupDiGetInterfaceDeviceDetail所需要的输出长度，定义足够大
#define INTERFACE_DETAIL_SIZE    (1024)
struct DeviceInfo
{
  string desc;
  string path;
  string service;
  string name;
};
// 
// typedef list<DeviceInfo> DeviceInfoList;
void GetDeviceList(list<DeviceInfo> &devicelist);