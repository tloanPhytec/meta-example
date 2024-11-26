RAUC_CERT_FILE = "${CERT_PATH}/cert.pem"
RAUC_KEYRING_FILE = "${CERT_PATH}/cert.pem"
RAUC_INTERMEDIATE_CERT_FILE = ""

DEPENDS:remove = "phytec-dev-ca-native"
DEPENDS += "softhsm-native"

do_bundle:prepend() {
	export SOFTHSM2_CONF="${HOME}/.config/softhsm2/softhsm2.conf"
	export PKCS11_MODULE_PATH="/usr/lib/softhsm/libsofthsm2.so"
}
