/*
    Copyright (C) 2003 Fons Adriaensen <fons.adriaensen@skynet.be>
    Copyright (C) 2011 Jacob Dawid <jacob.dawid@cybercatalyst.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef JNOISE_H
#define JNOISE_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "randomgenerator.h"

#define LRAND 1024
#define MRAND (LRAND - 1)

class JNoise {
public:
    JNoise();
    void process(int n, float *whiteNoiseBufferLeft, float *whiteNoiseBufferRight,
                 float *pinkNoiseBufferLeft, float *pinkNoiseBufferRight);

private:
    float m_frand[LRAND];
    RandomGenerator m_randomGenerator;
    float m_coeff[7];
};

#endif // JNOISE_H
