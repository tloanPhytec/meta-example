# Branch 8mp-scarthgap-flutterdemo

This branch serves as a reference for creating a custom image featuring a Flutter demo in PHYTEC's phyCORE-i.MX8M Plus BSP. Enabling this layer in your BSP will have the following effects:

- introduces a new phytec-flutterdemo-image recipe that largely copies phytec-qt6demo-image
- phytec-flutterdemo-image removes the qt6 demo/support and adds flutter demo/support instead.
- moves the active weston display from HDMI to the LVDS0 interface

This was tested with the phyCORE-i.MX8M Plus's BSP PD24.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b  8mp-scarthgap-flutterdemo
cd meta-example
git submodule update --init
```

Enable the layer and it's submodule meta-layers in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
bitbake-layers add-layer ../sources/meta-example/meta-flutter
bitbake-layers add-layer ../sources/meta-example/meta-flutter/meta-flutter-apps
```

Build the new flutter demo image with bitbake

```sh
bitbake phytec-flutterdemo-image
```
