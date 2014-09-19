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

// QJackClient includes
#include <QJackClient>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);

    // Setup QJackAudio
    QJackClient* jackClient = QJackClient::instance();
    if(jackClient->connectToServer("qjackaudio")) {
        // Create two inputs
        _in1 = jackClient->registerAudioInPort("in_1");
        _in2 = jackClient->registerAudioInPort("in_2");

        // Create two outpus
        _out1 = jackClient->registerAudioOutPort("out_1");
        _out2 = jackClient->registerAudioOutPort("out_2");

        // Create two equalizers, each for one side
        _equalizerLeft = new QEqualizer();
        _equalizerRight = new QEqualizer();

        // Create two equalizers, each for one side
        _compressorLeft = new QCompressor();
        _compressorRight = new QCompressor();

        connect(ui->inputGainDial, SIGNAL(valueChanged(int)), _compressorLeft, SLOT(setInputGain(int)));
        connect(ui->thresholdDial, SIGNAL(valueChanged(int)), _compressorLeft, SLOT(setThreshold(int)));
        connect(ui->ratioDial, SIGNAL(valueChanged(int)), _compressorLeft, SLOT(setRatio(int)));
        connect(ui->attackDial, SIGNAL(valueChanged(int)), _compressorLeft, SLOT(setAttack(int)));
        connect(ui->releaseDial, SIGNAL(valueChanged(int)), _compressorLeft, SLOT(setRelease(int)));
        connect(ui->makeupGainDial, SIGNAL(valueChanged(int)), _compressorLeft, SLOT(setMakeupGain(int)));
        connect(ui->bypassPushButton, SIGNAL(toggled(bool)), _compressorLeft, SLOT(setBypass(bool)));

        connect(ui->inputGainDial, SIGNAL(valueChanged(int)), _compressorRight, SLOT(setInputGain(int)));
        connect(ui->thresholdDial, SIGNAL(valueChanged(int)), _compressorRight, SLOT(setThreshold(int)));
        connect(ui->ratioDial, SIGNAL(valueChanged(int)), _compressorRight, SLOT(setRatio(int)));
        connect(ui->attackDial, SIGNAL(valueChanged(int)), _compressorRight, SLOT(setAttack(int)));
        connect(ui->releaseDial, SIGNAL(valueChanged(int)), _compressorRight, SLOT(setRelease(int)));
        connect(ui->makeupGainDial, SIGNAL(valueChanged(int)), _compressorRight, SLOT(setMakeupGain(int)));
        connect(ui->bypassPushButton, SIGNAL(toggled(bool)), _compressorRight, SLOT(setBypass(bool)));

        connect(_compressorLeft, SIGNAL(clipping()), this, SLOT(clipping()));
        connect(&_clipRemoveTimer, SIGNAL(timeout()), this, SLOT(clipRemove()));
        _clipRemoveTimer.setInterval(20);
        _clipRemoveTimer.setSingleShot(true);

        connect(_compressorLeft, SIGNAL(active()), this, SLOT(active()));
        connect(&_activeRemoveTimer, SIGNAL(timeout()), this, SLOT(activeRemove()));
        _activeRemoveTimer.setInterval(20);
        _activeRemoveTimer.setSingleShot(true);

        // Tell QJackAudio that this instance is responsible for processing
        // audio. QJackAudio will automatically call process() when needed.
        jackClient->setAudioProcessor(this);

        // Take off!
        jackClient->startAudioProcessing();
    }
}

void MainWindow::process()
{
    // Get handles to input buffers
    QSampleBuffer buffer1 = _in1->sampleBuffer();
    QSampleBuffer buffer2 = _in2->sampleBuffer();

    // Process input with EQs
    _equalizerLeft->process(buffer1);
    _equalizerRight->process(buffer2);

    _compressorLeft->process(buffer1);
    _compressorRight->process(buffer2);

    // Write result to output buffers
    buffer1.copyTo(_out1->sampleBuffer());
    buffer2.copyTo(_out2->sampleBuffer());
}

void MainWindow::clipping()
{
    ui->clipLabel->setStyleSheet("background: red;");
    _clipRemoveTimer.start();
}

void MainWindow::clipRemove()
{
    ui->clipLabel->setStyleSheet("background: black;");
}

void MainWindow::active()
{
    ui->activeLabel->setStyleSheet("background: blue;");
    _activeRemoveTimer.start();
}

void MainWindow::activeRemove()
{
    ui->activeLabel->setStyleSheet("background: black;");
}

MainWindow::~MainWindow()
{
    delete ui;
}
