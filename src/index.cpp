// hello.cc
#include <nan.h>
//#include "escpos.h"
#include "getDeviceList.h"
#include "escposPrint.h"
#include <list>
using namespace std;
// #pragma comment(lib, "ConsoleApplication1.lib")
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Array;
using v8::Exception;
using v8::ArrayBuffer;
static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string, string const& base64_chars) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}
string Utf8ToGbk(const std::string& strUtf8)//传入的strUtf8是UTF-8编码
{
  //UTF-8转unicode
  int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
  wchar_t * strUnicode = new wchar_t[len];//len = 2
  wmemset(strUnicode, 0, len);
  MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, strUnicode, len);

  //unicode转gbk
  len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
  char *strGbk = new char[len];//len=3 本来为2，但是char*后面自动加上了\0
  memset(strGbk, 0, len);
  WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strGbk, len, NULL, NULL);

  std::string strTemp(strGbk);//此时的strTemp是GBK编码
  delete[]strUnicode;
  delete[]strGbk;
  strUnicode = NULL;
  strGbk = NULL;
  return strTemp;
}

string GbkToUtf8(const std::string& strGbk)//传入的strGbk是GBK编码
{
  //gbk转unicode
  int len = MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, NULL, 0);
  wchar_t *strUnicode = new wchar_t[len];
  wmemset(strUnicode, 0, len);
  MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, strUnicode, len);

  //unicode转UTF-8
  len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
  char * strUtf8 = new char[len];
  WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, strUtf8, len, NULL, NULL);

  std::string strTemp(strUtf8);//此时的strTemp是UTF-8编码
  delete[]strUnicode;
  delete[]strUtf8;
  strUnicode = NULL;
  strUtf8 = NULL;
  return strTemp;
}
// void Print(const FunctionCallbackInfo<Value>& args) {
//   PrintRaw("你好");
//   Isolate* isolate = args.GetIsolate();
//   args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
// }
void GetUsbDeviceList(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = args.GetIsolate();
  list<DeviceInfo>  deviceList;
  GetDeviceList(deviceList);
  list<DeviceInfo>::iterator itor = deviceList.begin();
  Local<Array> resultArr = Array::New(isolate, deviceList.size());
  int count = 0;
  while (itor != deviceList.end())
  {
    
    Local<Object> info = Object::New(isolate);
    
    Local<String> pathKey = String::NewFromUtf8(isolate, "path");
    Local<String> pathValue = String::NewFromUtf8(isolate, (itor)->path.c_str());
    info->Set(pathKey, pathValue);
    info->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, (itor->name).c_str()));
    info->Set(String::NewFromUtf8(isolate,"desc"), String::NewFromUtf8(isolate, GbkToUtf8(itor->desc).c_str()));
    info->Set(String::NewFromUtf8(isolate, "service"), String::NewFromUtf8(isolate, GbkToUtf8(itor->service).c_str()));
    resultArr->Set(count, info);
    count++;
    itor++;
  }
  args.GetReturnValue().Set(resultArr);
}
/*
void PrintRaw(const FunctionCallbackInfo<Value>& args)
{
  printf(".....................................................");
  
  Isolate* isolate = args.GetIsolate();
  if (args.Length() < 2) 
  {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate,"Wrong number of arguments, must be 2")));
    return;
  }
  
  
  if (!args[0] ->IsString())
  {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "the first argument must be a string")));
    return;
  }
  
  if (!args[1]->IsString())
  {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "the second argument must be a string")));
    return;
  }
  Local<String> devicePath = args[0]->ToString();
  Local<String> raw = args[1]->ToString();
  char* deviceBf = (char *)malloc(devicePath->Utf8Length()+1);
  if (deviceBf == nullptr) {
    return;
  }
  char* rawBf = (char *)malloc(raw->Utf8Length()+1);
  if (rawBf == nullptr) {
    free(deviceBf);
    return;
  }
  devicePath->WriteUtf8(deviceBf, devicePath->Utf8Length());
  deviceBf[devicePath->Utf8Length()] = 0;
  raw->WriteUtf8(rawBf, raw->Utf8Length());
  rawBf[raw->Utf8Length()] = 0;
  PrintRawData(deviceBf, Utf8ToGbk(rawBf).c_str());
  free(deviceBf);
  free(rawBf);
  
}
*/
void PrintRaw(const FunctionCallbackInfo<Value>& args)
{
	printf(".....................................................");

	Isolate* isolate = args.GetIsolate();
	if (args.Length() < 2)
	{
		isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Wrong number of arguments, must be 2")));
		return;
	}


	if (!args[0]->IsString())
	{
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "the first argument must be a string")));
		return;
	}

	if (!args[1]->IsObject())
	{
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "the second argument must be a buffer")));
		return;
	}
	Local<String> devicePath = args[0]->ToString();
	Local<String> raw = args[1]->ToString();
	Local<Object> bufferObj = args[1]->ToObject();
	size_t bufferLength = node::Buffer::Length(bufferObj);
	char* bfData = node::Buffer::Data(bufferObj);
	printf("bf data is %s", bfData);
	char* deviceBf = (char *)malloc(devicePath->Utf8Length() + 1);
	if (deviceBf == nullptr) {
		return;
	}
	
	devicePath->WriteUtf8(deviceBf, devicePath->Utf8Length());
	deviceBf[devicePath->Utf8Length()] = 0;
	PrintRawData(deviceBf, bfData);
	free(deviceBf);
	// free(rawBf);

}
void Initialize(Local<Object> exports) {
  // NODE_SET_METHOD(exports, "Print", Print);
  NODE_SET_METHOD(exports, "GetUsbDeviceList", GetUsbDeviceList);
  NODE_SET_METHOD(exports, "PrintRaw", PrintRaw);
}



NODE_MODULE(addon, Initialize)