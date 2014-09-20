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

#ifndef QDIGITALFILTER_H
#define QDIGITALFILTER_H

// Qt includes
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

// Own includes
#include <QSampleBuffer>
#include <QUnits>

/**
 * @class QDigitalFilter
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 */
class QDigitalFilter : public QObject
{
    Q_OBJECT
public:
    /** Constructs a new processor. */
    QDigitalFilter(QObject *parent = 0) : QObject(parent) {
        _bypass = false;
    }

    /** Destructor. */
    virtual ~QDigitalFilter() { }

    /**
     * @brief Called whenever audio samples have to be processed.
     * Warning: This method is time-critical.
     */
    virtual void process(QSampleBuffer sampleBuffer) {
        Q_UNUSED(sampleBuffer);
    }

    /** @return true, when bypassed. */
    bool bypass() {
        QMutexLocker mutexLocker(&_mutex);
        return _bypass;
    }

signals:
    /** Emitted when signals is clipping. */
    void clipping();

    /** Emitted when signal is below threshold. */
    void active();

    /** Emitted when bypass changed. */
    void bypassChanged(bool bypass);

public slots:
    void setBypass(bool bypass) {
        QMutexLocker mutexLocker(&_mutex);
       _bypass = bypass;
       emit bypassChanged(bypass);
    }

protected:
    /** Bypass flag. */
    bool _bypass;

    /** Mutex for thread-safe access of filter parameters. */
    QMutex _mutex;
};

#endif // QDIGITALFILTER_H
