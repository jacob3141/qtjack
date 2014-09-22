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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "channelwidget.h"
#include "mainmixerwidget.h"

// QJackClient includes
#include <QJackClient>

// Qt includes
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);

    // Setup QJackAudio
    QJackClient* jackClient = QJackClient::instance();
    if(jackClient->connectToServer("MX2482")) {
        jackClient->setAudioProcessor(this);
    }

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addStretch();
    hBoxLayout->setSpacing(0);
    hBoxLayout->setMargin(0);

    MainMixerWidget *mainMixerWidget = new MainMixerWidget();
    for(int i = 0; i < 24; i++) {
        ChannelWidget *channelWidget = new ChannelWidget(i + 1);
        mainMixerWidget->registerChannel(i + 1, channelWidget);
        hBoxLayout->addWidget(channelWidget);
    }
    hBoxLayout->addWidget(mainMixerWidget);

    QWidget *widget = new QWidget();
    widget->setStyleSheet("background-color: rgb(120, 120, 120);");
    widget->setLayout(hBoxLayout);
    setCentralWidget(widget);

    // Take off!
    jackClient->startAudioProcessing();
}

void MainWindow::process()
{
//    // Get handles to input buffers
//    QSampleBuffer buffer1 = _in1->sampleBuffer();
//    QSampleBuffer buffer2 = _in2->sampleBuffer();

//    _noiseGateLeft->process(buffer1);
//    _noiseGateRight->process(buffer2);

//    // Process input with EQs
//    _equalizerLeft->process(buffer1);
//    _equalizerRight->process(buffer2);

//    _compressorLeft->process(buffer1);
//    _compressorRight->process(buffer2);

//    // Write result to output buffers
//    buffer1.copyTo(_out1->sampleBuffer());
//    buffer2.copyTo(_out2->sampleBuffer());

//    _signalGenerator->process(_outSignal->sampleBuffer());
}

MainWindow::~MainWindow()
{
    delete ui;
}
