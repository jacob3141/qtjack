Say hello to QJackAudio
=======================

The purpose of QJackAudio is to make it easy to interface with a JACK audio server from within a Qt application. JACK (JACK Audio Connection Kit) is the de-facto standard for professional audio processing on GNU/Linux, a low-latency audio server that runs on-top of numerous sound systems. Each JACK application is able to interface with any other JACK application by offering virtual in- and output through a standardized interface, just like you would be able to connect audio devices with cables. For maximum compatibility, JACK offers a C-style API found in libjack. qjackaudio tries to be a more convenient solution by wrapping all the C-stuff and offering often used, digital filters, like equalization, delay, reverb and the like.

How to build
============

QJackAudio relies on libjack-jack2-dev and libfftw3-dev. Install these using the following command:

`sudo apt-get install libjack-jack2-dev libfftw3-dev`

Like any other Qt application, QJackAudio relies on qmake and the toolchain qmake is orchestrating. There are ways to build a Qt application without qmake, but this is beyond the scope of this document. Navigate to the folder the .pro is located in and type *qmake* followed by *make*. If you want to integrate qjackaudio into your application, I strongly recommend adding it as a submodule to git, and as a subproject to your project. If you do it this way, you can fix bugs while simultaneously using the library and even commit a fix. On the other hand, you can simply update the library by doing a *git pull*.

How to use
==========

Sample code:
```cpp
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

    // Modify signal here

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

```

License
========
QJackAudio is licensed under the terms of the GNU GPL v3. I am not a huge fan of gratis licenses, though they're also free software. Gratis licenses have a huge negative economical impact for small projects, so either you can choose to use GPL and support the free software community by contributing back, or contact me to obtain a proprietary license (closed-source) at jacob@omg-it.works .

Happy hacking!



