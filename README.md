# Branch am62-kirkstone-installFile

This branch serves as a reference installing an application script to /usr/bin of the target's root filesystem.
This was tested with the phyCORE-AM62x's BSP PD23.2.1

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 

  cd $BUILDDIR/../sources

clone this repo and branch: 

  git clone https://github.com/tloanPhytec/meta-example.git -b am62-kirkstone-installFile

Enable the layer in your build: 

  cd $BUILDDIR bitbake-layers add-layer ../sources/meta-example

Enable the recipe example in your $BUILDDIR/conf/local.conf: 

  IMAGE_INSTALL:append = " mypythonapp"

Rebuild your target's image with bitbake
