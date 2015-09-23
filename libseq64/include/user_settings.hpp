#ifndef SEQ64_USER_SETTINGS_HPP
#define SEQ64_USER_SETTINGS_HPP

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
 * \file          user_settings.h
 *
 *  This module declares/defines just some of the global (gasp!) variables
 *  in this application.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-09-22
 * \updates       2015-09-22
 * \license       GNU GPLv2 or above
 *
 *  This collection of variables describes some facets of the
 *  "Patterns Panel" or "Sequences Window", which is visually presented by
 *  the Gtk::Window-derived class called mainwnd.
 *
 *  The Patterns Panel contains an 8-by-4 grid of "pattern boxes" or
 *  "sequence boxes".  All of the patterns in this grid comprise what is
 *  called a "set" (in the musical sense) or a "screen set".
 *
 *  We want to be able to change these defaults.
 */

#include <string>

#include "easy_macros.h"               // with platform_macros.h, too

/**
 *  A manifest constant for the normal number of semitones in an
 *  equally-tempered octave.  The name is short deliberately.
 */

#define OCTAVE_SIZE                      12

/**
 *  Manifest constant for the maximum value limit of a MIDI byte when used
 *  to limit the size of an array.
 */

#define MIDI_COUNT_MAX                  128

/**
 *  Default value for c_ppqn (global parts-per-quarter-note value).
 */

#define DEFAULT_PPQN                    192

/**
 *  Default value for c_bpm (global beats-per-minute)
 */

#define DEFAULT_BPM                     120

/**
 *  Default value for c_max_busses.
 */

#define DEFAULT_BUSS_MAX                 32

/**
 *  Default value for c_thread_trigger_width_ms.
 */

#define DEFAULT_TRIGWIDTH_MS              4

/**
 *  Default value for c_thread_trigger_width_ms.
 */

#define DEFAULT_TRIGLOOK_MS               2


#ifdef EXPOSE_THESE
const int c_mainwnd_rows = 4;
const int c_mainwnd_cols = 8;
const int c_seqs_in_set = c_mainwnd_rows * c_mainwnd_cols;
const int c_gmute_tracks = c_seqs_in_set * c_seqs_in_set;
const int c_max_sets = 32;
const int c_max_sequence = c_seqs_in_set * c_max_sets;
const int c_text_x =  6;
const int c_text_y = 12;
const int c_seqchars_x = 15;
const int c_seqchars_y =  5;
const int c_seqarea_x = c_text_x * c_seqchars_x;
const int c_seqarea_y = c_text_y * c_seqchars_y;
const int c_seqarea_seq_x = c_text_x * 13;
const int c_seqarea_seq_y = c_text_y * 2;
const int c_mainwid_border = 0;             // try 2 or 3instead of 0
const int c_mainwid_spacing = 2;            // try 4 or 6 instead of 2
const int c_control_height = 0;
const int c_mainwid_x =
(
    (c_seqarea_x + c_mainwid_spacing) * c_mainwnd_cols -
        c_mainwid_spacing + c_mainwid_border * 2
);
const int c_mainwid_y =
(
    (c_seqarea_y + c_mainwid_spacing) * c_mainwnd_rows +
         c_control_height + c_mainwid_border * 2
);
#endif


#if EXPOSE_THESE
user_midi_bus_definition   global_user_midi_bus_definitions[c_max_busses];
user_instrument_definition global_user_instrument_definitions[c_max_instruments];
#endif

/**
 *  Holds the current values of sequence settings and settings that can
 *  modify the number of sequences and the configuration of the
 *  user-interface.  These settings will eventually be made part of the
 *  "user" settings file.
 */

class user_settings
{

    /**
     *  Number of rows in the Patterns Panel.  The current value is 4, and
     *  if changed, many other values depend on it.  Together with
     *  m_mainwnd_cols, this value fixes the patterns grid into a 4 x 8
     *  set of patterns known as a "screen set".
     */

    int m_mainwnd_rows;

    /**
     *  Number of columns in the Patterns Panel.  The current value is 4,
     *  and probably won't change, since other values depend on it.
     *  Together with m_mainwnd_rows, this value fixes the patterns grid
     *  into a 4 x 8 set of patterns known as a "screen set".
     */

    int m_mainwnd_cols;

    /**
     *  Number of patterns/sequences in the Patterns Panel, also known as
     *  a "set" or "screen set".  This value is 4 x 8 = 32 by default.
     */

    int m_seqs_in_set;

    /**
     *  Number of group-mute tracks that can be support, which is
     *  m_seqs_in_set squared, or 1024.
     */

    int m_gmute_tracks;

    /**
     *  Maximum number of screen sets that can be supported.  Basically,
     *  that the number of times the Patterns Panel can be filled.  32
     *  sets can be created.
     */

    int m_max_sets;

    /*
     *  The maximum number of patterns supported is given by the number of
     *  patterns supported in the panel (32) times the maximum number of
     *  sets (32), or 1024 patterns.  It is basically the same value as
     *  m_max_sequence by default.
     */

     int m_total_seqs;

    /**
     *  The maximum number of patterns supported is given by the number of
     *  patterns supported in the panel (32) times the maximum number of
     *  sets (32), or 1024 patterns.
     */

    int m_max_sequence;

    /**
     *  Constants for the mainwid class.  The m_text_x and m_text_y
     *  constants help define the "seqarea" size.  It looks like these two
     *  values are the character width (x) and height (y) in pixels.
     *  Thus, these values would be dependent on the font chosen.  But
     *  that, currently, is hard-wired.  See the m_font_6_12[] array for
     *  the default font specification.
     *
     *  However, please not that font files are not used.  Instead, the
     *  fonts are provided by two pixmaps in the <tt> src/pixmap </tt>
     *  directory: <tt> font_b.xpm </tt> (black lettering on a white
     *  background) and <tt> font_w.xpm </tt> (white lettering on a black
     *  background).
     */

