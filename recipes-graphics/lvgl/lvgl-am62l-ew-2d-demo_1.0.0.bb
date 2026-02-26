SUMMARY = "Recipe for a LVGL v9.2 Demo for the phyFLEX-AM62L"
LICENSE = "MIT"
# Verified paths for v9.2.2: renamed from LICENCE.txt to LICENSE.txt
LIC_FILES_CHKSUM = " \
    file://LICENSE;md5=802d3d83ae80ef5f343050bf96cce3a4 \
    file://lvgl/LICENCE.txt;md5=bf1198c89ae87f043108cea62460b03a \
"
SRC_URI = " \
	git://github.com/lvgl/lv_port_linux.git;protocol=https;branch=master;name=demo \
	git://github.com/lvgl/lvgl;protocol=https;branch=release/v9.2;name=lvgl;subdir=git/lvgl \
	file://0001-2D-Image-Viewer-Demo-using-Weston.patch \
	file://0002-wip-am62l-2D-demo-add-two-finger-touch-support.patch \
	file://0003-wip-am62l-2D-demo-add-flip-button-support-and-FPS-HU.patch \
	file://0004-wip-demo-added-screensaver-mode-on-idle.patch \
	file://0005-demo-bug-fix.patch \
	file://0006-handheld-demo-add-back-button.patch \
	file://0001-lvgl-v9.1-add-two-finger-touch-support.patch;patchdir=lvgl \
	file://0002-lvgl-v9.1-fix-weston-frame-deadlock.patch;patchdir=lvgl \
	file://AM62L_Front.c \
	file://AM62L_Back.c \
	file://flip_icon.c \
	file://back_icon.c \
	file://am62l-2d-demo.service \
"

# v9.2.2 exact release hashes
SRCREV_lvgl = "7f07a129e8d77f4984fff8e623fd5be18ff42e74"
SRCREV_demo = "dccc6a1ca48372aa993dbea7a8e17dec6f42df6a"
SRCREV_FORMAT = "lvgl_demo"

S = "${WORKDIR}/git"

inherit cmake pkgconfig systemd

DEPENDS += "wayland wayland-native wayland-protocols libxkbcommon"


LVGL_CONFIG_USE_DRM = "0"
LVGL_CONFIG_USE_EVDEV = "0"
LVGL_CONFIG_USE_FBDEV = "0"
LVGL_CONFIG_USE_SDL = "0"
LVGL_CONFIG_USE_WAYLAND = "1"
LVGL_CONFIG_USE_OS = "LV_OS_PTHREAD"
LVGL_CONFIG_LV_USE_STDLIB_MALLOC = "LV_STDLIB_CLIB"
LVGL_CONFIG_LV_USE_STDLIB_STRING = "LV_STDLIB_CLIB"
LVGL_CONFIG_LV_USE_STDLIB_SPRINTF = "LV_STDLIB_CLIB"
LVGL_CONFIG_LV_USE_FLOAT = "1"
LVGL_CONFIG_LV_USE_VECTOR_GRAPHIC = "1"
LVGL_CONFIG_LV_LOG_LEVEL = "LV_LOG_LEVEL_ERROR"
LVGL_CONFIG_LV_COLOR_DEPTH = "32"
LVGL_CONFIG_LV_USE_ASSERT_MEM_INTEGRITY = "0"
LVGL_CONFIG_LV_USE_ASSERT_OBJ = "0"
LVGL_CONFIG_LV_LOG_TRACE_MEM = "0"
LVGL_CONFIG_LV_DEF_REFR_PERIOD = "16"

LVGL_CONFIG_USE_MATRIX = "1"
LVGL_CONFIG_LV_USE_DRAW_VG_LITE = "0"
LVGL_CONFIG_LV_DRAW_THREAD_STACK_SIZE = "(32 * 1024U)"

EXTRA_OECMAKE += " \
    -DLV_CONF_SKIP=OFF \
    -DLV_CONF_INCLUDE_SIMPLE=ON \
    -DLV_USE_WAYLAND=ON \
    -DCMAKE_EXE_LINKER_FLAGS="-lwayland-client -lwayland-cursor -lxkbcommon" \
"
TARGET_CFLAGS += " \
    -I${B}/lvgl/src/drivers/wayland \
    -DLV_USE_MATRIX=1 \
    -DLV_USE_FLOAT=1 \
    -DLV_DRAW_THREAD_STACK_SIZE=32768 \
"
TARGET_LDFLAGS += "-lwayland-client -lwayland-cursor -lxkbcommon"

do_configure:prepend() {
    # Define where the system stores the protocol XML
    XDG_SHELL_XML="${STAGING_DATADIR}/wayland-protocols/stable/xdg-shell/xdg-shell.xml"
    
    # Define the output directory (where lv_wayland.c expects to find it)
    GEN_DIR="${S}/lvgl/src/drivers/wayland"
    mkdir -p ${GEN_DIR}

    # Manually invoke the scanner to create the missing header and glue code
    wayland-scanner client-header ${XDG_SHELL_XML} ${GEN_DIR}/wayland_xdg_shell.h
    wayland-scanner private-code  ${XDG_SHELL_XML} ${GEN_DIR}/wayland_xdg_shell.c

    # also copy over converted image files
    cp ${WORKDIR}/AM62L_Front.c ${S}/
    cp ${WORKDIR}/AM62L_Back.c ${S}/
    cp ${WORKDIR}/flip_icon.c ${S}/
    cp ${WORKDIR}/back_icon.c ${S}/
}

