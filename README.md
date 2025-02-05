# Branch 8mp-scarthgap-blink

This branch serves as a reference for both a blink.c application leveraging the v2 libgpiod API and for creating a systemd service that start blink.c automatically upon boot in PHYTEC's phyCORE-i.MX8M Plus BSP.

This was tested with the phyCORE-i.MX8M Plus's BSP-Yocto-NXP-i.MX8MP-PD24.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-blink
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Enable the new blink recipe in your target image by adding the following line to your $BUILDDIR/conf/local.conf:

```sh
IMAGE_INSTALL:append = " blink"
```

Rebuild your target's image with bitbake

```sh
bitbake phytec-qt6demo-image
```

Upon boot, with an LED and appropriate series resistor connected to Pin 31 of the X6 Expansion Connector and a Ground pin, you should see the LED blink ON and OFF with a 1 second interval. 
