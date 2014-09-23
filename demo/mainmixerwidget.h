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
 * @brief The MainMixerWidget class
 */
class MainMixerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainMixerWidget(QWidget *parent = 0);
    ~MainMixerWidget();

    void registerChannel(int i, ChannelWidget *channelWidget);

    void process();

public slots:
    void updateInterface();

private:
    Ui::MainMixerWidget *ui;

    QTimer _updateTimer;

    QMap<int, ChannelWidget*> _registeredChannels;

    double _peak1;
    double _peak2;
    double _peak3;
    double _peak4;
    double _peak5;
    double _peak6;
    double _peak7;
    double _peak8;

    //

    QJackPort *_subGroup1Out;
    QJackPort *_subGroup2Out;
    QJackPort *_subGroup3Out;
    QJackPort *_subGroup4Out;
    QJackPort *_subGroup5Out;
    QJackPort *_subGroup6Out;
    QJackPort *_subGroup7Out;
    QJackPort *_subGroup8Out;

    QJackPort *_mainLeftOut;
    QJackPort *_mainRightOut;
};

#endif // MAINMIXERWIDGET_H
