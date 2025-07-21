BOOTENV_OVERLAYS_APPEND = "conf-imx8mp-phyboard-pollux-isp-csi1.dtbo#conf-imx8mp-phyboard-pollux-vm020-csi1-fpdlink-port0.dtbo#conf-imx8mp-phyboard-pollux-isp-csi2.dtbo#conf-imx8mp-phyboard-pollux-vm020-csi2-fpdlink-port0.dtbo"

do_deploy() {
    OVERLAYS_APPEND=$(echo "${BOOTENV_OVERLAYS_APPEND}" | sed -e "s/\s\+/ /g")
    if grep -q "^overlays=" ${DEPLOYDIR}/bootenv.txt; then
        sed -i -e "s/\(overlays=.*\)/\1 ${OVERLAYS_APPEND}/" ${DEPLOYDIR}/bootenv.txt
    else
        echo "overlays=${OVERLAYS_APPEND}" >> ${DEPLOYDIR}/bootenv.txt
    fi

    # Remove trailing whitespaces.
    sed -i -e "s/\ *$//g" ${DEPLOYDIR}/bootenv.txt
}
addtask deploy before do_build after do_unpack

COMPATIBLE_MACHINE .= "|phyboard-pollux-imx8mp-3"
