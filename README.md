# Branch am62-scarthgap-rauc+softHSM2

This branch serves as a reference for customizing PHYTEC's ampliphy-rauc reference distribution, provided in PHYTEC's phyCORE-AM62x BSP, to leverage keys stored in a softHSM using the PKCS#11 interface to sign RAUC bundles.

This was tested with **BSP-Yocto-Ampliphy-AM62x-PD24.1.0**

**IMPORTANT NOTE:** BSP-Yocto-Ampliphy-AM62x-PD24.1.0 has a known issue with installing images to the phyCORE-AM62x's eMMC using *partup*. In order to successfully evaluate this meta-example reference, you must update your BSP-Yocto-Ampliphy-AM62x-PD24.1.0's meta-phytec layer with this patch:

https://git.phytec.de/meta-phytec/commit/?h=scarthgap&id=be240b25792c7ad91a3fd6859acf3a9fdacaee26

To easily do that, use the following commands:

```sh
cd $BUILDDIR/../sources/meta-phytec
git cherry-pick be240b25792c7ad91a3fd6859acf3a9fdacaee26
cd $BUILDDIR
```

**_Helpful Background Context:_**  PHYTEC's default ampliphy-rauc reference distribution leverages https://github.com/phytec/phytec-dev-ca, which is a **development** reference for a Root Certificate Authority. It is not advisable to store cryptographic keys and certificates in a unencrypted directory/repository outside of a development context, since this leaves your keys exposed to nefarious actors who could potentially use them to impersonate you and gain access to your devices and secrets. Hardware Security Modules (HSMs) are often employed to prevent this. An HSM is typically a physical, tamper-resistant device (such as a USB "stick", but there are other form factors, including cloud-based HSM services) that stores your keys for you and provides strict access control. The idea with these is that you can setup your development environment, plug in the HSM, leverage it to sign your software (without ever having your private key leave the HSM), and when you are done you take the HSM with you (ensuring that your keys are safe, even in the event that your Host Machine is compromised). As a development reference, this example will use a "SoftHSM", which is basically a HSM emulator completely implemented in software (SoftHSM is not generally recommended for use outside of development too). SoftHSMs and "real" HSMs both conform to the PKCS#11 cyptographic standard, which is essentially an API for creating and manipulating cryptographic tokens stored within HSMs.

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-scarthgap-rauc+softHSM2
```

Install SoftHSM2 dependencies:

```sh
sudo apt-get install softhsm2 opensc opensc-pkcs11 libengine-pkcs11-openssl gnutls-bin
```

Create a directory to store your public key certificate:

```sh
mkdir $BUILDDIR/../certs
cd $BUILDDIR/../certs
```

Setup SoftHSM2 on your Host:

```sh
mkdir -p ~/.config/softhsm2
mkdir -p ~/.softhsm2/tokens
vi ~/.config/softhsm2/softhsm2.conf
```

Write the following content to softhsm2.conf (be sure to replace <username> with your unique username, such that the "tokendir" path properly reflects your user's $HOME directory):

```sh
directories.tokendir = /home/<username>/.softhsm2/tokens
objectstore.backend = file
log.level = INFO
```

Create your PKI tree using SoftHSM2 (these are basic steps provided as a proof of concept, more complex PKI trees are possible but not covered here).

**Note:** you'll be prompted to set a PIN for your HSM token, be sure to remember this:

```sh
# initialize token
softhsm2-util --init-token --slot 0 --label "RAUC Token"

# Generate a new private key in the HSM
pkcs11-tool --module=/usr/lib/softhsm/libsofthsm2.so --login --pin <PIN> --keypairgen --key-type rsa:2048 --label "RAUC Key" --id 01
 
# extract JUST the public key to generate our certificate
pkcs11-tool --module=/usr/lib/softhsm/libsofthsm2.so --login --pin <PIN> --read-object --type pubkey --id 01 --output-file rauc_key.pub
 
# get new PKCS#11 private token URI (this is not the private key itself, just a way to query it from the HSM)
p11tool --provider=/usr/lib/softhsm/libsofthsm2.so --list-all --login
 
# example private key URI (replace this with your unique URI in the following steps): pkcs11:model=SoftHSM%20v2;manufacturer=SoftHSM%20project;serial=9a662fb637970ccd;token=RAUC%20Token;id=%01;object=RAUC%20Key;type=private
 
# generate the certificate signing request (CSR)
openssl req -new -engine pkcs11 -keyform engine -key "pkcs11:model=SoftHSM%20v2;manufacturer=SoftHSM%20project;serial=9a662fb637970ccd;token=RAUC%20Token;id=%01;object=RAUC%20Key;type=private" -out rauc_key.csr -subj "/CN=RAUC Token"
 
# sign the certificate
openssl x509 -req -engine pkcs11 -keyform engine -in rauc_key.csr -signkey "pkcs11:model=SoftHSM%20v2;manufacturer=SoftHSM%20project;serial=9a662fb637970ccd;token=RAUC%20Token;id=%01;object=RAUC%20Key;type=private" -out cert.pem -days 365
```

The result of the above should be a PKI tree in the HSM, and a public key certificate *cert.pem* located at $BUILDDIR/../certs

Enable the layer in your build:

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Add the following lines to your $BUILDDIR/conf/local.conf

**Note:** Here we must add the PIN you set above to the end of the PKCS#11 URI. The URI is a semi-colon seperated list of atributes, and we must add the "pin-value" attribute set equal to your unique PIN. In the realworld case of a "real" HSM, you would take the HSM with you when not using the Host Machine, thus if the PIN is compromised your keys are still secure.

```sh
RAUC_KEY_FILE = "pkcs11:model=SoftHSM%20v2;manufacturer=SoftHSM%20project;serial=9a662fb637970ccd;token=RAUC%20Token;id=%01;object=RAUC%20Key;type=private;pin-value=<PIN>"
CERT_PATH = "${TOPDIR}/../certs"
```

Rebuild your target's image and bundle

```sh
DISTRO=ampliphy-rauc bitbake phytec-headless-image
DISTRO=ampliphy-rauc bitbake phytec-headless-bundle
```

Install phytec-headless-image to your phyCORE-AM62x's eMMC (RAUC is supported only on the eMMC by default). This guide can help you flash the eMMC:
https://docs.phytec.com/projects/yocto-phycore-am62x/en/bsp-yocto-ampliphy-am62x-pd24.1.0/installos/flashEMMC.html

Boot into Linux using the eMMC. This guide can help you configure your boot switch settings:
https://docs.phytec.com/projects/yocto-phycore-am62x/en/bsp-yocto-ampliphy-am62x-pd24.1.0/bootingessentials/index.html

Transfer your .raucb bundle to the running phyCORE-AM62x and verify it's new signature:

```sh
rauc info phytec-headless-bundle-phyboard-lyra-am62xx-3.raucb
```

If the signature is valid, the install command will work. Go ahead and install the bundle:

```sh
rauc install phytec-headless-bundle-phyboard-lyra-am62xx-3.raucb
```

Switch the active RAUC partition to the one we just updated and then *reboot* to boot into it:

```sh
rauc status mark-active other
reboot
```

You can confirm which RAUC partition you are booted into using the following command:

```sh
rauc status
```
