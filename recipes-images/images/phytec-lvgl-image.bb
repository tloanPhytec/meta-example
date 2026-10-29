require recipes-images/images/phytec-headless-image.bb

SUMMARY = "This image is designed to show development of a Qt demo \
           running as an application on wayland."

inherit populate_sdk_qt6_base

inherit populate_sdk_qt6

IMAGE_FEATURES += "\
    splash \
    ssh-server-openssh \
    hwcodecs \
    qtcreator-debug \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'weston', '', d)} \
"

LICENSE = "MIT"

IMAGE_INSTALL += "\
    packagegroup-base \
    \
    packagegroup-gstreamer \
    \
    ${@bb.utils.contains("DISTRO_FEATURES", "virtualization", "packagegroup-virtualization", "", d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'qtwayland qtwayland-plugins weston weston-init', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11 wayland', 'weston-xwayland', '', d)} \
    ti-lvgl-demo \
    libgfortran \
"
