FILESEXTRAPATHS:prepend := "${THISDIR}/${BPN}:"

SRC_URI:append:phyboard-pollux-imx8mp-3 = " file://weston-vnc.ini"

do_install:append:phyboard-pollux-imx8mp-3 () {
	install -m 0644 ${WORKDIR}/weston-vnc.ini ${D}${sysconfdir}/xdg/weston/weston.ini
}
