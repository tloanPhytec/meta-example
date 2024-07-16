# meta-example
branch = am64-kirkstone-bootenv

Reference for adding custom boot environment variables to PHYTEC's phyCORE-AM64x BSP (tested with BSP PD23.2.1)

With Yocto's build environment sourced:

    // Host Console
  
    cd $BUILDDIR/../sources
  
    git clone https://github.com/tloanPhytec/meta-example.git -b am64-kirkstone-bootenv
  
    cd $BUILDDIR
  
    bitbake-layers add-layer ../sources/meta-example

    bitbake phytec-headless-image

You will see a new dummy variable upon booting into U-Boot:

    // U-boot console
  
    printenv customVar
