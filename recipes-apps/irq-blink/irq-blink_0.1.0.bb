SUMMARY = "Systemd IRQ Blink Service Recipe"
DESCRIPTION = "This blink recipe showcases how to compile a libgpiod C application leveraging interrupts from a meta layer,\
		and how to set it up as a systemd service that starts automatically on boot."
LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

inherit systemd

SYSTEMD_SERVICE:${PN} = "irq-blink.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

SRC_URI = " \
	file://irq-blink.c 		\
	file://irq-blink.service 	\
"

DEPENDS += "libgpiod"

FILES:${PN} = "	\
	${systemd_unitdir}/system/irq-blink.service \
	${bindir}/irq-blink				\
"

do_compile() {
	${CC} `${PKG_CONFIG} --cflags libgpiod` ${CFLAGS} ${LDFLAGS} ${WORKDIR}/irq-blink.c `${PKG_CONFIG} --libs libgpiod` -o ${WORKDIR}/irq-blink -lgpiod
}

do_install() {
	install -d ${D}/${systemd_unitdir}/system
	install -m 0644 ${WORKDIR}/irq-blink.service ${D}/${systemd_unitdir}/system

	install -d ${D}/${bindir}
	install -m 0755 ${WORKDIR}/irq-blink ${D}/${bindir}
}

RDEPENDS:${PN} += " \
	bash \
	libgpiod \
"

COMPATIBLE_MACHINE = "phyboard-lyra-am62xx-3"
