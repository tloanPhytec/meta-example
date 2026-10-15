SUMMARY = "Headless Weston VNC configuration for phyBOARD-Pollux"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://vnc.conf \
           file://vnc-software.conf \
           file://weston-vnc-certificate.service \
           file://weston-vnc-certificate"

S = "${WORKDIR}"

inherit features_check

REQUIRED_DISTRO_FEATURES = "systemd pam x11 wayland"
COMPATIBLE_MACHINE = "^phyboard-pollux-imx8mp-3$"
PACKAGE_ARCH = "${MACHINE_ARCH}"

RDEPENDS:${PN} = "weston-init weston-xwayland openssl-bin"

do_install () {
	install -d ${D}${systemd_system_unitdir}/weston.service.d
	install -m 0644 ${WORKDIR}/vnc.conf ${D}${systemd_system_unitdir}/weston.service.d/vnc.conf
	install -d ${D}${systemd_system_unitdir}/qtphy.service.d
	install -m 0644 ${WORKDIR}/vnc-software.conf ${D}${systemd_system_unitdir}/qtphy.service.d/vnc-software.conf
	install -m 0644 ${WORKDIR}/weston-vnc-certificate.service ${D}${systemd_system_unitdir}/weston-vnc-certificate.service
	install -d ${D}${libexecdir}
	install -m 0755 ${WORKDIR}/weston-vnc-certificate ${D}${libexecdir}/weston-vnc-certificate

	sed -i -e 's,@BINDIR@,${bindir},g' \
	       -e 's,@SYSCONFDIR@,${sysconfdir},g' \
	       ${D}${systemd_system_unitdir}/weston.service.d/vnc.conf
	sed -i -e 's,@LIBEXECDIR@,${libexecdir},g' \
	       ${D}${systemd_system_unitdir}/weston-vnc-certificate.service
	sed -i -e 's,@SYSCONFDIR@,${sysconfdir},g' \
	       ${D}${libexecdir}/weston-vnc-certificate
}

FILES:${PN} += "${systemd_system_unitdir}"
