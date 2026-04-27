FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://swupdate.cfg"

PACKAGECONFIG:append = " archive"
