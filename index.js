const win32Escpos = function () {
  if (process.platform === 'win32') {
    const {GetDeviceList, PrintRaw} = require('./build/Release/addon.node');
    return {
      GetUsbDeviceList () {
        return GetDeviceList('USB');
      },
      GetLptDeviceList () {
        return GetDeviceList('LPT').map((item, index) => {
          const matchs = item.path.match(/lpt\d/);
          let path;
          if (matchs) {
            path = matchs[0].toUpperCase() + ':';
          }
          path = `LPT${index+1}:`;
          item.path = path;
          return item;
        });
      },
      GetDeviceList,
      PrintRaw 
    }
   }
   
   throw new Error("must be win32 platform to use");
}


module.exports = win32Escpos();