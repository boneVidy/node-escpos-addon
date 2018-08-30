    const addon = require('./build/Debug/addon.node');
    const iconv = require('iconv-lite');

    const list  = addon.GetUsbDeviceList();
    console.log(list);
    const printers = list.filter(item => item.service ==='usbprint' || item.deviceName==='USB 打印支持'|| item.service ==='WinUSB');
    const content = iconv.encode("你好啊\n halo cpp!\\n\n\n\n\nn\n\n\n\n", 'GB18030')
   
    addon.PrintRaw(printers[0].path, content);
    

    while(true){}

