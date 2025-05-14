# Branch 8mp-scarthgap-Qt6DualDisplay

This branch serves as a reference for changing the default graphics configuration of PHYTEC's phyCORE-i.MX8M Plus BSP. Enabling this layer in your BSP will have the following effects:

- Enables BOTH the PEB-AV-10 LVDS display accessory and the HDMI interface within the Weston configuration
- disables the Qt6 demo's systemd service such that it will no longer start automatically upon boot
- introduces a new recipe qt6-dual-display-demo (builds a Qt6 dual-display demo from source, and enables it as a systemd service)

This was tested with the phyCORE-i.MX8M Plus's BSP PD24.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-Qt6DualDisplay
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's Qt6 image with bitbake

```sh
bitbake phytec-qt6demo-image
```

Use the resulting image to boot your phyCORE-i.MX8M Plus Development Kit with it connected to the PEB-AV-10 LVDS Display Add-On and a HDMI monitor. The dual display demo will start automatically. 
