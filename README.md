# Branch am64-scarthgap-disableWatchdog

This branch serves as a reference for completly disabling Watchdog and ESM configs on PHYTEC's phyCORE-AM64x BSP.
This was tested with the phyCORE-AM64x's BSP PD24.1.1. 

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-disableWatchdog
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake

```sh
bitbake
```

Verify Watchdog & ESM configs were disabled

```sh
#U-boot ESM Config 
sed -n '/CONFIG_ESM_K3/s/^#//p' tmp-ampliphy/work/phyboard_electra_am64xx_2-phytec-linux/u-boot-phytec-ti/2024.04-10.00.08-phy5/git/configs/phycore_am62x_r5_defconfig

#U-boot Watchdog Configs
grep -E '# CONFIG_(SPL_WATCHDOG|SYSRESET_WATCHDOG|WATCHDOG|IMX_WATCHDOG|ULP_WATCHDOG) is not set' tmp-ampliphy/work/phyboard_electra_am64xx_2-phytec-linux/u-boot-phytec-ti/2024.04-10.00.08-phy5/deploy-u-boot-phytec-ti/u-boot.img-phyboard-electra-am64xx-2-2024.04-10.00.08-phy5-r0.config

#Kernel Watchdog Config
grep -E '# CONFIG_(IPMI_WATCHDOG|WATCHDOG|WQ_WATCHDOG) is not set' tmp-ampliphy/work/phyboard_electra_am64xx_2-phytec-linux/linux-phytec-ti/6.6.32-10.00.08-phy3/deploy-linux-phytec-ti/Image-linux-phytec-ti-6.6.32-10.00.08-phy3-r0.0-phyboard-electra-am64xx-2-20250213165532.config
```
