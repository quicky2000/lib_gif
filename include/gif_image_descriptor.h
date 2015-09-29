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
    inline gif_image_descriptor(const uint16_t & p_image_width,
                                const uint16_t & p_image_height);
    inline gif_image_descriptor(const uint16_t & p_image_width,
                                const uint16_t & p_image_height,
				const uint16_t & p_image_left_position,
				const uint16_t & p_image_top_position,
				bool p_local_color_table_flag=false,
				bool p_interface_flag=false,
				bool p_sorted_flag=false,
				const unsigned int & p_local_table_size=0);
    inline gif_image_descriptor(void);
    inline const uint16_t & get_image_left_position(void)const;
    inline const uint16_t & get_image_top_position(void)const;
    inline const uint16_t & get_image_width(void)const;
    inline const uint16_t & get_image_height(void)const;
    inline const uint8_t & get_packed_fields(void)const;
    inline void check(void);
    inline bool get_local_color_table_flag(void)const;
    inline void set_local_color_table_flag(bool p_flag);
    inline bool get_interlace_flag(void)const;
    inline void set_interlace_flag(bool p_flag);
    inline bool get_sort_flag(void)const;
    inline void set_sort_flag(bool p_flag);
    inline unsigned int get_decoded_size_of_local_color_table(void)const;
    inline void set_decoded_size_of_local_color_table(const unsigned int & p_size);
  private:
    inline unsigned int get_size_of_local_color_table(void)const;
    inline void set_size_of_local_color_table(const unsigned int p_size);

    uint16_t m_image_left_position;
    uint16_t m_image_top_position;
    uint16_t m_image_width;
    uint16_t m_image_height;
    uint8_t m_packed_fields;
  };

  //----------------------------------------------------------------------------
  gif_image_descriptor::gif_image_descriptor(const uint16_t & p_image_width,
                                             const uint16_t & p_image_height):
    m_image_left_position(0),
    m_image_top_position(0),
    m_image_width(p_image_width),
    m_image_height(p_image_height),
    m_packed_fields(0)
    {
    }

  //----------------------------------------------------------------------------
  gif_image_descriptor::gif_image_descriptor(void):
    m_image_left_position(0),
    m_image_top_position(0),
    m_image_width(0),
    m_image_height(0),
    m_packed_fields(0)
    {
    }

  //----------------------------------------------------------------------------
  gif_image_descriptor::gif_image_descriptor(const uint16_t & p_image_width,
					     const uint16_t & p_image_height,
					     const uint16_t & p_image_left_position,
					     const uint16_t & p_image_top_position,
					     bool p_local_color_table_flag,
					     bool p_interface_flag,
					     bool p_sorted_flag,
					     const unsigned int & p_local_table_size):
    m_image_left_position(p_image_left_position),
    m_image_top_position(p_image_top_position),
    m_image_width(p_image_width),
    m_image_height(p_image_height),
    m_packed_fields(0)
  {
    set_local_color_table_flag(p_local_color_table_flag);
    set_interlace_flag(p_interface_flag);
    set_sort_flag(p_sorted_flag);
    set_decoded_size_of_local_color_table(p_local_table_size);
  }

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
  void gif_image_descriptor::set_local_color_table_flag(bool p_flag)
  {
    if(p_flag)
      {
	m_packed_fields |= 0x80;
      }
    else
      {
	m_packed_fields &= ~(0x80);
      }
  }

  //----------------------------------------------------------------------------
  bool gif_image_descriptor::get_interlace_flag(void)const
  {
    return m_packed_fields & 0x40;
  }

  //----------------------------------------------------------------------------
  void gif_image_descriptor::set_interlace_flag(bool p_flag)
  {
    if(p_flag)
      {
	m_packed_fields |= 0x40;
      }
    else
      {
	m_packed_fields &= ~(0x40);
      }
  }

  //----------------------------------------------------------------------------
  bool gif_image_descriptor::get_sort_flag(void)const
  {
    return m_packed_fields & 0x20;
  }
  
  //----------------------------------------------------------------------------
  void gif_image_descriptor::set_sort_flag(bool p_flag)
  {
    if(p_flag)
      {
	m_packed_fields |= 0x20;
      }
    else
      {
	m_packed_fields &= ~(0x20);
      }
  }

  //----------------------------------------------------------------------------
  unsigned int gif_image_descriptor::get_size_of_local_color_table(void)const
  {
    return m_packed_fields & 0x7;
  }

  //----------------------------------------------------------------------------
  void gif_image_descriptor::set_size_of_local_color_table(const unsigned int p_size)
  {
    if(p_size > 7)
      {
	std::stringstream l_size_stream;
	l_size_stream << p_size;
	throw quicky_exception::quicky_logic_exception("Coded local table size is greater than 7 : " + l_size_stream.str(),__LINE__,__FILE__);
      }
    m_packed_fields &= ~(0x7);
    m_packed_fields |= p_size;
  }

  //----------------------------------------------------------------------------
  unsigned int gif_image_descriptor::get_decoded_size_of_local_color_table(void)const
  {
    return get_local_color_table_flag() ? 2 << get_size_of_local_color_table() : 0;
  }

  //----------------------------------------------------------------------------
  void gif_image_descriptor::set_decoded_size_of_local_color_table(const unsigned int & p_size)
  {
    if(p_size > 256)
      {
	std::stringstream l_size_stream;
	l_size_stream << p_size;
	throw quicky_exception::quicky_logic_exception("Decoded local table size is greater than 256 : " + l_size_stream.str(),__LINE__,__FILE__);
      }
    unsigned int l_size = p_size;
    unsigned int l_coded_size = 0;
    while(l_size > 2)
      {
	l_size = l_size >> 1;
	++l_coded_size;
      }
    set_size_of_local_color_table(l_coded_size);
  }

}
#endif
//EOF
