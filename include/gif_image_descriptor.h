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
#ifndef GIF_IMAGE_DESCRIPTOR_H
#define GIF_IMAGE_DESCRIPTOR_H

#include "quicky_exception.h"
#include <sstream>
#include <fstream>
#include <cinttypes>

namespace lib_gif
{
  class gif_image_descriptor
  {
  public:
    constexpr static inline uint8_t get_image_separator(void);
    constexpr static inline size_t get_size(void);
    inline const uint16_t & get_image_left_position(void)const;
    inline const uint16_t & get_image_top_position(void)const;
    inline const uint16_t & get_image_width(void)const;
    inline const uint16_t & get_image_height(void)const;
    inline const uint8_t & get_packed_fields(void)const;
    inline void check(void);
    inline bool get_local_color_table_flag(void)const;
    inline bool get_interlace_flag(void)const;
    inline bool get_sort_flag(void)const;
    inline unsigned int get_size_of_local_color_table(void)const;
    inline unsigned int get_decoded_size_of_local_color_table(void)const;
  private:
    uint16_t m_image_left_position;
    uint16_t m_image_top_position;
    uint16_t m_image_width;
    uint16_t m_image_height;
    uint8_t m_packed_fields;
  };

  //----------------------------------------------------------------------------
  constexpr uint8_t gif_image_descriptor::get_image_separator(void)
  {
    return 0x2C;
  }
  //----------------------------------------------------------------------------
  constexpr size_t gif_image_descriptor::get_size(void)
  {
    return sizeof(m_image_left_position) + sizeof(m_image_top_position) + sizeof(m_image_width) + sizeof(m_image_height) + sizeof(m_packed_fields);
  }

  //----------------------------------------------------------------------------
  const uint16_t & gif_image_descriptor::get_image_left_position(void)const
    {
      return m_image_left_position;
    }

  //----------------------------------------------------------------------------
  const uint16_t & gif_image_descriptor::get_image_top_position(void)const
    {
      return m_image_top_position;
    }

  //----------------------------------------------------------------------------
  const uint16_t & gif_image_descriptor::get_image_width(void)const
    {
      return m_image_width;
    }

  //----------------------------------------------------------------------------
  const uint16_t & gif_image_descriptor::get_image_height(void)const
    {
      return m_image_height;
    }

  //----------------------------------------------------------------------------
  const uint8_t & gif_image_descriptor::get_packed_fields(void)const
    {
      return m_packed_fields;
    }

  //----------------------------------------------------------------------------
  void gif_image_descriptor::check(void)
  {
    uint8_t l_reserved_bits = (m_packed_fields >> 3 ) & 0x3;
    if(l_reserved_bits)
      {
        std::stringstream l_stream;
        l_stream << std::hex << "0x" << l_reserved_bits ;
        throw quicky_exception::quicky_logic_exception("Image descriptor packet fields reserved bits should be set to zero : "+l_stream.str(),__LINE__,__FILE__);
      }
  }


  //----------------------------------------------------------------------------
  bool gif_image_descriptor::get_local_color_table_flag(void)const
  {
    return m_packed_fields & 0x80;
  }

   //----------------------------------------------------------------------------
 bool gif_image_descriptor::get_interlace_flag(void)const
 {
   return m_packed_fields & 0x40;
 }

   //----------------------------------------------------------------------------
 bool gif_image_descriptor::get_sort_flag(void)const
 {
   return m_packed_fields & 0x20;
 }
  
 //----------------------------------------------------------------------------
 unsigned int gif_image_descriptor::get_size_of_local_color_table(void)const
 {
   return m_packed_fields & 0x7;
 }
 //----------------------------------------------------------------------------
 unsigned int gif_image_descriptor::get_decoded_size_of_local_color_table(void)const
 {
   return get_local_color_table_flag() ? 2 << get_size_of_local_color_table() : 0;
 }

}
#endif
//EOF
