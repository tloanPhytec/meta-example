COMPATIBLE_MACHINE = "phyboard-zeta-imx7d-1"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://imx7-console-uart5.patch \
           "
