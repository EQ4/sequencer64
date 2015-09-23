#ifndef SEQ64_OPTIONS_SETTINGS_HPP
#define SEQ64_OPTIONS_SETTINGS_HPP

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
 * \file          options_settings.h
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
 *  This collection of variables describes the options of the application,
 *  accessible from the command-line or from the "rc" file.
 */

#include <string>

// #include "easy_macros.h"               // with platform_macros.h, too

/**
 *  This class contains the options formerly named "global_xxxxxx".
 */

class options_settings
{

private:

    bool m_legacy_format;               /* new 2015-08-16 */
    bool m_lash_support;                /* new 2015-08-27 */
    bool m_show_midi;
    bool m_priority;
    bool m_stats;
    bool m_pass_sysex;
    bool m_with_jack_transport;
    bool m_with_jack_master;
    bool m_with_jack_master_cond;
    bool m_jack_start_mode;
    bool m_manual_alsa_ports;
    bool m_is_pattern_playing;
    bool m_print_keys;
    bool m_device_ignore;               /* seq24 module  */
    int m_device_ignore_num;            /* seq24 module  */
    std::string m_filename;
    std::string m_jack_session_uuid;
    std::string m_last_used_dir;
    std::string m_config_directory;
    std::string m_config_filename;
    std::string m_user_filename;
    std::string m_config_filename_alt;
    std::string m_user_filename_alt;

public:

    options_settings ();
    options_settings (const options_settings & rhs);
    options_settings & operator = (const options_settings & rhs);

    /**
     * \accessor m_legacy_format
     */

    bool legacy_format () const
    {
        return m_legacy_format;
    }

    void legacy_format (bool flag)
    {
        m_legacy_format = flag;
    }

    /**
     * \accessor m_lash_support
     */

    bool lash_support () const
    {
        return m_lash_support;
    }

    void lash_support (bool flag)
    {
        m_lash_support = flag;
    }

    /**
     * \accessor m_show_midi
     */

    bool show_midi () const
    {
        return m_show_midi;
    }

    void show_midi (bool flag)
    {
        m_show_midi = flag;
    }

    /**
     * \accessor m_priority
     */

    bool priority () const
    {
        return m_priority;
    }

    void priority (bool flag)
    {
        m_priority = flag;
    }

    /**
     * \accessor m_stats
     */

    bool stats () const
    {
        return m_stats;
    }

    void stats (bool flag)
    {
        m_stats = flag;
    }

    /**
     * \accessor m_pass_sysex
     */

    bool pass_sysex () const
    {
        return m_pass_sysex;
    }

    void pass_sysex (bool flag)
    {
        m_pass_sysex = flag;
    }

    /**
     * \accessor m_with_jack_transport
     */

    bool with_jack_transport () const
    {
        return m_with_jack_transport;
    }

    void with_jack_transport (bool flag)
    {
        m_with_jack_transport = flag;
    }

    /**
     * \accessor m_with_jack_master
     */

    bool with_jack_master () const
    {
        return m_with_jack_master;
    }

    void with_jack_master (bool flag)
    {
        m_with_jack_master = flag;
    }

    /**
     * \accessor m_with_jack_master_cond
     */

    bool with_jack_master_cond () const
    {
        return m_with_jack_master_cond;
    }

    void with_jack_master_cond (bool flag)
    {
        m_with_jack_master_cond = flag;
    }

    /**
     * \accessor m_jack_start_mode
     */

    bool jack_start_mode () const
    {
        return m_jack_start_mode;
    }

    void jack_start_mode (bool flag)
    {
        m_jack_start_mode = flag;
    }

    /**
     * \accessor m_manual_alsa_ports
     */

    bool manual_alsa_ports () const
    {
        return m_manual_alsa_ports;
    }

    void manual_alsa_ports (bool flag)
    {
        m_manual_alsa_ports = flag;
    }

    /**
     * \accessor m_is_pattern_playing
     */

    bool is_pattern_playing () const
    {
        return m_is_pattern_playing;
    }

    void is_pattern_playing (bool flag)
    {
        m_is_pattern_playing = flag;
    }

    /**
     * \accessor m_print_keys
     */

    bool print_keys () const
    {
        return m_print_keys;
    }

    void print_keys (bool flag)
    {
        m_print_keys = flag;
    }

    /**
     * \accessor m_device_ignore
     */

    bool device_ignore () const
    {
        return m_device_ignore;
    }

    void device_ignore (bool flag)
    {
        m_device_ignore = flag;
    }

    /**
     * \accessor m_device_ignore_num
     */

    int device_ignore_num () const
    {
        return m_device_ignore_num;
    }

    void device_ignore_num (int value)
    {
        m_device_ignore_num = value;
    }

    /**
     * \accessor m_filename
     */

    const std::string & filename () const
    {
        return filename;
    }

    void filename (const std::string & value)
    {
        filename = value;
    }

    /**
     * \accessor m_jack_session_uuid
     */

    const std::string & jack_session_uuid () const
    {
        return jack_session_uuid;
    }

    void jack_session_uuid (const std::string & value)
    {
        jack_session_uuid = value;
    }

    /**
     * \accessor m_last_used_dir
     */

    const std::string & last_used_dir () const
    {
        return last_used_dir;
    }

    void last_used_dir (const std::string & value)
    {
        last_used_dir = value;
    }

    /**
     * \accessor m_config_directory
     */

    const std::string & config_directory () const
    {
        return config_directory;
    }

    void config_directory (const std::string & value)
    {
        config_directory = value;
    }

    /**
     * \accessor m_config_filename
     */

    const std::string & config_filename () const
    {
        return config_filename;
    }

    void config_filename (const std::string & value)
    {
        config_filename = value;
    }

    /**
     * \accessor m_user_filename
     */

    const std::string & user_filename () const
    {
        return user_filename;
    }

    void user_filename (const std::string & value)
    {
        user_filename = value;
    }

    /**
     * \accessor m_config_filename_alt;
     */

    const std::string & config_filename_alt; () const
    {
        return config_filename_alt;;
    }

    void config_filename_alt; (const std::string & value)
    {
        config_filename_alt; = value;
    }
    std::string m_user_filename_alt;

    /**
     * \accessor m_user_filename_alt
     */

    const std::string & user_filename_alt () const
    {
        return user_filename_alt;
    }

    void user_filename_alt (const std::string & value)
    {
        user_filename_alt = value;
    }

};

#endif  // SEQ64_OPTIONS_SETTINGS_HPP

/*
 * options_settings.h
 *
 * vim: sw=4 ts=4 wm=8 et ft=cpp
 */