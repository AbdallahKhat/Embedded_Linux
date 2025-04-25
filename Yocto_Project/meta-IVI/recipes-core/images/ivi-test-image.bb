
# 1. include base image --->
# 1. poky.
# 2. BSP.
require recipes-core/images/rpi-test-image.bb
# 2. Set of local variables.

SUMMARY="IVI Testing Image that include rpi func + Helloworld package recipe."

inherit audio


# 3. IMAGE_INSTALL & IMAGE_FEATURES
IMAGE_INSTALL:append= " helloworld openssh nano rpiplay scrcpy"


# 4. IMAGE FEATURES.
# 1. IMAGE_INSTALL ---> ssh
# 2. allow root access through ssh.
# 3. access root through ssh using empty password/
IMAGE_FEATURES:append=" debug-tweaks x11"