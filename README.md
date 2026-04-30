# Branch 8mp-scarthgap-swupdate

This branch serves as a reference for enabling the SWUpdate support for PHYTEC's phyCORE-i.MX8MPlus Development Kit. This BSP includes RAUC support by default and this layer was creaed to benefit developers wanting to evaluate SWUpdate as an alternative:

https://sbabic.github.io/swupdate/index.html

This was tested with BSP-Yocto-NXP-i.MX8MP-PD24.1.0

# BSP integration:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch, as well as the meta-swupdate layer it depends on: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-swupdate
git clone https://github.com/sbabic/meta-swupdate.git -b scarthgap                    # Tested with commit ID 81f4faa406e70dfa514e2f7642f97df33d3b84ae
```

Enable the layers in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
bitbake-layers add-layer ../sources/meta-swupdate
```

Build your target's image and RAUC bundle with bitbake:

```sh
MACHINE=phyboard-pollux-imx8mp-3 DISTRO=ampliphy-vendor-swupdate bitbake phytec-headless-image
MACHINE=phyboard-pollux-imx8mp-3 DISTRO=ampliphy-vendor-swupdate bitbake phytec-headless-swupdate-bundle
```

> **_NOTE:_**  With DISTRO=ampliphy-vendor-swupdate configured, 'bitbake phytec-headless-image' will only deploy a .partup image binary and this can only be flashed to the eMMC. This SWUpdate reference is only setup to support the eMMC.

# Verifying at Runtime:

You'll need to boot your development kit from an SD Card first (using a standard, pre-built image) and use 'partup' to flash the eMMC at runtime using the .partup image we just built:

```sh
partup install phytec-headless-image-phyboard-pollux-imx8mp-3.rootfs.partup /dev/mmcblk2
```

Once the eMMC is flashed, you can 'poweroff' the development kit, remove the SD Card, configure the boot switches for eMMC boot, and reconnect power in order to boot.

Once in Linux Userspace, you can confirm which root filesystem you have mounted with the following:

```sh
swupdate -g
```

You could also use the swupdate utility on the command line to install update bundles.

> **_NOTE:_**  With SWUpdate enabled, you'll have a A/B partition scheme with two copies of the operating system. One is used for normal operation, the other is left inactive and can be updated (OTA).

Swupdate, by default, configures a webserver that can also facilitate updates. 

With your phyCORE-i.Mx8M Plus Dvelopment Kit connected to the same LAN as your host, use a web browser on your host to access the following URL:

http://\<target ip address\>:8080/

To find your phyCORE-i.MX8MPlus Dev Kit's IP address, use the following:

```sh
ip addr
```

Once on the SWUpdate web dashboard, click the empty region titled "Software Update" and upload the  phytec-headless-swupdate-bundle-phyboard-pollux-imx8mp-3.rootfs.swu file we also built. This will install automatically and will be used for the next boot. 
