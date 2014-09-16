///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJackAudio.                                       //
//    Copyright (C) 2014 Jacob Dawid, jacob@omg-it.works                     //
//                                                                           //
//    QJackAudio is free software: you can redistribute it and/or modify     //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QJackAudio is distributed in the hope that it will be useful,          //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QJackAudio. If not, see <http://www.gnu.org/licenses/>.     //
//                                                                           //
//    It is possible to obtain a closed-source license of QJackAudio.        //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// Own includes:
#include <QJackClient>
#include <QSemaphoreLocker>

// Qt includes:
#include <QTime>

class QJackClientPrivate
{
public:
private:
};

QJackClient QJackClient::_instance;

QJackClient::QJackClient() :
   QObject(),
   _audioProcessor(new QAudioProcessor()),
   _d(new QJackClientPrivate) {
}

QJackClient::~QJackClient() {
    jack_client_close(_jackClient);
    delete _d;
}

void QJackClient::emitError(const QString &errorMessage) {
    emit error(errorMessage);
}

QJackClient *QJackClient::instance() {
    return &_instance;
}

bool QJackClient::connectToServer(QString name) {
    if((_jackClient = jack_client_open(name.toStdString().c_str(), JackNullOption, NULL)) == 0){
        emitError("Can't connect to JACK Server.");
        return false;
    }

    jack_set_process_callback(_jackClient, QJackClient::process, 0);
    jack_set_buffer_size_callback(_jackClient, QJackClient::bufferSizeCallback, 0);
    jack_set_sample_rate_callback(_jackClient, QJackClient::sampleRateCallback, 0);
    jack_set_error_function(QJackClient::errorCallback);
    jack_set_info_function(QJackClient::informationCallback);
    return true;
}

QJackPort *QJackClient::registerPort(QString name, QJackPort::PortType portType, JackPortFlags jackPortFlags)  {
    QString portTypeString;
    switch(portType) {
    case QJackPort::AudioPort: portTypeString = JACK_DEFAULT_AUDIO_TYPE; break;
    case QJackPort::MidiPort: portTypeString = JACK_DEFAULT_MIDI_TYPE; break;
    }

    QJackPort *jackPort = new QJackPort();
    jackPort->_port = jack_port_register(
        _jackClient,
        name.toStdString().c_str(),
        portTypeString.toStdString().c_str(),
        jackPortFlags, 0);
    jackPort->_name = name;
    emit portRegistered(name);
    return jackPort;
}

QJackPort *QJackClient::registerAudioOutPort(QString name) {
    return registerPort(name, QJackPort::AudioPort, JackPortIsOutput);
}

QJackPort *QJackClient::registerAudioInPort(QString name) {
    return registerPort(name, QJackPort::AudioPort, JackPortIsInput);
}

QJackPort *QJackClient::registerMidiOutPort(QString name) {
    return registerPort(name, QJackPort::MidiPort, JackPortIsOutput);
}

QJackPort *QJackClient::registerMidiInPort(QString name) {
    return registerPort(name, QJackPort::MidiPort, JackPortIsInput);
}

void QJackClient::startAudioProcessing() {
    if(jack_activate(_jackClient) != 0) {
        emitError("Activation of JACK client failed.");
    }
}

void QJackClient::stopAudioProcessing() {
    jack_deactivate(_jackClient);
}

int QJackClient::sampleRate() {
    return _sampleRate;
}

int QJackClient::bufferSize() {
    return _bufferSize;
}

float QJackClient::cpuLoad() {
    return jack_cpu_load(_jackClient);
}


void QJackClient::setAudioProcessor(QAudioProcessor *audioProcessor) {
    _audioProcessor = audioProcessor;
}

int QJackClient::process(jack_nframes_t sampleCount,
                        void *argument) {
    Q_UNUSED(argument);
    QJackClient::instance()->processPrivate(sampleCount);
    return 0;
}

void QJackClient::processPrivate(int samples) {
    if(_audioProcessor)
        _audioProcessor->process(samples);
}

int QJackClient::sampleRateCallback(jack_nframes_t sampleCount,
                                   void *argument) {
    Q_UNUSED(argument);
    instance()->_sampleRate = sampleCount;
    return 0;
}

int QJackClient::bufferSizeCallback(jack_nframes_t bufferSize,
                                   void *argument) {
    Q_UNUSED(argument);
    instance()->_bufferSize = bufferSize;
    return 0;
}

void QJackClient::errorCallback(const char *message) {
    instance()->emitError(QString(message));
}

void QJackClient::informationCallback(const char *message) {
    errorCallback(message);
}
