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
#include "client.h"

// Standard includes
#include <cstdlib>

// Qt includes
#include <QStringList>

namespace QJack {

Client::Client() :
   QObject(),
   _processor(0) {
    _jackClient = 0;
}

Client::~Client() {
    jack_client_close(_jackClient);
}

bool Client::connectToServer(QString name) {
    if((_jackClient = jack_client_open(name.toStdString().c_str(), JackNullOption, NULL)) == 0) {
        return false;
    } else {
        // Set callbacks
        jack_set_thread_init_callback(_jackClient, Client::threadInitCallback, (void*)this);
        jack_set_process_callback(_jackClient, Client::processCallback, (void*)this);
        jack_set_freewheel_callback(_jackClient, Client::freewheelCallback, (void*)this);
        jack_set_client_registration_callback(_jackClient, Client::clientRegistrationCallback, (void*)this);
        jack_set_port_registration_callback(_jackClient, Client::portRegistrationCallback, (void*)this);
        jack_set_port_connect_callback(_jackClient, Client::portConnectCallback, (void*)this);
        jack_set_port_rename_callback(_jackClient, Client::portRenameCallback, (void*)this);
        jack_set_graph_order_callback(_jackClient, Client::graphOrderCallback, (void*)this);
        jack_set_latency_callback(_jackClient, Client::latencyCallback, (void*)this);
        jack_set_buffer_size_callback(_jackClient, Client::bufferSizeCallback, (void*)this);
        jack_set_sample_rate_callback(_jackClient, Client::sampleRateCallback, (void*)this);
        jack_set_xrun_callback(_jackClient, Client::xrunCallback, (void*)this);
        jack_on_shutdown(_jackClient, Client::shutdownCallback, (void*)this);
        jack_on_info_shutdown(_jackClient, Client::infoShutdownCallback, (void*)this);

        emit connectedToServer();
        return true;
    }
}

bool Client::disconnectFromServer() {
    if(jack_deactivate(_jackClient) == 0 && jack_client_close(_jackClient) == 0) {
        emit disconnectedFromServer();
        return true;
    } else {
        return false;
    }
}

Port Client::registerPort(QString name, QString portType, JackPortFlags jackPortFlags)  {
    Port jackPort(jack_port_register(
                           _jackClient,
                           name.toStdString().c_str(),
                           portType.toStdString().c_str(),
                           jackPortFlags, 0));
    emit portRegistered(jackPort);
    return jackPort;
}

Port Client::registerAudioOutPort(QString name) {
    return registerPort(name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);
}

Port Client::registerAudioInPort(QString name) {
    return registerPort(name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput);
}

Port Client::registerMidiOutPort(QString name) {
    return registerPort(name, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput);
}

Port Client::registerMidiInPort(QString name) {
    return registerPort(name, JACK_DEFAULT_MIDI_TYPE, JackPortIsInput);
}

bool Client::connectPorts(Port portA, Port portB) {
    int result = jack_connect(_jackClient,
                              portA.fullName().toStdString().c_str(),
                              portB.fullName().toStdString().c_str());
    return result == 0;
}

bool Client::disconnectPorts(Port portA, Port portB) {
    int result = jack_disconnect(_jackClient,
                                 portA.fullName().toStdString().c_str(),
                                 portB.fullName().toStdString().c_str());
    return result == 0;
}

QStringList Client::clientList() {
    QStringList clientList;
    const char **ports = jack_get_ports(_jackClient, 0, 0, 0);
    for(int i = 0; ports && ports[i]; ++i) {
        QString clientName = Port(jack_port_by_name(_jackClient, ports[i])).clientName();
        if(!clientList.contains(clientName)) {
            clientList.append(clientName);
        }
    }

    if(ports) {
        jack_free(ports);
    }

    return clientList;
}

QList<Port> Client::portsForClient(QString clientName) {
    QList<Port> portList;
    const char **ports = jack_get_ports(_jackClient, 0, 0, 0);
    for(int i = 0; ports && ports[i]; ++i) {
        Port port(jack_port_by_name(_jackClient, ports[i]));
        if(port.clientName() == clientName) {
            portList.append(port);
        }
    }

    if(ports) {
        jack_free(ports);
    }

    return portList;
}

bool Client::activate() {
    if(jack_activate(_jackClient) != 0) {
        emit activated();
        return true;
    }
    return false;
}

bool Client::deactivate() {
    if(jack_deactivate(_jackClient) != 0) {
        emit deactivated();
        return true;
    }
    return false;
}

void Client::startTransport() {
    jack_transport_start(_jackClient);
}

void Client::stopTransport() {
    jack_transport_stop(_jackClient);
}

int Client::sampleRate() {
    return jack_get_sample_rate(_jackClient);
}

int Client::bufferSize() {
    return jack_get_buffer_size(_jackClient);
}

float Client::cpuLoad() {
    return jack_cpu_load(_jackClient);
}

bool Client::isRealtime() {
    return jack_is_realtime(_jackClient) == 1;
}

void Client::setProcessor(Processor *audioProcessor) {
    _processor = audioProcessor;
}

void Client::threadInit() {
}

void Client::process(int samples) {
    if(_processor) {
        _processor->process(samples);
    }
}

void Client::freewheel(int starting) {
    Q_UNUSED(starting);
}

void Client::clientRegistration(const char *name, int reg) {
    Q_UNUSED(name);
    Q_UNUSED(reg);
}

void Client::portRegistration(jack_port_id_t port, int reg) {
    Q_UNUSED(port);
    Q_UNUSED(reg);
}

void Client::portConnect(jack_port_id_t a, jack_port_id_t b, int connect) {
    Q_UNUSED(a);
    Q_UNUSED(b);
    Q_UNUSED(connect);
}

void Client::portRename(jack_port_id_t port, const char *oldName, const char *newName) {
    Q_UNUSED(port);
    Q_UNUSED(oldName);
    Q_UNUSED(newName);
}

void Client::graphOrder() {
}

void Client::latency(jack_latency_callback_mode_t mode) {
    Q_UNUSED(mode);
}

void Client::sampleRate(int samples) {
    emit sampleRateChanged(samples);
}

void Client::bufferSize(int samples) {
    emit bufferSizeChanged(samples);
}

void Client::xrun() {
    emit xrunOccured();
}

void Client::shutdown() {
}

void Client::infoShutdown(jack_status_t code, const char *reason) {
    Q_UNUSED(code);
    Q_UNUSED(reason);
}

// Static callbacks

int Client::processCallback(jack_nframes_t sampleCount,
                        void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->process(sampleCount);
    }
    return 0;
}

