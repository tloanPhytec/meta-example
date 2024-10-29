# Branch am64-scarthgap-kernelCfgFrag

This branch serves as a reference for customizing the Linux Kernel Configuration via kernel config fragments, targeting PHYTEC's phyCORE-AM64x BSP.

The kernel config fragment can be generated with:

```sh
# customize the kernel config, and be sure to save your changes
bitbake linux-phytec-ti -c menuconfig

# generate fragment.cfg
bitbake linux-phytec-ti -c diffconfig
```

In this case, CONFIG_INPUT_JOYDEV is enabled as a kernel Module. 
CONFIG_INPUT_JOYDEV enables the support required for a joystick or gamepad to be accessible as char device ( /dev/input/jsX ).

This was tested with BSP-Yocto-Ampliphy-AM64x-PD24.1.0

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-kernelCfgFrag
```

Enable the layer in your build:

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild the kernel

```sh
bitbake linux-phytec-ti -c clean && bitbake linux-phytec-ti
```

Rebuild your target's image with bitbake
