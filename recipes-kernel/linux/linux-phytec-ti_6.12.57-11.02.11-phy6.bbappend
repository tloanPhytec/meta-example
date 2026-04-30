COMPATIBLE_MACHINE .= "|phyboard-rigel-am67xx-1-mcu-spi1"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
	file://0001-Fix-Change-SPI0-reference-dtbo-to-MCU_SPI1.patch \
"
