COMPATIBLE_MACHINE = "phyboard-rigel-am67xx-1"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-board-phytec-phycore_am67x-Fixup-DDR-size.patch \
             file://0002-arch-arm-dts-k3-am67-phycore-som-lpddr4-8gb-Update-D.patch \
           "
