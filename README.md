# Branch am57-dunfell-disableCAN

This branch serves as a reference for disabling CAN0 and CAN1 on the phyCORE-AM57x development kit. A systemd-conf.bbappend file is created to disable the can0.service and can1.service from systemd-conf. The CAN0 interface is accessible on the phyBOARD-AM57x via a DB9 header connection located at X6, with the silkscreen label "CAN1." The CAN1 interface can be accessed through a 2x5 male pin header found at X5, labeled "CAN2."
This setup has been tested with the phyCORE-AM57x's BSP-Yocto-AM57x-PD23.1.1.

To evaluate this in your phyCORE-AM57x BSP setup, follow these steps:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am57-dunfell-disableCAN
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build your target's image with bitbake, now you can switch between two different MACHINE targets:

```sh
bitbake phytec-headless-image
```

Verify that the services are removed by running the following commands after booting into Linux on your phyBOARD-AM57x development kit with your new BSP image: 

```sh
systemctl status can0.service
systemctl status can1.service
```
