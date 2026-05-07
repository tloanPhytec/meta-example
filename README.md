# Branch am67-scarthgap-SPI

This branch serves as a fix for evaluating SPI on the phyCORE-AM67x Development Kit. 

Currently, the BSP-Yocto-Ampliphy-AM67x-PD25.1.1 kernel source includes a device tree overlay for enabling SPI0, and the documentation at docs.phytec.com (as of 5/7/2026) will guide you to use this to evaluate a loopback test on SPI0 to verify functionality. **A SPI0 loopback test cannot work** due to a conflict on the 1624.1 carrier board PCB revision. Specifically, the SPI0_D0 and SPI0_D1 lines are shared with the MIPI-DSI interface, there is a directional level shifter there that will always drive SPI0_D0 and prevent the loopback from working. 

This was tested with BSP-Yocto-Ampliphy-AM67x-PD25.1.1

# BSP integration:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am67-scarthgap-SPI
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image using the new MACHINE config provided in the layer:

```sh
MACHINE=phyboard-rigel-am67xx-1-mcu-spi1 bitbake phytec-headless-image
```

> **_NOTE:_**  MACHINE=phyboard-rigel-am67xx-1-mcu-spi1 inherits everything from the development kit MACHINE=phyboard-rigel-am67xx-1 and just replaces the SPI0 device tree overlay with a MCU_SPI1 device tree overlay.

# Verifying at Runtime:

Flash the resulting image to an SD Card and use it to boot your phyCORE-AM67x Development Kit. 

Stop in U-Boot and enable the overlay before booting into Linux:

```sh
setenv overlays k3-am6754-phyboard-rigel-x37-mcu-spi1.dtbo
saveenv
boot
```

Once in Linux Userspace, you can confirm that the device tree overlay was loaded by confirming the presence of the /dev/spidev1.0 file. 

```sh
ls /dev/spidev1.0
```

The BSP, by default, will include a spidev_test utility (the source for this is provided in the kernel source) and we can run our loopback test with it like so:

```sh
spidev_test -v -D /dev/spidev1.0
```

That will result in output like the following, which signifies the loopback test failed:

```sh
root@phyboard-rigel-am67xx-1-mcu-spi1:~# spidev_test -v -D /dev/spidev1.0
spi mode: 0x0
bits per word: 8
max speed: 500000 Hz (500 KHz)
TX | FF FF FF FF FF FF 40 00 00 00 00 95 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF F0 0D  | ......@......................ð.
RX | FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF  | ................................
```

In order for the loopback test to pass, we must short MCU_SPI0_D0 and MCU_SPI0_D1 together. We can do this using a M-M 2.54mm jumper cable to short pins 18 and 20 on the X37 Expansion Connector. Once done, the passing loopback test will look like this:

```sh
root@phyboard-rigel-am67xx-1-mcu-spi1:~# spidev_test -v -D /dev/spidev1.0
spi mode: 0x0
bits per word: 8
max speed: 500000 Hz (500 KHz)
TX | FF FF FF FF FF FF 40 00 00 00 00 95 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF F0 0D  | ......@......................ð.
RX | FF FF FF FF FF FF 40 00 00 00 00 95 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF F0 0D  | ......@......................ð.
```
