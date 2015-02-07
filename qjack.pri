INCLUDEPATH += \
    $$PWD

LIBS += \
    -L../qjack -lqjack

LIBS += -ljack -ljackserver -ljacknet
