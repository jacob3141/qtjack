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

All you need to do to be ready to go is calling:
```cpp
if(QJackClient::instance()->connectToServer("application")) {
    QJackClient::instance()->registerAudioOutPort("out");
    QJackClient::instance()->startAudioProcessing();
}
```

This will connect to a running JACK server, or run it with the latest settings as "application". You can then set up as many ports as you like. Finally, call startAudioProcessing() and you're good to go. Of course, this is not a very useful example, but it boils down to the absolute minimum to write a Qt application for JACK with QJackAudio. Neat, isn't it?
virtual method `process(int samples)`. Activate your processor by calling `setAudioProcessor(QAudioProcessor *processor)` on `QJackClient::instance()`. That's it.

But how do you actually process audio? The C API of JACK uses callbacks, QJackAudio uses an interface called the *QAudioProcessor*. Just subclass QAudioProcessor and do your processing in the
License
========
QJackAudio is licensed under the terms of the GNU GPL v3. I am not a huge fan of gratis licenses, though they're also free software. Gratis licenses have a huge negative economical impact for small projects, so either you can choose to use GPL and support the free software community by contributing back, or contact me to obtain a proprietary license (closed-source) at jacob@omg-it.works .

Happy hacking!



