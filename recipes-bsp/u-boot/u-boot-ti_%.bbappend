COMPATIBLE_MACHINE = "phyboard-lyra-am62xx-3"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-custom-u-boot-env-var.patch \
           "
