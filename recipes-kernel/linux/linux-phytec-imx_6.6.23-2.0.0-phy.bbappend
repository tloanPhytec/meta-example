COMPATIBLE_MACHINE .= "|phyboard-pollux-imx8mp-3-custom"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-arch-arm64-dts-imx8mp-phyboard-pollux-custom.dtso-ex.patch \
           "
