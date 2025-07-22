COMPATIBLE_MACHINE = "phyboard-electra-am64xx-2"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-arm64-dts-ti-k3-am642-phyboard-electra-peb-c-010.dts.patch \
           "
