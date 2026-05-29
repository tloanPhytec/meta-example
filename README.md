# Branch imx93-walnascar-NPU-fix

This branch serves as a fix for evaluating the ARM Ethos-U65 NPU featured on the phyCORE-i.MX93 SOM, specifically using the phyBOARD-Nash Development Kit. 

Currently, the BSP-Yocto-NXP-i.MX93-PD26.1.0 kernel source includes a device tree overlay for enabling the NPU, and the documentation (https://phytec.github.io/doc-bsp-yocto/bsp/imx9/imx91-93/pd26.1.0.html#running-npu-examples)
will guide you to enable this at runtime (by modifying /boot/overlays.txt) in order to evaluate some NPU inferencing/benchmarking examples. 

**Running these examples on the NPU with the unmodified, pre-built BSP-Yocto-NXP-i.MX93-PD26.1.0 images cannot work** due to a memory overlap in arch/arm64/boot/dts/freescale/imx93-phycore-npu.dtso. This layer serves to patch the kernel and enables by default all the required device tree overlays needed to evaluate the NPU.  

> **_Quick Issue Summary:_** The default imx93-phycore-npu.dtso was instructing the kernel to allocate a region in memory starting at 0x88000000 for the NPU, but this is exactly where the U-Boot environment is already setup to load the kernel into memory. As soon as you go to start the NPU using the default imx93-phycore-npu.dtso overlay the kernel will detect that the actively-running kernel is going to be overwritten and it will throw a segmentation fault to protect itself. The kernel patch included in this layer serves to move the NPU's memory allocation to a new location where it won't collide with anyting. 

This fix was tested with BSP-Yocto-NXP-i.MX93-PD26.1.0

# BSP integration:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b imx93-walnascar-NPU-fix
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image:

```sh
MACHINE=phyboard-nash-imx93-1 bitbake phytec-qt6demo-image
```

# Verifying at Runtime:

Flash the resulting image to an SD Card and use it to boot your phyBOARD-Nash Development Kit. 

Once in Linux Userspace, you can confirm that the device tree overlays required for exercising the NPU are already enabled by default: 

```sh
root@phyboard-nash-imx93-1:examples# cat /boot/overlays.txt
fit_overlay_conf=conf-imx93-phyboard-nash-peb-av-10-ph128800t006.dtbo#conf-imx93-phycore-rpmsg.dtbo#conf-imx93-phycore-npu.dtbo
```
Now you can run through the steps documented here successfully:

https://phytec.github.io/doc-bsp-yocto/bsp/imx9/imx91-93/pd26.1.0.html#running-npu-examples
