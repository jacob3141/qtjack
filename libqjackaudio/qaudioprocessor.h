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

#ifndef QAUDIOPROCESSOR_H
#define QAUDIOPROCESSOR_H

// Qt includes:
#include <QObject>

/**
 * @class QAudioProcessor
 *
 * @author Jacob Dawid ( jacob.dawid@cybercatalyst.net )
 * @author Otto Ritter ( otto.ritter.or@googlemail.com )
 * @date 09.2011
 *
 * @brief Defines an interface that must be met by audio processors.
 *
 */
class QAudioProcessor : public QObject
{
  Q_OBJECT
public:
    /** Constructs a new processor. */
    QAudioProcessor() : QObject() { }

    /** Destructor. */
    virtual ~QAudioProcessor() { }

    /**
     * @brief Called whenever audio samples have to be processed.
     * Warning: This method is time-critical.
     * @param samples Number of samples.
     */
    virtual void process(int samples) { Q_UNUSED(samples); }
};

#endif // QAUDIOPROCESSOR_H
