# Branch imx7-kirkstone-customMachine

This branch serves as a reference for creating a custom MACHINE that inherits PHYTEC's phyCORE_i.MX7D Development Kit config MACHINE=phyboard-zeta-imx7d-1 and can serve as a basis for further customizing a custom MACHINE seperately from PHYTEC's.
This was tested with the phyCORE-i.MX7's BSP-Yocto-NXP-i.MX7-PD23.1.1

In order to evaluate this in your phyCORE-i.MX7 BSP setup:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b imx7-kirkstone-customMachine
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build your target's image with bitbake, now you can switch between two different MACHINE targets:

```sh
MACHINE=phyboard-zeta-imx7d-1 bitbake phytec-headless-image
MACHINE=phyboard-zeta-imx7d-custom bitbake phytec-headless-image
```
