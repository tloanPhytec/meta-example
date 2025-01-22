FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " \
    file://custom-logo.png \
"

SPLASH_IMAGES = "file://custom-logo.png;outsuffix=default"

do_configure:prepend() {
    cp ${WORKDIR}/custom-logo.png ${S}/base-images/
    cp ${WORKDIR}/*.png ${S}/base-images/
}

