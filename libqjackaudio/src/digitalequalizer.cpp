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

#include "digitalequalizer.h"
#include <cmath>
#include <QFile>
#include <QDebug>
#include <QStringList>

DigitalEqualizer::DigitalEqualizer() {
    m_numberOfControls = MAX_NUMBER_OF_CONTROLS;
    for(int i = 0; i < MAX_NUMBER_OF_CONTROLS * 2; i++) {
        m_delayLine[i] = 0.0;
    }
    m_numberOfControlsAccessSemaphore = new QSemaphore(1);
    m_controlsAccessSemaphore = new QSemaphore(1);
    acquireControls();
    for(int i = 0; i < MAX_NUMBER_OF_CONTROLS; i++) {
        m_controls[i] = 1.0;
    }
    releaseControls();
    generateFilter();
}

DigitalEqualizer::~DigitalEqualizer() {
    delete m_numberOfControlsAccessSemaphore;
    delete m_controlsAccessSemaphore;
}

void DigitalEqualizer::setNumberOfControls(int n) {
    // Add a scope, so the Locker-Object will unlock.
    {
        SemaphoreLocker locker(m_numberOfControlsAccessSemaphore);
        m_numberOfControls = n;
    }
    // Since the amount of controls has changed, generate a new filter to keep
    // the equalizer in consistent state.
    generateFilter();
}

int DigitalEqualizer::numberOfControls() {
    SemaphoreLocker locker(m_numberOfControlsAccessSemaphore);
    return m_numberOfControls;
}

void DigitalEqualizer::acquireControls() {
    m_controlsAccessSemaphore->acquire();
}

double *DigitalEqualizer::controls() {
    return m_controls;
}

void DigitalEqualizer::releaseControls() {
    m_controlsAccessSemaphore->release();
}

bool DigitalEqualizer::saveControlsToFile(QString fileName) {
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    if(file.isOpen()) {
        file.write(serializeCSV().toAscii());
        file.close();
        return true;
    }
    return false;
}

bool DigitalEqualizer::loadControlsFromFile(QString fileName) {
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    if(file.isOpen()) {
        unserializeCSV(QString::fromAscii(file.readAll()));
        file.close();
        generateFilter();
        return true;
    }
    return false;
}

void DigitalEqualizer::generateFilter() {
    SemaphoreLocker locker(m_numberOfControlsAccessSemaphore);
    Q_UNUSED(locker);

    // Control values in frequency domain:
    // amplitude
    //
    // A
    // |
    // |                ooooooooooooo                o
    // |       ooooooooo             oooo          oo
    // |   oooo                          ooo    ooo
    // | oo                                 oooo
    // |o
    // |
    // |
    // |
    // |
    // +------------------------------------------------> frequency

    // Construct an ideal filter in the frequency domain.
    acquireControls(); // Lock access to equalizer controls.
    for(int i = 0; i < m_numberOfControls * 2; i++) {
        if(i < m_numberOfControls) {
            // "Draw" frequency response for the equalizer.
            m_idealFilter[i][0] = m_controls[i];
            m_idealFilter[i][1] = 0.0; // m_controls[i];
        } else {
            // Mirror frequency response for the second half.
            m_idealFilter[i][0] = m_controls[m_numberOfControls * 2 - 1 - i];
            m_idealFilter[i][1] = 0.0; // m_controls[m_numberOfControls * 2 - 1 - i];
        }
    }
    releaseControls(); // Release equalizer controls.


    // Translate into the time domain.
    FftwAdapter::performInverseFFT(m_idealFilter, m_ifftIdealFilter, m_numberOfControls * 2);

    // Time domain signal after inverse DFT:
    // value
    //
    // A------------------|FILTER_SPREAD
    // |o                                              o
    // | o                                            o
    // |
    // |  o        o                        o        o
    // |          o o                      o o
    // |   o                                        o
    // |         o   o                    o   o
    // |    o             o          o             o
    // |     o  o     o  o o o   o  o o  o     o  o
    // |      oo       oo   o ooo o    oo       oo
    // +------------------------------------------------> coefficients

    // Shift and cut coefficients in order to use as a filter.
    for(int i = 0; i < FILTER_SPREAD * 2 + 1; i++)
        if(i < FILTER_SPREAD) {
            m_filterCoefficients[i] = m_ifftIdealFilter[m_numberOfControls * 2 - FILTER_SPREAD + i][0];
        } else {
            m_filterCoefficients[i] = m_ifftIdealFilter[i - FILTER_SPREAD][0];
        }

    // Lower filter coefficients by cutting of samples (determined by FILTER_SPREAD)
    // and shift result:
    // value
    //
    // A
    // |                     o
    // |                    o o
    // |
    // |       o           o   o         o
    // |      o o                       o o
    // |                  o     o
    // |     o   o                     o   o
    // |o               o         o             o
    // | o  o     o    o           o  o     o  o
    // |  oo        oo              oo       oo
    // +------------------------------------------------> coefficients

    // Apply a hamming window
    for(int i = -FILTER_SPREAD; i <= FILTER_SPREAD; i++)
        m_filterCoefficients[i + FILTER_SPREAD] *= (0.54 + 0.46 * cos(M_PI * i / FILTER_SPREAD));

    // Apply a hamming windows to smooth the filter, which improves the frequency response a lot:
    // value
    //
    // A
    // |
    // |                     o
    // |                    o o
    // |                   o   o
    // |
    // |                  o     o
    // |       o                         o
    // |     o   o      o         o    o   o
    // |    o     o    o           o  o     o
    // |oooo        oo              oo       oooo
    // +------------------------------------------------> coefficients
}

void DigitalEqualizer::process(fftw_complex *sampleBuffer, fftw_complex *result, int samples) {
    SemaphoreLocker locker(m_numberOfControlsAccessSemaphore);
    Q_UNUSED(locker);

    for(int i = 0; i < samples; i++) {
        result[i][0] = 0.0;
        m_delayLine[0] = sampleBuffer[i][0];

        for(int j = 0; j < FILTER_SPREAD * 2 + 1; j++)
            result[i][0] += m_filterCoefficients[j] * m_delayLine[j];

        for(int j = m_numberOfControls - 2; j >= 0; j--)
            m_delayLine[j + 1] = m_delayLine[j];
    }
}

QString DigitalEqualizer::serializeCSV() {
    SemaphoreLocker locker(m_numberOfControlsAccessSemaphore);
    Q_UNUSED(locker);

    QString outStream;
    acquireControls();
    for(int i = 0; i < m_numberOfControls; i++) {
        outStream += QString("%1").arg(m_controls[i]);
        if(i < 2047)
            outStream += "; ";
    }
    releaseControls();
    return outStream;
}

void DigitalEqualizer::unserializeCSV(QString stream) {
    SemaphoreLocker locker(m_numberOfControlsAccessSemaphore);
    Q_UNUSED(locker);

    QStringList splitStream = stream.split(";", QString::SkipEmptyParts);
    int i = 0;
    acquireControls();
    foreach(QString value, splitStream) {
        if(i < m_numberOfControls) {
            bool ok;
            m_controls[i] = value.toDouble(&ok);
            if(!ok) {
                qDebug() << "Error parsing value to double in CSV: " << value;
            }
            i++;
        } else {
            qDebug() << "CSV file contains more control values than the equalizer has.";
        }
    }
    releaseControls();
}
