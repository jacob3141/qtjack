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

#ifndef MAINMIXERWIDGET_H
#define MAINMIXERWIDGET_H

// Qt includes
#include <QWidget>
#include <QMap>
#include <QTimer>

// QJackAudio includes
#include <QJackClient>

// Own includes
#include "channelwidget.h"

namespace Ui {
class MainMixerWidget;
}

/**
 * The main mixer section.
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 */
class MainMixerWidget : public QWidget
{
    Q_OBJECT

public:
    /** Constructor */
    explicit MainMixerWidget(QWidget *parent = 0);
    /** Destructor */
    ~MainMixerWidget();

    /**
     * Registers a a new channel widget to the main mixer widget.
     * @param i The channel number.
     * @param channelWidget The channel widget.
     */
    void registerChannel(int i, ChannelWidget *channelWidget);

    /** Performs all that is necessary to process the channel signal. */
    void process();

public slots:
    /** Update the visual interface. */
    void updateInterface();

private:
    Ui::MainMixerWidget *ui;

    /** Update timer used to update the visual interface periodically. */
    QTimer _updateTimer;

    /** Stores all registered channels. */
    QMap<int, ChannelWidget*> _registeredChannels;

    /** Used to store calculated peak for subgroup 1. */
    double _subgroupPeak1;
    /** Used to store calculated peak for subgroup 2. */
    double _subgroupPeak2;
    /** Used to store calculated peak for subgroup 3. */
    double _subgroupPeak3;
    /** Used to store calculated peak for subgroup 4. */
    double _subgroupPeak4;
    /** Used to store calculated peak for subgroup 5. */
    double _subgroupPeak5;
    /** Used to store calculated peak for subgroup 6. */
    double _subgroupPeak6;
    /** Used to store calculated peak for subgroup 7. */
    double _subgroupPeak7;
    /** Used to store calculated peak for subgroup 8. */
    double _subgroupPeak8;

    /** Used to store calculated peak for main 1 (left). */
    double _mainPeak1;
    /** Used to store calculated peak for main 2 (right). */
    double _mainPeak2;

    /** Subgroup 1 fader stage. */
    QAmplifier *_subgroup1FaderStage;
    /** Subgroup 2 fader stage. */
    QAmplifier *_subgroup2FaderStage;
    /** Subgroup 3 fader stage. */
    QAmplifier *_subgroup3FaderStage;
    /** Subgroup 4 fader stage. */
    QAmplifier *_subgroup4FaderStage;
    /** Subgroup 5 fader stage. */
    QAmplifier *_subgroup5FaderStage;
    /** Subgroup 6 fader stage. */
    QAmplifier *_subgroup6FaderStage;
    /** Subgroup 7 fader stage. */
    QAmplifier *_subgroup7FaderStage;
    /** Subgroup 8 fader stage. */
    QAmplifier *_subgroup8FaderStage;

    /** Main 1 fader stage. */
    QAmplifier *_main1FaderStage;
    /** Main 2 fader stage. */
    QAmplifier *_main2FaderStage;

    /** Subgroup 1 direct out. */
    QJackPort *_subGroup1Out;
    /** Subgroup 2 direct out. */
    QJackPort *_subGroup2Out;
    /** Subgroup 3 direct out. */
    QJackPort *_subGroup3Out;
    /** Subgroup 4 direct out. */
    QJackPort *_subGroup4Out;
    /** Subgroup 5 direct out. */
    QJackPort *_subGroup5Out;
    /** Subgroup 6 direct out. */
    QJackPort *_subGroup6Out;
    /** Subgroup 7 direct out. */
    QJackPort *_subGroup7Out;
    /** Subgroup 8 direct out. */
    QJackPort *_subGroup8Out;

    /** Main 1 direct out. */
    QJackPort *_mainLeftOut;
    /** Main 2 direct out. */
    QJackPort *_mainRightOut;
};

#endif // MAINMIXERWIDGET_H
