DEPENDS:append = " libcxx-native"

do_configure:prepend() {
    SRC_DIR="${S}/third_party/blink/renderer/core"
    TARGET_FILE="${SRC_DIR}/BUILD.gn"

    if [ -f "${TARGET_FILE}" ]; then
        echo "[INFO] Patching ${TARGET_FILE}: removing mnemonic assignment, unused assignments break Google's GN"
        # Delete any line that assigns mnemonic = "ELOC_PROTO"
        sed -i '/^[[:space:]]*mnemonic[[:space:]]*=[[:space:]]*"ELOC_PROTO"/d' "${TARGET_FILE}"
    else
        echo "[WARN] ${TARGET_FILE} not found (build layout may have changed)"
    fi
}
