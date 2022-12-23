# zyxel-reset

This utility allows to reset ZyXEL networking devices such as access point remotely.

The same method is utilized by ZyXEL switches supporting their proprietary "Ethernet Neighbor" feature.

## Compatible Devices

 - NWA50AX
 - NWA55AXE

## Usage

1. Connect your computer to the target device using an ethernet cable. Don't turn on the device just yet.

2. Start the `zyxel-reset` utility. Use the name of the interface the target device is connected to as the first positional argument.

3. Power on the target-device while keeping it connected to your computer using ethernet.

4. The device will erase it's configuration furing bootup.
   Once you were able to login using the default credentials (Usually `admin - 1234`) you can stop `zyxel-reset`