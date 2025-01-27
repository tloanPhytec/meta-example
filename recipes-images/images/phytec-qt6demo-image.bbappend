IMAGE_INSTALL:append = " chromium-ozone-wayland"

LICENSE_FLAGS_ACCEPTED += "commercial_libav commercial_x264"
PACKAGECONFIG:append:pn-chromium-ozone-wayland = " proprietary-codecs"
