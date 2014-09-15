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

#include "jnoise.h"

JNoise::JNoise() {
    int i;
    m_randomGenerator.init(0);
    float s = 0.0f;
    for(i = 0; i < LRAND; i++) {
        s += m_frand[i] = 0.5f * m_randomGenerator.grandf();
    }

    s /= LRAND;
    for (i = 0; i < LRAND; i++) {
        m_frand [i] -= s;
    }

    for(i = 0; i < 7; i++) {
        m_coeff[i] = 0.0f;
    }
}

void JNoise::process(int n, float *whiteNoiseBufferLeft, float *whiteNoiseBufferRight,
                            float *pinkNoiseBufferLeft, float *pinkNoiseBufferRight) {
    float x;
    while (n--) {
        x  = m_frand[m_randomGenerator.irand () & MRAND]
           + m_frand[m_randomGenerator.irand () & MRAND]
           + m_frand[m_randomGenerator.irand () & MRAND]
           + m_frand[m_randomGenerator.irand () & MRAND];

        float whiteNoiseValue = 0.1f * x;
        if(whiteNoiseBufferLeft)
            *whiteNoiseBufferLeft++ = whiteNoiseValue;
        if(whiteNoiseBufferRight)
            *whiteNoiseBufferRight++ = whiteNoiseValue;

        x *= 0.03f;
        m_coeff[0] = 0.99886f * m_coeff[0] + 0.0555179f * x;
        m_coeff[1] = 0.99332f * m_coeff[1] + 0.0750759f * x;
        m_coeff[2] = 0.96900f * m_coeff[2] + 0.1538520f * x;
        m_coeff[3] = 0.86650f * m_coeff[3] + 0.3104856f * x;
        m_coeff[4] = 0.55000f * m_coeff[4] + 0.5329522f * x;
        m_coeff[5] = -0.7616f * m_coeff[5] - 0.0168980f * x;

        float pinkNoiseValue = m_coeff[0] + m_coeff[1] + m_coeff[2] + m_coeff[3] + m_coeff[4] + m_coeff[5] + m_coeff[6] + x * 0.5362f;
        if(pinkNoiseBufferLeft)
            *pinkNoiseBufferLeft++ = pinkNoiseValue;
        if(pinkNoiseBufferRight)
            *pinkNoiseBufferRight++ = pinkNoiseValue;
        m_coeff[6] = x * 0.115926f;
    }
}
