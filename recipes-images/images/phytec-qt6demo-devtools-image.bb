require recipes-images/images/phytec-qt6demo-image.bb

IMAGE_INSTALL:append = " git packagegroup-core-buildessential systemd-bootchart python3-pip"
