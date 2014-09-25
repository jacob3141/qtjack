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

#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

// Qt includes
#include <QWidget>

// QJackAudio includes
#include <QJackClient>
#include <QEqualizer>
#include <QAmplifier>
#include <QJackPort>

namespace Ui {
class ChannelWidget;
}

/**
 * Widget that represents a single audio channel mixer line.
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 */
class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    /** Constructor */
    explicit ChannelWidget(int channelNumber, QWidget *parent = 0);
    /** Destructor */
    ~ChannelWidget();

    /** Process this channel mixer line, storing the result in targetSampleBuffer. */
    void process(QSampleBuffer targetSampleBuffer);

    /** Update all visual interface elements. */
    void updateInterface();

    /** @returns the panorama setting. 0.0 means left-most, 1.0 indicates right-most position. */
    double panorama();

    /** @returns whether this channel is contained in subgroup 1 and 2. */
    bool isInSubGroup12();

    /** @returns whether this channel is contained in subgroup 3 and 4. */
    bool isInSubGroup34();

    /** @returns whether this channel is contained in subgroup 5 and 6. */
    bool isInSubGroup56();

    /** @returns whether this channel is contained in subgroup 7 and 8. */
    bool isInSubGroup78();

    /** @returns whether this channel has been muted. */
    bool isMuted();

    /** @returns whether this channel has been soloed. */
    bool isSoloed();

    /** @returns true, when this channel is routed on main. */
    bool isOnMain();

private:
    Ui::ChannelWidget *ui;

    /** Input stage amplifier ("Gain"). */
    QAmplifier *_inputStage;
    /** Fader stage amplifier ("Volume"). */
    QAmplifier *_faderStage;
    /** Attenuation stage before sending signal to aux. */
    QAmplifier *_auxPre;
    /** Attenuation stage after receiving signal from aux. */
    QAmplifier *_auxPost;
    /** Equalizer for this channel. */
    QEqualizer *_equalizer;

    /** QJackAudio input port for this channel. */
    QJackPort *_channelIn;
    /** QJackAudio aux send output port for this channel. */
    QJackPort *_auxSend;
    /** QJackAudio aux receive input port for this channel. */
    QJackPort *_auxReturn;
    /** QJackAudio direct out output port for this channel. */
    QJackPort *_channelOut;

    /** Last peak value. */
    double _peak;
};

#endif // CHANNELWIDGET_H
