COMPATIBLE_MACHINE = "phyboard-electra-am64xx-2"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://fragment.cfg "

do_configure:prepend() {

    # Disable CONFIG_ESM_K3
    sed -i 's/CONFIG_ESM_K3=y/# CONFIG_ESM_K3 is not set/' ${S}/configs/phycore_am62x_r5_defconfig
}


