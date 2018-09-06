escpos native plugin for electron
```js
const iconv = require('iconv-lite');

const escpos =require('node-escpos-addon');
const usblist = escpos.GetUsbDeviceList();
const printer = usblist.find(item => item.service ==='usbprint' || item.deviceName==='USB 打印支持');
const content = iconv.encode("你好啊\n halo cpp!\\n\n\n\n\nn\n\n\n\n", 'GB18030');
escpos.PrintRaw(printer.path, content);
```