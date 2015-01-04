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

// Qt includes
#include <QObject>

// Forward declarations
class QEqualizer;

/**
 * @class QEqualizerControl
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 * @brief Represents a control with the equalizer.
 * An equalizer may have an arbitrary number of controls that will affect
 * its frequency response.
 */
class QEqualizerControl : public QObject
{
    Q_OBJECT
    friend class QEqualizer;
public:
    /**
     * The control type determines how the equalizer control parameters
     * will be interpreted.
     */
    enum ControlType {
        /** Classic low-shelf, amount will be applied below control frequency
         * and diverges to zero above. */
        LowShelf,

        Band,

        /** Classic high-shelf, amount will be applied above control frequency
         * and diverges to zero below. */
        HighShelf
    };

    /** @returns the gain in dB for a given frequency. */
    double gainForFrequency(double frequency);

    /** @returns the control type of this equalizer control. */
    ControlType controlType();

    /** @returns the control frequency.
     * Depending on the control type the control frequency can have different
     * meaning. For a bandpass, this is the center frequency (or resonance
     * frequency, from an electrical point of view), for a sheld filter, this
     * is the frequency where the curved part begins.
     */
    double controlFrequency();

    /** @returns the amount in dB. */
    double amount();

    /** @returns the filter quality.
     * This determines how the curve around the center frequency bends. */
    double q();

    /** @returns the bandwidth.
     * @attention This is not what is commonly understood as filters bandwidth,
     * which is usually defined as the the distance between those two points,
     * at which the input signal is lowered by 3dB.
     * This variable refers to the space at which the peak "flattens" over a
     * certain interval to the left and right from the control frequency,
     * before it diverges to zero on both sides
     */
    double bandwidth();

public slots:
    /** Sets the control type for this equalizer control. */
    void setControlType(ControlType controlType);

    /** Sets the amount for this equalizer control. */
    void setAmount(double amount);
    void setAmount(int amount) { setAmount((double)amount); }

    /** Sets the quality factor for this equalizer control. */
    void setQ(double q);
    void setQ(int q) { setQ((double)q); }

    /** Sets the control frequency for this equalizer control. */
    void setControlFrequency(double controlFrequency);
    void setControlFrequency(int controlFrequency) { setControlFrequency((double)controlFrequency); }

    /** Sets the bandwidth. @see bandwidth() for more information. */
    void setBandwidth(double bandwidth);
    void setBandwidth(int bandwidth) { setBandwidth((double)bandwidth); }

signals:
    /** Emitted when the control type changes. */
    void controlTypeChanged(ControlType controlType);

    /** Emitted when amount changes. */
    void amountChanged(double amount);
    void amountChanged(int amount);

    /** Emitted when q changes. */
    void qChanged(double q);
    void qChanged(int q);

    /** Emitted when the control frequency changes. */
    void controlFrequencyChanged(double controlFrequency);
    void controlFrequencyChanged(int controlFrequency);

    /** Emitted when the bandwidth changes. */
    void bandwidthChanged(double bandwidth);
    void bandwidthChanged(int bandwidth);

    /** Emitted whenever any property of the control changes. */
    void controlChanged();

private:
    QEqualizerControl(QEqualizer *equalizer, ControlType = Band, QObject* parent = 0);

    double gainForFrequencyLowShelf(double frequency);
    double gainForFrequencyBand(double frequency);
    double gainForFrequencyHighShelf(double frequency);

    /** Control type. */
    ControlType _controlType;

    /** Control frequency. */
    double _controlFrequency;

    /** Amount of gain for the control frequency. */
    double _amount;

    /** Bandwidth of bandpass filter. Only applies to bandpass filters. */
    double _bandwidth;

    /** Quality factor. */
    double _q;

    /** The equalizer this control belongs to. */
    QEqualizer *_equalizer;
};
