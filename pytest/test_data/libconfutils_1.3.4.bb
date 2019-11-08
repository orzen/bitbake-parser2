AXIS_NAME = "libs/confutils"
SECTION = "support"

LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://.axis-license;md5=6a4842bd6bc8d086023006ed4d033421"

DEPENDS = "filepp-native glib-2.0"

SRCREV = "R${PV}"

SRC_URI = "git://gittools.se.axis.com:29418/libs/confutils.git;protocol=ssh;nobranch=1"

S = "${WORKDIR}/git"

inherit axisbuild axislib axisunittest pkgconfig
