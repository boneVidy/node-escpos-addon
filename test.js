    const addon = require('./build/Debug/addon.node');
    const iconv = require('iconv-lite');
    console.log(addon);
    const list  = addon.GetDeviceList('COM');
    console.log(list);
    const content = iconv.encode("你好啊\n halo cpp!\n", 'GB18030')
    console.log(addon.PrintRaw(list[0].path,content )) ;
    console.log('disconnect result is', addon.DisConnect(list[0].path));
    
    setTimeout(() =>{
        console.log(addon.PrintRaw(list[0].path,content )) ;
        console.log(addon.PrintRaw(list[0].path,content )) ;
        while(true){}
    }, 3000);

