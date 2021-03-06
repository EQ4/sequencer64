#ifndef SEQ64_GUI_PALETTE_GTK2_HPP
#define SEQ64_GUI_PALETTE_GTK2_HPP

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
 * \file          gui_palette_gtk2.hpp
 *
 *  This module declares/defines the class for providing GTK/GDK colors.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-09-21
 * \updates       2015-10-04
 * \license       GNU GPLv2 or above
 *
 *  This module defines some Gdk::Color objects.  However, note that this
 *  object is <i> deprecated </i> in favor of Gdk::RGBA, defined by the
 *  gdkmm/rgba.h header file.
 */

#include <gtkmm/drawingarea.h>          // or #include <gtkmm/widget.h>

namespace seq64
{

/**
 *  Implements a stock palette of Gdk::Color elements.  Note that this
 *  class must be derived from Gtk::DrawingArea (or Gtk::Widget) in order
 *  to get access to the get_default_colormap() function used in the
 *  constructor.
 */

class gui_palette_gtk2 : public Gtk::DrawingArea    // or Gtk::Widget
{

protected:

    /**
     *  Provides a type for the color object.
     */

    typedef Gdk::Color Color;

private:                                // use the accessor functions
    const Color m_black;
    const Color m_white;
    const Color m_grey;
    const Color m_dk_grey;
    const Color m_lt_grey;
    const Color m_red;
    const Color m_orange;
    const Color m_yellow;
    const Color m_green;
    const Color m_blue;
    const Color m_cyan;

    Color m_bg_color;                   // m_background in some classes
    Color m_fg_color;                   // m_foreground in some classes

public:

    gui_palette_gtk2 ();
    ~gui_palette_gtk2 ();

    const Color & black () const
    {
        return m_black;
    }
    const Color & white () const
    {
        return m_white;
    }
    const Color & grey () const
    {
        return m_grey;
    }
    const Color & dark_grey () const
    {
        return m_dk_grey;
    }
    const Color & light_grey () const
    {
        return m_lt_grey;
    }
    const Color & red () const
    {
        return m_red;
    }
    const Color & orange () const
    {
        return m_orange;
    }
    const Color & yellow () const
    {
        return m_yellow;
    }
    const Color & green () const
    {
        return m_green;
    }
    const Color & blue () const
    {
        return m_blue;
    }
    const Color & cyan () const
    {
        return m_cyan;
    }

    const Color & bg_color () const
    {
        return m_bg_color;
    }
    void bg_color (const Color & c)
    {
        m_bg_color = c;
    }

    const Color & fg_color () const
    {
        return m_fg_color;
    }
    void fg_color (const Color & c)
    {
        m_fg_color = c;
    }

};

}           // namespace seq64

#endif      // SEQ64_GUI_PALETTE_GTK2_HPP

/*
 * gui_palette_gtk2.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
