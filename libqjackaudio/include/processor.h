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

#ifndef PROCESSOR_H
#define PROCESSOR_H

// Qt includes:
#include <QObject>

/**
 * @class Processor
 *
 * @author Jacob Dawid ( jacob.dawid@cybercatalyst.net )
 * @author Otto Ritter ( otto.ritter.or@googlemail.com )
 * @date 09.2011
 *
 * @brief Processor defines an interface that must be met by processors.
 *
 */
class Processor : public QObject
{
  Q_OBJECT
public:
    /** Constructs a new processor. */
    Processor() : QObject() { }

    /** Destructor. */
    virtual ~Processor() { }

    /**
     * @brief Called whenever samples have to be processed.
     * Warning: This method is time-critical.
     * @param samples Number of samples.
     */
    virtual void process(int samples) = 0;
};

#endif // PROCESSOR_H
