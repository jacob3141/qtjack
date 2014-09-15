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

// Standard includes:
#include <cmath>

// Own includes:
#include "earprocessor.h"

using namespace FftwAdapter;

EarProcessor::EarProcessor()
    : Processor() {
    m_microphoneName = "mic";
    m_signalSourceName = "source";
    m_mainOutName = "main";

    m_automaticAdaptionActive = true;

    JackAdapter* jackClient = JackAdapter::instance();
    jackClient->registerStereoInputPort(m_signalSourceName);
    jackClient->registerStereoInputPort(m_microphoneName);
    jackClient->registerStereoOutputPort(m_mainOutName);
    m_leftEqualizer = new DigitalEqualizer();
    m_rightEqualizer = new DigitalEqualizer();

    m_mode = ProcessingAudio;
    m_signalSource = ExternalSource;
    m_jNoise = new JNoise();

    while(m_leftLatencyBuffer.size() < 44100)
        m_leftLatencyBuffer.append(0.0);

    while(m_rightLatencyBuffer.size() < 44100)
        m_rightLatencyBuffer.append(0.0);

    // Set 12000 samplas as default latency for left and right channel.
    m_calibration.m_latency[0] = 12000;
    m_calibration.m_latency[1] = 12000;
    emit calibrationFinished();

    m_levelSemaphore = new QSemaphore(1);
    m_signalSourceSemaphore = new QSemaphore(1);
    m_automaticAdaptionSemaphore = new QSemaphore(1);
    m_bypassSemaphore = new QSemaphore(1);
}

EarProcessor::~EarProcessor() {
    delete m_leftEqualizer;
    delete m_rightEqualizer;
    delete m_jNoise;
}

DigitalEqualizer *EarProcessor::leftEqualizer() {
    return m_leftEqualizer;
}

DigitalEqualizer *EarProcessor::rightEqualizer() {
    return m_rightEqualizer;
}

void EarProcessor::setSignalSource(SignalSource signalSource) {
    SemaphoreLocker locker(m_signalSourceSemaphore);
    m_signalSource = signalSource;
}

EarProcessor::SignalSource EarProcessor::signalSource() {
    SemaphoreLocker locker(m_signalSourceSemaphore);
    return m_signalSource;
}

bool EarProcessor::automaticAdaptionActive() {
    SemaphoreLocker locker(m_automaticAdaptionSemaphore);
    return m_automaticAdaptionActive;
}

bool EarProcessor::bypassActive() {
    SemaphoreLocker locker(m_bypassSemaphore);
    return m_bypassActive;
}

