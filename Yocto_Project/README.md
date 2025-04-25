# Embedded Linux Yocto Build for Raspberry Pi 4

[//]: # (Demo Video Placeholder)
> **Demo Video:** [Watch a demo of scrcpy and rpi-play working on Raspberry Pi 4](TODO:_Add_URL)

A step-by-step guide to building a custom Yocto image for Raspberry Pi 4, including environment setup, layer management, and custom recipe creation.  
**Project Goal:** Build an infotainment system image with audio, graphical, and mirroring capabilities using Yocto Kirkstone (5.15.x kernel).

---

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Install Host Dependencies](#install-host-dependencies)
3. [Obtain and Prepare Poky](#obtain-and-prepare-poky)
4. [Initialize Build Environment](#initialize-build-environment)
5. [Configure Build Settings](#configure-build-settings)
6. [Workspace Structure](#workspace-structure)
7. [Layer Management](#layer-management)
8. [Custom Distributions and Layers](#custom-distributions-and-layers)
9. [Image Creation](#image-creation)
10. [Custom Recipe Implementations](#custom-recipe-implementations)
11. [Compile the Image](#compile-the-image)
12. [Deploy to SD Card](#deploy-to-sd-card)
13. [Activate Video Output](#activate-video-output)

---

## Prerequisites
- **Operating System:** Ubuntu or Debian-based
- **Hardware Target:** Raspberry Pi 4
- **Yocto Version:** Kirkstone (kernel v5.15.x)
- **Required Packages:** build-essential, chrpath, cpio, debianutils, diffstat, file, gawk, gcc, git, iputils-ping, libacl1, liblz4-tool, locales, python3, python3-git, python3-jinja2, python3-pexpect, python3-pip, python3-subunit, socat, texinfo, unzip, wget, xz-utils, zstd, make, inkscape, python3-saneyaml, python3-sphinx-rtd-theme, sphinx, texlive-latex-extra

## Install Host Dependencies
```bash
sudo apt update && sudo apt install \
  build-essential chrpath cpio debianutils diffstat file gawk gcc git iputils-ping \
  libacl1 liblz4-tool locales python3 python3-git python3-jinja2 python3-pexpect python3-pip \
  python3-subunit socat texinfo unzip wget xz-utils zstd make inkscape python3-saneyaml \
  python3-sphinx-rtd-theme sphinx texlive-latex-extra
```
Verify locale support:
```bash
locale --all-locales | grep en_US.utf8
```

## Obtain and Prepare Poky
Poky is the reference distribution for Yocto:
```bash
git clone https://github.com/yoctoproject/poky.git
cd poky
git checkout kirkstone
```

## Initialize Build Environment
```bash
source poky/oe-init-build-env build-raspberrypi4
```
This creates `build-raspberrypi4/` and loads necessary environment variables.

## Configure Build Settings
Edit `build-raspberrypi4/conf/local.conf`:
```ini
MACHINE ?= "raspberrypi4"
DL_DIR ?= "${TOPDIR}/../downloads"
BB_NUMBER_THREADS = "8"
PARALLEL_MAKE = "-j 8"
host_name:pn-base-files = "MyYocto"
# Pin kernel version (optional, recommended for reproducibility)
PREFERRED_VERSION_linux-raspberrypi ?= "5.15%"
```
> **Tip:** Set `BB_NUMBER_THREADS` and `PARALLEL_MAKE` to match your CPU cores for optimal performance.

## Workspace Structure
Arrange your project directories (assuming your folder is named `Yocto_Project`):
```
Yocto_Project/
├── build-raspberrypi4/   # Build workspace
├── downloads/            # Shared source cache
├── images/               # Output binaries
├── layers/               # Custom and community layers
└── poky/                 # Yocto reference code
```

## Layer Management
Clone required BSP and community layers:
```bash
cd layers
git clone -b kirkstone git://git.yoctoproject.org/meta-raspberrypi.git
git clone -b kirkstone git://git.openembedded.org/meta-openembedded.git
git clone -b kirkstone https://github.com/meta-qt5/meta-qt5.git
```

Add them in `bblayers.conf`:
```bitbake
BBLAYERS ?= " \
  ${TOPDIR}/../poky/meta \
  ${TOPDIR}/../poky/meta-poky \
  ${TOPDIR}/../poky/meta-yocto-bsp \
  ${TOPDIR}/../layers/meta-raspberrypi \
  ${TOPDIR}/../layers/meta-openembedded/meta-oe \
  ${TOPDIR}/../layers/meta-qt5 \
"
```

Set your custom layer's priority in `layer.conf`:
```bitbake
BBFILE_PRIORITY_meta-IVI = "8"
```

## Custom Distributions and Layers
1. **Create Distribution Layers**
   ```bash
   cd build-raspberrypi4
   bitbake-layers create-layer ../layers/meta-info-distro
   bitbake-layers create-layer ../layers/meta-audio-distro
   ```
2. **Enable Layers**
   ```bash
   cd build-raspberrypi4
   bitbake-layers add-layer ../layers/meta-info-distro
   bitbake-layers add-layer ../layers/meta-audio-distro
   ```
3. **Verify `bblayers.conf`** includes:
   ```bitbake
   BBLAYERS ?= " \
     ${TOPDIR}/../layers/meta-info-distro \
     ${TOPDIR}/../layers/meta-audio-distro \
   "
   ```
4. **Define Distros** under `layers/meta-info-distro/conf/distro/infotainment.conf` and `layers/meta-audio-distro/conf/distro/audio.conf` with respective DISTRO and features.

## Image Creation
Sample image recipe `layers/meta-IVI/recipes-core/images/ivi-test-image.bb`:
```bitbake
require recipes-core/images/rpi-test-image.bb
inherit audio qt-features network-features
IMAGE_INSTALL:append = " rpi-play vsomeip nano scrcpy"
IMAGE_FEATURES += "ssh-server-openssh tools-debug"
```

## Custom Recipe Implementations

Below are detailed steps for each custom Yocto recipe, illustrating how to create, configure, and integrate them into your image.

### 1. C++ HelloWorld Application (`cpp-helloworld`)
1. **Directory Structure**  
   ```bash
   mkdir -p layers/meta-IVI/recipes-cpp/cpp-helloworld/files
   ```
2. **Generate Base Recipe**  
   ```bash
   cd layers/meta-IVI/recipes-cpp/cpp-helloworld
   recipetool create -o cpp-helloworld_1.0.bb https://github.com/embeddedlinuxworkshop/y_t1.git
   ```
3. **Modify Recipe**  
   Edit `cpp-helloworld_1.0.bb` to include:
   ```bitbake
   S = "${WORKDIR}/git"
   do_compile() {
     ${CXX} ${S}/main.cpp -o cpp-app
   }
   do_install() {
     install -d ${D}${bindir}
     install -m 0755 cpp-app ${D}${bindir}
   }
   do_package_qa[noexec] = "1"
   ```

### 2. Local SSL Demo Application (`appssl`)
1. **Folder Layout**  
   ```bash
   mkdir -p layers/meta-IVI/recipes-cpp/appssl/files
   ```
2. **Place Source**  
   - Copy your `appssl.cpp` into `files/`.
3. **Recipe Definition**  
   ```bitbake
   SUMMARY = "C++ SHA256 hash demo with OpenSSL"
   LICENSE = "CLOSED"
   PV = "1.0"
   SRC_URI = "file://appssl.cpp"
   S = "${WORKDIR}"
   DEPENDS = "openssl"
   do_compile() {
     ${CXX} ${LDFLAGS} ${S}/appssl.cpp -lssl -lcrypto -o appssl
   }
   do_install() {
     install -d ${D}${bindir}
     install -m 0755 ${S}/appssl ${D}${bindir}
   }
   ```
> **Note:** If you use `LICENSE = "CLOSED"` or include proprietary software, ensure you comply with your organization’s licensing policies and any legal requirements.

### 3. GNU nano Editor (`nano`)
1. **Recipe via FTP**  
   ```bash
   mkdir -p layers/meta-IVI/recipes-editors/nano
   cd layers/meta-IVI/recipes-editors/nano
   recipetool create -o nano_1.0.bb https://ftp.gnu.org/gnu/nano/nano-7.2.tar.xz
   ```
2. **Patch `autogen.sh`**  
   - Remove private Gnulib cloning logic.  
   - Add patch file `0001-remove-gnulib-clone.patch` and include in `SRC_URI`.
   - Example patch:
     ```diff
     --- a/autogen.sh
     +++ b/autogen.sh
     @@ -1,9 +1,6 @@
     #!/bin/sh
     # Generate configure & friends for GIT users.
     -gnulib_url="git://git.sv.gnu.org/gnulib.git"
     -gnulib_hash="4631e9b4d5d3bd5ba42864c7001cf3509a00c7b3"
     -modules="
       canonicalize-lgpl
       futimens
     @@ -31,17 +28,7 @@ modules="
     # Make sure the local gnulib git repo is up-to-date.
     -if [ ! -d "gnulib" ]; then
     -	git clone --depth=2222 ${gnulib_url}
     -fi
     -cd gnulib >/dev/null || exit 1
     -curr_hash=$(git log -1 --format=%H)
     -if [ "${gnulib_hash}" != "${curr_hash}" ]; then
     -	echo "Pulling..."
     -	git pull
     -	git checkout --force ${gnulib_hash}
     -fi
     -cd .. >/dev/null || exit 1
     +
     rm -rf lib
     echo "Gnulib-tool..."
     ```
3. **Build Integration**
   - In `nano_1.0.bb`, ensure:
     - `inherit autotools gettext`  
     - `EXTRA_OECONF` runs `autogen.sh` before `do_configure`.

### 4. RPiPlay Screen Mirroring (`rpi-play`)
1. **Initial Structure**  
   ```bash
   mkdir -p layers/meta-IVI/recipes-screen/rpi-play
   cd layers/meta-IVI/recipes-screen/rpi-play
   recipetool create -o rpi-play_1.0.bb https://github.com/FD-/RPiPlay.git
   ```
2. **Resolve Dependencies**  
   - Add `meta-openembedded/meta-networking` for `avahi`.  
   - Patch `CMakeLists.txt` to search `/usr/src/hello_pi/libs/ilclient` instead of `/opt/vc`.

   > **Why patch?** The patch updates the CMake search path so ilclient is found in the sysroot, matching Yocto’s install location instead of the default `/opt/vc`.

3. **Recipe Adjustments**  
   ```bitbake
   DEPENDS = "userland avahi libplist gstreamer1.0"
   inherit cmake pkgconfig
   SRC_URI += " file://0001-include-ilclient.patch"
   EXTRA_OECMAKE += "-DCMAKE_SYSROOT_INCLUDE=/usr/src/hello_pi/libs"
   ```

### 5. SOME/IP Middleware (`vsomeip`)
1. **Layer Placement**  
   ```bash
   mkdir -p layers/meta-IVI/recipes-connectivity/vsomeip
   ```
2. **Bitbake Recipe**  
   ```bitbake
   SUMMARY = "The implementation of SOME/IP"
   SECTION = "base"
   LICENSE = "MPLv2"
   LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

   DEPENDS = "boost dlt-daemon"
   FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

   SRC_URI = "git://github.com/GENIVI/${BPN}.git;protocol=https"

   S = "${WORKDIR}/git"

   inherit cmake lib_package gitpkgv

   PACKAGES:remove = "${PN}-bin"
   FILES:${PN} += "${bindir}/vsomeipd ${sysconfdir}/${BPN}"
   FILES:${PN}-dev += "${libdir}/cmake"

   BBCLASSEXTEND = "nativesdk"

   do_install:append() {
     mv ${D}/usr/etc ${D}
   }
   ```
3. **Boost Version Pinning**  
   - Add `BBFILE_PRIORITY_meta-IVI = "8"` to prioritize your layer.

### 6. Android Mirroring (`scrcpy`)
1. **Recipe Initialization**  
   ```bash
   mkdir -p layers/meta-IVI/recipes-multimedia/scrcpy
   recipetool create -o scrcpy_1.0.bb git://github.com/Genymobile/scrcpy.git
   ```
2. **Configure Build**  
   ```bitbake
   DEPENDS = "meson-native ninja-native libusb1"
   RDEPENDS = "android-tools ffmpeg"
   inherit meson pkgconfig gettext
   EXTRA_OEMESON = "-Db_lto=true -Dprebuilt_server=${WORKDIR}/scrcpy-server-${PV}"
   ```
3. **License Handling**  
   - Add `LICENSE_FLAGS_ACCEPTED = "commercial_ffmpeg"` to `local.conf` for FFmpeg.
   > **Why?** FFmpeg is licensed as 'commercial' in Yocto. Without this flag, scrcpy and its dependencies will not build.

## Compile the Image

```bash
bitbake ivi-test-image
```
> **Output:** After a successful build, image files (such as `.wic` or `.img`) will be located in `build-raspberrypi4/tmp/deploy/images/raspberrypi4/`.

## Deploy to SD Card
### **Command to create image**  
| Command              | Usage                             | Example                                                  |
|----------------------|-----------------------------------|----------------------------------------------------------|
| `wic create <image>` | Generate a bootable `.wic` image. | `wic create sdimage-raspberrypi -e ivi-test-image `      |

### Flashing the Image to SD Card
```bash
sudo dd if=./<Output-Image> of=/dev/<SD-Card-Device> status=progress
```
> **Warning:** Double-check the SD card device path (e.g., `/dev/sdX` or `/dev/mmcblk0`) before running `dd`. Using the wrong device can overwrite your system disk and cause data loss.
> **Tip:** Use `lsblk` or `dmesg | tail` to identify your SD card device safely before flashing.

## Activate Video Output
Edit `/boot/config.txt` on the Raspberry Pi:
```ini
dtoverlay=vc4-fkms-v3d
max_framebuffers=2
```
> **Why?** These settings enable the Fake KMS (FKMS) driver, required for OpenGL/video acceleration on Raspberry Pi 4. Without them, some applications (like RPiPlay) may not display video output.  
Reboot to enable Fake KMS OpenGL support.

## Troubleshooting Build Errors
If you encounter build failures:
- Check logs in `build-raspberrypi4/tmp/log/` and per-task logs in `build-raspberrypi4/tmp/work/<machine>/<recipe>/temp/`.
- Use `bitbake -e <recipe>` to inspect environment variables.
- Review the error output in your terminal for missing dependencies or misconfigurations.
- Clean and rebuild a recipe with `bitbake -c clean <recipe>` then `bitbake <recipe>`.

For more help, consult the [Yocto Project Troubleshooting Guide](https://docs.yoctoproject.org/ref-manual/troubleshooting.html).

---

*Refer to [Yocto Docs](https://docs.yoctoproject.org/) for in-depth information.*

## Further Reading
- [Yocto Project Documentation](https://docs.yoctoproject.org/)
- [BitBake User Manual](https://docs.yoctoproject.org/bitbake/)
- [OpenEmbedded Layer Index](https://layers.openembedded.org/)
- [meta-raspberrypi Layer](https://github.com/agherzan/meta-raspberrypi)
- [Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/)

