COMPATIBLE_MACHINE .= "|phycore-am57xx-custom"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://0001-arm-dts-am5728-phytec-pcm-948-40300111I.dts-prevent-.patch \
	"

