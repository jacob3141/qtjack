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

// Own includes
#include "global.h"
#include "client.h"


namespace QtJack {
/**
 * @author Jacob Dawid ( jacob.dawid@omg-it.works )
 */
class Processor {
public:
    /** Constructs a new processor. */
    Processor(Client& client) :
        _client(client) {
    }

    /** Destructor. */
    virtual ~Processor() { }

    /**
     * @brief Called whenever audio samples have to be processed.
     * Warning: This method is time-critical.
     */
    virtual void process(int samples) { Q_UNUSED(samples); }

protected:
    Client& _client;
};

} // namespace QtJack
