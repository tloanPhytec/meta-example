# Branch am62x-scarthgap-partupDataPartition

This branch serves as a reference for setting up an additional "data" partition to the generated partup image yocto creates.

This was tested with BSP-Yocto-Ampliphy-AM62x-PD24.1.2

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 
```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62x-scarthgap-partupDataPartition
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake. 
