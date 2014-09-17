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

#include "mainwindow.h"
#include "ui_mainwindow.h"

// QJackClient includes
#include <QJackClient>

// Qt includes
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QJackClient* jackClient = QJackClient::instance();
    connect(jackClient, SIGNAL(error(QString)), this, SLOT(handleError(QString)));

    if(jackClient->connectToServer("qjackaudio")) {

        _in1 = jackClient->registerAudioInPort("in_1");
        _in2 = jackClient->registerAudioInPort("in_2");

        _out1 = jackClient->registerAudioOutPort("out_1");
        _out2 = jackClient->registerAudioOutPort("out_2");

        jackClient->setAudioProcessor(this);
        jackClient->startAudioProcessing();
    }
}

void MainWindow::process()
{
    QSampleBuffer buffer1 = _in1->sampleBuffer();
    QSampleBuffer buffer2 = _in2->sampleBuffer();

    buffer1.copyTo(_out1->sampleBuffer());
    buffer2.copyTo(_out2->sampleBuffer());
}

void MainWindow::handleError(QString error)
{
    qDebug() << error;
}


MainWindow::~MainWindow()
{
    delete ui;
}
