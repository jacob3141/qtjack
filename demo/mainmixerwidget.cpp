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
#include "mainmixerwidget.h"
#include "ui_mainmixerwidget.h"

// QJackAudio includes
#include <QJackClient>

MainMixerWidget::MainMixerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMixerWidget)
{
    ui->setupUi(this);

    connect(&_updateTimer, SIGNAL(timeout()), this, SLOT(updateInterface()));
    _updateTimer.setInterval(20);
    _updateTimer.setSingleShot(false);
    _updateTimer.start();
}

MainMixerWidget::~MainMixerWidget()
{
    delete ui;
}


void MainMixerWidget::registerChannel(int i, ChannelWidget *channelWidget)
{
    _registeredChannels.insert(i, channelWidget);
}

void MainMixerWidget::updateInterface()
{
    QJackClient *jackClient = QJackClient::instance();
    QString displayText;
    displayText += QString("<table width=\"100%\"><tr><td><b>JACK Client</b></td><td></td></tr>");
    displayText += QString("<tr><td>Realtime processing:</td><td>%1</td></tr>").arg(jackClient->isRealtime() ? "Yes" : "No");
    displayText += QString("<tr><td>Buffer size:</td><td>%1 Samples</td></tr>").arg(jackClient->bufferSize());
    displayText += QString("<tr><td>CPU load:</td><td>%1</td></tr>").arg(jackClient->cpuLoad() < 5.0 ? "Idle" : QString("%1 %").arg(jackClient->cpuLoad()));
    displayText += QString("<tr><td>Sample rate:</td><td>%1 Hz</td></tr></table>").arg(jackClient->sampleRate());
    ui->displayLabel->setText(displayText);
}
