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

#ifndef QEQUALIZERCONTROL_H
#define QEQUALIZERCONTROL_H

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
    enum ControlType {
        LowShelf,
        Band,
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

private:
    QEqualizerControl(QEqualizer *equalizer, ControlType = Band, QObject* parent = 0);

    /** Control type. */
    ControlType _controlType;

    /** Control frequency. */
    double _controlFrequency;

    /** Amount of gain for the control frequency. */
    double _amount;

    /** Quality factor. */
    double _q;

    /** The equalizer this control belongs to. */
    QEqualizer *_equalizer;
};

#endif // QEQUALIZERCONTROL_H
