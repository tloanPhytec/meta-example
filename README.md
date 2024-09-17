# Branch imx7-kirkstone-consoleUART5

This branch provides a reference for changing the UART serial console on PHYTEC's phyCORE-i.MX7 BSP. The bootloader patch in this example adjusts the debug console UART from the UART1 to the UART5 interface.
The UART5 interface is accessible on the phyCORE-i.MX7 carrier board via the 2x5 male pin header (X2). 
This was tested with the phyCORE-i.MX7 BSP version PD23.1.1.

In order to evaluate this on your phyCORE-i.MX7 Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b imx7-kirkstone-consoleUART5
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake
