SUMMARY = "SWUpdate bundle for PHYTEC Pollux"
LICENSE = "MIT"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit swupdate

SRC_URI = " \
    file://sw-description \
    file://update.sh \
"
IMAGE_DEPENDS = "phytec-headless-image virtual/kernel"
SWUPDATE_IMAGES = "phytec-headless-image-phyboard-pollux-imx8mp-3.rootfs fitImage"
SWUPDATE_IMAGES_FSTYPES[phytec-headless-image-phyboard-pollux-imx8mp-3.rootfs] = ".ext4"
SWUPDATE_IMAGES_FSTYPES[fitImage] = ""
