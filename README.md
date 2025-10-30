# Branch 8mp-scarthgap-chromium

This branch serves as a reference for enabling the chromium web browser in PHYTEC's phyCORE-i.MX8MPlus BSP. In order to evaluate the browser it is recommended to connect a USB key board and mouse to your phyCORE-i.MX8MPlus Dev Kit, as well as an HDMI monitor.

**This was tested with BSP-Yocto-NXP-i.MX8MP-PD24.1.0**

# BSP Integration

Navigate to your existing BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-chromium
```

Enable the layer:

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your phytec-qt6demo-image with bitbake and use the resulting image to boot your Development Kit. The chromium recipe included in the build-tree has been repaired and added to the phytec-qt6demo-image recipe for easy evaluation via the .bbappend files in the layer:

```sh
DISTRO=ampliphy-vendor-xwayland bitbake phytec-qt6-demo
```

# Runtime Evaluation

In order to launch the chromium browser to your connected HDMI display, run the following commands once in Linux userspace:

Turn off the default Qt6 demo:

```sh
systemctl stop qtphy
```

Start the chromium browser (here we are employing the "--no-sandbox" workaround in order to start chromium as the root user, which is normally not allowed. We are just doing this for basic verification):

```sh
chromium --no-sandbox
```
