# Branch am64-scarthgap-disESM-R5

This branch serves as a reference for disabling the CONFIG_ESM_K3 driver in U-Boot on PHYTEC's phyCORE-AM64x BSP. Disabling the ESM driver prevents the system from resetting when the Watchdog is triggered.
This was tested with the phyCORE-AM64x's BSP PD24.1.1. 

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-disESM-R5
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake

```sh
bitbake phytec-headless-image
```

Verify Watchdog & ESM configs were disabled

```sh
#U-boot ESM Config 
sed -n '/CONFIG_ESM_K3/s/^#//p' tmp-ampliphy/work/phyboard_electra_am64xx_2-phytec-linux/u-boot-phytec-ti/2024.04-10.00.08-phy5/git/configs/phycore_am62x_r5_defconfig
```
