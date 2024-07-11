# meta-example branch=am64-kirkstone-wks

This branch serves as a reference for both:
- defining a custom MACHINE that inherits everything from PHYTEC's provided BSPs (including references for getting the kernel & bootloader recipes to work with the new MACHINE)
- adding a custom kickstart file (.wks) to serve as a reference for adding additional data partitions to the generated .wic image yocto creates.
