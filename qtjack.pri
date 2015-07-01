INCLUDEPATH += \
    $$PWD

LIBS += \
    -L../qtjack -lqtjack

LIBS += -ljack -ljackserver

contains(DEFINES, QTJACK_JACK2_SUPPORT) {
  LIBS += -ljacknet
}
