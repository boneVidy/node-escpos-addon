#escpos native plugin for electron
#目前只支持 windows系统
```js
// usb printer
const iconv = require('iconv-lite');

const escpos =require('node-escpos-addon');
const usblist = escpos.GetUsbDeviceList();
const printer = usblist.find(item => item.service ==='usbprint' || item.deviceName==='USB 打印支持');
const content = iconv.encode("你好啊\n halo cpp!\\n\n\n\n\nn\n\n\n\n", 'GB18030');
const {success, err} = escpos.PrintRaw(printer.path, content);
```


```ts
// lpt printer
const iconv = require('iconv-lite');
const escpos =require('node-escpos-addon');
const lptList = escpos.GetLptDeviceList();
const printer = lptList[0];
const content = iconv.encode("你好啊\n halo cpp!\\n\n\n\n\nn\n\n\n\n", 'GB18030');
const {success, err} = escpos.PrintRaw(printer.path, content);

// disconnect
const isDisConnected = escpos.Disconnect(printer.path);

```