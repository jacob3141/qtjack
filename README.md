[![Qt Pods](http://qt-pods.org/assets/logo.png "Qt Pods")](http://qt-pods.org)

Support this and other free software projects of mine by donating bitcoins:
```cpp
1HdLeSj4SfRC77tb6pDUPqeXGiw3T2RqrG
```

Say Hello To QtJack
=======================

The purpose of QtJack is to make it easy to interface with a JACK audio server from within a Qt application. JACK (JACK Audio Connection Kit) is the de-facto standard for professional audio processing on GNU/Linux, a low-latency audio server that runs on-top of numerous sound systems. Each JACK application is able to interface with any other JACK application by offering virtual in- and output through a standardized interface, just like you would be able to connect audio devices with cables. For maximum compatibility, JACK offers a C-style API found in libjack. QtJack tries to be a more convenient solution by wrapping all the C-stuff and offering a convenient C++/Qt API.

I really like to develop based on practical aspects. If you find QtJack hard to understand or to use, please let me know. This is a clear indicator that it is lacking documentation or needs refactoring and will be treated as a bug.

Also checkout the example repository:
https://github.com/cybercatalyst/QtJack-Examples

How To Build
============

QtJack solely relies on a recent Qt and libjack-jack2-dev. Install using the following command on Ubuntu, for example:

`sudo apt-get install libjack-jackd2-dev`

You can add QtJack to your project easily by using qt-pods. Read more about qt-pods here:
https://github.com/cybercatalyst/qt-pods

How to use QtJack to build a JACK server
==========

```cpp
#include "QtJack/server.h"

// ..


_jackServer = new QtJack::Server();

QtJack::DriverMap drivers = _jackServer->availableDrivers();
QtJack::Driver alsaDriver = drivers["alsa"];

QtJack::ParameterMap parameters = alsaDriver.parameters();
parameters["rate"].setValue(44100);
parameters["device"].setValue("hw:PCH,0");

_jackServer->start(alsaDriver);

// ..

_jackServer->stop();

```

How to use QtJack to build a JACK client
==========

A complete JACK app with QtJack
```cpp
#include <QCoreApplication>

#include <Client>
#include <Processor>

class MyProcessor : public QtJack::Processor {
public:
    MyProcessor(QtJack::Client& client)
        : Processor(client)  {
        in = client.registerAudioInPort("in");
        out = client.registerAudioOutPort("out");
        memoryBuffer = QtJack::Buffer::createMemoryAudioBuffer(4096);
    }

    void process(int samples) {
        // Copy from input buffer to memory buffer
        in.buffer(samples).copyTo(memoryBuffer);

        // Process
        memoryBuffer.multiply(0.5);

        // Copy result to output buffer
        memoryBuffer.copyTo(out.buffer(samples));
    }

private:
    QtJack::Port in;
    QtJack::Port out;

    QtJack::Buffer memoryBuffer;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QtJack::Client client;
    client.connectToServer("attenuator_demo");

    MyProcessor processor(client);
    client.activate();

    return a.exec();
}

```

License
========
QtJack is licensed under the terms of the GNU GPL v3. Contact me to obtain a proprietary license (closed-source) at jacob@omg-it.works .

Happy hacking!



