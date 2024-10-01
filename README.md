# Branch am62-kirkstone-customMachine

This branch serves as a reference for creating a custom MACHINE that inherits PHYTEC's phyCORE-AM62x Development Kit config (MACHINE=phyboard-lyra-am62xx-3) and can serve as a basis for further customizing a custom MACHINE seperately from PHYTEC's.
This was tested with the phyCORE-AM62x BSP version PD23.2.1

In order to evaluate this in your phyCORE-AM62x BSP setup:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-kirkstone-customMachine
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build your target's image with bitbake, now you can switch between two different MACHINE targets:

```sh
MACHINE=phyboard-lyra-am62xx-3 bitbake phytec-headless-image
MACHINE=phyboard-lyra-am62xx-3-custom bitbake phytec-headless-image
```