    int m_text_x;
    int m_text_y;

    /**
     *  Constants for the mainwid class.  The m_seqchars_x and
     *  m_seqchars_y constants help define the "seqarea" size.  These look
     *  like the number of characters per line and the number of lines of
     *  characters, in a pattern/sequence box.
     */

    int m_seqchars_x;
    int m_seqchars_y;

    /**
     *  The m_seqarea_x and m_seqarea_y constants are derived from the
     *  width and heights of the default character set, and the number of
     *  characters in width, and the number of lines, in a
     *  pattern/sequence box.
     *
     *  Compare these two constants to m_seqarea_seq_x(y), which was in
     *  mainwid.h, but is now in this file.
     */

    int m_seqarea_x;
    int m_seqarea_y;

    /**
     * Area of what?  Doesn't look at all like it is based on the size of
     * characters.  These are used only in the mainwid module.
     */

    int m_seqarea_seq_x;
    int m_seqarea_seq_y;

    /**
     *  These control sizes.  We'll try changing them and see what
     *  happens.  Increasing these value spreads out the pattern grids a
     *  little bit and makes the Patterns panel slightly bigger.  Seems
     *  like it would be useful to make these values user-configurable.
     */

    int m_mainwid_border;             // try 2 or 3 instead of 0
    int m_mainwid_spacing;            // try 4 or 6 instead of 2

    /**
     *  This constants seems to be created for a future purpose, perhaps
     *  to reserve space for a new bar on the mainwid pane.  But it is
     *  used only in this header file, to define m_mainwid_y, but doesn't
     *  add anything to that value.
     */

    int m_control_height;

    /**
     * The width of the main pattern/sequence grid, in pixels.  Affected
     * by the m_mainwid_border and m_mainwid_spacing values.
     */

    int m_mainwid_x;

    /*
     * The height  of the main pattern/sequence grid, in pixels.  Affected by
     * the m_mainwid_border and m_control_height values.
     */

    int m_mainwid_y;

public:

    user_settings ();
    user_settings (const user_settings & rhs);
    user_settings & operator = (const user_settings & rhs);

    void set_defaults ();
    void normalize ();
    void globalize_settings ();

    /**
     * \accessor m_mainwnd_rows
     */

    int mainwnd_rows () const
    {
        return m_mainwnd_rows;
    }

    /**
     * \accessor m_mainwnd_cols
     */

    int mainwnd_cols () const
    {
        return m_mainwnd_cols;
    }

    /**
     * \accessor m_seqs_in_set
     */

    int seqs_in_set () const
    {
        return m_seqs_in_set;
    }

    /**
     * \accessor m_gmute_tracks
     */

    int gmute_tracks () const
    {
        return m_gmute_tracks;
    }

    /**
     * \accessor m_max_sets
     */

    int max_sets () const
    {
        return m_max_sets;
    }

    /**
     * \accessor m_max_sequence
     */

    int max_sequence () const
    {
        return m_max_sequence;
    }

    /**
     * \accessor m_text_x
     */

    int text_x () const
    {
        return m_text_x;
    }

    /**
     * \accessor m_text_y
     */

    int text_y () const
    {
        return m_text_y;
    }

    /**
     * \accessor m_seqchars_x
     */

    int seqchars_x () const
    {
        return m_seqchars_x;
    }

    /**
     * \accessor m_seqchars_y
     */

    int seqchars_y () const
    {
        return m_seqchars_y;
    }

    /**
     * \accessor m_seqarea_x
     */

    int seqarea_x () const
    {
        return m_seqarea_x;
    }

    /**
     * \accessor m_seqarea_y
     */

    int seqarea_y () const
    {
        return m_seqarea_y;
    }

    /**
     * \accessor m_seqarea_seq_x
     */

    int seqarea_seq_x () const
    {
        return m_seqarea_seq_x;
    }

    /**
     * \accessor m_seqarea_seq_y
     */

    int seqarea_seq_y () const
    {
        return m_seqarea_seq_y;
    }

    /**
     * \accessor m_mainwid_border
     */

    int mainwid_border () const
    {
        return m_mainwid_border;
    }

    /**
     * \accessor m_mainwid_spacing
     */

    int mainwid_spacing () const
    {
        return m_mainwid_spacing;
    }

    /**
     * \accessor m_control_height
     */

    int control_height () const
    {
        return m_control_height;
    }

    /**
     * \accessor m_mainwid_x
     */

    int mainwid_x () const
    {
        return m_mainwid_x;
    }

    /**
     * \accessor m_mainwid_y
     */

    int mainwid_y () const
    {
        return m_mainwid_y;
    }

    void mainwnd_rows (int value);
    void mainwnd_cols (int value);
    void seqs_in_set (int value);
    void gmute_tracks (int value);
    void max_sets (int value);
    void max_sequence (int value);
    void text_x (int value);
    void text_y (int value);
    void seqchars_x (int value);
    void seqchars_y (int value);
    void seqarea_x (int value);
    void seqarea_y (int value);
    void seqarea_seq_x (int value);
    void seqarea_seq_y (int value);
    void mainwid_border (int value);
    void mainwid_spacing (int value);
    void control_height (int value);
    void mainwid_x (int value);
    void mainwid_y (int value);

};

#endif  // SEQ64_USER_SETTINGS_HPP

/*
 * user_settings.h
 *
 * vim: sw=4 ts=4 wm=8 et ft=cpp
 */
