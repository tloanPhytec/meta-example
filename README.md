# Branch am67x-scarthgap-8GBRam

This branch serves as a reference for how to enable support for 8GB RAM on PHYTEC's phyCORE-AM67x BSP. 
This was tested with the phyCORE-AM67x's BSP PD25.1.0. 

In order to evaluate this on your phyCORE-AM67x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am67x-scarthgap-8GBRam
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```
Rebuild your target's image with bitbake
