# Branch stm-kirkstone-addUserPasswd

This branch serves as a reference for adding three users with differing permissions and passwords, as well as, disabling autologin on the phyCORE-STM32MP15x using PHYTEC's BSP.
Tested on BSP version PD23.1.1.

This example creates three users:
* user: bainbridge
  * Restricted permissions
  * Password defined by PASSWD_BI
* user: phytec
  * Root permissions
  * Password defined by PASSWD_PHY
* user: root
  * Root permissions.
  * Password is disabled (not usable by default)
  * See note at bottom to enable login for root

In order to evaluate this on your phyCORE-STM32MP15x Development Kit:

Navigate to your BSP's layers directory: 
```sh
cd $BUILDDIR/../layers
```
Clone this repo and branch: 
```sh
git clone https://github.com/tloanPhytec/meta-example.git -b stm-kirkstone-addUserPasswd
```
Generate hashed passwords. Yocto requires pre-hashed passwords. Use OpenSSL to generate hashes.
```sh
openssl passwd -1 yourpassword1
openssl passwd -1 yourpassword2
```
Replace the hash in the variables PASSWD_BI and PASSWD_PHY. The password will be the same text you used to generate the hash.

**Note**: Escape all $ signs with a backslash (\), as $ is a special character in Yocto syntax. 
```sh
vim meta-example/recipes-core/images/st-image-weston.bbappend
```
Enable the layer in your build: 
```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```
Rebuild your target's image with bitbake. 

After booting with the new image: 
* Login as either bainbridge or phytec with the passwords you configured.
* The root user will not be able to log in by default.

**Note: To re-enable Root Login**
If you'd like to enable login for root, edit st-image-weston.bbappend and remove or modify this line:
```sh
usermod -p '!' root;
```
This line disables root by setting its password hash to !. You can replace '!' with a valid hash (as done for the other users) if desired.
