# Branch 8mp-scarthgap-kernelCfgFrag

This branch serves as a reference for customizing the Linux Kernel Configuration via kernel config fragments, targeting PHYTEC's phyCORE-i.MX8M Plus BSP.

# Kernel Config Fragment Context
https://docs.yoctoproject.org/5.0.13/kernel-dev/common.html#creating-configuration-fragments

Kernel config fragments can be generated with:

```sh
# customize the kernel config, and be sure to save your changes
bitbake linux-phytec-ti -c menuconfig

# generate fragment.cfg
bitbake linux-phytec-ti -c diffconfig
```

The location of the fragment.cfg file will be output to the console. These fragments can be added to a custom meta layer in the same way that this reference layer does in order to introduce modular changes to the underlying kernel recipe (via a .bbappend).

In this case, CONFIG_LAN743X is enabled staticly (=y) in the kernel. CONFIG_LAN743X enables the support for the Microchip LAN743x PCI Express Ethernet devices and allows the kernel to recognize them. 

**This was tested with BSP-Yocto-NXP-i.MX8MP-PD24.1.0**

# Integration

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-kernelCfgFrag
```

Enable the layer in your build:

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your desired target image with bitbake:

```sh
bitbake phytec-headless-image
```

Connect your LAN7430 PCIe card to the **powered-off** development kit before booting it with the resulting image. 
