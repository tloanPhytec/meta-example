SUMMARY = "Systemd Python Service Recipe"
DESCRIPTION = "This python app recipe showcases how to incorporate a custom Python app in your target image from a meta layer,\
		and how to set it up as a systemd service that starts automatically on boot."
LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

inherit systemd

SYSTEMD_SERVICE:${PN} = "pythonapp.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

SRC_URI = " \
	file://pythonapp.py 		\
	file://pythonapp.service 	\
"

FILES:${PN} = "	\
	${systemd_unitdir}/system/pythonapp.service 	\
	/opt/pythonapp/pythonapp.py			\
"

do_install() {
  install -d ${D}/${systemd_unitdir}/system
  install -m 0644 ${WORKDIR}/pythonapp.service ${D}/${systemd_unitdir}/system

  install -d ${D}/opt/pythonapp
  install -m 0755 ${WORKDIR}/pythonapp.py ${D}/opt/pythonapp
}

RDEPENDS:${PN} += "bash python3"

COMPATIBLE_MACHINE .= "|phyboard-lyra-am62xx-3"
