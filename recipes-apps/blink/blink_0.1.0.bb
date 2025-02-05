SUMMARY = "Systemd Blink Service Recipe"
DESCRIPTION = "This blink recipe showcases how to compile a libgpiod C application from a meta layer,\
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

DEPENDS += "libgpiod"

FILES:${PN} = "	\
	${systemd_unitdir}/system/blink.service \
	${bindir}/blink				\
"

do_compile() {
	${CC} `${PKG_CONFIG} --cflags libgpiod` ${CFLAGS} ${LDFLAGS} ${WORKDIR}/blink.c `${PKG_CONFIG} --libs libgpiod` -o ${WORKDIR}/blink -lgpiod
}

do_install() {
	install -d ${D}/${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/blink.service ${D}/${systemd_unitdir}/system

	install -d ${D}/${bindir}
	install -m 0755 ${WORKDIR}/blink ${D}/${bindir}
}

RDEPENDS:${PN} += " \
	bash \
	libgpiod \
"

COMPATIBLE_MACHINE .= "|phyboard-pollux-imx8mp-3"
