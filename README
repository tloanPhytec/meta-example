# Branch am62-kirkstone-staticIP

This branch provides a reference for creating custom edits to network files in PHYTEC's phyCORE-AM62x BSP. It demonstrates how to modify the 10-eth0.network file to assign a static IP address that remains consistent across BSP builds.
These changes have been tested with the phyCORE-AM62x BSP version PD23.2.1

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory:

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-kirkstone-staticIP
```

Enable the layer in your build: 

```sh
cd $BUILDDIR bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake
