COMPATIBLE_MACHINE .= "|imx8mp-libra-fpsc-1"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://0001-wip-8mp-fpsc-rauc-fix.patch \
             file://0001-wip-8mp-fpsc-rauc-whitelist-RAUC-env-vars.patch \
           "
