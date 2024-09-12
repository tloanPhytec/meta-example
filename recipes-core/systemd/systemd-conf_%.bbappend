FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
    file://10-eth0.network \
"

do_install:append() {
    install -d ${D}${systemd_unitdir}/network/
    for file in $(find ${WORKDIR} -maxdepth 1 -type f -name *.network); do
        install -m 0644 "$file" ${D}${systemd_unitdir}/network/
    done
}

COMPATIBLE_MACHINE:phyboard-lyra-am62xx-3 = "phyboard-lyra-am62xx-3"
