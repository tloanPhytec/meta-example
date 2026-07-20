# Branch 8mp-scarthgap-customRaucBundle

This branch serves to demonstrate how to define custom RAUC Bundles for the PHYTEC phyCORE-i.MX8M Plus SOM. The resulting custom-bundle-phyboard-pollux-imx8mp-3.raucb just inherits everything from phytec-headless-bundle-phyboard-pollux-imx8mp-3.raucb.

This was tested with BSP-Yocto-NXP-i.MX8MP-PD24.1.0

# BSP integration:

Navigate to your BSP's sources directory:

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch:

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-customRaucBundle
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build the new RAUC Bundle recipe:

```sh
DISTRO=ampliphy-vendor-rauc MACHINE=phyboard-pollux-imx8mp-3 bitbake custom-bundle
```
