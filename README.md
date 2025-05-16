# Branch stm-kirkstone-disableAutologin

This branch serves as a reference for disabling the autologin feature on the phyCORE-STM32MP15x on PHYTEC's BSP.
This was tested with the phyCORE-STM32MP15x's BSP PD23.1.1

In order to evaluate this on your phyCORE-STM32MP15x Development Kit:

Navigate to your BSP's layers directory: 

```sh
cd $BUILDDIR/../layers
```
Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b stm-kirkstone-disableAutologin
```
Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake. 

Now, when you boot a system based on this image, you will be prompted to log in manually by typing root at the login prompt.
