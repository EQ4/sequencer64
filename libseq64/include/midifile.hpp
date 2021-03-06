#ifndef SEQ64_MIDIFILE_HPP
#define SEQ64_MIDIFILE_HPP

/*
 *  This file is part of seq24/sequencer64.
 *
 *  seq24 is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  seq24 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with seq24; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          midifile.hpp
 *
 *  This module declares/defines the base class for MIDI files.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-07-24
 * \updates       2015-09-29
 * \license       GNU GPLv2 or above
 *
 *  The Seq24 MIDI file is a standard, Format 1 MIDI file, with some extra
 *  "proprietary" tracks that hold information needed to set up the song
 *  in Seq24.
 */

#include <string>
#include <list>
#include <vector>

namespace seq64
{

class perform;                          // forward reference

/**
 *  This class handles the parsing and writing of MIDI files.  In addition
 *  to the standard MIDI tracks, it also handles some "private" or
 *  "proprietary" tracks specific to Seq24.  It does not, however,
 *  handle SYSEX events.
 */

class midifile
{

private:

    /**
     *  Holds the position in the MIDI file.  This is at least a 32-bit
     *  value in the recent architectures running Linux and Windows.
     */

    int m_pos;

    /**
     *  The unchanging name of the MIDI file.
     */

    const std::string m_name;

    /**
     *  This vector of characters holds our MIDI data.  We could also use
     *  a string of characters, unsigned.
     */

    std::vector<unsigned char> m_data;

    /**
     *  Provides a list of characters.
     */

    std::list<unsigned char> m_char_list;

    /**
     *  Use the new format for the proprietary footer section of the Seq24
     *  MIDI file.  In this new format, each sequencer-specfic value
     *  (0x242400xx, as defined in the globals module) is preceded by the
     *  sequencer-specific prefix, 0xFF 0x7F len id/date). By default,
     *  this value is true, but the user can specify the --legacy (-l)
     *  option, or make a soft link to the sequence24 binary called
     *  "seq24",  to write the data in the old format. [We will eventually
     *  add the --legacy option to the <tt> ~/.seq24rc </tt> configuration
     *  file.]  Note that reading can handle either format transparently.
     */

    bool m_new_format;

public:

    midifile (const std::string & name, bool propformat = true);
    ~midifile ();

    bool parse (perform & a_perf, int a_screen_set);
    bool write (perform & a_perf);

private:

    unsigned long parse_prop_header (int file_size);
    bool parse_proprietary_track (perform & a_perf, int file_size);
    unsigned long read_long ();
    unsigned short read_short ();
    unsigned char read_byte ();
    unsigned long read_varinum ();
    void write_long (unsigned long);
    void write_short (unsigned short);

    /**
     *  Writes 1 byte.  The byte is written to the m_char_list member, using a
     *  call to push_back().
     */

    void write_byte (unsigned char c)
    {
        m_char_list.push_back(c);
    }

    void write_varinum (unsigned long);
    void write_track_name (const std::string & trackname);
    void write_seq_number (unsigned short seqnum);
    void write_track_end ();
    void write_prop_header (unsigned long tag, long len);
    bool write_proprietary_track (perform & a_perf);
    long varinum_size (long len) const;
    long prop_item_size (long datalen) const;
    long track_name_size (const std::string & trackname) const;

    /**
     *  Returns the size of a sequence-number event, which is always 5
     *  bytes, plus one byte for the delta time that precedes it..
     */

    long seq_number_size () const
    {
        return 6;
    }

    /**
     *  Returns the size of a track-end event, which is always 3 bytes.
     */

    long track_end_size () const
    {
        return 3;
    }

};

}           // namespace seq64

#endif      // SEQ64_MIDIFILE_HPP

/*
 * midifile.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
