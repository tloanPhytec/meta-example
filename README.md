# Meta-Example Branch am64-scarthgap-AB-Bootloaders

This branch serves as a reference for configuring redundant A/B Bootloaders on the eMMC hardware boot partitions of the phyCORE-AM64x SOM in order to facilitate software updates of the bootloader with failure recovery. Auto-switching between the eMMC's boot0/boot1 in case of boot failure is not currently supported in the AM64x boot ROM, it only supports falling back to a different boot interface entirely (eMMC fails -> try SPI NOR, for example). Thus, this reference implementation will make use of a recovery bootloader on the SPI NOR Flash that manages which of the eMMC boot partitions is used for boot, and handles the switch on failure.

Inspiration for this implementation was taken from here:

https://e2e.ti.com/support/processors-group/processors/f/processors-forum/1251330/faq-redundant-boot-from-emmc-boot-partitions-on-am62x

This was tested with **BSP-Yocto-Ampliphy-AM64x-PD25.2.1**

## BSP Integration

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-AB-Bootloaders
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

The resulting binaries you need are:

* tispl-spi.bin
* u-boot-spi.img
* phytec-headless-image-phyboard-electra-am64xx-2.rootfs.partup

## Runtime Instructions

First, flash an SD Card with a standard pre-built image if you do not already have one (we just need an SD Card that can boot into Linux):

```sh
# identify your SD Card
mount

umount /dev/sdX*
sudo bmaptool copy phytec-headless-image-phyboard-electra-am64xx-2.rootfs.wic.xz /dev/sdX
```

Once the SD Card is flashed, expand the SD Card's root filesystem:

```sh
sudo parted /dev/sde resizepart 2 100%
sudo e2fsck -f /dev/sde2
sudo resize2fs /dev/sde2
```

Mount the root filesystem. The easiest way to do this is to just click the root partition of the connected SD Card in your system tray:

<img width="699" height="470" alt="systemtray" src="https://github.com/user-attachments/assets/686b6cf1-e692-44a0-833a-cf348a5fafc1" />

Copy your newly generated eMMC image and SPI bootloaders to the SD Card's root filesystem:

```sh
sudo cp tispl-spi.bin u-boot-spi.img phytec-headless-image-phyboard-electra-am64xx-2.rootfs.partup /media/user/root/ && sync
```

Boot your phyCORE-AM64x Development Kit into Linux from the SD Card.

Install the .partup image to the eMMC. The following partup command will flash the bootloader to both of the eMMC's boot0 and boot1 hardware partitions, and enable boot0 as the one to be used by default). 

```sh
partup install /phytec-headless-image-phyboard-electra-am64xx-2.rootfs.partup /dev/mmcblk0
```

Now install the SPI bootloader binaries to the SPI NOR Flash. The SPI bootloader has a special environment and config that allows it to behave as the recovery bootloader (should eMMC boot0 fail to boot, the boot ROM will fall back to SPI. The SPI bootloader will toggle the EXT_CSD partition flag in the eMMC and then reset, causing the boot ROM to use eMMC boot1 on the next attempt).

```sh
flash_erase /dev/mtd1 0 0
flashcp /boot/tiboot3.bin /dev/mtd1
flash_erase /dev/mtd2 0 0
flashcp /tispl-spi.bin /dev/mtd2
flash_erase /dev/mtd3 0 0
flashcp /u-boot-spi.img /dev/mtd3
```

And clear the SPI NOR's environment partitions (in order to remove any old environment data that may or may not be there):
```sh
flash_erase /dev/mtd4 0 0
flash_erase /dev/mtd5 0 0
```

Once you have both the SPI NOR and eMMC flashed with your binaries, `poweroff` the the development kit and remove the power supply and SD Card (removing the SD Card can help developers be confident in which boot media is being used for boot, and which is not being used for boot). 

Configure eMMC primary boot mode, SPI as fallback boot mode

<img width="256" height="300" alt="pb-07225_boot-emmc_fallback-spi" src="https://github.com/user-attachments/assets/b0ae345d-de24-4748-900b-2e77ba7492b7" />

Finally, connect the power supply and boot into Linux. Note that you'll boot from eMMC boot0 by default.