void EarProcessor::fetchInputBuffers(int samples) {
    int microphoneLevel[2] = {0, 0}, signalSourceLevel[2] = {0, 0};
    // Get the microphone input and copy it to fftw_complex buffers.
    StereoPort microphoneInputPort = JackAdapter::instance()->stereoInputPort(m_microphoneName);
    blit(microphoneInputPort.leftChannelBuffer(samples), m_microphoneBuffer[LeftChannel], samples);
    blit(microphoneInputPort.rightChannelBuffer(samples), m_microphoneBuffer[RightChannel], samples);

    // Get the signal source and it copy it to fftw_complex buffers.
    StereoPort sourceInputPort = JackAdapter::instance()->stereoInputPort(m_signalSourceName);

    switch(signalSource()) {
    case ExternalSource: {
        // When transferring music, read directly from JACK buffers.
        blit(sourceInputPort.leftChannelBuffer(samples), m_signalSourceBuffer[LeftChannel], samples);
        blit(sourceInputPort.rightChannelBuffer(samples), m_signalSourceBuffer[RightChannel], samples);
        break;
        }
    case WhiteNoise: {
        // Query for true buffer, so JACK thinks we've fetched them.
        (void)sourceInputPort.leftChannelBuffer(samples);
        (void)sourceInputPort.rightChannelBuffer(samples);
        // Pretend a fake buffer instead of the JACK buffer,
        // so the noise generator can produce noise in it.
        m_jNoise->process(samples, m_fakeSampleBufferLeft, m_fakeSampleBufferRight, 0, 0);
        blit(m_fakeSampleBufferLeft, m_signalSourceBuffer[LeftChannel], samples);
        blit(m_fakeSampleBufferRight, m_signalSourceBuffer[RightChannel], samples);
        break;
        }
    case PinkNoise: {
        // Query for true buffer, so JACK thinks we've fetched them.
        (void)sourceInputPort.leftChannelBuffer(samples);
        (void)sourceInputPort.rightChannelBuffer(samples);
        // Pretend a fake buffer instead of the JACK buffer,
        // so the noise generator can produce noise in it.
        m_jNoise->process(samples, 0, 0, m_fakeSampleBufferLeft, m_fakeSampleBufferRight);
        blit(m_fakeSampleBufferLeft, m_signalSourceBuffer[LeftChannel], samples);
        blit(m_fakeSampleBufferRight, m_signalSourceBuffer[RightChannel], samples);
        break;
        }
    }

    microphoneLevel[LeftChannel] = 0;
    microphoneLevel[RightChannel] = 0;
    signalSourceLevel[LeftChannel] = 0;
    signalSourceLevel[RightChannel] = 0;
    double previous, current;

    for(int i = 0; i < samples; i++) {
        previous = microphoneLevel[LeftChannel], current = abs(100 * m_microphoneBuffer[LeftChannel][i][0]);
        microphoneLevel[LeftChannel] = previous > current ? previous : current;

        previous = microphoneLevel[RightChannel], current = abs(100 * m_microphoneBuffer[RightChannel][i][0]);
        microphoneLevel[RightChannel] = previous > current ? previous : current;

        previous = signalSourceLevel[LeftChannel], current = abs(100 * m_signalSourceBuffer[LeftChannel][i][0]);
        signalSourceLevel[LeftChannel] = previous > current ? previous : current;

        previous = signalSourceLevel[RightChannel], current = abs(100 * m_signalSourceBuffer[RightChannel][i][0]);
        signalSourceLevel[RightChannel] = previous > current ? previous : current;
    }

    SemaphoreLocker locker(m_levelSemaphore);
    Q_UNUSED(locker);

    m_microphoneLevel[LeftChannel]      = microphoneLevel[LeftChannel];
    m_microphoneLevel[RightChannel]     = microphoneLevel[RightChannel];
    m_signalSourceLevel[LeftChannel]    = signalSourceLevel[LeftChannel];
    m_signalSourceLevel[RightChannel]   = signalSourceLevel[RightChannel];
}

void EarProcessor::writeOutputBuffers(int samples) {
    // Write result into the output buffers.
    StereoPort outputPort = JackAdapter::instance()->stereoOutputPort(m_mainOutName);
    blit(m_outputBuffer[LeftChannel], outputPort.leftChannelBuffer(samples), samples);
    blit(m_outputBuffer[RightChannel], outputPort.rightChannelBuffer(samples), samples);
}

int EarProcessor::microphoneLevelLeft() {
    SemaphoreLocker locker(m_levelSemaphore);
    return m_microphoneLevel[LeftChannel];
}

int EarProcessor::microphoneLevelRight() {
    SemaphoreLocker locker(m_levelSemaphore);
    return m_microphoneLevel[RightChannel];
}

int EarProcessor::signalSourceLevelLeft() {
    SemaphoreLocker locker(m_levelSemaphore);
    return m_signalSourceLevel[LeftChannel];
}

int EarProcessor::signalSourceLevelRight() {
    SemaphoreLocker locker(m_levelSemaphore);
    return m_signalSourceLevel[RightChannel];
}

void EarProcessor::process(int samples) {
    switch(m_mode)
    {
    case CalibratingLatency:
        processCalibration(samples);
        break;
    case ProcessingAudio:
        processRectification(samples);
        break;
    };
}

