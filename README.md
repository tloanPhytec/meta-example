# Branch 8mp-scarthgap-bootenv.txt

This branch serves as a reference for customizing the bootenv.txt file included in the rootfs of PHYTEC's phyCORE-i.MX8M Plus BSP (this file enables kernel overlays at bootime). In this example, we enable the VM-020/120 phyCAM-L camera, see the following guide for more information:
https://wiki.phytec.com/spaces/TLP/pages/972554558/PD24.1.0+-+Stream+VM-120-C-M-02-S2.A0+to+LVDS+Display

This was tested with the phyCORE-i.MX8M Plus's BSP PD24.1.0

In order to evaluate this on your phyCORE-i.MX8M Plus Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-bootenv.txt
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake
