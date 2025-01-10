SUMMARY = "Example Recipe For Installing a File"
DESCRIPTION = "Installs a bash script to the rootfs"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM=""

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
	file://example-app.sh \
"

RDEPENDS:${PN} = "bash"

do_install() {
	install -d ${D}/${bindir}
	install -m 0755 ${WORKDIR}/example-app.sh ${D}/${bindir}/example-app.sh
}

FILES:${PN} += " \
	${bindir}/example-app.sh \
"

COMPATIBLE_MACHINE .= "|phyboard-lyra-am62xx-3"
