#ifndef SEQ64_SEQROLL_HPP
#define SEQ64_SEQROLL_HPP

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
 * \file          seqroll.hpp
 *
 *  This module declares/defines the base class for drawing on the piano
 *  roll of the patterns editor.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-07-24
 * \updates       2015-10-03
 * \license       GNU GPLv2 or above
 *
 */

#include "globals.h"
#include "gui_drawingarea_gtk2.hpp"
#include "fruityseqroll.hpp"
#include "seq24seqroll.hpp"

namespace Gtk
{
    class Adjustment;
}

namespace seq64
{

class sequence;
class perform;
class seqdata;
class seqevent;
class seqkeys;

/**
 *  A small helper class representing a rectangle.
 */

class rect
{
public:

    int x, y, height, width;
};

/**
 *  Implements the piano roll section of the pattern editor.
 */

class seqroll : public gui_drawingarea_gtk2
{
    friend struct FruitySeqRollInput;
    friend struct Seq24SeqRollInput;

private:

    rect m_old;
    rect m_selected;
    sequence & m_seq;
    sequence * m_clipboard;
    seqkeys & m_seqkeys_wid;
    FruitySeqRollInput m_fruity_interaction;
    Seq24SeqRollInput m_seq24_interaction;
    int m_pos;

    /**
     * one pixel == m_zoom ticks*
     */

    int m_zoom;

    int m_snap;
    int m_note_length;
    int m_scale;
    int m_key;

    /**
     *  Indicates what is the data window currently editing.
     */

    unsigned char m_status;
    unsigned char m_cc;

    /**
     *  When highlighting a bunch of events.
     */

    bool m_selecting;
    bool m_moving;
    bool m_moving_init;
    bool m_growing;
    bool m_painting;
    bool m_paste;
    bool m_is_drag_pasting;
    bool m_is_drag_pasting_start;
    bool m_justselected_one;

    /**
     *  Tells where the dragging started.
     */

    int m_move_delta_x;
    int m_move_delta_y;
    int m_move_snap_offset_x;
    int m_old_progress_x;
    int m_scroll_offset_ticks;
    int m_scroll_offset_key;
    int m_scroll_offset_x;
    int m_scroll_offset_y;
    int m_background_sequence;
    bool m_drawing_background_seq;
    bool m_ignore_redraw;

public:

    seqroll
    (
        perform & perf,
        sequence & seq,
        int zoom, int snap,
//      seqdata & seqdata_wid,
//      seqevent & seqevent_wid,
        seqkeys & seqkeys_wid,
        int pos,
        Gtk::Adjustment & hadjust,
        Gtk::Adjustment & vadjust
    );
    ~seqroll ();

    void reset ();
    void redraw ();
    void redraw_events ();
    void set_key (int key);
    void set_scale (int scale);

    /**
     *  Sets the snap to the given value, and then resets the view.
     */

    void set_snap (int snap)
    {
        m_snap = snap;
        reset();
    }

    void set_zoom (int zoom);

    /**
     * \setter m_note_length
     */

    void set_note_length (int note_length)
    {
        m_note_length = note_length;
    }

    /**
     * \setter m_ignore_redraw
     */

    void set_ignore_redraw (bool ignore)
    {
        m_ignore_redraw = ignore;
    }

    void set_data_type (unsigned char status, unsigned char control);
    void set_background_sequence (bool state, int seq);
    void update_pixmap ();
    void update_sizes ();
    void update_background ();
    void draw_background_on_pixmap ();
    void draw_events_on_pixmap ();
    void draw_selection_on_window ();
    void draw_progress_on_window ();
    int idle_redraw ();
    void start_paste ();

private:

    void convert_xy (int x, int y, long & ticks, int & note);
    void convert_tn (long ticks, int note, int & x, int & y);
    void snap_y (int & y)
    {
        y -= (y % c_key_y);
    }

    void snap_x (int & x);
    void xy_to_rect
    (
        int x1, int y1, int x2, int y2,
        int & x, int & y, int & w, int & h
    );
    void convert_tn_box_to_rect
    (
        long tick_s, long tick_f, int note_h, int note_l,
        int & x, int & y, int & w, int & h
    );
    void draw_events_on (Glib::RefPtr<Gdk::Drawable> draw);
    int idle_progress ();
    void change_horz ();
    void change_vert ();
    void force_draw ();

private:            // special dual setters for friends

    void set_current_drop_x (int x)
    {
        m_current_x = m_drop_x = x;
    }

    void set_current_drop_y (int y)
    {
        m_current_y = m_drop_y = y;
    }


private:            // callbacks

    void on_realize ();
    bool on_expose_event (GdkEventExpose * ev);
    bool on_button_press_event (GdkEventButton * ev);
    bool on_button_release_event (GdkEventButton * ev);
    bool on_motion_notify_event (GdkEventMotion * ev);
    bool on_focus_in_event (GdkEventFocus *);
    bool on_focus_out_event (GdkEventFocus *);
    bool on_key_press_event (GdkEventKey * ev);
    bool on_scroll_event (GdkEventScroll * a_ev);
    void on_size_allocate (Gtk::Allocation &);
    bool on_leave_notify_event (GdkEventCrossing * p0);
    bool on_enter_notify_event (GdkEventCrossing * p0);

};

}           // namespace seq64

#endif      // SEQ64_SEQROLL_HPP

/*
 * seqroll.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
