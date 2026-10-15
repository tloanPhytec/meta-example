# meta-example Branch 8mp-scarthgap-remoteWeston

[![Platforms](https://img.shields.io/badge/Platform-phyCORE--i.MX8MPlus-teal)](https://www.phytec.com/product/phycore-imx8m-plus/)

Headless Weston VNC setup for `phyboard-pollux-imx8mp-3` on Scarthgap.

This branch enables Weston's native VNC backend and Neat VNC TLS support.
Weston provides a 1280x800 desktop on TCP port 5901 using the Pixman software
renderer, thus making it possible to develop/control a GUI without a physical 
display panel. The `qtphy` systemd service gets
`QT_QUICK_BACKEND=software` so its Qt Quick scene runs inside Weston.

This was tested with BSP-Yocto-NXP-i.MX8MP-PD24.1.0

## BSP Integration

Navigate to your BSP's sources directory:

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch:

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b 8mp-scarthgap-remoteWeston
```

Add this layer to the PHYTEC i.MX8MP BSP:

```sh
cd $BUILDDIR
bitbake-layers add-layer $BUILDDIR/../sources/meta-example
```
Rebuild your target image:

```sh
DISTRO=ampliphy-vendor-xwayland MACHINE=phyboard-pollux-imx8mp-3 bitbake phytec-qt6demo-image
```

## Setup on First Boot

On the target, login as root and then set the Weston account password:

```sh
passwd weston
```

Download a TLS-capable VNC viewer such as tigerVNC:

https://sourceforge.net/projects/tigervnc/files/stable/

and use it to connect to `<target-ip>:5901` with username `weston`
and that password that you set. Accept the device's self-signed certificate when prompted.
Weston uses its existing `/etc/pam.d/weston-remote-access` policy; this layer
does not embed a password or replace that policy.

## Verify on the target

```sh
systemctl status weston-vnc-certificate weston qtphy
journalctl -u weston-vnc-certificate -u weston -b
ss -ltn | grep ':5901'
```
