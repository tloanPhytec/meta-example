SUMMARY = "PHYTEC's demo for kivy inspired by the CatalogKivy folder in the kivy examples"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=966903411056714b6ab7eb638477a2ae"

SRC_URI = "git://github.com/phytec/kivy-demo;protocol=https;branch=main"
SRC_URI += "file://${BPN}.service"

SRCREV = "a1d631d82f761ce78bfb4bdaeed9f673f4a3bed4"

S = "${WORKDIR}"

inherit systemd setuptools3 

SYSTEMD_SERVICE:${PN} = "${BPN}.service"

S = "${WORKDIR}/git"

do_install:append(){
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/${BPN}.service ${D}${systemd_system_unitdir}
}

RDEPENDS:${PN} += " \
    weston \
    weston-init \
"
