const addon = require('./build/Debug/addon.node');
const iconv = require('iconv-lite');

    // console.log(addon)
    
    // console.log(addon.GetUsbDeviceList());
    // console.log(addon.Print()); 

    const list  = addon.GetUsbDeviceList();
    
    // console.log(list);
    ///addon.PrintRaw(list[0].path, "你好啊\n halo cpp!\n");
    const printers = list.filter(item => item.service ==='usbprint' || item.deviceName==='USB 打印支持'|| item.service ==='WinUSB');
    console.log(printers[0]);
    const content = iconv.encode("你好啊\n halo cpp!\n", 'GB18030')
    const data = Buffer.from("\x1b\x61\x01你好啊\n halo cpp!\n");
    console.log(data);
    addon.PrintRaw(printers[0].path, content);
    

    while(true){}

