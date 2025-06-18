COMPATIBLE_MACHINE = "phyboard-electra-am64xx-2"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-k3-am642-phyboard-electra-rdk-enable-9xUARTs.patch \
             file://fragment.cfg \
           "
