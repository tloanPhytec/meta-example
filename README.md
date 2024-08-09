meta-example branch=am62-kirkstone-wks
This branch serves as a reference for both:

defining a custom MACHINE that inherits everything from PHYTEC's provided BSPs (including references for getting the kernel & bootloader recipes to work with the new MACHINE)
adding a custom kickstart file (.wks) to serve as a reference for adding additional data partitions to the generated .wic image yocto creates.
This was tested with the phyCORE-AM62x's BSP PD23.2.1

In order to evaluate this on your phyCORE-AM62x Development Kit:
  1) Navigate to your BSP's sources directory:
      cd $BUILDDIR/../sources
  2) clone this repo and branch
      git clone https://github.com/tloanPhytec/meta-example.git -b am62-kirkstone-wks
  3) Enable the layer in your build
      cd $BUILDDIR
      bitbake-layers add-layer ../sources/meta-example
  4) Configure the custom MACHINE reference in your $BUILDDIR/conf/local.conf
      MACHINE ?= "phyboard-lyra-am62xx-3-custom"
  5) Rebuild your target's image with bitbake
