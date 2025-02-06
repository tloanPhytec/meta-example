SUMMARY = "Example HelloWorld Recipe"
DESCRIPTION = "Compiles and installs a helloworld executable to the rootfs"
LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
	file://hello.c \
"
do_compile() {
         ${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/hello.c -o ${WORKDIR}/hello-world
}

do_install() {
	install -d ${D}/${bindir}
	install -m 0755 ${WORKDIR}/hello-world ${D}/${bindir}/hello-world
}

FILES:${PN} += " \
	${bindir}/hello-world \
"
