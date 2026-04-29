# Override this when building update bundles
SWUPDATE_SW_REVISION ?= "1.0"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
	file://defconfig.cfg \
"

do_install:append() {
    install -d ${D}${sysconfdir}

    echo "${MACHINE} ${SWUPDATE_SW_REVISION}" > ${D}${sysconfdir}/hwrevision

    chmod 0644 ${D}${sysconfdir}/hwrevision
}

FILES:${PN} += "${sysconfdir}/hwrevision"

PACKAGE_ARCH = "${MACHINE_ARCH}"
