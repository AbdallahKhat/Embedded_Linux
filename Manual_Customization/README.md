<!--
  Raspberry Pi 4 Customization Quick Reference
  ============================================
  This document summarizes the workflow for cross-compiling, bootloader and kernel customization, and root filesystem creation for the Raspberry Pi 4.
-->

---

## Toolchain Setup

> **Purpose:** Prepare a cross-compilation environment for ARM64.

**Install dependencies:**
```
sudo apt-get install autoconf automake bison bzip2 cmake flex g++ gawk gcc gettext git gperf help2man libncurses5-dev libstdc++6 libtool libtool-bin make patch python3-dev rsync texinfo unzip wget xz-utils
```

**crosstool-NG steps:**
1. Clone: `git clone https://github.com/crosstool-ng/crosstool-ng.git`
2. Build:
   ```
   cd crosstool-ng
   ./bootstrap
   ./configure --prefix=${PWD}
   make
   make install
   export PATH=$PATH:~/x-tools/aarch64-rpi4-linux-gnu/bin
   ```
3. Configure & build toolchain:
   ```
   bin/ct-ng distclean
   bin/ct-ng list-samples
   bin/ct-ng aarch64-rpi4-linux-gnu
   bin/ct-ng menuconfig
   bin/ct-ng build
   ```

---

## U-Boot Bootloader

> **Purpose:** Initialize hardware and load the kernel.

**Dependencies:**
```
sudo apt-get install gparted qemu-system device-tree-compiler libgnutls28-dev
```

**Build U-Boot:**
- Clone: `git clone git://git.denx.de/u-boot.git`
- Configure & build:
  ```
  cd u-boot
  make rpi_4_defconfig
  make CROSS_COMPILE=aarch64-rpi4-linux-gnu-
  ```

**Add start.elf header:**
- Clone tools: `git clone https://github.com/raspberrypi/tools --depth 1`
- Run script:
  ```
  sudo apt update
  sudo apt install python2
  python2 ./tools/mkimage/imagetool-uncompressed.py /path/to/u-boot/u-boot
  ```

---

## Kernel Compilation

> **Purpose:** Build and configure the Linux kernel for the Pi 4.

**Dependencies:**
```
sudo apt install libncurses5-dev flex bison subversion libssl-dev
sudo apt-get install qemu-system device-tree-compiler
sudo apt install bc libncurses5-dev
```

**Clone & configure:**
- `git clone --depth=1 https://github.com/raspberrypi/linux`
- `cd linux`
- `make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- bcm2711_defconfig`
- `make ARCH=arm64 menuconfig`

**Key config options:**
- `CONFIG_SERIAL_AMBA_PL011` (built-in)
- `CONFIG_SERIAL_AMBA_PL011_CONSOLE` (built-in)
- `CONFIG_DEVTMPFS` & `CONFIG_DEVTMPFS_MOUNT` (built-in)
- `CONFIG_BLK_DEV_INITRD` (built-in)
- *(Optional)* Early console: `CONFIG_EARLY_PRINTK` or `CONFIG_EARLYCON`

**Build:**
```
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- Image modules dtbs
```
- Output: `arch/arm64/boot/Image`

**Test with QEMU:**
```
qemu-system-aarch64 -M virt -cpu cortex-a53 -m 1G -kernel Image -append "console=ttyAMA0" -nographic
```

---

## Root Filesystem (RFS)

> **Purpose:** Provide a minimal environment for system boot using BusyBox.

**Create directories:**
```
mkdir -p rootfs/{bin,sbin,proc,sys,usr/bin,usr/sbin}
```

**Merge sysroot:**
```
rsync -avh path/to/sysroot rootfs/
```

**BusyBox build/install:**
- Clone: `git clone git://busybox.net/busybox.git`
- Checkout: `git checkout 1_36_stable`
- Configure: `make ARCH=arm64 CROSS_COMPILE=aarch64-rpi4-linux-gnu- menuconfig`
  - *(Disable Network → tc if needed)*
- Install: `make ARCH=arm64 CROSS_COMPILE=aarch64-rpi4-linux-gnu- install`

**Create initramfs:**
```
cd rootfs
find . -print0 | cpio --null -ov --format=newc > initramfs.cpio
gzip initramfs.cpio
```

**Boot with kernel & initramfs:**
```
qemu-system-aarch64 -M virt -cpu cortex-a53 -m 1G -kernel Image -append "console=ttyAMA0 earlycon=pl011,0x9000000 rdinit=/bin/sh" -initrd <initramfs> -nographic
```

---

<details>
<summary><strong>QEMU Command-Line Flags Breakdown</strong></summary>

| Flag                | Description                                                                 |
|---------------------|-----------------------------------------------------------------------------|
| <code>-M virt</code>         | Selects QEMU’s generic ARM virtual platform.                              |
| <code>-cpu cortex-a53</code> | Specifies the emulated CPU type (e.g., Cortex-A53).                       |
| <code>-m 1G</code>           | Allocates 1 GB of RAM to the virtual machine.                              |
| <code>-nographic</code>      | Disables graphical output; uses the terminal for serial console I/O.       |
| <code>-kernel</code>         | Path to the kernel image to boot.                                         |
| <code>-initrd</code>         | Path to the initramfs archive (CPIO).                                     |
| <code>-append</code>         | Kernel command-line arguments:                                            |
|                           | &nbsp;&nbsp;&bull; <code>console=ttyAMA0</code>: Use PL011 UART as main console.         |
|                           | &nbsp;&nbsp;&bull; <code>earlycon=pl011,0x9000000</code>: Early boot messages on UART. |
|                           | &nbsp;&nbsp;&bull; <code>rdinit=/bin/sh</code>: Set BusyBox shell as init process.     |

</details>

---

> **Tip:** If you see a blank screen, press Enter to get a BusyBox shell prompt.

---

*This quick reference is designed for rapid review and hands-on use. Adjust steps as needed for your project.*