void EarProcessor::processRectification(int samples) {
    fetchInputBuffers(samples);

    // Shift samples of the reference signals into the latency buffers.
    for(int i = 0; i < samples; i++) {
        m_leftLatencyBuffer.append(m_signalSourceBuffer[LeftChannel][i][0]);
        m_rightLatencyBuffer.append(m_signalSourceBuffer[RightChannel][i][0]);
    }

    // Drops samples that are older than 44100 samples.
    while(m_leftLatencyBuffer.size() > 44100)
        m_leftLatencyBuffer.removeFirst();
    while(m_rightLatencyBuffer.size() > 44100)
        m_rightLatencyBuffer.removeFirst();


    if(automaticAdaptionActive()) {
        // As soon as we have enogh samples accumulated, we can start to compare.
        if(m_leftLatencyBuffer.size() > (leftLatency() - samples)) {
            // Extract delayed samples ready for a comparison.
            for(int i = 0; i < samples; i++) {
                m_delayedLeftSignalSource[i][0]
                    = m_leftLatencyBuffer.at(m_leftLatencyBuffer.size() - leftLatency() - samples + i);
                m_delayedLeftSignalSource[i][1] = 0;
            }

            // Get the spectrum by performing the dft.
            performFFT(m_microphoneBuffer[LeftChannel], m_leftMicrophoneFrequencyDomain, samples);
            performFFT(m_delayedLeftSignalSource, m_leftSignalSourceFrequencyDomain, samples);

            // Gain exclusive access to equalizer controls.
            m_leftEqualizer->acquireControls();
            double *leftEqualizerControls = m_leftEqualizer->controls();

            // Compare microphone signal and signal source (ie. music signal).
            for(int i = 0; i < 2048; i++) {
                double leftMicrophoneAmplitude
                    = sqrt(m_leftMicrophoneFrequencyDomain[i][0]
                         * m_leftMicrophoneFrequencyDomain[i][0]
                         + m_leftMicrophoneFrequencyDomain[i][1]
                         * m_leftMicrophoneFrequencyDomain[i][1]);
                double leftSignalAmplitude
                    = sqrt(m_leftSignalSourceFrequencyDomain[i][0]
                         * m_leftSignalSourceFrequencyDomain[i][0]
                         + m_leftSignalSourceFrequencyDomain[i][1]
                         * m_leftSignalSourceFrequencyDomain[i][1]);

                leftEqualizerControls[i]
                    += (leftSignalAmplitude - leftMicrophoneAmplitude) / 2
                        / (double)((2049 - i));
            }

            // Average filter for smoothing the controls.
            for(int i = 1; i < 2047; i++) {
                leftEqualizerControls[i] = (leftEqualizerControls[i-1]
                                          + leftEqualizerControls[i]
                                          + leftEqualizerControls[i+1])
                                            / 3.0;
            }

            // Limit controls to 0.01 .. 1.0.
            for(int i = 0; i < 2048; i++) {
                if(leftEqualizerControls[i] > 1.0)
                    leftEqualizerControls[i] = 1.0;
                if(leftEqualizerControls[i] < 0.01)
                    leftEqualizerControls[i] = 0.01;
            }

            // We're done manipulating the controls, release them.
            m_leftEqualizer->releaseControls();
        }

        // As soon as we have enogh samples accumulated, we can start to compare.
        if(m_rightLatencyBuffer.size() > (rightLatency() - samples)) {
            // Extract delayed samples ready for a comparison.
            for(int i = 0; i < samples; i++) {
                m_delayedRightSignalSource[i][0]
                    = m_rightLatencyBuffer.at(m_rightLatencyBuffer.size() - rightLatency() - samples + i);
                m_delayedRightSignalSource[i][1] = 0;
            }

            // Get the spectrum by performing the dft.
            performFFT(m_microphoneBuffer[RightChannel], m_rightMicrophoneFrequencyDomain, samples);
            performFFT(m_delayedRightSignalSource, m_rightSignalSourceFrequencyDomain, samples);

            // Gain exclusive access to equalizer controls.
            m_rightEqualizer->acquireControls();
            double *rightEqualizerControls = m_rightEqualizer->controls();

            // Compare microphone signal and signal source (ie. music signal).
            for(int i = 0; i < 2048; i++) {
                double rightMicrophoneAmplitude
                    = sqrt(m_rightMicrophoneFrequencyDomain[i][0]
                         * m_rightMicrophoneFrequencyDomain[i][0]
                         + m_rightMicrophoneFrequencyDomain[i][1]
                         * m_rightMicrophoneFrequencyDomain[i][1]);

                double rightSignalAmplitude
                    = sqrt(m_rightSignalSourceFrequencyDomain[i][0]
                         * m_rightSignalSourceFrequencyDomain[i][0]
                         + m_rightSignalSourceFrequencyDomain[i][1]
                         * m_rightSignalSourceFrequencyDomain[i][1]);

                rightEqualizerControls[i]
                    += (rightSignalAmplitude - rightMicrophoneAmplitude) / 2
                        / (double)((2049 - i));
            }

            // Average filter for smoothing the controls.
            for(int i = 1; i < 2047; i++) {
                rightEqualizerControls[i] = (rightEqualizerControls[i-1]
                                           + rightEqualizerControls[i]
                                           + rightEqualizerControls[i+1])
                                             / 3.0;
            }

            // Limit controls to 0.01 .. 1.0.
            for(int i = 0; i < 2048; i++) {
                if(rightEqualizerControls[i] > 1.0)
                    rightEqualizerControls[i] = 1.0;
                if(rightEqualizerControls[i] < 0.01)
                    rightEqualizerControls[i] = 0.01;
            }

            // We're done manipulating the controls, release them.
            m_rightEqualizer->releaseControls();
        }

        // We're done updating the controls, now generate a new filter.
        m_leftEqualizer->generateFilter();
        m_rightEqualizer->generateFilter();
    }    

    if(!bypassActive()) {
        // Run signals through equalizers into the output buffers.
        m_leftEqualizer->process(m_signalSourceBuffer[LeftChannel], m_outputBuffer[LeftChannel], samples);
        m_rightEqualizer->process(m_signalSourceBuffer[RightChannel], m_outputBuffer[RightChannel], samples);
    } else {
        // Bypass equalizers and copy the signal source in the output buffers.
        blit(m_signalSourceBuffer[LeftChannel], m_outputBuffer[LeftChannel], samples);
        blit(m_signalSourceBuffer[RightChannel], m_outputBuffer[RightChannel], samples);
    }
    writeOutputBuffers(samples);
}

