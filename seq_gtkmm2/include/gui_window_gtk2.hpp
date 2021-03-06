#ifndef SEQ64_GUI_WINDOW_GTK2_HPP
#define SEQ64_GUI_WINDOW_GTK2_HPP

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
 * \file          gui_window_gtk2.hpp
 *
 *  This module declares/defines the base class for the Performance Editor,
 *  also known as the Song Editor.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-09-22
 * \updates       2015-10-02
 * \license       GNU GPLv2 or above
 *
 */

/*
 *  Since these items are pointers, we were able to move (most) of the
 *  included header files to the cpp file.   Except for the items that
 *  come from widget.h, perhaps because GdkEventAny was a typedef.
 */

namespace Gtk
{
    class Adjustment;
    class HScrollbar;
    class VScrollbar;
}

namespace seq64
{

class perform;

/**
 *  This class supports a basic interface for Gtk::Window-derived objects.
 */

class gui_window_gtk2 : public Gtk::Window
{

private:

    perform & m_mainperf;

    /**
     *  Window sizes.  Could make this constant, but some windows are
     *  resizable.
     */

    int m_window_x;
    int m_window_y;

    bool m_modified;

public:

    gui_window_gtk2
    (
        perform & p,
        int window_x = 0,
        int window_y = 0
    );
    ~gui_window_gtk2 ();

    /**
     * \getter m_modified
     */

    bool is_modified () const
    {
        return m_modified;
    }

protected:

    /**
     * \getter m_mainperf
     */

    perform & perf ()
    {
        return m_mainperf;
    }

    /**
     * \setter m_modified
     */

    void is_modified (bool flag)
    {
        m_modified = flag;
    }

    virtual void quit ()
    {
        // TO DO!!!!
    }

};

}           // namespace seq64

#endif      // SEQ64_GUI_WINDOW_GTK2_HPP

/*
 * gui_window_gtk2.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