void Client::threadInitCallback(void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->threadInit();
    }
}

void Client::freewheelCallback(int starting, void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->freewheel(starting);
    }
}

void Client::clientRegistrationCallback(const char* name, int reg, void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->clientRegistration(name, reg);
    }
}

void Client::portRegistrationCallback(jack_port_id_t port, int reg, void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->portRegistration(port, reg);
    }
}

void Client::portConnectCallback(jack_port_id_t a, jack_port_id_t b, int connect, void* argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->portConnect(a, b, connect);
    }
}

int Client::portRenameCallback(jack_port_id_t port, const char* oldName, const char* newName, void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->portRename(port, oldName, newName);
    }
    return 0;
}

int Client::graphOrderCallback(void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->graphOrder();
    }
    return 0;
}

void Client::latencyCallback(jack_latency_callback_mode_t mode, void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->latency(mode);
    }
}

int Client::sampleRateCallback(jack_nframes_t sampleCount,
                                   void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->sampleRate(sampleCount);
    }
    return 0;
}

int Client::bufferSizeCallback(jack_nframes_t bufferSize,
                                   void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->bufferSize(bufferSize);
    }
    return 0;
}

int Client::xrunCallback(void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->xrun();
    }
    return 0;
}

void Client::shutdownCallback(void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->shutdown();
    }
}

void Client::infoShutdownCallback(jack_status_t code, const char* reason, void *argument) {
    Client *jackClient = static_cast<Client*>(argument);
    if(jackClient) {
        jackClient->infoShutdown(code, reason);
    }
}

}
