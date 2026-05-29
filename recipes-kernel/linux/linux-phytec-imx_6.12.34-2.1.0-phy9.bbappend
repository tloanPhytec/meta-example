COMPATIBLE_MACHINE .= "|phyboard-nash-imx93-1"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-PD26.1.0-kernel-imx93-phycore-npu-memory-overlap-fix.patch \
           "
