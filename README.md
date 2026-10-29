# meta-example Branch am62l-scarthgap-lvgl

[![Platforms](https://img.shields.io/badge/Platform-phyFLEX--AM62L-teal)](https://www.phytec.com/product/phyflex-am62lx/)

TI LVGL Demo ported to the phyFLEX-AM62L Development Kit 

* verified with HW revisions

  * 1627.0 SOM
  * 1618.2 Libra CB
  * Powertip AC209 FPSC Development Kit Display Add-On (connected to LVDS1 on the Libra CB)

This was tested with BSP-Yocto-Ampliphy-AM62Lx-ALPHA2

## BSP Integration

Navigate to your BSP's sources directory:

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch, and it's dependency meta-layer:

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62l-scarthgap-lvgl
git clone git clone https://github.com/nnstreamer/meta-neural-network.git -b scarthgap      # verified with commit ID: c14458c94ebddc0dc5045da5d9aafd699c2e50dd
```

Add the layers to the PHYTEC BSP:

```sh
cd $BUILDDIR
bitbake-layers add-layer $BUILDDIR/../sources/meta-example
bitbake-layers add-layer $BUILDDIR/../sources/meta-neural-network
```
Build the new LVGL target image:

```sh
DISTRO=ampliphy-vendor MACHINE=am62lxx-libra-fpsc-1 bitbake phytec-lvgl-image
```

The TI LVGL Demo should start automatically on the display.

## Service Verification on the target

```sh
systemctl status ti-lvgl-demo.service
```
