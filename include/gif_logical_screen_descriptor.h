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
#ifndef GIF_LOGICAL_SCREEN_DESCRIPTOR_H
#define GIF_LOGICAL_SCREEN_DESCRIPTOR_H

#include <fstream>
#include <cinttypes>
#include <cassert>
#include <iostream>
#include <cmath>

namespace lib_gif
{
    class gif_logical_screen_descriptor
    {

      public:

        inline
        gif_logical_screen_descriptor();

        inline
        gif_logical_screen_descriptor( const uint16_t & p_width
                                     , const uint16_t & p_height
                                     );

        inline
        const uint16_t & get_width() const;

        inline
        const uint16_t & get_height() const;

        inline
        const uint8_t  & get_packed_fields() const;

        inline
        const uint8_t  & get_background_index() const;

        inline
        const uint8_t  & get_pixel_aspect_ratio() const;

        inline
        float get_decoded_pixel_aspect_ratio() const;

        // Access to members of packed fields
        inline
        bool get_global_color_table_flag() const;

        inline
        void set_global_color_table_flag(bool);

        inline
        unsigned int get_global_color_resolution() const;

        inline
        unsigned int get_decoded_global_color_resolution() const;

        inline
        bool get_sort_flag() const;

        inline
        unsigned int get_size_of_global_color_table() const;

        inline
        unsigned int get_decoded_size_of_global_color_table() const;

        inline
        void set_size_of_global_color_table(const unsigned int & p_size);

        inline
        void set_decoded_size_of_global_color_table(const unsigned int & p_size);

        static inline
        size_t get_size();

      private:

        uint16_t m_width;

        uint16_t m_height;

        uint8_t m_packed_fields;

        uint8_t m_background_index;

        uint8_t m_pixel_aspect_ratio;
    };

    //----------------------------------------------------------------------------
    inline
    std::ostream & operator<<( std::ostream & p_stream
                             , const gif_logical_screen_descriptor & p_descriptor
                             )
    {
        p_stream << "Width : " << p_descriptor.get_width() << " pixels" << std::endl ;
        p_stream << "Height : " << p_descriptor.get_height() << " pixels" << std::endl ;
        p_stream << "Packed fields : " << std::hex << "0x" << (unsigned int)p_descriptor.get_packed_fields() << std::dec << std::endl ;
        p_stream << "\tGlobal Color Table Flag : " << p_descriptor.get_global_color_table_flag() << std::endl ;
        p_stream << "\tGlobal Color Resolution " << p_descriptor.get_decoded_global_color_resolution() << " bits per channel" << std::endl;
        p_stream << "\tSorted Flag " << p_descriptor.get_sort_flag() << std::endl ;
        p_stream << "\tSize of global table : " << p_descriptor.get_decoded_size_of_global_color_table() << " entries" << std::endl ;
        p_stream << "Background Index : " << (unsigned int)p_descriptor.get_background_index() << std::endl ;
        p_stream << "Pixel aspect Ratio : " << (unsigned int)p_descriptor.get_decoded_pixel_aspect_ratio() << std::endl ;
        return p_stream;
    }

    //----------------------------------------------------------------------------
    size_t gif_logical_screen_descriptor::get_size()
    {
        return sizeof(m_width) + sizeof(m_height) + sizeof(m_packed_fields) + sizeof(m_background_index) + sizeof(m_pixel_aspect_ratio);
    }

    //----------------------------------------------------------------------------
    gif_logical_screen_descriptor::gif_logical_screen_descriptor()
    : m_width(0)
    , m_height(0)
    , m_packed_fields(0)
    , m_background_index(0)
    , m_pixel_aspect_ratio(0)
    {
    }

    //----------------------------------------------------------------------------
    gif_logical_screen_descriptor::gif_logical_screen_descriptor( const uint16_t & p_width
                                                                , const uint16_t & p_height
                                                                )
    : m_width(p_width)
    , m_height(p_height)
    , m_packed_fields(0)
    , m_background_index(0)
    , m_pixel_aspect_ratio(0)
    {
        set_decoded_size_of_global_color_table(256);
    }

    //----------------------------------------------------------------------------
    const uint16_t & gif_logical_screen_descriptor::get_width() const
    {
        return m_width;
    }

    //----------------------------------------------------------------------------
    const uint16_t & gif_logical_screen_descriptor::get_height() const
    {
        return m_height;
    }

    //----------------------------------------------------------------------------
    const uint8_t & gif_logical_screen_descriptor::get_packed_fields() const
    {
        return m_packed_fields;
    }

    //----------------------------------------------------------------------------
    const uint8_t  & gif_logical_screen_descriptor::get_background_index() const
    {
        return m_background_index;
    }

    //----------------------------------------------------------------------------
    const uint8_t  & gif_logical_screen_descriptor::get_pixel_aspect_ratio() const
    {
        return m_pixel_aspect_ratio;
    }
  
    //----------------------------------------------------------------------------
    float gif_logical_screen_descriptor::get_decoded_pixel_aspect_ratio() const
    {
        return m_pixel_aspect_ratio ? (((float)m_pixel_aspect_ratio) + 15.0f) / 64.0f : 0.0f;
    }
  
    //----------------------------------------------------------------------------
    bool gif_logical_screen_descriptor::get_global_color_table_flag() const
    {
        return m_packed_fields & 0x80u;
    }

    //----------------------------------------------------------------------------
    void gif_logical_screen_descriptor::set_global_color_table_flag(bool p_flag)
    {
        if(p_flag)
        {
            m_packed_fields |= 0x80u;
        }
        else
        {
            m_packed_fields &= ~(0x80u);
        }
    }

    //----------------------------------------------------------------------------
    unsigned int gif_logical_screen_descriptor::get_decoded_global_color_resolution() const
    {
        return 1 + ((((unsigned int)m_packed_fields) & 0x70u) >> 4u);
    }

    //----------------------------------------------------------------------------
    bool gif_logical_screen_descriptor::get_sort_flag() const
    {
        return m_packed_fields & 0x8u;
    }

    //----------------------------------------------------------------------------
    unsigned int gif_logical_screen_descriptor::get_decoded_size_of_global_color_table() const
    {
        return get_global_color_table_flag() ? 2u << get_size_of_global_color_table() : 0;
    }

    //----------------------------------------------------------------------------
    unsigned int gif_logical_screen_descriptor::get_global_color_resolution() const
    {
        return (((unsigned int)m_packed_fields) & 0x70u) >> 4u;
    }

    //----------------------------------------------------------------------------
    unsigned int gif_logical_screen_descriptor::get_size_of_global_color_table() const
    {
        return m_packed_fields & 0x7u;
    }

    //----------------------------------------------------------------------------
    void gif_logical_screen_descriptor::set_size_of_global_color_table(const unsigned int & p_size)
    {
        uint8_t l_size = p_size & 0x7u;
        assert(((unsigned int)l_size) == p_size);
        m_packed_fields |= l_size;
    }

    //----------------------------------------------------------------------------
    void gif_logical_screen_descriptor::set_decoded_size_of_global_color_table(const unsigned int & p_size)
    {
        set_global_color_table_flag(p_size);
        if(p_size)
        {
            set_size_of_global_color_table((unsigned int)(log(p_size) /log(2)) - 1);
        }
        else
        {
            set_size_of_global_color_table(0);
        }
    }
}
#endif
//EOF
