/* This file is part of EAR, an audio processing tool.
 *
 * Copyright (C) 2011 Otto Ritter, Jacob Dawid
 * otto.ritter.or@googlemail.com
 * jacob.dawid@cybercatalyst.net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Affero GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// JACK includes:
#include "jackadapter.h"

// Qt includes:
#include <QTime>
#include <QDebug>

JackAdapter JackAdapter::m_instance;

JackAdapter::JackAdapter() :
   QObject (),
   m_processor(0) {
}

JackAdapter::~JackAdapter() {
    jack_client_close(m_jackClient);
}

void JackAdapter::emitError(const QString &errorMessage) {
    emit error(errorMessage);
}

JackAdapter *JackAdapter::instance() {
    return &m_instance;
}

bool JackAdapter::connectToServer(QString name) {
    if((m_jackClient = jack_client_open(name.toStdString().c_str(), JackNullOption, NULL)) == 0){
        return false;
        qDebug() << "Can't connect to JACK Server.";
    }

    jack_set_process_callback(m_jackClient, JackAdapter::process, 0);
    jack_set_buffer_size_callback(m_jackClient, JackAdapter::bufferSizeCallback, 0);
    jack_set_sample_rate_callback(m_jackClient, JackAdapter::sampleRateCallback, 0);
    jack_set_error_function(JackAdapter::errorCallback);
    jack_set_info_function(JackAdapter::informationCallback);
    return true;
}

void JackAdapter::startAudioProcessing() {
    if(jack_activate(m_jackClient) != 0) {
        emitError("Activation of JACK client failed.");
    }
}

void JackAdapter::stopAudioProcessing() {
    jack_deactivate(m_jackClient);
}

int JackAdapter::sampleRate() {
    return m_sampleRate;
}

int JackAdapter::bufferSize() {
    return m_bufferSize;
}

float JackAdapter::cpuLoad() {
    return jack_cpu_load(m_jackClient);
}

StereoPort JackAdapter::stereoInputPort(QString label) {
    return m_stereoInputPorts[label];
}

StereoPort JackAdapter::stereoOutputPort(QString label) {
    return m_stereoOutputPorts[label];
}

void JackAdapter::registerStereoInputPort(QString label) {
    StereoPort stereoPort;
    stereoPort.m_leftPort = jack_port_register(m_jackClient,
        (label + "_1").toStdString().c_str(), JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsInput, 0);
    stereoPort.m_rightPort = jack_port_register(m_jackClient,
        (label + "_2").toStdString().c_str(), JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsInput, 0);
    m_stereoInputPorts[label] = stereoPort;
}

void JackAdapter::registerStereoOutputPort(QString label) {
    StereoPort stereoPort;
    stereoPort.m_leftPort = jack_port_register(m_jackClient,
        (label + "_1").toStdString().c_str(), JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsOutput, 0);
    stereoPort.m_rightPort = jack_port_register(m_jackClient,
        (label + "_2").toStdString().c_str(), JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsOutput, 0);
    m_stereoOutputPorts[label] = stereoPort;
}

void JackAdapter::setProcessor(Processor *processor) {
    m_processor = processor;
}

int JackAdapter::process(jack_nframes_t sampleCount,
                        void *argument) {
    Q_UNUSED(argument);
    JackAdapter::instance()->processPrivate(sampleCount);
    return 0;
}

void JackAdapter::processPrivate(int samples) {
    if(m_processor)
        m_processor->process(samples);
}

int JackAdapter::sampleRateCallback(jack_nframes_t sampleCount,
                                   void *argument) {
    Q_UNUSED(argument);
    instance()->m_sampleRate = sampleCount;
    return 0;
}

int JackAdapter::bufferSizeCallback(jack_nframes_t bufferSize,
                                   void *argument) {
    Q_UNUSED(argument);
    instance()->m_bufferSize = bufferSize;
    return 0;
}

void JackAdapter::errorCallback(const char *message) {
    instance()->emitError(QString(message));
}

void JackAdapter::informationCallback(const char *message) {
    errorCallback(message);
}
