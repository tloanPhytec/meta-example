SUMMARY = "Systemd Blink Service Recipe"
DESCRIPTION = "This blink recipe showcases how to compile a C application from a meta layer,\
		and how to set it up as a systemd service that starts automatically on boot."
LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

inherit systemd

SYSTEMD_SERVICE:${PN} = "blink.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

SRC_URI = " \
	file://blink.c 		\
	file://blink.service 	\
"

FILES:${PN} = "	\
	${systemd_unitdir}/system/blink.service \
	${bindir}/blink				\
"

do_compile() {
         ${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/blink.c -o ${WORKDIR}/blink
}

do_install() {
  install -d ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/blink.service ${D}/${systemd_unitdir}/system

  install -d ${D}/${bindir}
  install -m 0755 ${WORKDIR}/blink ${D}/${bindir}
}

RDEPENDS:${PN} += "bash"

COMPATIBLE_MACHINE:phyboard-electra-am64xx-2 = "phyboard-electra-am64xx-2"

INSANE_SKIP:${PN}-dbg += "buildpaths"
