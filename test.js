const addon = require('./build/Debug/addon.node');
const iconv = require('iconv-lite');

    // console.log(addon)
    
    // console.log(addon.GetUsbDeviceList());
    // console.log(addon.Print()); 

    const list  = addon.GetUsbDeviceList();
    
    // console.log(list);
    ///addon.PrintRaw(list[0].path, "你好啊\n halo cpp!\n");
    const printers = list.filter(item => item.service ==='usbprint' || item.deviceName==='USB 打印支持');
    console.log(printers[0]);
    const content = iconv.encode("你好啊\n halo cpp!\n", 'GBK')
    const data = Buffer.from("\x1b\x61\x01你好啊\n halo cpp!\n");
    console.log(data);
    addon.PrintRaw(printers[0].path, content);
    // list.forEach(element => {
    //     addon.PrintRaw(element.path, "你好啊\n halo cpp!\n");
    // });
    // addon.PrintRaw(printer.path, "你好啊\n halo cpp!\n");
    // console.log(list);
    // try {
    //     list.array.forEach(element => {
    //         if (element) {
    //             addon.PrintRaw(element.path, "halohaojaodjoadfoawjdfoasudfoi\n");
                
    //         }
    //         // console.log(element.path)
    //     }); 
    // } catch (error) {
    //   consoel.error(error);  
    // }
    

    // while(true){}

