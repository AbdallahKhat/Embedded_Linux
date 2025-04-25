# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)


# TODO 1. Documentation variables.

SUMMARY = "Example for Native C++ application for testing YOCTO"
DESCRIPTION = "Example for Native C++ application for testing YOCTO Provided by bullet.guru courses."
HOMEPAGE = "https://github.com/embeddedlinuxworkshop/y_t1"



# Unable to find any files that looked like license statements. Check the accompanying
# documentation and source headers and set LICENSE and LIC_FILES_CHKSUM accordingly.
#
# NOTE: LICENSE is being set to "CLOSED" to allow you to at least start building - if
# this is not accurate with respect to the licensing of the software being built (it
# will not be in most cases) you must specify the correct value before using this
# recipe for anything other than initial testing/development!

# TODO 2. License variables.
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

# TODO 3. Source code variables.

SRC_URI = "git://github.com/embeddedlinuxworkshop/y_t1;protocol=https;branch=master"

# Modify these as desired
SRCREV = "49600e3cd69332f0e7b8103918446302457cd950"

# Source code (main.cpp) ---> will be downloaded here.
S = "${WORKDIR}/git"

# NOTE: no Makefile found, unable to determine what needs to be done

# TODO. 4. Tasks executed through ---> bitbake

do_compile () {
	# Specify compilation commands here
	${CXX} ${S}/main.cpp -o hello
}

inherit helloworld