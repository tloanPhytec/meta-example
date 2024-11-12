# Branch am62-scarthgap-kernelCfgFrag

This branch serves as a reference for customizing the Linux Kernel Configuration via kernel config fragments, targeting PHYTEC's phyCORE-AM62x BSP.

The kernel config fragment can be generated with:

```sh
# customize the kernel config, and be sure to save your changes
bitbake linux-phytec-ti -c menuconfig

# generate fragment.cfg
bitbake linux-phytec-ti -c diffconfig
```

In this case, CONFIG_THERMAL_GOV_BANG_BANG is enabled directly into the kernel. CONFIG_THERMAL_GOV_BANG_BANG enables the bang bang govener. Enabling the bang-bang thermal governor for GPIO-based fans provides smoother control by leveraging the hysteresis value. This prevents abrupt on/off switching, allowing fans to operate more efficiently. The bang-bang governor uses hysteresis to manage fan state transitions, particularly useful for fans that only support binary on/off operation rather than variable throttling.

This was tested with BSP-Yocto-Ampliphy-AM62x-PD24.1.0

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-scarthgap-kernelCfgFrag
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
