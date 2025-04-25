

do_install () {
	# Manp. ---> ${WORKDIR}/image
	# Specify install commands here
	# 1. create directory ${WORKDIR}/image/usr/bin
	install -d ${D}/${bindir}
	# 2. installing hello bin ---> dir ${WORKDIR}/image/usr/bin
	install -m 0755 hello ${D}/${bindir}
}

# ignore ---> do_package_qa
do_package_qa[noexec]="1"