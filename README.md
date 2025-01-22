# Branch 8mp-scarthgap-bootenv

This branch serves as a reference for adding custom boot environment variables to PHYTEC's phyCORE-i.MX8M Plus BSP. The bootloader patch used in this example just adds a deummy variable.

This was tested with the phyCORE-i.MX8M Plus's BSP PD24.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-bootenv
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake
