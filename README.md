# Branch am62-scarthgap-gpioIRQ

This branch serves demonstrates how to handle hardware GPIO interrupts within Linux user-space on the PHYTEC phyCORE-AM62x SOM. It includes a custom C application utilizing libgpiod and poll() to efficiently sleep the userspace application until a physical button press triggers a hardware interrupt, which wakes the application to toggle an external LED.

In order to evaluate this example, you'll need the following:

* An LED and Series resistor connected to GPIO0_36 and GND (these can be accessed at pin 18 and pin 6 of the X17 Expansion Connector, respectively).
  * Alternatively, you could rework the example to leverage the LEDs on the SOM. Feel free to reach out to PHYTEC's Support for some pointers. 

This was tested with BSP-Yocto-Ampliphy-AM62x-PD25.1.1

# BSP integration:

Navigate to your BSP's sources directory: 

```sh
cd $BUILDDIR/../sources
```

Clone this repo and branch: 

```sh
git clone https://github.com/tloanPhytec/meta-example.git -b am62-scarthgap-gpioIRQ
```

Enable the layer in your build: 

```sh
cd $BUILDDIR
bitbake-layers add-layer ../sources/meta-example
```

Rebuild your target's image:

```sh
MACHINE=phyboard-lyra-am62xx-3 bitbake phytec-qt6demo-image
```

# Verifying at Runtime:

Flash the resulting image to an SD Card and use it to boot your phyCORE-AM62x Development Kit. 

Once in Linux Userspace, you can confirm that the irq-blink.service is running:

```sh
root@phyboard-lyra-am62xx-3:~# systemctl status irq-blink
* irq-blink.service - IRQ Blink Service
     Loaded: loaded (/usr/lib/systemd/system/irq-blink.service; enabled; preset: enabled)
     Active: active (running) since Thu 2025-05-29 18:48:29 UTC; 25min ago
   Main PID: 306 (irq-blink)
      Tasks: 1 (limit: 2225)
     Memory: 240.0K (peak: 1.5M)
        CPU: 130ms
     CGroup: /system.slice/irq-blink.service
             `-306 /usr/bin/irq-blink

May 29 18:48:29 phyboard-lyra-am62xx-3 systemd[1]: Started IRQ Blink Service.
```

Now you can press the S1 User Button to generate an interrupt. The kernel's GPIO driver defines the ISR that then wakes up the userspace application to toggle the LED. 
