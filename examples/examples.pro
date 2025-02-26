TEMPLATE=app
TARGET=app
DESTDIR=$$PWD/../lib
INCLUDEPATH += "$$PWD/.."
LIBS += -L. -L$$PWD/../lib -lods

message("PWD: "$$PWD)
message("DESTDIR: "$$DESTDIR)
message("INCLUDEPATH: "$$INCLUDEPATH)
message("LIBS: "$$LIBS)

SOURCES += examples1.cc \
examples2.cc \
Invoice.cpp \
InvoiceItem.cpp \
main.cpp \
util.cc


HEADERS += examples1.hh \
examples2.hh \
Invoice.hpp \
InvoiceItem.hpp \
util.hh
