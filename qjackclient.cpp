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

#include <cstdlib>

// Qt includes
#include <QStringList>

QJackClient::QJackClient() :
   QObject(),
   _audioProcessor(0) {
    _jackClient = 0;
}

QJackClient::~QJackClient() {
    jack_client_close(_jackClient);
}

bool QJackClient::connectToServer(QString name) {
    if((_jackClient = jack_client_open(name.toStdString().c_str(), JackNullOption, NULL)) == 0) {
        return false;
    } else {
        jack_set_process_callback(_jackClient, QJackClient::processCallback, (void*)this);
        jack_set_buffer_size_callback(_jackClient, QJackClient::bufferSizeCallback, (void*)this);
        jack_set_sample_rate_callback(_jackClient, QJackClient::sampleRateCallback, (void*)this);
        jack_set_xrun_callback(_jackClient, QJackClient::xrunCallback, (void*)this);

        emit connectedToServer();
        return true;
    }
}

bool QJackClient::disconnectFromServer() {
    if(jack_deactivate(_jackClient) == 0 && jack_client_close(_jackClient) == 0) {
        emit disconnectedFromServer();
        return true;
    } else {
        return false;
    }
}

QJackPort QJackClient::registerPort(QString name, QString portType, JackPortFlags jackPortFlags)  {
    QJackPort jackPort(jack_port_register(
                           _jackClient,
                           name.toStdString().c_str(),
                           portType.toStdString().c_str(),
                           jackPortFlags, 0));
    emit portRegistered(jackPort);
    return jackPort;
}

QJackPort QJackClient::registerAudioOutPort(QString name) {
    return registerPort(name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);
}

QJackPort QJackClient::registerAudioInPort(QString name) {
    return registerPort(name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput);
}

QJackPort QJackClient::registerMidiOutPort(QString name) {
    return registerPort(name, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput);
}

QJackPort QJackClient::registerMidiInPort(QString name) {
    return registerPort(name, JACK_DEFAULT_MIDI_TYPE, JackPortIsInput);
}

bool QJackClient::connectPorts(QJackPort portA, QJackPort portB) {
    int result = jack_connect(_jackClient,
                              portA.fullName().toStdString().c_str(),
                              portB.fullName().toStdString().c_str());
    return result == 0;
}

bool QJackClient::disconnectPorts(QJackPort portA, QJackPort portB) {
    int result = jack_disconnect(_jackClient,
                                 portA.fullName().toStdString().c_str(),
                                 portB.fullName().toStdString().c_str());
    return result == 0;
}

QStringList QJackClient::clientList() {
    QStringList clientList;
    const char **ports = jack_get_ports(_jackClient, 0, 0, 0);
    for(int i = 0; ports && ports[i]; ++i) {
        QString clientName = QJackPort(jack_port_by_name(_jackClient, ports[i])).clientName();
        if(!clientList.contains(clientName)) {
            clientList.append(clientName);
        }
    }

    if(ports) {
        jack_free(ports);
    }

    return clientList;
}

QList<QJackPort> QJackClient::portsForClient(QString clientName) {
    QList<QJackPort> portList;
    const char **ports = jack_get_ports(_jackClient, 0, 0, 0);
    for(int i = 0; ports && ports[i]; ++i) {
        QJackPort port(jack_port_by_name(_jackClient, ports[i]));
        if(port.clientName() == clientName) {
            portList.append(port);
        }
    }

    if(ports) {
        jack_free(ports);
    }

    return portList;
}

bool QJackClient::activate() {
    if(jack_activate(_jackClient) != 0) {
        emit activated();
        return true;
    }
    return false;
}

bool QJackClient::deactivate() {
    if(jack_deactivate(_jackClient) != 0) {
        emit deactivated();
        return true;
    }
    return false;
}

void QJackClient::startTransport() {
    jack_transport_start(_jackClient);
}

void QJackClient::stopTransport() {
    jack_transport_stop(_jackClient);
}

int QJackClient::sampleRate() {
    return jack_get_sample_rate(_jackClient);
}

