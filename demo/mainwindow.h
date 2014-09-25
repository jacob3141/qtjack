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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt includes
#include <QMainWindow>
#include <QTimer>

// QJackClient includes
#include <QAudioProcessor>

// Own includes
#include "mainmixerwidget.h"

namespace Ui {
class MainWindow;
}

/**
 * Main window for this application.
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 */
class MainWindow : public QMainWindow, public QAudioProcessor
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /** @overload */
    void process();

protected:
    /** @overload */
    void closeEvent(QCloseEvent *closeEvent);

private:
    Ui::MainWindow *ui;

    /** The main mixer widget. */
    MainMixerWidget *_mainMixerWidget;
};

#endif // MAINWINDOW_H
