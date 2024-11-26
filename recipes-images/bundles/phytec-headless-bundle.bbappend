RAUC_KEY_FILE = "pkcs11:model=SoftHSM%20v2;manufacturer=SoftHSM%20project;serial=9a662fb637970ccd;token=RAUC%20Token;id=%01;object=RAUC%20Key;type=private;pin-value=1234"
RAUC_CERT_FILE = "${CERT_PATH}/cert.pem"
RAUC_KEYRING_FILE = "${CERT_PATH}/cert.pem"
RAUC_INTERMEDIATE_CERT_FILE = ""

DEPENDS:remove = "phytec-dev-ca-native"
DEPENDS += "softhsm-native"

do_bundle:prepend() {
	export SOFTHSM2_CONF="${HOME}/.config/softhsm2/softhsm2.conf"
	export PKCS11_MODULE_PATH="/usr/lib/softhsm/libsofthsm2.so"
}
