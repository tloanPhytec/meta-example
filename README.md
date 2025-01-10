# Branch am62-scarthgap-linuxApps

This branch serves as a reference for adding custom Linux applications to PHYTEC's phyCORE-AM62x BSP.

- blink: this C code Blink example runs as a systemd service automatically upon boot. It blinks the User LED D24 on the phyCORE-AM62x Development Kit carrier board.
- hello: this C code HelloWorld example installs itself to the /usr/bin/ directory and can be called from userspace.

This was tested with BSP-Yocto-Ampliphy-AM62x-PD24.1.1

In order to evaluate this on your phyCORE-AM62x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-scarthgap-linuxApps
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
IMAGE_INSTALL:append = " blink helloworld"
```

Rebuild your target's image with bitbake and use the resulting image to boot your Development Kit.

- The blink systemd service will start automatically upon boot. It can be stopped and started with the following:

  ```sh
  systemctl stop blink.service
  systemctl start blink.service
  ```

- In order to evaluate the helloworld example, run the following in userspace:

  ```sh
  hello
  ```
