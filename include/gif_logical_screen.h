/*    This file is part of lib_gif
      Copyright (C) 2015  Julien Thevenon ( julien_thevenon at yahoo.fr )

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef GIF_LOGICAL_SCREEN_H
#define GIF_LOGICAL_SCREEN_H

#include "gif_logical_screen_descriptor.h"
#include "gif_color_table.h"
#include <fstream>
#include <iostream>

namespace lib_gif
{

    class gif_logical_screen
    {
        friend std::ostream & operator<<(std::ostream & p_stream,const gif_logical_screen & p_screen);

      public:

        inline
        gif_logical_screen();

        inline
        gif_logical_screen( const uint16_t & p_width
                          , const uint16_t & p_height
                          );

        inline
        void read(std::ifstream & p_file);

        inline
        void write(std::ostream & p_stream);

        inline
        const uint16_t & get_width() const;

        inline
        const uint16_t & get_height() const;

        inline
        bool get_global_color_table_flag() const;

        inline
        const gif_color_table & get_global_color_table() const;

        inline
        void set_color_in_global_color_table( const size_t & p_index
                                            , const gif_color & p_color
                                            );

        inline
        const uint8_t  & get_background_index() const;

        inline
        ~gif_logical_screen();

      private:

        gif_logical_screen_descriptor m_descriptor;

        gif_color_table  *m_global_color_table;
    };

    //----------------------------------------------------------------------------
    gif_logical_screen::gif_logical_screen()
    : m_global_color_table(nullptr)
    {
    }

    //----------------------------------------------------------------------------
    gif_logical_screen::gif_logical_screen( const uint16_t & p_width
                                          , const uint16_t & p_height
                                          )
    : m_descriptor(p_width, p_height)
    , m_global_color_table(m_descriptor.get_global_color_table_flag() ? new gif_color_table(m_descriptor.get_decoded_size_of_global_color_table()) : nullptr)
    {
    }

    //----------------------------------------------------------------------------
    gif_logical_screen::~gif_logical_screen()
    {
        delete m_global_color_table;
    }

    //----------------------------------------------------------------------------
    bool gif_logical_screen::get_global_color_table_flag() const
    {
        return m_descriptor.get_global_color_table_flag();
    }

    //----------------------------------------------------------------------------
    const gif_color_table & gif_logical_screen::get_global_color_table() const
    {
        if(m_global_color_table) return *m_global_color_table;
        throw quicky_exception::quicky_logic_exception("Try to access to non existing global color table",__LINE__,__FILE__);
    }

    //----------------------------------------------------------------------------
    void gif_logical_screen::set_color_in_global_color_table( const size_t & p_index
                                                            , const gif_color & p_color
                                                            )
    {
        if(m_global_color_table)
        {
            m_global_color_table->set_color(p_index,p_color);
        }
        else
        {
            throw quicky_exception::quicky_logic_exception("Try to access to non existing global color table", __LINE__, __FILE__);
        }
    }

    //----------------------------------------------------------------------------
    const uint16_t & gif_logical_screen::get_width() const
    {
        return m_descriptor.get_width();
    }

    //----------------------------------------------------------------------------
    const uint16_t & gif_logical_screen::get_height() const
    {
        return m_descriptor.get_height();
    }

    //----------------------------------------------------------------------------
    const uint8_t  & gif_logical_screen::get_background_index() const
    {
        return m_descriptor.get_background_index();
    }
  
    //----------------------------------------------------------------------------
    void gif_logical_screen::write(std::ostream & p_stream)
    {
        p_stream.write((char*) & m_descriptor, m_descriptor.get_size());

        if(m_descriptor.get_global_color_table_flag())
        {
            m_global_color_table->write(p_stream);
        }
    }

    //----------------------------------------------------------------------------
    void gif_logical_screen::read(std::ifstream & p_file)
    {
        p_file.read((char*) & m_descriptor,m_descriptor.get_size());
        if((p_file.rdstate() & std::ifstream::failbit) || (p_file.rdstate() & std::ifstream::eofbit))
        {
            throw quicky_exception::quicky_logic_exception("Incomplete logical screen descriptor", __LINE__, __FILE__);
        }
        if(m_descriptor.get_global_color_table_flag())
        {
            m_global_color_table = new gif_color_table(m_descriptor.get_decoded_size_of_global_color_table(),p_file);
        }
    }

    //----------------------------------------------------------------------------
    inline
    std::ostream & operator<<( std::ostream & p_stream
                             , const gif_logical_screen & p_screen
                             )
    {
        p_stream << "----------------------------" << std::endl ;
        p_stream << "GIF Logical screen :" << std::endl ;
        p_stream << "----------------------------" << std::endl ;
        p_stream << p_screen.m_descriptor;
        if(p_screen.m_descriptor.get_global_color_table_flag())
        {
            assert(p_screen.m_global_color_table);
            p_stream << p_screen.get_global_color_table();
        }
        return p_stream;
    }

}
#endif
//EOF
