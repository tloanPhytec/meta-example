# Branch am62-kirkstone-bootenv

This branch serves as a reference for adding custom boot environment variables to PHYTEC's phyCORE-AM62x BSP.
This was tested with the phyCORE-AM62x's BSP PD23.2.1

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-kirkstone-bootenv
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake

You will see new logic added to the default boot environment upon booting into U-Boot and using the following U-Boot console command:

```sh
printenv args_all
```

The 'args_all' boot variable is run as a part of every standard boot. Allow boot to occur, either automatically or by issuing the 'boot' command.

You will see the SOM's Heartbeat LED turn ON for 5 seconds, and then OFF for 5 seconds (as result of this new boot environment) before boot proceeds normally. 