```sh
U-Boot SPL 2025.07 (Sep 09 2026 - 19:18:30 +0000)
Resetting on cold boot to workaround ErrataID:i2331
Please resend tiboot3.bin in case of UART/DFU boot
resetting ...

U-Boot SPL 2025.07 (Sep 09 2026 - 19:18:30 +0000)
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
ECC is enabled, priming DDR which will take several seconds.
ECC: priming DDR completed in 1148 msec
SPL initial stack usage: 13392 bytes
Trying to boot from MMC1
 -> Accessing eMMC boot0 partition                                                        # <= eMMC boot0 used
 ```

> [!TIP]
> We can manually set which eMMC boot partition is used on the subsequent boot using the following commands in Linux userspace:
> ```sh
> # enable eMMC boot0
> mmc bootpart enable 1 1 /dev/mmcblk0
> 
> # enable eMMC boot1
> mmc bootpart enable 2 1 /dev/mmcblk0
> ```

## Simulate Failed Updates, Exercise Recovery

#### Perhaps we lost power while updating eMMC boot0 and corrupted the entirety of the first-stage bootloader, tiboot3.bin:

```sh
echo 0 > /sys/class/block/mmcblk0boot0/force_ro
dd if=/dev/zero of=/dev/mmcblk0boot0 conv=fsync
reboot
```

Observe that eMMC boot0 doesn't start at all. You fallback to SPI boot immediately, the SPI bootloader enables eMMC boot1 and then resets.

```sh
[  174.276673] reboot: Restarting system
                                                                                          # <= eMMC boot0 doesn't start
U-Boot SPL 2025.07 (Apr 08 2026 - 16:15:41 +0000)
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
ECC is enabled, priming DDR which will take several seconds.
ECC: priming DDR completed in 1149 msec
SPL initial stack usage: 13392 bytes
Trying to boot from SPI                                                                   # <= fallback to SPI boot
Authentication passed
Authentication passed
Loading Environment from nowhere... OK
Authentication passed
Authentication passed
Starting ATF on ARM64 core...

NOTICE:  BL31: v2.10.4(release):lts-v2.10.4-dirty
NOTICE:  BL31: Built : 16:23:33, Apr 26 2024

U-Boot SPL 2025.07 (Apr 08 2026 - 16:15:41 +0000)
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
Trying to boot from SPI
Authentication passed
Authentication passed
Authentication passed
Authentication passed
Authentication passed
Authentication passed


U-Boot 2025.07 (Apr 08 2026 - 16:15:41 +0000)

SoC:   AM64X SR2.0 HS-FS
Model: PHYTEC phyBOARD-Electra-AM64x RDK
DRAM:  1.8 GiB
Core:  83 devices, 33 uclasses, devicetree: separate
WDT:   Not starting watchdog@e000000
MMC:   mmc@fa10000: 0, mmc@fa00000: 1
Loading Environment from SPIFlash... SF: Detected mt35xu512aba with page size 256 Bytes, erase size 4 KiB, total 64 MiB
*** Warning - bad CRC, using default environment

In:    serial@2800000
Out:   serial@2800000
Err:   serial@2800000
Net:   eth0: ethernet@8000000port@1
Hit any key to stop autoboot:  0
switch to partitions #0, OK
mmc0(part 0) is current device
Set to BOOT_BUS_WIDTH = 0x2, RESET = 0x0, BOOT_MODE = 0x0                                  # <= toggle EXT_CSD (flip eMMC boot partition used)
Saving Environment to SPIFlash... Erasing SPI flash...Writing to SPI flash...done
Valid environment: 2
OK
resetting ...

U-Boot SPL 2025.07 (Apr 08 2026 - 16:15:41 +0000)
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
ECC is enabled, priming DDR which will take several seconds.
ECC: priming DDR completed in 1148 msec
SPL initial stack usage: 13392 bytes
Trying to boot from MMC1
 -> Accessing eMMC boot1 partition                                                        # <= recovers using eMMC boot1
Authentication passed
Authentication passed
Loading Environment from nowhere... OK
init_env from device 9 not supported!
Authentication passed
Authentication passed
Starting ATF on ARM64 core...

NOTICE:  BL31: v2.10.4(release):lts-v2.10.4-dirty
NOTICE:  BL31: Built : 16:23:33, Apr 26 2024

U-Boot SPL 2025.07 (Apr 08 2026 - 16:15:41 +0000)
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
Trying to boot from MMC1
 -> Accessing eMMC boot1 partition
Authentication passed
Authentication passed
Authentication passed
Authentication passed
Authentication passed
Authentication passed


U-Boot 2025.07 (Apr 08 2026 - 16:15:41 +0000)

SoC:   AM64X SR2.0 HS-FS
Model: PHYTEC phyBOARD-Electra-AM64x RDK
DRAM:  1.8 GiB
Core:  83 devices, 33 uclasses, devicetree: separate
WDT:   Not starting watchdog@e000000
MMC:   mmc@fa10000: 0, mmc@fa00000: 1
Loading Environment from MMC... Reading from MMC(0)... OK
In:    serial@2800000
Out:   serial@2800000
Err:   serial@2800000
Net:   eth0: ethernet@8000000port@1
Hit any key to stop autoboot:  0
WDT:   Started watchdog@e000000 without servicing  (60s timeout)
Watchdog started, timeout 60000 ms
Scanning for bootflows in all bootdevs
Seq  Method       State   Uclass    Part  Name                      Filename
---  -----------  ------  --------  ----  ------------------------  ----------------
Scanning bootdev 'mmc@fa10000.bootdev':
  0  script       ready   mmc          1  mmc@fa10000.bootdev.part_ /boot.scr.uimg
** Booting bootflow 'mmc@fa10000.bootdev.part_1' with script
37 bytes read in 0 ms
20869632 bytes read in 119 ms (167.3 MiB/s)
63993 bytes read in 2 ms (30.5 MiB/s)
Working FDT set to 88000000
## Flattened Device Tree blob at 88000000
   Booting using the fdt blob at 0x88000000
Working FDT set to 88000000
   Loading Device Tree to 00000000ece8b000, end 00000000ece9d9f8 ... OK
Working FDT set to ece8b000

Starting kernel ...
```

