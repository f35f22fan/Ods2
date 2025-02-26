TEMPLATE = subdirs
CONFIG = cmdline
SUBDIRS += zlib libzip ods examples

examples.depends = ods
ods.depends = libzip
libzip.depends = zlib
