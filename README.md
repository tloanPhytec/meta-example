# Meta-Example Branch am64-scarthgap-MaxUarts

This branch serves as a reference for customizing the phyCORE-AM64x SOM such that it enables the maximum 9x UARTs available to the AM64x processor. 

While the nonstandard pinout of the phyCORE-AM64x SOM was designed to accomodate all features of the AM64x processor, the development kit was not designed with this flexability in mind. Thus, when using this software with the development kit carrier board, some of the UARTs are not conventiently broken out to expansion headers and some of the UARTs will conflict with other peripheral interfaces (such as the User Button and MCAN0 interfaces) that are implemented onboard. It is therefore neccessary to utilize flying probes and/or rework the development kit carrier board to verify these interfaces. 

This was tested with **BSP-Yocto-Ampliphy-AM64x-PD25.1.0**

## BSP Integration

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-MaxUarts
```

Enable the layer in your build:

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake

```sh
MACHINE=phyboard-electra-am64xx-2 DISTRO=ampliphy bitbake phytec-headless-image
```

## Runtime Instructions

The following images outline the physical locations on the development kit carrier board where the UARTs enabled in this meta-example reference can be accessed. It is advised to connect external-hardware/serial-cables/flying-probes to these while the development kit is powered off (be sure to confirm your TX/RX connections using the Development Kit carrier board schematic):

![Electra-9xUARTs-top](https://github.com/user-attachments/assets/1a9a191a-da53-4273-b5cd-7b2ceb56404c)
![Electra-9xUARTs-bottom](https://github.com/user-attachments/assets/d3151c4f-3485-46e1-a228-c212b2a27eaf)

At runtime, we can confirm the available UART interfaces like so:

```sh
dmesg | grep serial
```

The above should give you the following output:

```sh
root@phyboard-electra-am64xx-2:~# dmesg | grep serial
[    1.277083] 4a00000.serial: ttyS0 at MMIO 0x4a00000 (irq = 314, base_baud = 6000000) is a 8250
[    1.288309] 4a10000.serial: ttyS1 at MMIO 0x4a10000 (irq = 315, base_baud = 6000000) is a 8250
[    1.299416] 2800000.serial: ttyS2 at MMIO 0x2800000 (irq = 316, base_baud = 3000000) is a 8250
[    1.335717] 2810000.serial: ttyS3 at MMIO 0x2810000 (irq = 317, base_baud = 3000000) is a 8250
[    1.346111] 2820000.serial: ttyS4 at MMIO 0x2820000 (irq = 318, base_baud = 3000000) is a 8250
[    1.356468] 2830000.serial: ttyS5 at MMIO 0x2830000 (irq = 319, base_baud = 3000000) is a 8250
[    1.367032] 2840000.serial: ttyS6 at MMIO 0x2840000 (irq = 320, base_baud = 3000000) is a 8250
[    1.377623] 2850000.serial: ttyS7 at MMIO 0x2850000 (irq = 321, base_baud = 3000000) is a 8250
[    1.388031] 2860000.serial: ttyS8 at MMIO 0x2860000 (irq = 322, base_baud = 3000000) is a 8250
[    3.371910] systemd[1]: Created slice Slice /system/serial-getty.
```

To configure the serial settings (such as the baud rate) for these UARTs, leverage the `stty` utility:

For example, we can conveniently copy the known-working 115200 8n1 serial settings from our USB Debug console (/dev/ttyS2) and apply it to the other UARTs like /dev/ttyS0 (MCU_UART0) with the following:

```sh
stty -F /dev/ttyS0 "$(stty -F /dev/ttyS2 -g)"
```

Connect MCU_UART0 to a TTL Serial Cable and open a serial terminal (configured for 115200 baud, 8n1 data) for it on your Host Machine.

In order to verify transmit capabilities use (verify host recieves):

```sh
echo "Test Tx" > /dev/ttyS0
```

In order to verify receive capabilities use (send from host and verify development kit receives):

```sh
cat /dev/ttyS0
```
