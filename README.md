# Branch 8mp-scarthgap-devtools

This branch serves as a reference for creating a custom image that adds packages to PHYTEC's phyCORE-i.MX8M Plus BSP's default phytec-qt6demo-image. Enabling this layer in your BSP will have the following effects:

- creates a new recipe for phytec-qt6demo-devtools-image

  - this new image inherits everything from phytec-qt6demo-image
  - this new image incorporates the following additional packages: git, make, gcc, g++, autoconf, python3-pip, systemd-bootchart, and others.

This was tested with the phyCORE-i.MX8M Plus's BSP PD24.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-devtools
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Build the new target image with bitbake

```sh
bitbake phytec-qt6demo-devtools-image
```