Once back in Linux, your OTA Update service should re-attempt updating eMMC boot0.

> [!TIP]
> Here are some quick reference commands to help you restore eMMC boot0 with working bootloaders:
> 
> ```sh
> echo 0 > /sys/class/block/mmcblk0boot0/force_ro
> dd if=/boot/tiboot3.bin of=/dev/mmcblk0boot0 count=1024 conv=fsync
> dd if=/boot/tispl.bin of=/dev/mmcblk0boot0 seek=1024 count=3072 conv=fsync
> dd if=/boot/u-boot.img of=/dev/mmcblk0boot0 seek=5120 count=3072 conv=fsync
> 
> # re-enable eMMC boot0
> mmc bootpart enable 1 1 /dev/mmcblk0
> ```

#### Perhaps we lost power while updating eMMC boot1. ALL of tiboot3.bin and tispl.bin got written, but only half of U-Boot proper:

```sh
echo 0 > /sys/class/block/mmcblk0boot1/force_ro
dd if=/dev/zero of=/dev/mmcblk0boot1 conv=fsync
dd if=/boot/tiboot3.bin of=/dev/mmcblk0boot1 count=1024 conv=fsync
dd if=/boot/tispl.bin of=/dev/mmcblk0boot1 seek=1024 count=3072 conv=fsync
dd if=/boot/u-boot.img of=/dev/mmcblk0boot1 seek=5120 count=1536 conv=fsync            # note the halved count= here compared to above.

# enable eMMC boot1
mmc bootpart enable 2 1 /dev/mmcblk0

reboot
```

Observe that eMMC boot1 attempts to boot. The boot ROM finds a valid tiboot3.bin and starts it. tiboot3.bin is then running and locates tispl.bin and starts it. tispl.bin encoutners an error while loading U-Boot proper:

