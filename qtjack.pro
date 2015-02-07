###############################################################################
##                                                                           ##
##    This file is part of QJack.                                            ##
##    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               ##
##                                                                           ##
##    QJack is free software: you can redistribute it and#or modify          ##
##    it under the terms of the GNU General Public License as published by   ##
##    the Free Software Foundation, either version 3 of the License, or      ##
##    (at your option) any later version.                                    ##
##                                                                           ##
##    QJack is distributed in the hope that it will be useful,               ##
##    but WITHOUT ANY WARRANTY; without even the implied warranty of         ##
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          ##
##    GNU General Public License for more details.                           ##
##                                                                           ##
##    You should have received a copy of the GNU General Public License      ##
##    along with QJack. If not, see <http:##www.gnu.org#licenses#>.          ##
##                                                                           ##
##    It is possible to obtain a closed-source license of QJack.             ##
##    If you're interested, contact me at: jacob@omg-it.works                ##
##                                                                           ##
###############################################################################

TEMPLATE = lib
CONFIG += staticlib
TARGET = qtjack

OBJECTS_DIR = .obj
MOC_DIR = .moc

SOURCES += \
    system.cpp \
    buffer.cpp \
    client.cpp \
    driver.cpp \
    parameter.cpp \
    port.cpp \
    server.cpp \
    netslave.cpp \
    netmaster.cpp \
    audioport.cpp \
    midiport.cpp \
    audiobuffer.cpp \
    midibuffer.cpp

HEADERS += \
    system.h \
    System \
    Buffer \
    buffer.h \
    Client \
    client.h \
    Driver \
    driver.h \
    Parameter \
    parameter.h \
    Port \
    port.h \
    Server \
    server.h \
    processor.h \
    Processor \
    ringbuffer.h \
    RingBuffer \
    netslave.h \
    NetSlave \
    netmaster.h \
    NetMaster \
    audioport.h \
    AudioPort \
    midiport.h \
    audiobuffer.h \
    midibuffer.h \
    AudioBuffer \
    MidiBuffer \
    global.h \
    MidiPort
