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

#ifndef QSEMAPHORELOCKER_H
#define QSEMAPHORELOCKER_H

#include <QSemaphore>

/**
  * @class SemaphoreLocker
  * @author Jacob Dawid ( jacob.dawid@cybercatalyst.net )
  * Helper class for locking semaphores in scopes.
  */
class QSemaphoreLocker {
public:
    /** Constructs a locker, locks the given semaphore on construction. */
    QSemaphoreLocker(QSemaphore *semaphore) {
        Q_ASSERT(semaphore != 0);
        _semaphore = semaphore;
        if(_semaphore)
            _semaphore->acquire();
    }

    /** Destructor. Unlocks the semaphore. */
    ~QSemaphoreLocker() {
        if(_semaphore)
            _semaphore->release();
    }
private:
    /** Semaphore that has been locked. */
    QSemaphore *_semaphore;
};

#endif // QSEMAPHORELOCKER_H