```sh
[   76.404482] reboot: Restarting system

U-Boot SPL 2025.07 (Apr 08 2026 - 16:15:41 +0000)                             # <= eMMC boot1's tiboot3.bin starts
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
ECC is enabled, priming DDR which will take several seconds.
ECC: priming DDR completed in 1148 msec
SPL initial stack usage: 13392 bytes
Trying to boot from MMC1
 -> Accessing eMMC boot1 partition           
Authentication passed
Authentication passed
Loading Environment from nowhere... OK
init_env from device 9 not supported!
Authentication passed
Authentication passed
Starting ATF on ARM64 core...

NOTICE:  BL31: v2.10.4(release):lts-v2.10.4-dirty
NOTICE:  BL31: Built : 16:23:33, Apr 26 2024

U-Boot SPL 2025.07 (Apr 08 2026 - 16:15:41 +0000)                            # <= eMMC boot1's tispl.bin starts
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
Trying to boot from MMC1
 -> Accessing eMMC boot1 partition
mmc_load_image_raw_sector: mmc block read error
Partition 1 invalid on device 0
spl_register_fat_device: fat register err - -1
spl_load_image_fat: error reading image u-boot.img, err - -1                # <= tispl.bin fails to load U-Boot
Error: -2
Trying to boot from SPI                                                     # fallbacks SPI for next bootloader stage.
Authentication passed
Authentication passed
Authentication passed
Authentication passed
Authentication passed
Authentication passed


U-Boot 2025.07 (Apr 08 2026 - 16:15:41 +0000)

SoC:   AM64X SR2.0 HS-FS
Model: PHYTEC phyBOARD-Electra-AM64x RDK
DRAM:  1.8 GiB
Core:  83 devices, 33 uclasses, devicetree: separate
WDT:   Not starting watchdog@e000000
MMC:   mmc@fa10000: 0, mmc@fa00000: 1
Loading Environment from SPIFlash... SF: Detected mt35xu512aba with page size 256 Bytes, erase size 4 KiB, total 64 MiB
OK
In:    serial@2800000
Out:   serial@2800000
Err:   serial@2800000
Net:   eth0: ethernet@8000000port@1
Hit any key to stop autoboot:  0
switch to partitions #0, OK
mmc0(part 0) is current device
Set to BOOT_BUS_WIDTH = 0x2, RESET = 0x0, BOOT_MODE = 0x0                    # <= toggle EXT_CSD (flip eMMC boot partition used)
Saving Environment to SPIFlash... Erasing SPI flash...Writing to SPI flash...done
Valid environment: 2
OK
resetting ...

U-Boot SPL 2025.07 (Apr 08 2026 - 16:15:41 +0000)
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
ECC is enabled, priming DDR which will take several seconds.
ECC: priming DDR completed in 1148 msec
SPL initial stack usage: 13392 bytes
Trying to boot from MMC1
 -> Accessing eMMC boot0 partition                                            # <= recovers using eMMC boot0
Authentication passed
Authentication passed
Loading Environment from nowhere... OK
init_env from device 9 not supported!
Authentication passed
Authentication passed
Starting ATF on ARM64 core...

NOTICE:  BL31: v2.10.4(release):lts-v2.10.4-dirty
NOTICE:  BL31: Built : 16:23:33, Apr 26 2024

U-Boot SPL 2025.07 (Apr 08 2026 - 16:15:41 +0000)
SYSFW ABI: 4.0 (firmware rev 0x000b '11.2.5--v11.02.05 (Fancy Rat)')
Trying to boot from MMC1
 -> Accessing eMMC boot0 partition
Authentication passed
Authentication passed
Authentication passed
Authentication passed
Authentication passed
Authentication passed


U-Boot 2025.07 (Apr 08 2026 - 16:15:41 +0000)

SoC:   AM64X SR2.0 HS-FS
Model: PHYTEC phyBOARD-Electra-AM64x RDK
DRAM:  1.8 GiB
Core:  83 devices, 33 uclasses, devicetree: separate
WDT:   Not starting watchdog@e000000
MMC:   mmc@fa10000: 0, mmc@fa00000: 1
Loading Environment from MMC... Reading from MMC(0)... OK
In:    serial@2800000
Out:   serial@2800000
Err:   serial@2800000
Net:   eth0: ethernet@8000000port@1
Hit any key to stop autoboot:  0
WDT:   Started watchdog@e000000 without servicing  (60s timeout)
Watchdog started, timeout 60000 ms
Scanning for bootflows in all bootdevs
Seq  Method       State   Uclass    Part  Name                      Filename
---  -----------  ------  --------  ----  ------------------------  ----------------
Scanning bootdev 'mmc@fa10000.bootdev':
  0  script       ready   mmc          1  mmc@fa10000.bootdev.part_ /boot.scr.uimg
** Booting bootflow 'mmc@fa10000.bootdev.part_1' with script
37 bytes read in 0 ms
20869632 bytes read in 119 ms (167.3 MiB/s)
63993 bytes read in 2 ms (30.5 MiB/s)
Working FDT set to 88000000
## Flattened Device Tree blob at 88000000
   Booting using the fdt blob at 0x88000000
Working FDT set to 88000000
   Loading Device Tree to 00000000ece8b000, end 00000000ece9d9f8 ... OK
Working FDT set to ece8b000

Starting kernel ...
```
