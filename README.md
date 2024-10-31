# Branch am64-scarthgap-dataPartition

This branch serves as a reference for defining a custom MACHINE that inherits everything from PHYTEC's provided BSPs, and adding a custom kickstart file (.wks) that sets up an additional "data" partition to the generated .wic image yocto creates.

This was tested with BSP-Yocto-Ampliphy-AM64x-PD24.1.0

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-dataPartition
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake. Be sure to enable the new custom MACHINE, either on the cmd line or in your $BUILDDIR/conf/local.conf:

```sh
MACHINE=phyboard-electra-am64xx-2-custom bitbake phytec-container-image
```
