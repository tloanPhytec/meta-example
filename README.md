# Branch 8mp-scarthgap-lvgldemo

This branch serves as a reference for creating a custom image featuring a LVGL demo in PHYTEC's phyCORE-i.MX8M Plus BSP. Enabling this layer in your BSP will have the following effects:

- introduces a new phytec-lvgldemo-image recipe that largely copies phytec-qt6demo-image
- phytec-lvgldemo-image removes the qt6 demo/support and adds lvgl demo/support instead.
- moves the active weston display from HDMI to the LVDS0 interface

This was tested with the phyCORE-i.MX8M Plus's BSP PD24.1.0

This meta-example reference was taken largely from https://github.com/phytec/meta-lvgl-demo and was added here in an effort to consolidate examples in one place. Checkout out the original project for updates and un-clobbered commits.

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b  8mp-scarthgap-lvgldemo
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Downgrade the default weston version (v12) in your conf/local.conf by adding the following line (PHYTEC's lvgl-demo has a specific requirement for weston's wl_shell extension, which was deprecated and removed in weston v12, thus requiring the downgrade to v10):

```sh
PREFERRED_VERSION_weston:imx-nxp-bsp = "10.0.5.imx"
```

Rebuild your target's image with bitbake

```sh
bitbake phytec-lvgldemo-image
```
