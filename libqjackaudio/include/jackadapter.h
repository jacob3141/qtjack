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

#ifndef JACKADAPTER_H
#define JACKADAPTER_H

// JACK includes:
#include <jack/jack.h>

// Standard includes:
#include <iostream>

// Own includes:
#include "fftwadapter.h"
#include "processor.h"

// Qt includes:
#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include <QSemaphore>

/**
  * @class SemaphoreLocker
  * @author Jacob Dawid ( jacob.dawid@cybercatalyst.net )
  * Helper class for locking semaphores in scopes.
  */
class SemaphoreLocker {
public:
    /** Constructs a locker, locks the given semaphore on construction. */
    SemaphoreLocker(QSemaphore *semaphore) {
        Q_ASSERT(semaphore != 0);
        m_semaphore = semaphore;
        if(m_semaphore)
            m_semaphore->acquire();
    }

    /** Destructor. Unlocks the semaphore. */
    ~SemaphoreLocker() {
        if(m_semaphore)
            m_semaphore->release();
    }
private:
    /** Semaphore that has been locked. */
    QSemaphore *m_semaphore;
};

/** Defines a stereo port. */
typedef struct StereoPort {
    friend class JackAdapter;
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





/**
 * @class JackAdapter
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
class JackAdapter : public QObject {
    Q_OBJECT
public:

    /** Returns the instance for this singleton. */
    static JackAdapter *instance();

    /**
      * This method attempts to connect to the audio server.
      * @param name Name that will be used to register this
      * application as a client.
      */
    bool connectToServer(QString name);

    /**
      * Tries to register a new stereo input port.
      * @param label Name that will be used to register this port.
      */
    void registerStereoInputPort(QString label);

    /**
      * Tries to register a new stereo output port.
      * @param label Name that will be used to register this port.
      */
    void registerStereoOutputPort(QString label);

    /**
      * Assigns a processor that will handle audio processing.
      * @param processor The processor that will handle audio processing.
      */
    void setProcessor(Processor *processor);

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

private:
    /** Constructor. Initializes a new JackAdapter instance. */
    JackAdapter();

    /** Destructor. */
    ~JackAdapter();

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
    jack_client_t *m_jackClient
    ;
    /** Pointer to the current processor object. */
    Processor *m_processor;

    /** The current sample rate. */
    int m_sampleRate;

    /** The current sample buffer size. */
    int m_bufferSize;

    /** Maps input port identifiers to stereo ports. */
    QMap<QString, StereoPort> m_stereoInputPorts;

    /** Maps output port identifiers to stereo ports. */
    QMap<QString, StereoPort> m_stereoOutputPorts;

    /** Singleton instance for this class. */
    static JackAdapter m_instance;
};

#endif // JACKADAPTER_H
