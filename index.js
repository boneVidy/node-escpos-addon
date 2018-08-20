module.exports = function () {
    if (process.platform === 'win32') {
       return require('./build/Release/addon.node'); 
    }
    
    throw new Error("must be win32 platform to use");
}