Say Hello To QJack
=======================

The purpose of QJack is to make it easy to interface with a JACK audio server from within a Qt application. JACK (JACK Audio Connection Kit) is the de-facto standard for professional audio processing on GNU/Linux, a low-latency audio server that runs on-top of numerous sound systems. Each JACK application is able to interface with any other JACK application by offering virtual in- and output through a standardized interface, just like you would be able to connect audio devices with cables. For maximum compatibility, JACK offers a C-style API found in libjack. QJack tries to be a more convenient solution by wrapping all the C-stuff and offering a convenient C++/Qt API.

I really like to develop based on practical aspects. If you find QJack hard to understand or to use, please let me know. This is a clear indicator that it is lacking documentation or needs refactoring and will be treated as a bug.

How To Build
============

QJack solely relies on a recent Qt and libjack-jack2-dev. Install using the following command on Ubuntu, for example:

`sudo apt-get install libjack-jackd2-dev`

Like any other Qt application, QJack relies on qmake and the toolchain qmake is orchestrating. There are ways to build a Qt application without qmake, but this is beyond the scope of this document. Navigate to the folder the .pro is located in and type *qmake* followed by *make*. If you want to integrate qjackaudio into your application, I strongly recommend adding it as a submodule to git, and as a subproject to your project. If you do it this way, you can fix bugs while simultaneously using the library and even commit a fix. On the other hand, you can simply update the library by doing a *git pull*.

In the root of your project create a Qt subdirs project by creating a file *project.pro* with the following contents:
```
TEMPLATE = subdirs
SUBDIRS = myapp qjack

myapp.subdir = myapp
myapp.depends = qjack

qjack.subdir = qjack
qjack.depends =
```

*myapp* is the name of your app. You app has to be located under myapp/ and have a project file called myapp.pro. Inside myapp.pro, make sure build against QJack by filing in the following lines:
```
INCLUDEPATH += .. ../qjack

LIBS += -L../qjack/lib \
                -lqjack \
                # Only needed when using the client API
                -ljack \
                # Only needed when using the server API
                -ljackserver
```

Next, we need to add QJack as a submodule to git. Do that by executing the following command:
```
git submodule add https://github.com/cybercatalyst/qjack.git
```

Now you are all set up. Open the *project.pro* file with QtCreator and start developing. Make sure you clone your repository with the *--recursive*-option, so git will also clone all submodules, too, when cloning your repository.

How to use QJack to build a JACK server
==========

```cpp
#include <QJackServer>

// ..


_jackServer = new QJack::Server();

QJack::DriverMap drivers = _jackServer->availableDrivers();
QJack::Driver alsaDriver = drivers["alsa"];

QJack::ParameterMap parameters = alsaDriver.parameters();
parameters["rate"].setValue(44100);
parameters["device"].setValue("hw:PCH,0");

_jackServer->start(alsaDriver);

// ..

_jackServer->stop();

```

How to use QJackAudio to build a JACK client
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

    // Setup QJack
    _jackClient = new QJack::Client();
    if(jackClient->connectToServer("qjackClientName")) {
        // Create two inputs
        _in1 = jackClient->registerAudioInPort("in_1");
        _in2 = jackClient->registerAudioInPort("in_2");

        // Create two outpus
        _out1 = jackClient->registerAudioOutPort("out_1");
        _out2 = jackClient->registerAudioOutPort("out_2");

        // Tell QJack that this instance is responsible for processing
        // audio. QJack will automatically call process() when needed.
        jackClient->setProcessor(this);

        // Take off!
        jackClient->activate();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ..

void MyProcessor::process()
{
    // Get handles to input buffers
    QJack::Buffer buffer1 = _in1->buffer();
    QJack::Buffer buffer2 = _in2->buffer();

    // .. Perform time-critical operations

    // Write result to output buffers
    buffer1.copyTo(_out1->buffer());
    buffer2.copyTo(_out2->buffer());
}

```

License
========
QJackAudio is licensed under the terms of the GNU GPL v3. Contact me to obtain a proprietary license (closed-source) at jacob@omg-it.works .

Happy hacking!



