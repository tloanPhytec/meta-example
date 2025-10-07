# Branch 8mp-fpsc-scarthgap-rauc

This branch serves as a "fix" for enabling the RAUC support in BSP-Yocto-NXP-i.MX8MP-FPSC-ALPHA1 for PHYTEC's phyFLEX-i.MX8MPlus FPSC Development Kit. This ALPHA1 BSP for the FPSC i.MX8M Plus SOM did not include RAUC support by default and this layer was creaed to benefit developers requiring RAUC support ahead of a production BSP release.

This was tested with BSP-Yocto-NXP-i.MX8MP-FPSC-ALPHA1

# BSP integration:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-fpsc-scarthgap-rauc
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build your target's image and RAUC bundle with bitbake:

```sh
MACHINE=imx8mp-libra-fpsc-1 DISTRO=ampliphy-vendor-rauc bitbake phytec-headless-image
MACHINE=imx8mp-libra-fpsc-1 DISTRO=ampliphy-vendor-rauc bitbake phytec-headless-bundle
```

> **_NOTE:_**  With DISTRO=ampliphy-vendor-rauc configured, 'bitbake phytec-headless-image' will only deploy a .partup image binary and this can only be flashed to the eMMC. RAUC is only supported on the eMMC.

# Verifying at Runtime:

You'll need to boot your development kit from an SD Card first (using a astandard, non-RAUC image) and use 'partup' to flash the eMMC at runtime using the .partup RAUC image we just built:

```sh
partup install phytec-headless-image-imx8mp-libra-fpsc-1.rootfs.partup /dev/mmcblk2
```

Once the eMMC is flashed, you can 'poweroff' the development kit, remove the SD Card, configure the boot switches for eMMC boot, and reconnect power in order to boot.

Once in Linux Userspace, you can confirm which RAUC system you are booted into with the following:

```sh
rauc status
```

> **_NOTE:_**  With RAUC enabled, you'll have a A/B partition scheme with two copies of the operating system. One is used for normal operation, the other is left inactive and can be updated (OTA) or rolled-back to. These A and B systems are reffered to as "Slots" in the 'rauc status' output. 

You can switch RAUC systems like this:

```sh
rauc status mark-active other
```

You can update the inactive system like this:

```sh
rauc install phytec-headless-bundle-imx8mp-libra-fpsc-1.raucb
```
