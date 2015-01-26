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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QJack includes
#include <Client>
#include <Processor>
#include <RingBuffer>

// Qt includes
#include <QMainWindow>
#include <QAudioDecoder>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public QJack::Processor
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupJackClient();
    void setupMp3Decoder();
    void process(int samples);

protected slots:
    void on_toolButtonFileChoose_clicked();

    void transferSamples();

protected:
    void timerEvent(QTimerEvent*);

private:
    Ui::MainWindow *ui;

    QAudioDecoder _audioDecoder;

    QJack::Client _client;
    QJack::AudioPort _outLeft;
    QJack::AudioPort _outRight;

    QJack::AudioRingBuffer _ringBufferLeft;
    QJack::AudioRingBuffer _ringBufferRight;
};

#endif // MAINWINDOW_H
