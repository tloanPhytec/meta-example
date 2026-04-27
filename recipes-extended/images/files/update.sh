#!/bin/sh
PHASE=$1

# Safely and definitively detect the active root partition
ACTIVE_ROOT=$(mount | grep ' on / ' | awk '{print $1}')

# If running on Slot A (Root 5), target Slot B (Root 6 / Boot 2)
if [ "$ACTIVE_ROOT" = "/dev/mmcblk2p5" ]; then
    TARGET_ROOT_DEV="/dev/mmcblk2p6"
    TARGET_BOOT_DEV="/dev/mmcblk2p2"
    NEXT_UBOOT_PART="2"

# If running on Slot B (Root 6), target Slot A (Root 5 / Boot 1)
elif [ "$ACTIVE_ROOT" = "/dev/mmcblk2p6" ]; then
    TARGET_ROOT_DEV="/dev/mmcblk2p5"
    TARGET_BOOT_DEV="/dev/mmcblk2p1"
    NEXT_UBOOT_PART="1"

else
    echo "SWUpdate Pre-install FATAL: Unknown active root $ACTIVE_ROOT"
    exit 1
fi

if [ "$PHASE" = "preinst" ]; then
    echo "SWUpdate Pre-install: Active OS is on $ACTIVE_ROOT. Targeting $TARGET_ROOT_DEV."
    
    # Symlink the target Rootfs partition
    ln -sf $TARGET_ROOT_DEV /dev/update_root
    
    # Symlink the target Boot partition (so SWUpdate can flash the new fitImage)
    ln -sf $TARGET_BOOT_DEV /dev/update_boot
    
    exit 0
fi

if [ "$PHASE" = "postinst" ]; then
    echo "SWUpdate Post-install: Flipping U-Boot mmcpart to $NEXT_UBOOT_PART..."
    fw_setenv mmcpart $NEXT_UBOOT_PART
    exit 0
fi

exit 0
