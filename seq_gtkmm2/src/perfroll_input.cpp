/*
 *  This file is part of seq24/sequencer64.
 *
 *  seq24 is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General)mm Public License as published by
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
 * \file          perfroll_input.cpp
 *
 *  This module declares/defines the base class for the Performance window
 *  mouse input.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-07-24
 * \updates       2015-10-03
 * \license       GNU GPLv2 or above
 *
 */

#include <gtkmm/button.h>
#include <gdkmm/cursor.h>

#include "click.hpp"                    /* CLICK_IS_LEFT(), etc.    */
#include "perform.hpp"
#include "perfroll_input.hpp"
#include "perfroll.hpp"
#include "sequence.hpp"

namespace seq64
{

/**
 *  Updates the mouse pointer, implementing a context-sensitive mouse.
 *  Note that perform::convert_xy() returns its values via side-effects on the
 *  last two parameters.
 */

void
FruityPerfInput::updateMousePtr (perfroll & roll)
{
    perform & p = roll.perf();
    long droptick;
    int dropseq;
    roll.convert_xy(m_current_x, m_current_y, droptick, dropseq);
    if (p.is_active(dropseq))
    {
        long start, end;
        if (p.get_sequence(dropseq)->intersectTriggers(droptick, start, end))
        {
            int wscalex = c_perfroll_size_box_click_w * c_perf_scale_x;
            int ymod = m_current_y % c_names_y;
            if
            (
                start <= droptick && droptick <= (start + wscalex) &&
                (ymod <= c_perfroll_size_box_click_w + 1)
            )
            {
                roll.get_window()->set_cursor(Gdk::Cursor(Gdk::RIGHT_PTR));
            }
            else if
            (
                droptick <= end && (end - wscalex) <= droptick &&
                ymod >= (c_names_y - c_perfroll_size_box_click_w - 1)
            )
            {
                roll.get_window()->set_cursor(Gdk::Cursor(Gdk::LEFT_PTR));
            }
            else
                roll.get_window()->set_cursor(Gdk::Cursor(Gdk::CENTER_PTR));
        }
        else
            roll.get_window()->set_cursor(Gdk::Cursor(Gdk::PENCIL));
    }
    else
        roll.get_window()->set_cursor(Gdk::Cursor(Gdk::CROSSHAIR));
}

/**
 *  Handles a button-press event in the Fruity manner.
 */

bool
FruityPerfInput::on_button_press_event (GdkEventButton * a_ev, perfroll & roll)
{
    perform & p = roll.perf();
    roll.grab_focus();
    int & dropseq = roll.m_drop_sequence;       /* reference needed         */
    if (p.is_active(dropseq))
    {
        p.get_sequence(dropseq)->unselect_triggers();
        roll.draw_all();
    }
    roll.m_drop_x = int(a_ev->x);
    roll.m_drop_y = int(a_ev->y);
    m_current_x = int(a_ev->x);
    m_current_y = int(a_ev->y);
    roll.convert_xy                             /* side-effects             */
    (
        roll.m_drop_x, roll.m_drop_y, roll.m_drop_tick, dropseq
    );
    if (CLICK_IS_LEFT(a_ev->button))            /* left mouse button        */
    {
        on_left_button_pressed(a_ev, roll);
    }
    else if (CLICK_IS_RIGHT(a_ev->button))      /* right mouse button       */
    {
        on_right_button_pressed(a_ev, roll);
    }
    else if (CLICK_IS_MIDDLE(a_ev->button))     /* left-ctrl, middle: split */
    {
        if (p.is_active(dropseq))
        {
            long droptick = roll.m_drop_tick;
            bool state = p.get_sequence(dropseq)->get_trigger_state(droptick);
            if (state)
                roll.split_trigger(dropseq, droptick);
        }
    }
    updateMousePtr(roll);
    return true;
}

/**
 *  Handles the left button of the mouse.
 *
 *  It can handle splitting triggers (?), adding notes, and the following
 *  clicks to resize the event, or move it, depending on where clicked:
 *
 *      -   clicked left side: begin a grow/shrink for the left side
 *      -   clicked right side: grow/shrink the right side
 *      -   clicked in the middle - move it
 *
 *  I don't get it, though... all three buttons are handled in the generic
 *  button-press callback.
 */

void
FruityPerfInput::on_left_button_pressed (GdkEventButton * a_ev, perfroll & roll)
{
    perform & p = roll.perf();
    int dropseq = roll.m_drop_sequence;
    if (a_ev->state & GDK_CONTROL_MASK)
    {
        if (p.is_active(dropseq))
        {
            bool state = p.get_sequence(dropseq)->
                get_trigger_state(roll.m_drop_tick);

            if (state)
                roll.split_trigger(dropseq, roll.m_drop_tick);
        }
    }
    else    /* add a new note */
    {
        long tick = roll.m_drop_tick;
        m_adding_pressed = true;
        if (p.is_active(dropseq))
        {
            long seqlength = p.get_sequence(dropseq)->get_length();
            bool state = p.get_sequence(dropseq)->get_trigger_state(tick);

            /* resize the event, or move it, depending on where clicked */

            if (state)
            {
                m_adding_pressed = false;
                p.push_trigger_undo();
                p.get_sequence(dropseq)->select_trigger(tick);
                long start_tick = p.get_sequence(dropseq)->
                    get_selected_trigger_start_tick()
                    ;
                long end_tick = p.get_sequence(dropseq)->
                    get_selected_trigger_end_tick()
                    ;
                int wscalex = c_perfroll_size_box_click_w * c_perf_scale_x;
                int ydrop = roll.m_drop_y % c_names_y;
                if
                (
                    tick >= start_tick && tick <= (start_tick + wscalex) &&
                    ydrop <= (c_perfroll_size_box_click_w + 1)
                )
                {
                    /*
                     * Clicked left side: begin a grow/shrink for the left side.
                     */

                    roll.m_growing = true;
                    roll.m_grow_direction = true;
                    roll.m_drop_tick_trigger_offset = roll.m_drop_tick -
                        p.get_sequence(dropseq)->
                            get_selected_trigger_start_tick();
                }
                else if
                (
                    tick >= (end_tick - wscalex) && tick <= end_tick &&
                    ydrop >= (c_names_y - c_perfroll_size_box_click_w - 1)
                )
                {
                    /*
                     * Clicked right side: grow/shrink the right side.
                     */

                    roll.m_growing = true;
                    roll.m_grow_direction = false;
                    roll.m_drop_tick_trigger_offset = roll.m_drop_tick -
                        p.get_sequence(dropseq)->
                            get_selected_trigger_end_tick() ;
                }
                else
                {
                    /*
                     * Clicked in the middle - move it.
                     */

                    roll.m_moving = true;
                    roll.m_drop_tick_trigger_offset = roll.m_drop_tick -
                         p.get_sequence(dropseq)->
                             get_selected_trigger_start_tick() ;
                }
                roll.draw_all();
            }
            else                                    // add an event
            {
                tick -= (tick % seqlength);         // snap to sequence length
                p.push_trigger_undo();
                p.get_sequence(dropseq)->add_trigger(tick, seqlength);
                roll.draw_all();
            }
        }
    }
}

/**
 *  Handles the right button of the mouse.
 *
 *  I don't get it, though... all three buttons are handled in the generic
 *  button-press callback.
 */

void
FruityPerfInput::on_right_button_pressed (GdkEventButton * a_ev, perfroll & roll)
{
    perform & p = roll.perf();
    long tick = roll.m_drop_tick;
    int dropseq = roll.m_drop_sequence;
    if (p.is_active(dropseq))
    {
        bool state = p.get_sequence(dropseq)->get_trigger_state(tick);
        if (state)
        {
            p.push_trigger_undo();
            p.get_sequence(dropseq)->del_trigger(tick);
        }
    }
}

/**
 *  Handles a button-release event.  Why is m_adding_pressed modified
 *  conditionally when the same modification is then made unconditionally?
 */

bool
FruityPerfInput::on_button_release_event (GdkEventButton * a_ev, perfroll & roll)
{
    m_current_x = (int) a_ev->x;
    m_current_y = (int) a_ev->y;
    if (CLICK_IS_LEFT(a_ev->button) || CLICK_IS_RIGHT(a_ev->button))
        m_adding_pressed = false;                   // done here...

    perform & p = roll.perf();
    roll.m_moving = false;
    roll.m_growing = false;
    m_adding_pressed = false;                       // and here...???
    if (p.is_active(roll.m_drop_sequence))
        roll.draw_all();

    updateMousePtr(roll);
    return true;
}

/**
 *  Handles a Fruity motion-notify event.
 */

bool
FruityPerfInput::on_motion_notify_event (GdkEventMotion * a_ev, perfroll & roll)
{
    int x = (int) a_ev->x;
    perform & p = roll.perf();
    int dropseq = roll.m_drop_sequence;
    m_current_x = int(a_ev->x);
    m_current_y = int(a_ev->y);
    if (m_adding_pressed)
    {
        long tick;
        roll.convert_x(x, tick);
        if (p.is_active(dropseq))
        {
            long seqlength = p.get_sequence(dropseq)->get_length();
            tick -= (tick % seqlength);

            long length = seqlength;
            p.get_sequence(dropseq)->grow_trigger(roll.m_drop_tick, tick, length);
            roll.draw_all();
        }
    }
    else if (roll.m_moving || roll.m_growing)
    {
        if (p.is_active(dropseq))
        {
            long tick;
            roll.convert_x(x, tick);
            tick -= roll.m_drop_tick_trigger_offset;
            tick -= tick % roll.m_snap;
            if (roll.m_moving)
                p.get_sequence(dropseq)->move_selected_triggers_to(tick, true);

            if (roll.m_growing)
            {
                if (roll.m_grow_direction)
                    p.get_sequence(dropseq)->
                        move_selected_triggers_to(tick, false, 0);
                else
                    p.get_sequence(dropseq)->
                        move_selected_triggers_to(tick - 1, false, 1);
            }
            roll.draw_all();
        }
    }
    updateMousePtr(roll);
    return true;
}

/*
 * The normal Seq24 section, at last!
 */

/**
 *  A popup menu (which one?) calls this.  What does it mean?
 */

void
Seq24PerfInput::set_adding (bool adding, perfroll & roll)
{
    if (adding)
    {
        roll.get_window()->set_cursor(Gdk::Cursor(Gdk::PENCIL));
        m_adding = true;
    }
    else
    {
        roll.get_window()->set_cursor(Gdk::Cursor(Gdk::LEFT_PTR));
        m_adding = false;
    }
}

/**
 *  Handles the normal variety of button-press event.
 *
 *  Is there any easy way to use ctrl-left-click as the middle button
 *  here?
 */

bool
Seq24PerfInput::on_button_press_event (GdkEventButton * a_ev, perfroll & roll)
{
    perform & p = roll.perf();
    int & dropseq = roll.m_drop_sequence;
    roll.grab_focus();
    if (p.is_active(dropseq))
    {
        p.get_sequence(dropseq)->unselect_triggers();
        roll.draw_all();
    }
    roll.m_drop_x = (int) a_ev->x;
    roll.m_drop_y = (int) a_ev->y;
    roll.convert_xy                                 /* side-effects */
    (
        roll.m_drop_x, roll.m_drop_y, roll.m_drop_tick, dropseq
    );

    if (CLICK_IS_LEFT(a_ev->button))
    {
        long tick = roll.m_drop_tick;
        if (m_adding)         /* add a new note if we didn't select anything */
        {
            m_adding_pressed = true;
            if (p.is_active(dropseq))
            {
                long seqlength = p.get_sequence(dropseq)->get_length();
                bool state = p.get_sequence(dropseq)->get_trigger_state(tick);
                if (state)
                {
                    p.push_trigger_undo();
                    p.get_sequence(dropseq)->del_trigger(tick);
                }
                else
                {
                    tick -= (tick % seqlength);    // snap to sequence length
                    p.push_trigger_undo();
                    p.get_sequence(dropseq)->add_trigger(tick, seqlength);
                    roll.draw_all();
                }
            }
        }
        else
        {
            if (p.is_active(dropseq))
            {
                p.push_trigger_undo();
                p.get_sequence(dropseq)->select_trigger(tick);

                long start_tick = p.get_sequence(dropseq)->
                        get_selected_trigger_start_tick()
                        ;
                long end_tick = p.get_sequence(dropseq)->
                        get_selected_trigger_end_tick()
                        ;

                int wscalex = c_perfroll_size_box_click_w * c_perf_scale_x;
                int ydrop = roll.m_drop_y % c_names_y;
                if
                (
                    tick >= start_tick && tick <= (start_tick + wscalex) &&
                    ydrop <= c_perfroll_size_box_click_w + 1
                )
                {
                    roll.m_growing = true;
                    roll.m_grow_direction = true;
                    roll.m_drop_tick_trigger_offset = roll.m_drop_tick -
                         p.get_sequence(dropseq)->
                             get_selected_trigger_start_tick() ;
                }
                else if
                (
                    tick >= (end_tick - wscalex) && tick <= end_tick &&
                    ydrop >= c_names_y - c_perfroll_size_box_click_w - 1
                )
                {
                    roll.m_growing = true;
                    roll.m_grow_direction = false;
                    roll.m_drop_tick_trigger_offset = roll.m_drop_tick -
                        p.get_sequence(dropseq)->
                            get_selected_trigger_end_tick() ;
                }
                else
                {
                    roll.m_moving = true;
                    roll.m_drop_tick_trigger_offset = roll.m_drop_tick -
                         p.get_sequence(dropseq)->
                             get_selected_trigger_start_tick() ;
                }
                roll.draw_all();
            }
        }
    }
    else if (CLICK_IS_RIGHT(a_ev->button))
    {
        set_adding(true, roll);
    }
    else if (CLICK_IS_MIDDLE(a_ev->button))                   /* split    */
    {
        if (p.is_active(dropseq))
        {
            bool state = p.get_sequence(dropseq)->
                get_trigger_state(roll.m_drop_tick);

            if (state)
                roll.split_trigger(dropseq, roll.m_drop_tick);
        }
    }
    return true;
}

/**
 *  Handles various button-release events.
 *
 *  Any use for the middle-button or ctrl-left-click we can add?
 */

bool
Seq24PerfInput::on_button_release_event (GdkEventButton * a_ev, perfroll & roll)
{
    if (CLICK_IS_LEFT(a_ev->button))
    {
        if (m_adding)
            m_adding_pressed = false;
    }
    else if (CLICK_IS_RIGHT(a_ev->button))
    {
        /*
         * Minor new feature.  If the Super (Mod4, Windows) key is
         * pressed when release, keep the adding-state in force.  One
         * can then use the unadorned left-click key to add material.  Right
         * click to reset the adding mode.  This feature is enabled only
         * if allowed by Options / Mouse  (but is true by default).
         * See the same code in seq24seqroll.cpp.
         */

        bool addmode_exit = ! global_allow_mod4_mode;
        if (! addmode_exit)
            addmode_exit = ! (a_ev->state & GDK_MOD4_MASK); // Mod4 held?

        if (addmode_exit)
        {
            m_adding_pressed = false;
            set_adding(false, roll);
        }
    }

    perform & p = roll.perf();
    roll.m_moving = roll.m_growing = m_adding_pressed = false;
    if (p.is_active(roll.m_drop_sequence))
        roll.draw_all();

    return true;
}

/**
 *  Handles the normal motion-notify event.
 */

bool
Seq24PerfInput::on_motion_notify_event (GdkEventMotion * a_ev, perfroll & roll)
{
    int x = int(a_ev->x);
    perform & p = roll.perf();
    int dropseq = roll.m_drop_sequence;
    if (m_adding && m_adding_pressed)
    {
        long tick;
        roll.convert_x(x, tick);
        if (p.is_active(dropseq))
        {
            long seqlength = p.get_sequence(dropseq)->get_length();
            tick -= (tick % seqlength);

            long length = seqlength;
            p.get_sequence(dropseq)->grow_trigger(roll.m_drop_tick, tick, length);
            roll.draw_all();
        }
    }
    else if (roll.m_moving || roll.m_growing)
    {
        if (p.is_active(dropseq))
        {
            long tick;
            roll.convert_x(x, tick);
            tick -= roll.m_drop_tick_trigger_offset;
            tick -= tick % roll.m_snap;
            if (roll.m_moving)
            {
                p.get_sequence(dropseq)->move_selected_triggers_to(tick, true);
            }
            if (roll.m_growing)
            {
                if (roll.m_grow_direction)
                    p.get_sequence(dropseq)->
                        move_selected_triggers_to(tick, false, 0);
                else
                    p.get_sequence(dropseq)->
                        move_selected_triggers_to(tick - 1, false, 1);
            }
            roll.draw_all();
        }
    }
    return true;
}

}           // namespace seq64

/*
 * perfroll_input.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
