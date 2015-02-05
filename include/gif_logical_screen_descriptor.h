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

namespace lib_gif
{
  class gif_logical_screen_descriptor
  {
  public:
    inline gif_logical_screen_descriptor(void);
    inline const uint16_t & get_width(void)const;
    inline const uint16_t & get_height(void)const;
    inline const uint8_t  & get_packed_fields(void)const;
    inline const uint8_t  & get_background_index(void)const;
    inline const uint8_t  & get_pixel_aspect_ratio(void)const;
    inline const float get_decoded_pixel_aspect_ratio(void)const;
    // Access to members of packed fields
    inline bool get_global_color_table_flag(void)const;
    inline unsigned int get_global_color_resolution(void)const; 
    inline unsigned int get_decoded_global_color_resolution(void)const; 
    inline bool get_sort_flag(void)const;
    inline unsigned int get_size_of_global_color_table(void)const;
    inline unsigned int get_decoded_size_of_global_color_table(void)const;
    static inline size_t get_size(void);
  private:
    uint16_t m_width;
    uint16_t m_height;
    uint8_t m_packed_fields;
    uint8_t m_background_index;
    uint8_t m_pixel_aspect_ratio;
  };

  //----------------------------------------------------------------------------
  size_t gif_logical_screen_descriptor::get_size(void)
  {
    return sizeof(m_width) + sizeof(m_height) + sizeof(m_packed_fields) + sizeof(m_background_index) + sizeof(m_pixel_aspect_ratio);
  }

  //----------------------------------------------------------------------------
  gif_logical_screen_descriptor::gif_logical_screen_descriptor(void):
    m_width(0),
    m_height(0),
    m_packed_fields(0),
    m_background_index(0),
    m_pixel_aspect_ratio(0)
    {
    }

  //----------------------------------------------------------------------------
  const uint16_t & gif_logical_screen_descriptor::get_width(void)const
    {
      return m_width;
    }
  //----------------------------------------------------------------------------
  const uint16_t & gif_logical_screen_descriptor::get_height(void)const
    {
      return m_height;
    }
  //----------------------------------------------------------------------------
  const uint8_t & gif_logical_screen_descriptor::get_packed_fields(void)const
    {
      return m_packed_fields;
    }

  //----------------------------------------------------------------------------
  const uint8_t  & gif_logical_screen_descriptor::get_background_index(void)const
    {
      return m_background_index;
    }

  //----------------------------------------------------------------------------
  const uint8_t  & gif_logical_screen_descriptor::get_pixel_aspect_ratio(void)const
    {
      return m_pixel_aspect_ratio;
    }
  
  //----------------------------------------------------------------------------
  const float gif_logical_screen_descriptor::get_decoded_pixel_aspect_ratio(void)const
    {
      return m_pixel_aspect_ratio ? (((float)m_pixel_aspect_ratio) + 15.0) / 64.0 : 0;
    }
  
  //----------------------------------------------------------------------------
  bool gif_logical_screen_descriptor::get_global_color_table_flag(void)const
  {
    return m_packed_fields & 0x80;
  }
  //----------------------------------------------------------------------------
  unsigned int gif_logical_screen_descriptor::get_decoded_global_color_resolution(void)const
  {
    return 1 + ((m_packed_fields & 0x70) >> 4);
  }
  //----------------------------------------------------------------------------
  bool gif_logical_screen_descriptor::get_sort_flag(void)const
  {
    return m_packed_fields & 0x8;
  }

  //----------------------------------------------------------------------------
  unsigned int gif_logical_screen_descriptor::get_decoded_size_of_global_color_table(void)const
  {
    return get_global_color_table_flag() ? 2 << get_size_of_global_color_table() : 0;
  }

  //----------------------------------------------------------------------------
  unsigned int gif_logical_screen_descriptor::get_global_color_resolution(void)const
  {
    return (m_packed_fields & 0x70) >> 4;
  }

  //----------------------------------------------------------------------------
  unsigned int gif_logical_screen_descriptor::get_size_of_global_color_table(void)const
  {
    return m_packed_fields & 0x7;
  }

}
#endif
//EOF
