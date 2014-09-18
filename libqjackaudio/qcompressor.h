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

#ifndef QCOMPRESSOR_H
#define QCOMPRESSOR_H

// Own includes
#include <QJackClient>
#include <QDigitalFilter>
#include <QUnits>

/**
 * @class QCompressor
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * @brief Digital compressor.
 */
class QCompressor : public QDigitalFilter
{
public:
    /** Constructs a new digital compressor. */
    QCompressor();

    /** Destructor. */
    ~QCompressor();

    /** @overload QDigitalFilter */
    void process(QSampleBuffer sampleBuffer);

    /**
     * Sets the threshold above which the compressor starts acting.
     * @brief setThreshold
     * @param threshold Threshold in dB.
     */
    void setThreshold(double threshold);

    /** @returns threshold. */
    double threshold();

    /**
     * Compression determines how strong loud signals will be decreased
     * in amplitude. A ration of 4 means, that a signal of 4dB above the
     * threshold will be reduced to 1dB above threshold.
     *
     * A compressor with a compression ratio greater 60 is often referred
     * to as a limiter. A compressor with a compression ratio below 1 can
     * be understood as an expander or noise gate.
     * @brief setRatio
     * @param ratio Compression ratio.
     */
    void setRatio(double ratio);

    /** @returns compression ratio. */
    double ratio();

    /**
     * Attack determines how fast the compressor reacts when the signal
     * goes above the threshold. Compression will increase linearly and
     * reach its designated maximum value after the attack time has passed.
     * @brief setAttack
     * @param attack Attack time in ms.
     */
    void setAttack(double attack);

    /** @returns attack in ms. */
    double attack();

    /**
     * Release determines how fast the compressor reacts when the signal
     * goes below the threshold. Compression will decrease linearly and
     * reach 1:1 compression after the release time has passed.
     * @brief setRelease
     * @param release Release time in ms.
     */
    void setRelease(double release);

    /** @returns release in ms. */
    double release();

    /**
     * Input gain is applied to the signal before compression.
     * @brief setInputGain
     * @param inputGain Input gain in dB.
     */
    void setInputGain(double inputGain);

    /** @returns input gain in dB. */
    double inputGain();

    /** Makeup gain is applied after compression.
     * @brief setMakeupGain
     * @param makeupGain Makeup gain in dB.
     */
    void setMakeupGain(double makeupGain);

    /** @returns makeup gain in dB. */
    double makeupGain();

private:
    /** Threshold in dB. */
    double _threshold;

    /** Ratio. */
    double _ratio;

    /** Attack in ms. */
    double _attack;

    /** Release in ms. */
    double _release;

    /** Input gain. */
    double _inputGain;

    /** Make-up gain in dB. */
    double _makeupGain;
};

#endif // QCOMPRESSOR_H
