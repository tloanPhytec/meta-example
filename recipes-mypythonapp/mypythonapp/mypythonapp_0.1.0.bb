SUMMARY = "Example Recipe For Installing a File"
DESCRIPTION = "Installs a python2 script to the rootfs"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM=""

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
	file://myPythonApp.py \
"

do_install() {
	install -d ${D}/${bindir}
	install -m 0755 ${WORKDIR}/myPythonApp.py ${D}/${bindir}/myPythonApp.py
}

FILES:${PN} += " \
	${bindir}/myPythonApp.py \
"

COMPATIBLE_MACHINE:phyboard-lyra-am62xx-3 = "phyboard-lyra-am62xx-3"
