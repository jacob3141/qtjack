TEMPLATE = subdirs
SUBDIRS = demo libqjackaudio

demo.subdir = demo
demo.depends = libqjackaudio

libqjackaudio.subdir = libqjackaudio
libqjackaudio.depends =
