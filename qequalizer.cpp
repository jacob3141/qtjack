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

// Own includes
#include <QEqualizer>
#include <QFFTW>
#include <QUnits>

// Qt includes
#include <cmath>

QEqualizer::QEqualizer(int resolution, int convResolution, QObject *parent)
    : QDigitalFilter(parent)
{
    if(resolution > 2048) {
        resolution = 2048;
    }

    if(resolution < 64) {
        resolution = 64;
    }

    if(convResolution > resolution / 2) {
        convResolution = resolution / 2;
    }

    if(convResolution < 32) {
        convResolution = 32;
    }

    _delayLineSize = resolution * 2;
    _delayLine = new double[_delayLineSize];

    _controlsSize = resolution;
    _controls = new double[_controlsSize];

    _filterCoefficientsSize = convResolution + 1;
    _filterCoefficients = new double[_filterCoefficientsSize];

    for(int i = 0; i < _delayLineSize; i++) {
        _delayLine[i] = 0.0;
    }

    for(int i = 0; i < _controlsSize; i++) {
        _controls[i] = 0.0;
    }

    update();
}

QEqualizer::~QEqualizer() {
}

void QEqualizer::update()
{
    fftw_complex idealFilter[_controlsSize * 2];
    fftw_complex ifftIdealFilter[_controlsSize * 2];

    for(int i = 0; i < _controlsSize; i++) {
        _controls[i] = 0.0;
    }

    double frequencyRange = QJackClient::instance()->sampleRate() / 2.0;
    foreach(QEqualizerControl *equalizerControl, _equalizerControls) {
        for(int i = 0; i < _controlsSize; i++) {
            _controls[i] = QUnits::sumDb(_controls[i], equalizerControl->gainForFrequency(frequencyRange * i / _controlsSize));
        }
    }

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
    for(int i = 0; i < _controlsSize * 2; i++) {
        if(i < _controlsSize) {
            // "Draw" frequency response for the equalizer.
            idealFilter[i][0] = QUnits::dbToLinear(_controls[i]);
            idealFilter[i][1] = 0.0;
        } else {
            // Mirror frequency response for the second half.
            idealFilter[i][0] = QUnits::dbToLinear(_controls[_controlsSize * 2 - 1 - i]);
            idealFilter[i][1] = 0.0;
        }
    }

    // Translate into the time domain.
    QFFTW::performInverseFFT(idealFilter, ifftIdealFilter, _controlsSize * 2);

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

    int alpha = (_filterCoefficientsSize - 1) / 2;

    QMutexLocker mutexLocker(&_mutex);
    // Shift and cut coefficients in order to use as a filter.
    for(int i = 0; i < _filterCoefficientsSize; i++) {
        if(i < alpha) {
            _filterCoefficients[i] = ifftIdealFilter[_controlsSize * 2 - alpha + i][0];
        } else {
            _filterCoefficients[i] = ifftIdealFilter[i - alpha][0];
        }
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
    for(int i = -alpha; i <= alpha; i++)
        _filterCoefficients[i + alpha] *= (0.54 + 0.46 * cos(M_PI * i / alpha));

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

void QEqualizer::process(QSampleBuffer sampleBuffer)
{
    QMutexLocker mutexLocker(&_mutex);
    int bufferSize = sampleBuffer.size();
    for(int i = 0; i < bufferSize; i++) {
        double result = 0.0;
        _delayLine[0] = sampleBuffer.readAudioSample(i);

        for(int j = 0; j < _filterCoefficientsSize; j++)
            result += _filterCoefficients[j] * _delayLine[j];

        sampleBuffer.writeAudioSample(i, result);

        for(int j = _controlsSize - 2; j >= 0; j--)
            _delayLine[j + 1] = _delayLine[j];
    }
}

QEqualizerControl* QEqualizer::createEqualizerControl(QEqualizerControl::ControlType controlType)
{
    QEqualizerControl *equalizerControl = new QEqualizerControl(this, controlType);
    connect(equalizerControl, SIGNAL(controlChanged()), this, SLOT(update()));

    _equalizerControls.append(equalizerControl);
    return equalizerControl;
}
