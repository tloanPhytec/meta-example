SUMMARY = "AM62P 3D GPU Demo"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = " \
	file://main.c \
	file://model_data.h \
	file://back-icon.bmp \
"
	
S = "${WORKDIR}"

# Critical for GPU access
DEPENDS = "virtual/libsdl2 virtual/libgles2 wayland"
RDEPENDS:${PN} = "libsdl2 wayland"

do_compile() {
    # Compile linking GLESv2 and SDL2
    ${CC} main.c -o am62p-3d-demo -lSDL2 -lGLESv2 -lm ${LDFLAGS}
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 am62p-3d-demo ${D}${bindir}

    install -Dm 0644 ${WORKDIR}/back-icon.bmp ${D}${datadir}/${PN}/back-icon.bmp
}

FILES:${PN} += "${datadir}/${PN}/my_image.bmp"
