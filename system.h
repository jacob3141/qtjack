///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QtJack.                                           //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                                                                           //
//    QtJack is free software: you can redistribute it and/or modify         //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QtJack is distributed in the hope that it will be useful,              //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QtJack. If not, see <http://www.gnu.org/licenses/>.         //
//                                                                           //
//    It is possible to obtain a closed-source license of QtJack.            //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

// Qt includes
#include <QObject>

// Own includes
#include "global.h"

namespace QtJack {

class System : public QObject {
    Q_OBJECT
public:
    /** @returns the instance for this singleton. */
    static System *instance();

    /**
      * Callback for JACK's C API. Called when an error occurs.
      * @param message JACK's error message.
      */
    static void errorCallback(const char* message);

    /**
      * Callback for JACK's C API. Called when there is an
      * information message available.
      * @param message JACK's information message.
      */
    static void informationCallback(const char* message);


Q_SIGNALS:
    /** This signal will be emitted when an error occurs. */
    void error(QString errorMessage);

    /** This signal will be emitted when an information is available. */
    void information(QString informationMessage);

private:
    System();

    void emitError(QString errorMessage);
    void emitInformation(QString informationMessage);

    /** Singleton instance for this class. */
    static System _instance;
};

} // namespace QtJack
