#include "escposPrint.h"
#include <windows.h>
#include <map>
using namespace std;

typedef struct HInfo {
	BOOL isInit;
	HANDLE handle;
} HandlerInfo;
typedef pair <string, HandlerInfo> HandlerInfo_Pair;

map<string, HandlerInfo> handlerMap;
//USB类的GUID
const GUID USB_GUID = { 0xa5dcbf10, 0x6530, 0x11d2,{ 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } };
BOOL PrintRawDataByLpt(string devicePath, char*  meg, size_t size, PrintResult *result)
{
	DWORD dwWrite;
	HANDLE hLPT = CreateFile(devicePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (hLPT == INVALID_HANDLE_VALUE) {
		SetPrintResult(result, FALSE, GetLastError());
		return FALSE;
	}
	BOOL b = WriteFile(
		hLPT,
		meg,
		(DWORD)size,
		&dwWrite,
		NULL);
	if (!b) {
		SetPrintResult(result, FALSE, GetLastError());
		return FALSE;
	}
	FlushFileBuffers(hLPT);
	CloseHandle(hLPT);
	SetPrintResult(result, TRUE, NULL);
	return TRUE;
}
BOOL DisConnectDevice(string devicePath) {
	HANDLE handle;
	HandlerInfo handlerInfo = HandlerInfo();
	if ((handlerMap.size()) ==  0) {
		return FALSE;
	}
	handlerInfo = handlerMap[devicePath];
	if (handlerInfo.isInit) {
	  BOOL	ret =  CloseHandle(handlerInfo.handle);
	  handlerInfo.isInit = !ret;
	  handlerMap.erase(devicePath);
	  return ret;
	}
	return FALSE;
}

BOOL PrintRawData(string devicePath, char*  meg, size_t size, PrintResult *result)
{
	// string endstr = "\x1D\x56\x41\x00";
	PrintDevice device;
	HANDLE handle;
	InitializeDevicePar(device);
	device.Port = devicePath;
	HandlerInfo handlerInfo = HandlerInfo();
	if ((handlerMap.size())>  0) {
	
		handlerInfo = handlerMap[(devicePath)];
	}
	if (  handlerInfo.isInit == FALSE) {
		handlerInfo =  HandlerInfo();
		// free(handle);
		handle = InitPort(device);
		handlerInfo.handle = handle;
		handlerInfo.isInit = TRUE;
		handlerMap.insert(HandlerInfo_Pair(devicePath, handlerInfo));
		// handlerMap.insert(device, handlerInfo);
	}
	

	BOOL r = WriteRawData(meg, handlerInfo.handle, size);
	if (!r)
	{
		SetPrintResult(result, FALSE, GetLastError());
		CloseHandle(handlerInfo.handle);
		handlerInfo.isInit = FALSE;
		return FALSE;
	}
	printf("write file result is %d", r);
	int err = GetLastError();
	printf("getlast err is %d", err);
	// CloseHandle(handle);
	SetPrintResult(result, TRUE, err);
	// Sleep(2000);
	return TRUE;
}

BOOL WriteRawData(const char * str, HANDLE hPort, size_t size)
{
	DWORD dwWrite;

	return WriteFile(hPort, str, (DWORD)size, &dwWrite, NULL);
}



void InitializeDevicePar(PrintDevice &device)
{
	device.Port = "COM4";
	device.BawdRate = 9600;
	device.DataBits = 8;
	device.StopBits = ONESTOPBIT;
	device.Parity = NOPARITY;  //NONE
	device.ReceiveBuffer = 256;
}
HANDLE InitPort(PrintDevice &device)
{
	HANDLE handle = CreateFile(device.Port.c_str(), GENERIC_READ | GENERIC_WRITE,
		0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (handle == INVALID_HANDLE_VALUE)
	{	// 打开端口失败
		return FALSE;
	}
	else
	{
		//设置端口缓冲
		SetupComm(handle, 1024, 1024);

		// 设定通讯端口超时参数
		COMMTIMEOUTS tmouts;
		tmouts.ReadIntervalTimeout = 100;
		tmouts.ReadTotalTimeoutMultiplier = 100;
		tmouts.ReadTotalTimeoutConstant = 100;
		tmouts.WriteTotalTimeoutConstant = 100;
		tmouts.WriteTotalTimeoutMultiplier = 100;
		SetCommTimeouts(handle, &tmouts);

		//设定通讯端口通讯参数
		DCB dcb;
		BOOL bol = TRUE;

		//dcb.DCBlength = sizeof(dcb);
		bol = GetCommState(handle, &dcb);
		dcb.BaudRate = device.BawdRate;
		dcb.ByteSize = device.DataBits;
		dcb.StopBits = device.StopBits;
		dcb.Parity = device.Parity;

		bol = SetCommState(handle, &dcb); //配置串口
										  // 清除通讯端口缓存
		PurgeComm(handle, PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT);

		// 初始化重叠IO对象
		OVERLAPPED m_OverlappedRead;
		OVERLAPPED m_OverlappedWrite;
		HANDLE m_hStopCommEvent;
		HANDLE m_hDataReady;
		memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
		m_OverlappedRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));
		m_OverlappedWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		// 初始化事件对象
		m_hStopCommEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_hDataReady = CreateEvent(NULL, FALSE, FALSE, NULL);

		//初始化打印ESC @
		DWORD iBytesLength;
		const char* chInitCode = "\x0D\x1B\x40";
		WriteFile(handle, chInitCode, (DWORD)3L, &iBytesLength, NULL);
		
		if (!WriteFile(handle, chInitCode, (DWORD)3L, &iBytesLength, NULL))
		{
			cout << "last err is " << GetLastError() << endl;
			return FALSE;

		}
	}

	return handle;
}
void SetPrintResult(PrintResult *result, BOOL success, DWORD errCode)
{
	result->success = success;
	result->err = errCode;
}