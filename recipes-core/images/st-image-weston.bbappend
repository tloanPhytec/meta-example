# FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# IMAGE_INSTALL:append = " set-passwd"

#SUMMARY = "Set root password"
#LICENSE = "MIT"

inherit extrausers

#PASSWD_BI = washington
#PASSWD_PHY= america
PASSWD_BI = "\$1\$LqdHX70M\$d1yLxhBiyI0ZKeVZD/RAV1"
PASSWD_PHY = "\$1\$1oeLMdHV\$dE5g8tCmARXnzjRJ2cres/"


#bainbridge is restricted, phytec has root access
EXTRA_USERS_PARAMS = "\
    useradd -p '${PASSWD_BI}' bainbridge; \
    usermod -s /bin/sh bainbridge; \
    useradd -u 0 -o -p '${PASSWD_PHY}' phytec; \
    usermod -s /bin/sh phytec; \
    usermod -p '!' root; \
"

