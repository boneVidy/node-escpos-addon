export interface EscposPrinter  {
    name: string;
    desc: string;
    path: string;
    service: string | 'usbprint' | 'usbccgp' | 'libusb0' |'winusb' | "Unknown";
}
export type PrintResult  = {
    success: boolean;
    /*err is  c/c++ GetLastError()'s result;*/
    err: number
};
export type DeviceType = 'USB' |'LPT' | 'COM'
export function GetUsbDeviceList (): EscposPrinter[];
export function GetLptDeviceList(): EscposPrinter[];
export function GetComDeviceList(): EscposPrinter[];
export function GetDeviceList(type?:DeviceType): EscposPrinter[];
export function DisConnect(path:string): boolean;

/**
 * 
 * @param {string} path 
 * @param {Buffer} rowBf 
 * @returns {PrintResult}
 * @description ;
 */
export function PrintRaw  (path: EscposPrinter['path'], rowBf: Buffer): PrintResult


