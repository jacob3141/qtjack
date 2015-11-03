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

// JACK includes
#include <jack/types.h>
#include <jack/transport.h>
#include <jack/midiport.h>

namespace QtJack {
typedef jack_default_audio_sample_t AudioSample;
typedef jack_midi_data_t MidiData;

enum TransportState {
    TransportStateStopped,
    TransportStateRolling,
    TransportStateLooping,
    TransportStateStarting,
    TransportStateNetStarting,
    TransportStateUnknown
};

struct TransportPosition {
    TransportPosition() { }
    TransportPosition(jack_position_t jackPosition) {
        _uniqueId           = (long)jackPosition.unique_1;
        _microseconds       = (long)jackPosition.usecs;
        _framesPerSecond    = (int)jackPosition.frame_rate;
        _frameNumber        = (int)jackPosition.frame;

        _timeCode._frameTimeSeconds     = jackPosition.frame_time;
        _timeCode._nextFrameTimeSeconds = jackPosition.next_time;

        _bbt._bar            = (int)jackPosition.bar;
        _bbt._beat           = (int)jackPosition.beat;
        _bbt._tick           = (int)jackPosition.tick;
        _bbt._barStartTick   = jackPosition.bar_start_tick;

        _bbt._timeSignatureNominator     = jackPosition.beats_per_bar;
        _bbt._timeSignatureDenominator   = jackPosition.beat_type;
        _bbt._ticksPerBeat               = jackPosition.ticks_per_beat;
        _bbt._beatsPerMinute             = jackPosition.beats_per_minute;

        _bbt._frameOffset    = (int)jackPosition.bbt_offset;

        _video._audioFramesPerVideoFrame = jackPosition.audio_frames_per_video_frame;
        _video._audioToVideoFrameOffset  = (int)jackPosition.video_offset;

        _valid      = jackPosition.valid;
        _uniqueId2  = (long)jackPosition.unique_2;
    }

    long uniqueId()         { return _uniqueId; }
    long microseconds()     { return _microseconds; }
    int framesPerSecond()   { return _framesPerSecond; }
    int frameNumber()       { return _frameNumber; }

    struct TimeCode {
        double  _frameTimeSeconds;
        double  _nextFrameTimeSeconds;
    } _timeCode;

    struct BBT {
        int     _bar;
        int     _beat;
        int     _tick;
        double  _barStartTick;

        float   _timeSignatureNominator;
        float   _timeSignatureDenominator;
        double  _ticksPerBeat;
        double  _beatsPerMinute;

        int     _frameOffset;
    } _bbt;

    struct Video {
        float   _audioFramesPerVideoFrame;
        int     _audioToVideoFrameOffset;
    } _video;

    bool bbtDataValid() { return _valid & JackPositionBBT; }
    bool timeCodeValid() { return _valid & JackPositionTimecode; }
    bool bbtFrameOffsetValid() { return _valid & JackBBTFrameOffset; }
    bool audioVideoRatioValid() { return _valid & JackAudioVideoRatio; }
    bool audioToVideoFrameOffsetValid() { return _valid & JackVideoFrameOffset; }

    bool contentsConsistent() { return _uniqueId == _uniqueId2; }

    jack_position_t toJackPosition() {
        jack_position_t jackPosition;
        jackPosition.frame_time         = _timeCode._frameTimeSeconds;
        jackPosition.next_time          = _timeCode._nextFrameTimeSeconds;

        jackPosition.bar                = _bbt._bar;
        jackPosition.beat               = _bbt._beat;
        jackPosition.tick               = _bbt._tick;
        jackPosition.bar_start_tick     = _bbt._barStartTick;
        jackPosition.beats_per_bar      = _bbt._timeSignatureNominator;
        jackPosition.beat_type          = _bbt._timeSignatureDenominator;
        jackPosition.ticks_per_beat     = _bbt._ticksPerBeat;
        jackPosition.beats_per_minute   = _bbt._beatsPerMinute;
        jackPosition.bbt_offset         = _bbt._frameOffset;

        jackPosition.audio_frames_per_video_frame = _video._audioFramesPerVideoFrame;
        jackPosition.video_offset                 = _video._audioToVideoFrameOffset;

        jackPosition.valid              = _valid;
        return jackPosition;
    }

private:
    // These four cannot be set from clients, the server sets them.
    // So we make them non-writable for the user to not confuse him.
    long    _uniqueId;
    long    _microseconds;
    int     _framesPerSecond;
    int     _frameNumber;

    // Validity
    jack_position_bits_t _valid;

    long _uniqueId2;
};

} // namespace QtJack

// Marker to state that methods are realtime safe. That means, that in general
// those methods do not block for a large amount of time and thus are suited
// for realtime operation.
#define REALTIME_SAFE
