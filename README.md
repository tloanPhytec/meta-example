# Branch 8mp-scarthgap-customOverlay

This branch serves as a reference for creating a custom MACHINE that inherits PHYTEC's phyCORE-i.MX8MPlus Development Kit config (MACHINE=phyboard-pollux-imx8mp-3)and patches the kernel in order to provision a new device tree overlay (imx8mp-phyboard-pollux-custom.dtbo) for that MACHINE. This layer can serve as a basis for further customization of the custom MACHINE seperately from PHYTEC's.

This was tested with BSP-Yocto-NXP-i.MX8MP-PD24.1.0

In order to evaluate this in your phyCORE-i.MX8MPlus BSP setup:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-customOverlay
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build your target's image with bitbake, now you can switch between two different MACHINE targets:

```sh
MACHINE=phyboard-pollux-imx8mp-3 bitbake phytec-headless-image
MACHINE=phyboard-pollux-imx8mp-3-custom bitbake phytec-headless-image
```

At runtime, you can enable the overlay like so in Linux userspace:

```sh
vi /boot/bootenv.txt
```

Modify the file to reflect the following:

```sh
overlays=conf-imx8mp-phyboard-pollux-custom.dtbo
```

Then reboot.