do_configure:append() {
    sed -r -e "s|#if 0 .*Set it to \"1\" to enable content.*|#if 1 // Enabled by ${PN}|" \
	-e "s|^([[:space:]]*#define LV_USE_LINUX_DRM[[:space:]]).*|\1${LVGL_CONFIG_USE_DRM}|" \
	-e "s|^([[:space:]]*#define LV_USE_EVDEV[[:space:]]).*|\1${LVGL_CONFIG_USE_EVDEV}|" \
	-e "s|^([[:space:]]*#define LV_USE_FBDEV[[:space:]]).*|\1${LVGL_CONFIG_USE_FBDEV}|" \
	-e "s|^([[:space:]]*#define LV_USE_SDL[[:space:]]).*|\1${LVGL_CONFIG_USE_SDL}|" \
	-e "s|^([[:space:]]*#define LV_USE_OS[[:space:]]).*|\1${LVGL_CONFIG_USE_OS}|" \
        -e "s|^([[:space:]]*#define LV_USE_STDLIB_MALLOC[[:space:]]).*|\1${LVGL_CONFIG_LV_USE_STDLIB_MALLOC}|" \
        -e "s|^([[:space:]]*#define LV_USE_STDLIB_STRING[[:space:]]).*|\1${LVGL_CONFIG_LV_USE_STDLIB_STRING}|" \
        -e "s|^([[:space:]]*#define LV_USE_STDLIB_SPRINTF[[:space:]]).*|\1${LVGL_CONFIG_LV_USE_STDLIB_SPRINTF}|" \
        -e "s|^([[:space:]]*#define LV_USE_DRAW_VG_LITE[[:space:]]).*|\1${LVGL_CONFIG_LV_USE_DRAW_VG_LITE}|" \
	-e "s|^([[:space:]]*#define LV_USE_FLOAT[[:space:]]).*|\1${LVGL_CONFIG_LV_USE_FLOAT}|" \
	-e "s|^([[:space:]]*#define LV_USE_VECTOR_GRAPHIC[[:space:]]).*|\1${LVGL_CONFIG_LV_USE_VECTOR_GRAPHIC}|" \
	-e "s|^([[:space:]]*#define LV_LOG_LEVEL[[:space:]]).*|\1${LVGL_CONFIG_LV_LOG_LEVEL}|" \
	-e "s|^([[:space:]]*#define LV_COLOR_DEPTH[[:space:]]).*|\1${LVGL_CONFIG_LV_COLOR_DEPTH}|" \
	-e "s|^([[:space:]]*#define LV_USE_ASSERT_MEM_INTEGRITY[[:space:]]).*|\1${LVGL_CONFIG_LV_USE_ASSERT_MEM_INTEGRITY}|" \
	-e "s|^([[:space:]]*#define LV_USE_ASSERT_OBJ[[:space:]]).*|\1${LVGL_CONFIG_LV_USE_ASSERT_OBJ}|" \
	-e "s|^([[:space:]]*#define LV_LOG_TRACE_MEM[[:space:]]).*|\1${LVGL_CONFIG_LV_LOG_TRACE_MEM}|" \
	-e "s|^([[:space:]]*#define LV_DEF_REFR_PERIOD[[:space:]]).*|\1${LVGL_CONFIG_LV_DEF_REFR_PERIOD}|" \
        \
        -i "${S}/lv_conf.h"

	if grep -q "^#define LV_USE_MATRIX" ${S}/lv_conf.h; then
		sed -i "s|^#define LV_USE_MATRIX.*|#define LV_USE_MATRIX ${LVGL_CONFIG_USE_MATRIX}|" ${S}/lv_conf.h
	else
		echo "#define LV_USE_MATRIX ${LVGL_CONFIG_USE_MATRIX}" >> ${S}/lv_conf.h
	fi

	if grep -q "^#define LV_USE_WAYLAND" ${S}/lv_conf.h; then
		sed -i "s|^#define LV_USE_WAYLAND.*|#define LV_USE_WAYLAND ${LVGL_CONFIG_USE_WAYLAND}|" ${S}/lv_conf.h
	else
		echo "#define LV_USE_WAYLAND ${LVGL_CONFIG_USE_WAYLAND}" >> ${S}/lv_conf.h
	fi
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/bin/main ${D}${bindir}/lvgl

	install -Dm 0644 ${WORKDIR}/am62l-2d-demo.service ${D}${systemd_system_unitdir}/am62l-2d-demo.service
}

FILES:${PN} += "${systemd_unitdir}"

SYSTEMD_SERVICE:${PN} = "am62l-2d-demo.service"
