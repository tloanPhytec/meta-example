FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://serial-getty@.service"

do_install:append() {
    install -m 0644 ${WORKDIR}/serial-getty@.service ${D}${systemd_system_unitdir}/
}
