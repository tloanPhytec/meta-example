# Branch imx93-scarthgap-carrierboard1616.0

BSP-Yocto-NXP-i.MX93-PD24.2.0 effectively deprecated the phyCORE-i.MX93 Deveopment Kit carrier board revision 1616.0 (specifcally, the PEB-AV-10 LCD Display Add-On will no longer work with the 1616.0 carrier board by default, but other interfaces should work). This can be seen in the following note from the release notes: 
  
  https://download.phytec.de/Software/Linux/BSP-Yocto-i.MX93/BSP-Yocto-NXP-i.MX93-PD24.2.0/ReleaseNotes

      With phyBOARD-Nash-i.MX 93 rev 1616.1 the logic of the the backlight signal for
      the PEB-AV-10 A/V adapter was inverted (from active-low to active-high).
      The BSP release PD24.2.0 implements that new logic.
      When using PD24.2.0 on a phyBOARD-Nash-i.MX 93 rev 1616.0 a customer must change
      logic again to active-low to get backlight working.

  This branch of meta-example re-enables 1616.0 support by setting up a new MACHINE=phyboard-nash-imx93-0 and patches the kernel appropriately so that the LCD display Add-On properly functions with that older carrier board.


  MACHINE=phyboard-nash-imx93-0 = Carrier Board PCB# 1616.0
  
  MACHINE=phyboard-nash-imx93-1 = Carrier Board PCB# 1616.1
  
**This was tested with BSP-Yocto-NXP-i.MX93-PD24.2.0**

In order to enable this support

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b imx93-scarthgap-carrierboard1616.0
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image with bitbake. Be sure to enable the new custom MACHINE, either on the cmd line or in your $BUILDDIR/conf/local.conf:

```sh
MACHINE=phyboard-nash-imx93-0 bitbake phytec-qt6demo-image
```
