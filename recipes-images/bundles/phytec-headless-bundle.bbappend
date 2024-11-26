# RAUC_KEY_FILE and RAUC_CERT_FILE point to your key and certificate files,
# these are used during the build process to sign your bundle.
#
# RAUC_KEY_FILE in this example should be a PKCS#11 URI that queries a private key directly from softHSM2.
# RAUC_CERT_FILE is the public certificate corresponding to the private key.
#
# Note that the RAUC_KEY_FILE needs to be set in $BUILDDIR/conf/local.conf

RAUC_CERT_FILE = "${CERT_PATH}/cert.pem"

# RAUC_KEYRING_FILE points to the public keyring provisioned on the target,
# this is used for bundle verification, prior to install.

RAUC_KEYRING_FILE = "${CERT_PATH}/cert.pem"

# PHYTEC's RAUC reference implementation also supports the use case of Intermediate Certificates.
# This is out of scope for this particular example and thus we just disable that here.

RAUC_INTERMEDIATE_CERT_FILE = ""

DEPENDS:remove = "phytec-dev-ca-native"
DEPENDS += "softhsm-native"

# Setting these env variables is neccessary in order for the build system to access the Host's softHSM2 module.

do_bundle:prepend() {
	export SOFTHSM2_CONF="${HOME}/.config/softhsm2/softhsm2.conf"
	export PKCS11_MODULE_PATH="/usr/lib/softhsm/libsofthsm2.so"
}
