# Branch am64-kirkstone-bootenv

This branch serves as a reference for adding custom boot environment variables to PHYTEC's phyCORE-AM64x BSP.
This was tested with the phyCORE-AM64x's BSP PD23.2.1

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-kirkstone-bootenv
```

Enable the layer in your build: 

```sh
cd $BUILDDIR bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake

You will see a new dummy variable upon booting into U-Boot and using the following U-Boot console command:

```sh
printenv customVar
```
