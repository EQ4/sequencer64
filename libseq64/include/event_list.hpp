#ifndef SEQ64_EVENT_LIST_HPP
#define SEQ64_EVENT_LIST_HPP

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
 * \file          event_list.hpp
 *
 *  This module provides a stand-alone module for the event-list container
 *  used by the application.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-09-19
 * \updates       2015-09-20
 * \license       GNU GPLv2 or above
 *
 *  This module extracts the event-list functionality from the sequencer
 *  module, so that it is easier to try to replace it with some better
 *  container later.
 *
 *  We should leverage "for-each" functionality.
 *
 *  List versus Map:  #ifdef or derivation from an interface?  For our
 *  purposes, #ifdef might be simplest, and we only want to pick the
 *  fastest one, ultimately.
 */

#include <string>
#include <stack>

/**
 * This macro indicates an experimental feature where we are tyring to see
 * if using std::multimap as an event-container has any benefits over
 * using std::list.  Define this macro to use the multimap.
 */

#define USE_EVENT_MAP                   /* this seems to work well! */

#ifdef USE_EVENT_MAP
#include <map>
#else
#include <list>
#endif

/**
 *  Provides a brief, very searchable notation for the use of the
 *  event_list::dref() function that allows treating list and multimap
 *  iterators the same in client code.  It turned out to be worth the
 *  time, the experiment, and the somewhat obtuse notation, to be able to
 *  switch between std::multimap and std::list at compile time.
 */

#define DREF(e)     event_list::dref(e)

#include "event.hpp"

namespace seq64
{

/**
 *  The event_list class is a receptable for MIDI events.  Two
 *  implementations, an std::multimap, and the original, an std::list, are
 *  provided for comparison, and are selected at build time, by manually
 *  defining the USE_EVENT_MAP macro near the top of this module.
 */

class event_list
{

    friend class sequence;              // access to event_list::iterator

private:

#ifdef USE_EVENT_MAP

    /**
     *  Provides a key value for an event map.  Its types match the
     *  m_timestamp and get_rank() function of this event class.
     */

    class event_key
    {

    private:

        unsigned long m_timestamp;
        int m_rank;

    public:

        event_key (unsigned long tstamp, int rank);
        event_key (const event & e);
        bool operator < (const event_key & rhs) const;

    };

    /**
     *  Types to use to swap between list and multimap implementations.
     */

    typedef std::multimap<event_key, event> Events;
    typedef std::pair<event_key, event> EventsPair;
    typedef std::multimap<event_key, event>::iterator iterator;
    typedef std::multimap<event_key, event>::const_iterator const_iterator;

#else   // use std::list here

    typedef std::list<event> Events;
    typedef std::list<event>::iterator iterator;
    typedef std::list<event>::const_iterator const_iterator;

#endif  // USE_EVENT_MAP

private:

    /**
     *  This list holds the current pattern/sequence events.
     */

    Events m_events;

public:

    event_list ();
    event_list (const event_list & a_rhs);
    event_list & operator = (const event_list & a_rhs);
    ~event_list ();

    /**
     * \getter m_events.begin(), non-constant version.
     */

    iterator begin ()
    {
        return m_events.begin();
    }

    /**
     * \getter m_events.begin(), constant version.
     */

    const_iterator begin () const
    {
        return m_events.begin();
    }

    /**
     * \getter m_events.end(), non-constant version.
     */

    iterator end ()
    {
        return m_events.end();
    }

    /**
     * \getter m_events.end(), constant version.
     */

    const_iterator end () const
    {
        return m_events.end();
    }

    /**
     *  Returns the number of events stored in m_events.  We like returning
     *  an integer instead of size_t, and rename the function so nobody is
     *  fooled.
     */

    int count () const
    {
        return int(m_events.size());
    }

    void add (const event & e, bool postsort = true);

#ifndef USE_EVENT_MAP

    /**
     *  Needed as a special case when std::list is used.
     */

    void push_back(event & e)
    {
        m_events.push_back(e);
    }

#endif

    /**
     *  Provides a wrapper for the iterator form of erase(), which is the
     *  only one that sequence uses.
     */

    void remove (iterator ie)
    {
        m_events.erase(ie);
    }

    /**
     *  Provides a wrapper for clear().
     */

    void clear ()
    {
        m_events.clear();
    }

    void merge (event_list & el, bool presort = true);

    /**
     *  Wrapper for std::list::sort(), or, since multimaps are always sorted,
     *  an empty function.
     */

#ifdef USE_EVENT_MAP
    void sort ()
    {
        // Empty body
    }
#else
    void sort ()
    {
        m_events.sort();
    }
#endif

    /**
     *  Dereference access for list or map.
     */

    static event & dref (iterator ie)
    {
#ifdef USE_EVENT_MAP
        return ie->second;
#else
        return *ie;
#endif
    }

private:                                // functions for friend sequence

    /*
     * The following functions provide internal for-loops that do not
     * involved data from the caller.
     */

    void link_new ();
    void clear_links ();
    void verify_and_link (long slength);
    void mark_selected ();
    void mark_out_of_range (long slength);
    void unmark_all ();
    void unpaint_all ();
    int count_selected_notes ();
    int count_selected_events (unsigned char status, unsigned char cc);
    void select_all ();
    void unselect_all ();
    void print ();

    /**
     * \getter m_events
     */

    const Events & events () const
    {
        return m_events;
    }

};

}           // namespace seq64

#endif      // SEQ64_EVENT_LIST_HPP

/*
 * event_list.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
