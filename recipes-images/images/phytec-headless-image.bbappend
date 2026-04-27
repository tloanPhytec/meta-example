SWUPDATE_LAYOUT_DIR := "${THISDIR}/files"

IMAGE_INSTALL:append = " swupdate swupdate-www"
PARTUP_LAYOUT_CONFIG:swupdate-ab = "${SWUPDATE_LAYOUT_DIR}/layout-swupdate.yaml"

IMAGE_FSTYPES:append = " ext4"
