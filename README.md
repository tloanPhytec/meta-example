# Branch am62-scarthgap-pythonApp

This branch serves as a reference for adding custom Python applications to PHYTEC's phyCORE-AM62x BSP, along with a systemd service for starting it automatically at boot.

This was tested with BSP-Yocto-Ampliphy-AM62x-PD24.1.2

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-scarthgap-pythonApp
```

Enable the layer in your build:

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Open your BSP's local build configuration:

```sh
vi $BUILDDIR/conf/local.conf
```

Add the following lines to enable the recipes in your next build:

```sh
IMAGE_INSTALL:append = " pythonapp"
```

Rebuild your target's image with bitbake and use the resulting image to boot your Development Kit.

The pythonapp systemd service will start automatically upon boot (it is essentially a HelloWorld program on a 10 second loop). We can check the systemd journal logs to confirm it's running:

```sh
journalctl -u pythonapp.service -f
```

It can be stopped and started with the following:

```sh
systemctl stop pythonapp.service
systemctl start pythonapp.service
```
