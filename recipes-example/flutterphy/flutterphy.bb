SUMMARY = "Flutter demo application"
DESCRIPTION = "Recipe for a Flutter demo application based on the Flutter Materials 3 demo"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

LICENSE = "CLOSED"

DEPENDS += " flutter-wayland-client flutter-samples-material-3-demo wayland libxkbcommon"

SRC_URI = " \
    file://flutter-demo.service \
"

inherit systemd

SYSTEMD_SERVICE:${PN} = "flutter-demo.service"

FILES:${PN} += "${systemd_unitdir}"

do_install:append() {
    install -Dm 0644 ${WORKDIR}/flutter-demo.service ${D}${systemd_system_unitdir}/flutter-demo.service
}
