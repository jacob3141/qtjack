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

#pragma once

// JACK includes:
#include <jack/jack.h>

// Standard includes:
#include <iostream>

// Own includes:
#include <QFFTW>
#include <QAudioProcessor>
#include <QJackPort>

// Qt includes:
#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include <QSemaphore>

/**
 * @class QJackClient
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * @brief C++ Wrapper for the JACK Audio Connection Kit client API.
 * This class wraps a singleton around the C API of JACK in order to provide
 * some additional features. Since it derives from QObject it can be integrated
 * into Qt's signals and slots system. Also, it includes a few routines
 * to simplify the integration of fftw.
 */
class QJackClient : public QObject {
    Q_OBJECT
public:
    QJackClient();
    virtual ~QJackClient();

    /**
      * This method attempts to connect to the audio server.
      * @param name Name that will be used to register this
      * application as a client.
      */
    bool connectToServer(QString name);

    /**
     * Disconnects from the server.
     * @returns true, when successfully disconnected.
     */
    bool disconnectFromServer();

    /** Registers a port. Only possible, if connected to a JACK server. */
    QJackPort registerPort(QString name, QString portType, JackPortFlags jackPortFlags);

    /** Registers an audio output port. Only possible, if connected to a JACK server. */
    QJackPort registerAudioOutPort(QString name);

    /** Registers an audio input port. Only possible, if connected to a JACK server. */
    QJackPort registerAudioInPort(QString name);

    /** Registers a midi output port. Only possible, if connected to a JACK server. */
    QJackPort registerMidiOutPort(QString name);

    /** Registers a midi input port. Only possible, if connected to a JACK server. */
    QJackPort registerMidiInPort(QString name);

    /** Connects two ports. */
    bool connectPorts(QJackPort portA, QJackPort portB);

    /** Disconnects two ports. */
    bool disconnectPorts(QJackPort portA, QJackPort portB);

    /**
     * @returns a list of connected clients, that means their name to be specific.
     * This will only list client that offer ports.
     */
    QStringList clientList();

    /**
     * @param clientName The name of the client the ports should be listed of.
     * @returns a list of ports of this client.
     * @see clientList() to obtain a list of available clients.
     */
    QList<QJackPort> portsForClient(QString clientName);

    /** Assigns a processor that will handle audio processing.
      * @param processor The processor that will handle audio processing.
      */
    void setAudioProcessor(QAudioProcessor *processor);

    /** Activates audio processing for this client. */
    bool activate();

    /** Deactivates audio processing for this client. */
    bool deactivate();

    void startTransport();
    void stopTransport();

    /** @returns the sample rate in Hz. */
    int sampleRate();

    /** @returns the current buffer size in samples. */
    int bufferSize();

    /** @returns the current CPU load in percent. */
    float cpuLoad();

    /** @returns true, when running in realtime mode. */
    bool isRealtime();

    /** @returns the version string of the JACK version in use. */
    QString version();

signals:
    /** Emitted when successfully connected to JACK server. */
    void connectedToServer();

    /** Emitted when successfully disconnected from JACK server. */
    void disconnectedFromServer();

    /** Emitted when audio processing has been started successfully. */
    void activated();

    /** Emitted when audio processing has been stopped successfully. */
    void deactivated();

    /** Emitted whenever a port has been registered successfully by this client. */
    void portRegistered(QJackPort port);

    /** Emitted on change of the sample rate. */
    void sampleRateChanged(int sampleRate);

    /** Emitted on change of the buffer size. */
    void bufferSizeChanged(int bufferSize);

    /** Emitted when an xrun occurred. */
    void xrunOccured();

private:
    /**
      * Private method to forward the request to process samples
      * from static methods.
      * @param samples Number of samples.
      */
    void process(int samples);
    void sampleRate(int samples);
    void bufferSize(int samples);
    void xrun();

    /**
      * Callback for JACK's C API. Called when new samples
      * need to be processed.
      * @param sampleCount Number of samples.
      * @param argument Not in use.
      * @return Result of operation.
      */
    static int processCallback(jack_nframes_t sampleCount, void *argument);

    /**
      * Callback for JACK's C API. Called when sample rate changes.
      * @param sampleCount Number of samples.
      * @param argument Not in use.
      * @return Result of operation.
      */
    static int sampleRateCallback(jack_nframes_t sampleCount, void *argument);

    /**
      * Callback for JACK's C API. Called when the sample buffer size changes.
      * @param sampleCount Number of samples.
      * @param argument Not in use.
      * @return Result of operation.
      */
    static int bufferSizeCallback(jack_nframes_t sampleCount, void *argument);

    static int xrunCallback(void *argument);

    /** JACK's C API client. */
    jack_client_t *_jackClient;

    /** Pointer to the current processor object. */
    QAudioProcessor *_audioProcessor;
};
