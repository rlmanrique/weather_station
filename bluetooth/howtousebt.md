# Connect to BT module using Raspberry pi

1. Pair the device using bluetoothctrl
2. Scan it again. Use hcitool scan
3. Connect using RFCOMM rfcomm connect 1 MAC 1 
4. Read data in /dev/rfcommX

Important:

* Use stty to set the correct baud rate: stty 19200 /dev/rfcomm0
* Tehre are other interesnting tools like hciconfig
