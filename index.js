const win32Escpos = function () {
  if (process.platform === 'win32') {
    const {GetUsbDeviceList, PrintRaw} = require('./build/Release/addon.node');
    return {
      GetUsbDeviceList,
      PrintRaw 
    }
   }
   
   throw new Error("must be win32 platform to use");
}


module.exports = win32Escpos();