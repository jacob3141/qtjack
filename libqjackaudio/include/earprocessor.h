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

#ifndef EARPROCESSOR_H
#define EARPROCESSOR_H

// Own includes:
#include "jackadapter.h"
#include "processor.h"
#include "digitalequalizer.h"
#include "jnoise.h"

// Qt includes:
#include <QVector>
#include <QMap>
#include <QDebug>
#include <QSemaphore>

/**
 * @class EarProcessor.
 *
 * @author Jacob Dawid ( jacob.dawid@cybercatalyst.net )
 * @author Otto Ritter ( otto.ritter.or@googlemail.com )
 * @date 09.2011
 *
 * @brief EarProcessor performs the central task of audio processing.
 *
 * The EAR processor compares the incoming delayed reference signal with the
 * recorded signal usually obtained by the microphone. In order to compensate
 * the differences in the the frequency spectrum it uses two independent
 * equalizers.
 */
class EarProcessor : public Processor
{
  Q_OBJECT
public:
    /** Operating modes. */
    enum OperationMode {
        CalibratingLatency,
        ProcessingAudio
    };

    /** Different kinds of signal sources. */
    enum SignalSource {
        ExternalSource,
        WhiteNoise,
        PinkNoise
    };

    /** Signal channels. */
    enum Channel {
        LeftChannel     = 0,
        RightChannel    = 1
    };

    /** Constructs a new EarProcessor. */
    EarProcessor();
    /** Destructor. */
    ~EarProcessor();

    /** Returns a pointer to the equalizer object for the left channel. */
    DigitalEqualizer *leftEqualizer();
    /** Returns a pointer to the equalizer object for the right channel. */
    DigitalEqualizer *rightEqualizer();

    /** Immediately sets the signal source to use. */
    void setSignalSource(SignalSource signalSource);
    /** Returns the the signal source that is used for processing. */
    SignalSource signalSource();

    /** Returns true, when the automatic adaption is active. */
    bool automaticAdaptionActive();
    /** Returns true, when the signal is being bypassed. */
    bool bypassActive();

    /** Returns the average microphone amplitude for the left channel. */
    int microphoneLevelLeft();

    /** Returns the average microphone amplitude for the right channel. */
    int microphoneLevelRight();

    /** Returns the average signal source amplitude for the left channel. */
    int signalSourceLevelLeft();

    /** Returns the average signal source amplitude for the right channel. */
    int signalSourceLevelRight();

    /**
      * Reimplemented from process. This will be called whenever there are new samples
      * available for processing.
      * Attention: This method is time critical.
      *
      * @param samples Number of samples that are available.
      */
    void process(int samples);

    /**
     * Provides the latency for the left channel.
     *
     * @return Latency measured in number of samples.
     * @see EarProcessor::processCalibration(int samples)
     */
    int leftLatency() { return m_calibration.m_latency[0]; }

    /**
     * Provides the latency for the right channel.
     * @return Latency measured in number of samples.
     * @see EarProcessor::processCalibration(int samples)
     */
    int rightLatency() { return m_calibration.m_latency[1]; }

public slots:
    /** Resets the calibration. */
    void calibrate();

    /** Set mode to "Rectification". */
    void setModeToRectification();

    /** Activates/deactivates automatic adaption. */
    void setAutomaticAdaptionActive(bool on);

    /** Activates/deactivates bypassing. */
    void setBypassActive(bool on);

signals:
    /** This signal is emitted when the calibration finished. */
    void calibrationFinished();

private:
    /** This method will fetch all input buffers to be ready for processing. */
    void fetchInputBuffers(int samples);

    /** Usually called at the end of audio processing, copies buffers back. */
    void writeOutputBuffers(int samples);

    /** Processes audio. */
    void processRectification(int samples);

    /** Processes calibration. */
    void processCalibration(int samples);

    /** Buffer for microphone input. This will be updated by fetchInputBuffers(). */
    fftw_complex m_microphoneBuffer[2][4096];

    /** Buffer for signal source input. This will be updated by fetchInputBuffers(). */
    fftw_complex m_signalSourceBuffer[2][4096];

    /** Buffer for signal output. The contents of this buffer will be written on
      * a call for writeOutputBuffers(). */
    fftw_complex m_outputBuffer[2][4096];

    /** Buffer for the left channel from microphone
      * in the frequency domain after dft. */
    fftw_complex m_leftMicrophoneFrequencyDomain[4096];

    /** Buffer for the right channel from microphone
      * in the frequency domain after dft. */
    fftw_complex m_rightMicrophoneFrequencyDomain[4096];

    /** Buffer for the left channel from delay buffer transformed
      * in the frequency domain after dft. */
    fftw_complex m_leftSignalSourceFrequencyDomain[4096];

    /** Buffer for the right channel from delay buffer transformed
      * in the frequency domain after dft. */
    fftw_complex m_rightSignalSourceFrequencyDomain[4096];

    /** Buffer for the left channel to compare
      * with the microphone feedback. */
    fftw_complex m_delayedLeftSignalSource[4096];

    /** Buffer for the right channel to compare
      * with the microphone feedback. */
    fftw_complex m_delayedRightSignalSource[4096];

    /** Contains the current operation mode for this unit. */
    OperationMode m_mode;

    /** Left equalizer. */
    DigitalEqualizer *m_leftEqualizer;

    /** Right equalizer. */
    DigitalEqualizer *m_rightEqualizer;

    /** Latency buffer for the left channel reference input. */
    QList<double> m_leftLatencyBuffer;

    /** Latency buffer for the right channel reference input. */
    QList<double> m_rightLatencyBuffer;

    /** This struct contains attributes that refer
      * to the calibration process. */
    struct Calibration {
        /** This is true if we are waiting for a click. */
        bool m_waitingForClick;
        /** Latency for the left and right channel. */
        int m_latency[2];
        /** Offset in case we are waiting for more than
          * a sample buffer period. */
        int m_offset;
        /** All previous latency measures. */
        QVector<int> m_latencyMeasures[2];
        /** Weighted map that correlates the number of occurences
          * of a measure to the measure itself. */
        QMap<int, int> m_weightedMeasures[2];
    } m_calibration;

    /** Identifier for the microphone input. */
    QString m_microphoneName;

    /** Identifier for the reference input. */
    QString m_signalSourceName;

    /** Identifier for the main output. */
    QString m_mainOutName;

    /** Ensures thread-safety. */
    QSemaphore *m_levelSemaphore;

    /** The average absolute amplitude for microphone input. */
    int m_microphoneLevel[2];

    /** The average absolute amplitude for the signal source input. */
    int m_signalSourceLevel[2];

    /** Semaphore for multithreaded access to the signal source. */
    QSemaphore *m_signalSourceSemaphore;

    /** Semaphore for multithreaded access to the automatic adaption state member. */
    QSemaphore *m_automaticAdaptionSemaphore;

    /** Semaphore for multithreaded access to the bypass state member. */
    QSemaphore *m_bypassSemaphore;

    /** The current signal source. */
    SignalSource m_signalSource;

    /** Pointer to the noise generator object. */
    JNoise *m_jNoise;

    /** Automatic adaption state. */
    bool m_automaticAdaptionActive;

    /** Bypass state. */
    bool m_bypassActive;

    /** Fake left sample buffer for generating noise. */
    jack_default_audio_sample_t m_fakeSampleBufferLeft[4096];

    /** Fake right sample buffer for generating noise. */
    jack_default_audio_sample_t m_fakeSampleBufferRight[4096];
};

#endif // EARPROCESSOR_H