int QJackClient::bufferSize() {
    return jack_get_buffer_size(_jackClient);
}

float QJackClient::cpuLoad() {
    return jack_cpu_load(_jackClient);
}

bool QJackClient::isRealtime() {
    return jack_is_realtime(_jackClient) == 1;
}

QString QJackClient::version() {
    return QString(jack_get_version_string());
}

void QJackClient::setAudioProcessor(QAudioProcessor *audioProcessor) {
    _audioProcessor = audioProcessor;
}

void QJackClient::threadInit() {
}

void QJackClient::process(int samples) {
    Q_UNUSED(samples);
    if(_audioProcessor)
        _audioProcessor->process();
}

void QJackClient::freewheel(int starting) {
    Q_UNUSED(starting);
}

void QJackClient::clientRegistration(const char *name, int reg) {
    Q_UNUSED(name);
    Q_UNUSED(reg);
}

void QJackClient::portRegistration(jack_port_id_t port, int reg) {
    Q_UNUSED(port);
    Q_UNUSED(reg);
}

void QJackClient::portConnect(jack_port_id_t a, jack_port_id_t b, int connect) {
    Q_UNUSED(a);
    Q_UNUSED(b);
    Q_UNUSED(connect);
}

void QJackClient::portRename(jack_port_id_t port, const char *oldName, const char *newName) {
    Q_UNUSED(port);
    Q_UNUSED(oldName);
    Q_UNUSED(newName);
}

void QJackClient::graphOrder() {
}

void QJackClient::latency(jack_latency_callback_mode_t mode) {
    Q_UNUSED(mode);
}

void QJackClient::sampleRate(int samples) {
    emit sampleRateChanged(samples);
}

void QJackClient::bufferSize(int samples) {
    emit bufferSizeChanged(samples);
}

void QJackClient::xrun() {
    emit xrunOccured();
}

void QJackClient::shutdown() {
}

void QJackClient::infoShutdown(jack_status_t code, const char *reason) {
    Q_UNUSED(code);
    Q_UNUSED(reason);
}

// Static callbacks

int QJackClient::processCallback(jack_nframes_t sampleCount,
                        void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->process(sampleCount);
    }
    return 0;
}

int QJackClient::threadInitCallback(void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->threadInit();
    }
    return 0;
}

int QJackClient::freewheelCallback(int starting, void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->freewheel(starting);
    }
    return 0;
}

int QJackClient::clientRegistrationCallback(const char* name, int reg, void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->clientRegistration(name, reg);
    }
    return 0;
}

int QJackClient::portRegistrationCallback(jack_port_id_t port, int reg, void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->portRegistration(port, reg);
    }
    return 0;
}

int QJackClient::portConnectCallback(jack_port_id_t a, jack_port_id_t b, int connect, void* argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->portConnect(a, b, connect);
    }
    return 0;
}

int QJackClient::portRenameCallback(jack_port_id_t port, const char* oldName, const char* newName, void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->portRename(port, oldName, newName);
    }
    return 0;
}

int QJackClient::graphOrderCallback(void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->graphOrder();
    }
    return 0;
}

int QJackClient::latencyCallback(jack_latency_callback_mode_t mode, void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->latency(mode);
    }
    return 0;
}

int QJackClient::sampleRateCallback(jack_nframes_t sampleCount,
                                   void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->sampleRate(sampleCount);
    }
    return 0;
}

int QJackClient::bufferSizeCallback(jack_nframes_t bufferSize,
                                   void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->bufferSize(bufferSize);
    }
    return 0;
}

int QJackClient::xrunCallback(void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->xrun();
    }
    return 0;
}

int QJackClient::shutdownCallback(void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->shutdown();
    }
    return 0;
}

int QJackClient::infoShutdownCallback(jack_status_t code, const char* reason, void *argument) {
    QJackClient *jackClient = static_cast<QJackClient*>(argument);
    if(jackClient) {
        jackClient->infoShutdown(code, reason);
    }
    return 0;
}
