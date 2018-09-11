export interface EscposPrinter  {
    name: string;
    desc: string;
    path: string;
    service: string | 'usbprint' | 'usbccgp' | 'libusb0' |'winusb';
}
export type DeviceType = 'usb' |'com/lpt'
export function GetUsbDeviceList   (type?: DeviceType): EscposPrinter[];
export function PrintRaw  (path: string, rowBf: Buffer): void


