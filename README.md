# Branch 8mp-scarthgap-CelebFaceMatch

This branch serves as a reference for adding PHYTEC's Celeb Face Match demo to a standard BSP-Yocto-NXP-i.MX8MP-PD24.1.0.

- Documentation for this Celeb Face Match AI Demo can be found here (note that much of this is out of date and is provided as a reference on how the demo works): https://www.phytec.de/cdocuments/?doc=NgCTP

This was tested with the phyCORE-i.MX8M Plus's BSP PD24.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b  8mp-scarthgap-CelebFaceMatch
cd meta-example
git submodule update --init
```

Enable the layer and it's submodule meta-layers in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
bitbake-layers add-layer ../sources/meta-example/meta-celebrity-face-match
```

Build the new flutter demo image with bitbake

```sh
DISTRO=ampliphy-vendor-xwayland bitbake phytec-vision-image
```
