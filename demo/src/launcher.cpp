/* This file is part of EAR, an audio processing tool.
 *
 * Copyright (C) 2011 Otto Ritter, Jacob Dawid
 * otto.ritter.or@googlemail.com
 * jacob.dawid@cybercatalyst.net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Affero GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

/**
* @mainpage EAR Audio Rectifier
<h2>Description</h2>
This project started as an academic elaboration at the University of Applied Science in Iserlohn, the Institute CV & CI (Institute for Computer Science, Vision and Computational Intelligence). EAR tries to compensate the difference between the audio signal source as a reference and the measured input obtained by a microphone. This way, EAR tries to compensate the impacts of the loudspeakers on the music.
<br />
<h2>Jack</h2>
JACK is a system for handling real-time, low latency audio (and MIDI). It runs on GNU/Linux, Solaris, FreeBSD, OS X and Windows (and can be ported to other POSIX-conformant platforms). It can connect a number of different applications to an audio device, as well as allowing them to share audio between themselves. Its clients can run in their own processes (ie. as normal applications), or can they can run within the JACK server (ie. as a "plugin"). JACK also has support for distributing audio processing across a network, both fast & reliable LANs as well as slower, less reliable WANs.
<br />
JACK was designed from the ground up for professional audio work, and its design focuses on two key areas: synchronous execution of all clients, and low latency operation.
<br />
This description was copied from <a href="http://jackaudio.org/" target="_new">Jack Audio Connection Kit - Copyright 2001-2006 Paul Davis</a> at 21.09.2011.
<br />
<h3>Setting up EAR</h3>
EAR is running on top of JACK. At first, you need to download and install JACK for your operating system. Though JACK is running as a separate process and can be configured via the command line it is recommended to use QJackControl, which provides a graphical user interface that allows you to set up the JACK audio server, draw connections and view system messages.
<br /><br />
<strong>Required JACK server Settings for Windows XP and Windows 7:</strong>
- Driver: portaudio
- Real-time: On
- Frames: 4096
- SampleRate: 44100
- Buffer: 2

<br />
<strong>Required JACK server settings for Ubuntu:</strong>
- Driver: alsa
- Real-time: On
- Frames: 4096
- SampleRate: 44100
- Buffer: 2

Make sure the server is running and launch EAR. Connect your music player of choice to the 'source' input. On Windows, the developers have successfully used Mixxx, which provides native JACK support on Windows. Connect the 'main'-output to your speakers. Now you should be able to loop through music. Finally, connect a microphone to your computer and connect it to the 'mic'-input in JACK.
<br />
<h2>Latency Calibration</h2>
To achieve best results, you will need to find out the latency for the regulating loop. Click on 'Calibrate'. This will send out a click tone on your speakers that will be received through the microphone. As soon as the click sound will be received, the next will be send. Try to avoid making noise during the calibration process.
* @author Jacob Dawid (jacob.dawid@cybercatalyst.net)
* @author Otto Ritter (otto.ritter.or@googlemail.com)
*/

int main (int argc, char* argv[])
{
    QApplication qApplication(argc, argv);
    QSplashScreen splash(QPixmap(":/Splash.png"));
    splash.show();
    qApplication.processEvents();

    MainWindow *mainWindow = new MainWindow();
    splash.finish(mainWindow);
    mainWindow->show();

    return qApplication.exec();
}
