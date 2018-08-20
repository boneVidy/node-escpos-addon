#include "getDeviceList.h"
using namespace std;


const GUID USB_GUID = { 0xa5dcbf10, 0x6530, 0x11d2,{ 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } };
typedef BOOL (WINAPI *FN_SetupDiGetDevicePropertyW)(
  __in       HDEVINFO DeviceInfoSet,
  __in       PSP_DEVINFO_DATA DeviceInfoData,
  __in       const DEVPROPKEY *PropertyKey,
  __out      DEVPROPTYPE *PropertyType,
  __out_opt  PBYTE PropertyBuffer,
  __in       DWORD PropertyBufferSize,
  __out_opt  PDWORD RequiredSize,
  __in       DWORD Flags
);
string Unicode2Utf8(const wchar_t* unicode)
{
  int len;
  len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, NULL, 0, NULL, NULL);
  char *szUtf8 = (char*)malloc(len + 1);
  memset(szUtf8, 0, len + 1);
  WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, szUtf8, len, NULL, NULL);
  string res(szUtf8);
  free(szUtf8);
  return res;
}
void GetDeviceList(list<DeviceInfo> &devicelist)
{
  const LPGUID lpGuid = (LPGUID)&USB_GUID;
  HDEVINFO hDevInfoSet;
  SP_DEVINFO_DATA spDevInfoData;
  SP_DEVICE_INTERFACE_DATA ifData;
  PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
  int nCount;
  int nTotle;
  BOOL bResult;
  // 取得一个该GUID相关的设备信息集句柄
  hDevInfoSet = ::SetupDiGetClassDevs(lpGuid,     // class GUID 
    NULL,                    // 无关键字 
    NULL,                    // 不指定父窗口句柄 
    DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);    // 目前存在的设备

                                               // 失败...
  if (hDevInfoSet == INVALID_HANDLE_VALUE)
  {
    printf_s("invalid handler");
    return ;
  }
  // 申请设备接口数据空间
  pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)::GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);

  pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

  nTotle = -1;
  nCount = 0;
  bResult = TRUE;
  printf_s("pdetail pointer is %p\n", pDetail);
  printf_s("pdetail cbsize is %d\n", pDetail->cbSize);
  // 设备序号=0,1,2... 逐一测试设备接口，到失败为止
  while (bResult)
  {
    nTotle++;
    spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // 枚举符合该GUID的设备接口
    bResult = ::SetupDiEnumDeviceInfo(
      hDevInfoSet,     // 设备信息集句柄
      (ULONG)nTotle,   // 设备信息集里的设备序号
      &spDevInfoData);        // 设备接口信息

    if (bResult)
    {
      DWORD DataT;
      char buf[MAX_PATH];
      char serviceBuf[MAX_PATH];
      char nameBuf[MAX_PATH];
      DWORD nSize = 0;
      // SPDRP_SERVICE  DEVPKEY_Device_BusReportedDeviceDesc
      if (SetupDiGetDevicePropertyW(hDevInfoSet, &spDevInfoData,
        &DEVPKEY_Device_BusReportedDeviceDesc, &DataT, (PBYTE)nameBuf, sizeof(nameBuf), &nSize, 0))
      {

      }
      else
      {
        lstrcpy(nameBuf, _T("Unknown"));
      }

      if (SetupDiGetDeviceRegistryProperty(hDevInfoSet, &spDevInfoData,
        SPDRP_SERVICE, &DataT, (PBYTE)serviceBuf, sizeof(serviceBuf), &nSize))
      {

      }
      else
      {
        lstrcpy(serviceBuf, _T("Unknown"));
      }
      // get Friendly Name or Device Description
      if (SetupDiGetDeviceRegistryProperty(hDevInfoSet, &spDevInfoData,
        SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buf, sizeof(buf), &nSize)) {
      }
      else if (SetupDiGetDeviceRegistryProperty(hDevInfoSet, &spDevInfoData,
        SPDRP_DEVICEDESC, &DataT, (PBYTE)buf, sizeof(buf), &nSize)) {
      }
      else {
        lstrcpy(buf, _T("Unknown"));
      }

      //是否是要找的设备类型
      //if (strcmp(buf, _T("USB 打印支持")) != 0)
      //continue;

      ifData.cbSize = sizeof(ifData);

      // 枚舉符合該GUID的設備接口
      bResult = ::SetupDiEnumDeviceInterfaces(
        hDevInfoSet,     // 設備信息集句柄
        NULL,            // 不需額外的設備描述
        lpGuid,          // GUID
        (ULONG)nTotle,   // 設備信息集里的設備序號
        &ifData);        // 設備接口信息

      if (bResult)
      {
        // 取得该设备接口的细节(设备路径)
        bResult = SetupDiGetInterfaceDeviceDetail(
          hDevInfoSet,    // 设备信息集句柄
          &ifData,        // 设备接口信息
          pDetail,        // 设备接口细节(设备路径)
          INTERFACE_DETAIL_SIZE,    // 输出缓冲区大小
          NULL,           // 不需计算输出缓冲区大小(直接用设定值)
          NULL);          // 不需额外的设备描述

        if (bResult)
        {
          DeviceInfo deviceinfo = { buf,  pDetail->DevicePath, serviceBuf,  Unicode2Utf8((const WCHAR *)nameBuf)  };
          devicelist.push_back(deviceinfo);
          // 复制设备路径到输出缓冲区
          //::strcpy_s(pszDevicePath[nCount], 256, pDetail->DevicePath);
          // 调整计数值
          nCount++;
        }
      }
    }
  }

  // 释放设备接口数据空间
  ::GlobalFree(pDetail);

  // 关闭设备信息集句柄
  ::SetupDiDestroyDeviceInfoList(hDevInfoSet);
}