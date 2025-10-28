# Branch am62-scarthgap-chromium

This branch serves as a reference for adding the chromium web browser to PHYTEC's phyCORE-AM62x BSP. In order to evaluate the browser it is recommended to connect a USB key board and mouse to your phyCORE-AM62x Dev Kit, as well as an HDMI monitor (see other meta-example branches for changing the default display).

This was tested with BSP-Yocto-Ampliphy-AM62x-PD24.1.1

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-scarthgap-chromium
cd meta-example
git submodule update --init
```

Enable the layer and its sub-layers (this meta-example branch depends on meta-browser and meta-clang, both are set up as git submodules in this meta-example branch):

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
bitbake-layers add-layer ../sources/meta-example/meta-clang
bitbake-layers add-layer ../sources/meta-example/meta-browser/meta-chromium
```

Open your BSP's local build configuration:

```sh
vi $BUILDDIR/conf/local.conf
```

Rebuild your phytec-qt6demo-image with bitbake and use the resulting image to boot your Development Kit.

```sh
bitbake phytec-qt6-demo
```

In order to launch the chromium browser to your connected HDMI display, run the following command once in Linux userspace:

```sh
chromium
```
