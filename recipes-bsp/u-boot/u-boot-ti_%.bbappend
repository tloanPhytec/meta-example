COMPATIBLE_MACHINE = "phyboard-electra-am64xx-2"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-custom-u-boot-env-var.patch \
           "
