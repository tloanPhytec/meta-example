COMPATIBLE_MACHINE = "phyboard-pollux-imx8mp-3"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://customBootEnv.patch \
           "
