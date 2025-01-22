require recipes-images/images/phytec-headless-image.bb

SUMMARY =  "This image is designed to show development of a Flutter application \
            running on wayland."

IMAGE_FEATURES += "\
    splash \
    ssh-server-openssh \
    hwcodecs \
    weston \
"

LICENSE = "MIT"

IMAGE_INSTALL += "\
    packagegroup-base \
    \
    packagegroup-gstreamer \
    \
    weston weston-init \
    weston-xwayland \
    flutter-wayland-client \
    flutter-samples-material-3-demo \
    flutterphy \
"
