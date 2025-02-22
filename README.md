# Branch am64-scarthgap-disableWatchdog

This branch serves as a reference for completly disabling Watchdog and ESM configs on PHYTEC's phyCORE-AM64x BSP.
This was tested with the phyCORE-AM64x's BSP PD24.1.1. 

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-disableWatchdog
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake
