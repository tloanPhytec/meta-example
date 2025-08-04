# Meta-Example Branch am64-scarthgap-PEB-C-010

This branch serves as a reference for enabling the KPEB-C-010.A0 Ethernet Expansion Board on the phyCORE-AM64x Development Kit.

This was tested with **BSP-Yocto-Ampliphy-AM64x-PD25.1.0**

## BSP Integration

In order to evaluate this on your phyCORE-AM64x Development Kit:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am64-scarthgap-PEB-C-010
```

Enable the layer in your build:

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake

```sh
MACHINE=phyboard-electra-am64xx-2 DISTRO=ampliphy bitbake phytec-headless-image
```

## Runtime Instructions

Prior to powering ON the development kit, connect your PEB-C-010 Ethernet Expansion Board to the phyCORE-AM64x Development Kit's X28 Expansion Connector. 

![IMG_0131](https://github.com/user-attachments/assets/e9ae92e1-a28c-440c-a88b-a08a7a26c422)

> **_NOTE:_**  **Connect the PEB-C-010 in the orientation shown above, the expansion board should hang off of the development kit carrier board.**
>
> The reverse orientation DOES NOT WORK.

Once connected boot your target's image all the way into Linux (this meta-example reference enables the overlay by default, so you don't have to manually enable the overlay in the bootloader console). 

Once booted into Linux Userspace, we can confirm that the new network interfaces eth3 and eth4 are now available like this:

```sh
root@phyboard-electra-am64xx-2:~# ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute
       valid_lft forever preferred_lft forever
2: eth0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN group default qlen 1000
    link/ether 1c:63:49:1a:f9:f8 brd ff:ff:ff:ff:ff:ff
3: main_mcan0: <NOARP,UP,LOWER_UP,ECHO> mtu 72 qdisc pfifo_fast state UP group default qlen 10
    link/can
4: main_mcan1: <NOARP,UP,LOWER_UP,ECHO> mtu 72 qdisc pfifo_fast state UP group default qlen 10
    link/can
5: eth1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN group default qlen 1000
    link/ether 62:65:6b:92:12:9f brd ff:ff:ff:ff:ff:ff
6: eth2: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN group default qlen 1000
    link/ether fe:27:c8:28:30:6f brd ff:ff:ff:ff:ff:ff
7: eth3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP group default qlen 1000
    link/ether ee:9f:9d:05:88:8b brd ff:ff:ff:ff:ff:ff
    inet 172.22.30.129/24 metric 1024 brd 172.22.30.255 scope global dynamic eth3
       valid_lft 85753sec preferred_lft 85753sec
    inet6 fe80::ec9f:9dff:fe05:888b/64 scope link proto kernel_ll
       valid_lft forever preferred_lft forever
8: eth4: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN group default qlen 1000
    link/ether 62:3f:92:3f:a6:da brd ff:ff:ff:ff:ff:ff
```