void EarProcessor::processCalibration(int samples) {
    // The calibration process basically consists of two states:
    // 1.) Sending a signal
    // 2.) Waiting to receive the signal
    // The signal will be sent at the end of a buffer period, so that it could
    // be received - theoretically - in the next sample of the next period.

    // We are not waiting for a click, so we can prepare to send a new one.
    if(!m_calibration.m_waitingForClick) {
        // Reset the offset. We need the offset in case that it takes more
        // samples to receive the click than we are processing right now.
        m_calibration.m_offset = 0;

        // Generate new click. A click containes a single high sample at the
        // very end of the buffer period.
        for(int i = 0; i < samples; i++) {
            m_outputBuffer[LeftChannel][i][0] = (i < (samples - 1)) ? 0 : 1.0;
            m_outputBuffer[LeftChannel][i][1] = 0;
            m_outputBuffer[RightChannel][i][0] = (i < (samples - 1)) ? 0 : 1.0;
            m_outputBuffer[RightChannel][i][1] = 0;
        }

        // Now let's indicate we are waiting for the click to return.
        m_calibration.m_waitingForClick = true;
    } else {
        fetchInputBuffers(samples);

        // Find the first occurence of the maximum value.
        int maxLeft = 0;
        int maxRight = 0;
        for(int i = 0; i < samples; i++) {
            if(m_microphoneBuffer[LeftChannel][i][0] > m_microphoneBuffer[LeftChannel][maxLeft][0])
                maxLeft = i;
            if(m_microphoneBuffer[RightChannel][i][0] > m_microphoneBuffer[RightChannel][maxRight][0])
                maxRight = i;
        }

        // With maxLeft and maxRight we have two concrete candidates that may
        // be the incoming click. In order to not recognize noise as incoming
        // clicks, we use a threshold. Also, we suppose to have a minimum
        // latency to avoid echoing effects.
        double threshold = 0.8;
        int minLatency = 512;

        if(m_microphoneBuffer[LeftChannel][maxLeft][0] > threshold
        && m_microphoneBuffer[RightChannel][maxRight][0] > threshold
        && (maxLeft + m_calibration.m_offset > minLatency)
        && (maxRight + m_calibration.m_offset > minLatency)) {
            // This is an incoming click. Add the offsets to get the absolute
            // latency, because maxLeft and maxRight only contain the samples
            // in this period.
            maxLeft  += m_calibration.m_offset;
            maxRight += m_calibration.m_offset;

            // Append the result to the list of measures.
            m_calibration.m_latencyMeasures[0].append(maxLeft);
            m_calibration.m_latencyMeasures[1].append(maxRight);

            // Check if we have enough measures to determine the latency.
            if(m_calibration.m_latencyMeasures[0].size() > 20) {
                // Set the mode to Running, we are done with measuring.
                m_mode = ProcessingAudio;

                // Assort all measures in a weighted map. That way, we can
                // see how often a measured value appeared.
                foreach(int value, m_calibration.m_latencyMeasures[0])
                    m_calibration.m_weightedMeasures[0][value]++;
                foreach(int value, m_calibration.m_latencyMeasures[1])
                    m_calibration.m_weightedMeasures[1][value]++;

                // Now pick the measured value that appeared most. This is
                // more precise than just calculating the intermediate value,
                // because in practice the correct value appears multiple
                // times.
                int candidateLeft = 0;
                int candidateRight = 0;
                QList<int> keysLeft = m_calibration.m_weightedMeasures[0].keys();
                foreach(int key, keysLeft) {
                    if(m_calibration.m_weightedMeasures[0][key]
                        > m_calibration.m_weightedMeasures[0][candidateLeft])
                        candidateLeft = key;
                }

                QList<int> keysRight = m_calibration.m_weightedMeasures[1].keys();
                foreach(int key, keysRight) {
                    if(m_calibration.m_weightedMeasures[1][key]
                        > m_calibration.m_weightedMeasures[1][candidateRight])
                        candidateRight = key;
                }

                // Pick the best hit and take it as the latency.
                m_calibration.m_latency[0] = candidateLeft + 50;
                m_calibration.m_latency[1] = candidateRight + 50;

                emit calibrationFinished();
            }

            // We are not waiting for the click anymore,
            // send out the next one.
            m_calibration.m_waitingForClick = false;
        } else {
            // If we could not detect the click,
            // it may not be in this buffer period.
            m_calibration.m_offset += samples;
        }

        // Generate silence, we don't want to send anything out right now.
        for(int i = 0; i < samples; i++) {
            m_outputBuffer[LeftChannel][i][0] = 0;
            m_outputBuffer[LeftChannel][i][1] = 0;
            m_outputBuffer[RightChannel][i][0] = 0;
            m_outputBuffer[RightChannel][i][1] = 0;
        }
    }

    writeOutputBuffers(samples);
}

void EarProcessor::calibrate() {
    m_mode = CalibratingLatency;
    m_calibration.m_waitingForClick = false;
    m_calibration.m_latencyMeasures[0].clear();
    m_calibration.m_latencyMeasures[1].clear();
    m_calibration.m_weightedMeasures[0].clear();
    m_calibration.m_weightedMeasures[1].clear();
}

void EarProcessor::setModeToRectification() {
    m_mode = ProcessingAudio;
    m_calibration.m_waitingForClick = false;
    m_calibration.m_latencyMeasures[0].clear();
    m_calibration.m_latencyMeasures[1].clear();
    m_calibration.m_weightedMeasures[0].clear();
    m_calibration.m_weightedMeasures[1].clear();
}

void EarProcessor::setAutomaticAdaptionActive(bool on) {
    SemaphoreLocker locker(m_automaticAdaptionSemaphore);
    m_automaticAdaptionActive = on;
}

void EarProcessor::setBypassActive(bool on) {
    SemaphoreLocker locker(m_bypassSemaphore);
    m_bypassActive = on;
}

