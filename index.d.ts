export interface EscposPrinter  {
    name: string;
    desc: string;
    path: string;
    service: string | 'usbprint' | 'usbccgp' | 'libusb0' |'winusb';
}

export function GetUsbDeviceList   (): EscposPrinter[];
export function PrintRaw  (path: string, rowBf: Buffer): void


