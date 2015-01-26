///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJack.                                            //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                                                                           //
//    QJack is free software: you can redistribute it and/or modify          //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QJack is distributed in the hope that it will be useful,               //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QJack. If not, see <http://www.gnu.org/licenses/>.          //
//                                                                           //
//    It is possible to obtain a closed-source license of QJack.             //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// Own includes
#include "mainwindow.h"
#include "ui_mainwindow.h"

// Qt includes
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    QJack::Processor(*this) {

    ui->setupUi(this);
    setupJackClient();
    setupMp3Decoder();

    connect(&_audioDecoder, SIGNAL(bufferReady()),
            this, SLOT(transferSamples()));

    startTimer(2000);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_toolButtonFileChoose_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open mp3 file", QString(), "*.mp3");
    if(!fileName.isEmpty()) {
        ui->lineEditFileName->setText(fileName);
        _audioDecoder.setSourceFilename(fileName);
        _audioDecoder.start();
    }
}

void MainWindow::setupJackClient() {
    // Connect to JACK server
    _client.connectToServer("mp3_player");

    // Create a 1000 seconds buffer (at 44,1kHz)
    _ringBufferLeft  = QJack::AudioRingBuffer(44100 * 1000);
    _ringBufferRight = QJack::AudioRingBuffer(44100 * 1000);

    // Register two output ports
    _outLeft     = _client.registerAudioOutPort("out_1");
    _outRight    = _client.registerAudioOutPort("out_2");

    _client.setProcessor(this);

    // Activate client.
    _client.activate();

    // Connect automatically to system playback.
    _client.connect(_outLeft, _client.portByName("system:playback_1"));
    _client.connect(_outRight, _client.portByName("system:playback_2"));
}

void MainWindow::setupMp3Decoder() {
    QAudioFormat targetAudioFormat;
    targetAudioFormat.setSampleRate(_client.sampleRate());
    targetAudioFormat.setSampleType(QAudioFormat::SignedInt);
    targetAudioFormat.setChannelCount(2);
    targetAudioFormat.setCodec("audio/x-raw");
    _audioDecoder.setAudioFormat(targetAudioFormat);
}

void MainWindow::transferSamples() {
    QAudioBuffer audioBuffer = _audioDecoder.read();
    if(audioBuffer.isValid()) {
        int frames = audioBuffer.frameCount();
        QJack::AudioSample left[frames];
        QJack::AudioSample right[frames];


        const QAudioBuffer::S16S *stereoBuffer = audioBuffer.constData<QAudioBuffer::S16S>();
        for (int i = 0; i < frames; i++) {
            left[i]     = (QJack::AudioSample)(stereoBuffer[i].left / 65536.0);
            right[i]    = (QJack::AudioSample)(stereoBuffer[i].right / 65536.0);
        }

        _ringBufferLeft.write(left, frames);
        _ringBufferRight.write(right, frames);

    }
}


void MainWindow::timerEvent(QTimerEvent*) {
    qDebug() << "Samples remaining in the ring buffer: "
             << _ringBufferLeft.numberOfElementsAvailableForRead()
             << " / "
             << _ringBufferRight.numberOfElementsAvailableForRead();
}

void MainWindow::process(int samples) {
    // Just shift samples from the ringbuffers to the outputs buffers.
    _outLeft.buffer(samples).pop(_ringBufferLeft);
    _outRight.buffer(samples).pop(_ringBufferRight);
}
