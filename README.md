# Branch am57-kirkstone-customMachine

This branch serves as a reference for creating a custom MACHINE that inherits PHYTEC's phyCORE-AM57x Development Kit config (MACHINE=phycore-am57xx-1) and can serve as a basis for further customizing a custom MACHINE seperately from PHYTEC's.
This was tested with the phyCORE-AM57x's BSP-Yocto-Ampliphy-AM57x-PD24.1.0

In order to evaluate this in your phyCORE-AM57x BSP setup:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am57-kirkstone-customMachine
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build your target's image with bitbake, now you can switch between two different MACHINE targets:

```sh
MACHINE=phycore-am57xx-1 bitbake phytec-headless-image
MACHINE=phycore-am57xx-custom bitbake phytec-headless-image
```
