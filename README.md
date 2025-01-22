# Branch 8mp-scarthgap-psplash

This branch serves as a reference for changing the default splash screen of PHYTEC's phyCORE-i.MX8M Plus BSP's phytec-qt6demo-image. Enabling this layer in your BSP will have the following effects:

- moves the active weston display from HDMI to the LVDS0 interface
- updates the splash screen with a custom logo

This was tested with the phyCORE-i.MX8M Plus's BSP PD24.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-psplash
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake

