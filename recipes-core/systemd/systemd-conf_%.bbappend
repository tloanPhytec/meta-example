# meta-custom/recipes-core/systemd/systemd-conf_%.bbappend

FILESEXTRAPATHS_prepend := "${THISDIR}/${BPN}:"

inherit systemd

# Remove the CAN services from being included in the build
SRC_URI_remove = "file://can0.service file://can1.service"

# Disable the systemd services for CAN
SYSTEMD_SERVICE_${PN} = ""

do_install_append() {
    # Remove any accidental installation of the CAN services
    rm -f ${D}${systemd_system_unitdir}/can0.service
    rm -f ${D}${systemd_system_unitdir}/can1.service
}

