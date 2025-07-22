COMPATIBLE_MACHINE = "phyboard-electra-am64xx-2"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-am64-env-phytec-Enable-PEB-C-010-Expansion-Overlay.patch \
           "
