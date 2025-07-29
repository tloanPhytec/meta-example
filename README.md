# Branch am57-kirkstone-earlyGPIO

This branch serves as a reference for creating a custom MACHINE that inherits PHYTEC's phyCORE-AM57x Development Kit config (MACHINE=phycore-am57xx-1) and introduces a reference for setting GPIO1_23 HIGH very early during boot (during the SPL) prior to U-boot proper. The GPIO will remain HIGH all the way into Linux userspace, where you can then assume control of the GPIO.

This was tested with the phyCORE-AM57x's BSP-Yocto-Ampliphy-AM57x-PD24.1.0

> **_NOTE:_**  The gpio used in this example is GPIO1_23 and this is brought out on the WIFI/BT Header X26 on the phyCORE-A57x Development Kit carrier board. Thus, to prevent issues/conflicts with the WIFI/BT module PCM-949, the device tree overlay for that was disabled for the custom MACHINE. 

In order to evaluate this in your phyCORE-AM57x BSP setup:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am57-kirkstone-earlyGPIO
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build your target's image with bitbake (be sure to specify the custom MACHINE setup for this example):

```sh
MACHINE=phycore-am57xx-custom bitbake phytec-headless-image
```

Upon booting the resulting image, you'll see GPIO1_23 go HIGH during the SPL. Once booted into Linux, you can assume control of the GPIO with the sysfs interface:

```sh
echo 2039  > /sys/class/gpio/export
echo 0 > /sys/class/gpio/gpio2039/value # gpio OFF
echo 1 > /sys/class/gpio/gpio2039/value # gpio ON
```
