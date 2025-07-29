COMPATIBLE_MACHINE .= "|phycore-am57xx-custom"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://0001-arm-dts-am57xx-phytec-pcm-948.dts-hog-gpio1_23-outpu.patch \
	    file://0002-arm-spl-phycore-am57x-set-gpio-high-before-u-boot-pr.patch \
	"
