Say hello to QJackAudio
=======================

The purpose of QJackAudio is to make it easy to interface with a JACK audio server from within a Qt application. JACK (JACK Audio Connection Kit) is the de-facto standard for professional audio processing on GNU/Linux, a low-latency audio server that runs on-top of numerous sound systems. Each JACK application is able to interface with any other JACK application by offering virtual in- and output through a standardized interface, just like you would be able to connect audio devices with cables. For maximum compatibility, JACK offers a C-style API found in libjack. qjackaudio tries to be a more convenient solution by wrapping all the C-stuff and offering often used, digital filters, like equalization, delay, reverb and the like.

![Demo Screenshot](https://github.com/cybercatalyst/qjackaudio/blob/master/mx2482.png "Demo screenshot")

How to build
============

QJackAudio relies on libjack-jack2-dev and libfftw3-dev. Install these using the following command:

`sudo apt-get install libjack-jack2-dev libfftw3-dev`

Like any other Qt application, QJackAudio relies on qmake and the toolchain qmake is orchestrating. There are ways to build a Qt application without qmake, but this is beyond the scope of this document. Navigate to the folder the .pro is located in and type *qmake* followed by *make*. If you want to integrate qjackaudio into your application, I strongly recommend adding it as a submodule to git, and as a subproject to your project. If you do it this way, you can fix bugs while simultaneously using the library and even commit a fix. On the other hand, you can simply update the library by doing a *git pull*.

How to use
==========

Sample code:
```cpp
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

    // Write result to output buffers
    buffer1.copyTo(_out1->sampleBuffer());
    buffer2.copyTo(_out2->sampleBuffer());
}

MainWindow::~MainWindow()
{
    delete ui;
}

```

License
========
QJackAudio is licensed under the terms of the GNU GPL v3. Contact me to obtain a proprietary license (closed-source) at jacob@omg-it.works .

Happy hacking!



