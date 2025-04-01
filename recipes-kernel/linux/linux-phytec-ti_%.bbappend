COMPATIBLE_MACHINE .= "|phyboard-lyra-am62xx-3-custom"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://0001-Custom-device-tree-reference.patch \
	"
