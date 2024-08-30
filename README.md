# Branch 8mp-kirkstone-bootenv

This branch serves as a reference for adding custom boot environment variables to PHYTEC's phyCORE-i.MX8M Plus BSP. The bootloader patch used in this example enables the 32.768 kHz clock on X_RTC_CLKOUT/nTEMP_ALERT. 
Note: The kernel will reset the RTC by default if the clkout is not consumed by another driver, turning clkout off if using this example alone.
This was tested with the phyCORE-i.MX8M Plus's BSP PD23.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-kirkstone-bootenv
```

Enable the layer in your build: 

```sh
cd $BUILDDIR bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake
