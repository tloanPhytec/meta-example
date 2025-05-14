SUMMARY = "Qt6 dual display demo"
DESCRIPTION = "Basic demo for showcasing multi-display support with Qt6"

LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

inherit qt6-qmake systemd

SRC_URI = "git://github.com/tloanPhytec/qt6-dual-display-demo.git;branch=main;protocol=https"
SRCREV = "26af26f5a1b89aaded991a4d136e192ba952647c"

SRC_URI += " \
	file://qt6-dual-display-demo.service \
"

DEPENDS = "packagegroup-qt6-essentials qtmultimedia"

S = "${WORKDIR}/git"

SYSTEMD_SERVICE:${PN} = "qt6-dual-display-demo.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/qt6-dual-display-demo ${D}/${bindir}/qt6-dual-display-demo

    install -d ${D}/${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/qt6-dual-display-demo.service ${D}/${systemd_unitdir}/system/qt6-dual-display-demo.service
}

FILES:${PN} += " \
	${bindir}/qt6-dual-display-demo \
	${systemd_unitdir}/system/qt6-dual-display-demo.service \
"

RDEPENDS:${PN} += " \
    weston \
    weston-init \
    qtbase \
    qtdeclarative \
"
