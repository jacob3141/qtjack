Say Hello To QJack
=======================

The purpose of QJack is to make it easy to interface with a JACK audio server from within a Qt application. JACK (JACK Audio Connection Kit) is the de-facto standard for professional audio processing on GNU/Linux, a low-latency audio server that runs on-top of numerous sound systems. Each JACK application is able to interface with any other JACK application by offering virtual in- and output through a standardized interface, just like you would be able to connect audio devices with cables. For maximum compatibility, JACK offers a C-style API found in libjack. QJack tries to be a more convenient solution by wrapping all the C-stuff and offering a convenient C++/Qt API.

See a demo app here:
https://github.com/cybercatalyst/mx2482
![Demo Screenshot](https://github.com/cybercatalyst/mx2482/blob/master/mx2482.png "Demo screenshot")

I really like to develop based on practical aspects. If you find QJack hard to understand or to use, please let me know. This is a clear indicator that it is lacking documentation or needs refactoring and will be treated as a bug.

Frequently Asked Questions
==========================
When presenting QJack to others, I gained quite a lot of insights on what people think and how they see Qt and JACK playing their role in the software world. I just want to answer a few questions that I have been asked often, hopefully giving you a better idea why this project exists.

Q: Why are you mixing GUI with low-level code?
A: A common misunderstanding about Qt is that it is a GUI library, since it has been often compared to other GUI toolkits like GTK, FLTK, wxWidgets and the like. Qt is not a GUI toolkit, in fact, you can write very nice command line apps with Qt, because Qt is an application framework. It gives you very convenient classes for handling networking, accessing a database and so on. This is where QJackAudio rows in, it gives you convenient access to the JACK audio interface.

Q: Wrapping of low-level code should start much lower in the hierarchy.
A: On GNU/Linux, there is a mess of audio drivers and software handling audio. JACK itself consists of an audio server and is interfaced like any other audio server. If you want to handle all sorts of audio architectures you are welcome to write a wrapper that does just that. But, JACK already wraps itself around the most common audio architectures. The idea is to leave each part of the stack doing what it does best. In the past years, JACK has evolved to a very common audio server for the GNU/Linux world regarding professional audio. Almost every other professional audio app can interface JACK, so you're safe to just do that.

Q: Why can't I use JACK within my Qt app directly?
A: You can. Go ahead! You will end up spending countless hours replicating my work. The JACK client library offers a C-Style API and does not play nicely with C++/Qt unless you wrap it properly - which is what actually QJack does.

How To Build
============

QJackAudio relies on libjack-jack2-dev and libfftw3-dev. Install these using the following command:

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

        // Create two equalizers, each for one side
        _equalizerLeft = new QEqualizer();
        _equalizerRight = new QEqualizer();

        // Tell QJack that this instance is responsible for processing
        // audio. QJack will automatically call process() when needed.
        jackClient->setProcessor(this);

        // Take off!
        jackClient->activate();
    }
}

void MainWindow::process()
{
    // Get handles to input buffers
    QJack::Buffer buffer1 = _in1->buffer();
    QJack::Buffer buffer2 = _in2->buffer();

    // Process input with EQs
    _equalizerLeft->process(buffer1);
    _equalizerRight->process(buffer2);

    // Write result to output buffers
    buffer1.copyTo(_out1->buffer());
    buffer2.copyTo(_out2->buffer());
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



