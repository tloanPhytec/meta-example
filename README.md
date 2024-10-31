# Branch am64-scarthgap-dataPartition-ROrootfs

This branch serves as a reference for defining a custom MACHINE that inherits everything from PHYTEC's provided BSP, configures the Root Filesystem as read-only, and adds a custom kickstart file (.wks) that sets up an additional (writeable) "data" partition to the generated .wic image yocto creates.

This reference could be useful to those wanting to realize a read-only filesystem partition (for security or sudden-loss-of-power-tolerance purposes) but also require some non-voltaile location for storing data (perhaps sensor readings for eventual offload).

This was tested with BSP-Yocto-Ampliphy-AM64x-PD24.1.0

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-dataPartition-ROrootfs
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake. Be sure to enable the new custom MACHINE, either on the cmd line or in your $BUILDDIR/conf/local.conf:

```sh
MACHINE=phyboard-electra-am64xx-2-custom bitbake phytec-container-image
```

Upon boot, confirm that the rootfs is mounted to '/' as read-only and that the writeable data partition is mounted to '/var/volatile/tmp/data' with read+write access by using the following command:

```sh
mount
```
