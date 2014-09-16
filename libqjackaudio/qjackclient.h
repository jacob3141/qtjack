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

#ifndef QJACKAUDIO_H
#define QJACKAUDIO_H

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


/** Defines a stereo port. */
typedef struct StereoPort {
    friend class QJackClient;
    /**
     * Provides the address to the sample bufferfor the left channel.
     * @param samples Number of samples.
     * @return Pointer to samples buffer.
     */
    jack_default_audio_sample_t *leftChannelBuffer(int samples)
    {
        return (jack_default_audio_sample_t*)jack_port_get_buffer(m_leftPort, samples);
    }

    /**
     * Provides the address to the sample buffer for the right channel.
     * @param samples Number of samples.
     * @return Pointer to samples buffer.
     */
    jack_default_audio_sample_t *rightChannelBuffer(int samples)
    {
        return (jack_default_audio_sample_t*)jack_port_get_buffer(m_rightPort, samples);
    }
private:
    /** Left JACK port associated with this stereo port. */
    jack_port_t *m_leftPort;
    /** Right JACK port associated with this stereo port. */
    jack_port_t *m_rightPort;
} StereoPort;

class QJackClientPrivate;

/**
 * @class QJackAudio
 *
 * @author Jacob Dawid ( jacob.dawid@cybercatalyst.net )
 * @author Otto Ritter ( otto.ritter.or@googlemail.com )
 * @date 09.2011
 *
 * @brief C++ Wrapper for the JACK Audio Connection Kit client API.
 *
 * This class wraps a singleton around the C API of JACK in order to provide
 * some additional features. Since it derives from QObject it can be integrated
 * into Qt's signals and slots system. Also, it includes a few routines
 * to simplify the integration of fftw.
 *
 */
class QJackClient : public QObject {
    Q_OBJECT
public:
    virtual ~QJackClient();

    /** Returns the instance for this singleton. */
    static QJackClient *instance();

    /**
      * This method attempts to connect to the audio server.
      * @param name Name that will be used to register this
      * application as a client.
      */
    bool connectToServer(QString name);


    QJackPort *registerPort(QString name, QJackPort::PortType portType, JackPortFlags jackPortFlags);
    QJackPort *registerAudioOutPort(QString name);
    QJackPort *registerAudioInPort(QString name);
    QJackPort *registerMidiOutPort(QString name);
    QJackPort *registerMidiInPort(QString name);


    /**
      * Assigns a processor that will handle audio processing.
      * @param processor The processor that will handle audio processing.
      */
    void setProcessor(QAudioProcessor *processor);

    /** Activates audio processing. */
    void startAudioProcessing();

    /** Deactivates audio processing and disconnects from the audio server. */
    void stopAudioProcessing();

    /**
      * Returns the current sample rate.
      * @return Sample rate in Hz.
      */
    int sampleRate();

    /**
      * Returns the current sample buffer size.
      * @return Buffer size in samples.
      */
    int bufferSize();

    /** Returns the current CPU load in percent. */
    float cpuLoad();

    /**
      * Returns the input stereo port associated with this label.
      * @param label Name of the stereo port.
      * @return Stereo port that has been requested.
      */
    StereoPort stereoInputPort(QString label);

    /**
      * Returns the output stereo port associated with this label.
      * @param label Name of the stereo port.
      * @return Stereo port that has been requested.
      */
    StereoPort stereoOutputPort(QString label);

signals:
    /** This signal will be emitted when an error occurs. */
    void error(const QString& errorMessage);

    void portRegistered(QString name);

private:
    /** Constructor. Initializes a new JackAdapter instance. */
    QJackClient();

    /** This signal will be emitted whenever an error occurs. */
    void emitError(const QString& errorMessage);

    /**
      * Private method to forward the request to process samples
      * from static methods.
      * @param samples Number of samples.
      */
    void processPrivate(int samples);

    /**
      * Callback for JACK's C API. Called when new samples
      * need to be processed.
      * @param sampleCount Number of samples.
      * @param argument Not in use.
      * @return Result of operation.
      */
    static int process(jack_nframes_t sampleCount, void *argument);

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

    /**
      * Callback for JACK's C API. Called when an error occurs.
      * @param message JACK's error message.
      */
    static void errorCallback(const char* message);

    /**
      * Callback for JACK's C API. Called when there is an
      * information message available.
      * @param message JACK's information message.
      */
    static void informationCallback(const char* message);

    /** JACK's C API client. */
    jack_client_t *_jackClient;

    /** Pointer to the current processor object. */
    QAudioProcessor *_audioProcessor;

    /** The current sample rate. */
    int _sampleRate;

    /** The current sample buffer size. */
    int _bufferSize;

    /** Maps input port identifiers to stereo ports. */
    QMap<QString, StereoPort> m_stereoInputPorts;

    /** Maps output port identifiers to stereo ports. */
    QMap<QString, StereoPort> m_stereoOutputPorts;

    /** Singleton instance for this class. */
    static QJackClient _instance;

    QJackClientPrivate *_d;
};

#endif // QJACKAUDIO_H
