COMPATIBLE_MACHINE .= "|phyboard-pollux-imx8mp-3"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append:swupdate-ab = " \
	file://0001-env-phycore-imx8mp-Enable-SWUpdate-A-B-chooser-logic.patch \
"
