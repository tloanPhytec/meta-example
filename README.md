# Branch am62-scarthgap-installFile

This branch serves as a reference installing an application script to /usr/bin of the target's root filesystem.

This was tested with the BSP-Yocto-Ampliphy-AM62x-PD24.1.1

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-scarthgap-installFile
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Enable the recipe example in your $BUILDDIR/conf/local.conf: 

```sh
IMAGE_INSTALL:append = " example-app"
```

Rebuild your target's image with bitbake

To evaluate the installed example script, use the following command:

```sh
example-app.sh
```
