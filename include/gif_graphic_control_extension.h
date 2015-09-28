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
#ifndef GIF_GRAPHIC_CONTROL_EXTENSION
#define GIF_GRAPHIC_CONTROL_EXTENSION

#include "gif_extension_block.h"
#include <fstream>

namespace lib_gif
{
  class gif_graphic_control_extension: public gif_extension_block
  {
  public:
    inline gif_graphic_control_extension(const unsigned int & p_delay_time_ms,
					 bool p_transparent_color,
					 const unsigned int & p_transparent_color_index=0);
    inline gif_graphic_control_extension(std::ifstream & p_file);
    inline const uint16_t & get_delay_time(void)const;
    inline const uint8_t & get_transparent_color_index(void)const;
    inline const unsigned int get_disposal_method(void)const;
    inline bool get_user_input_flag(void)const;
    inline void print(std::ostream & p_stream)const;
    inline bool get_transparent_color_flag(void)const;
    inline void set_transparent_color_flag(bool p_flag);
    inline void write_extension(std::ofstream & p_file)const;
  private:					
    inline const unsigned int get_reserved_field(void)const;
    uint8_t m_block_size;
    uint8_t m_packed_fields;
    uint16_t m_delay_time;
    uint8_t m_transparent_color_index;
    uint8_t m_block_terminator;
  };

  //----------------------------------------------------------------------------
  gif_graphic_control_extension::gif_graphic_control_extension(const unsigned int & p_delay_time_ms,
							       bool p_transparent_color,
							       const unsigned int & p_transparent_color_index):
    gif_extension_block(t_gif_data_block_type::GRAPHICAL_CONTROL_EXTENSION),
    m_block_size(4),
    m_packed_fields(p_transparent_color ? 0x1 : 0x0),
    m_delay_time(p_delay_time_ms / 10),
    m_transparent_color_index(p_transparent_color_index)
    {
    }

  //----------------------------------------------------------------------------
  gif_graphic_control_extension::gif_graphic_control_extension(std::ifstream & p_file):
    gif_extension_block(t_gif_data_block_type::GRAPHICAL_CONTROL_EXTENSION)
    {
      p_file.read((char*)&m_block_size,6);
      if(4 != m_block_size) 
	{
	  std::stringstream l_size_stream;
	  l_size_stream << m_block_size;
	  throw quicky_exception::quicky_logic_exception("Bad graphical extension block size ("+l_size_stream.str()+") should be 4",__LINE__,__FILE__);
	}
      if(get_reserved_field())
	{
	  std::stringstream l_reserved_stream;
	  l_reserved_stream << "0x" << std::hex << get_reserved_field();
	  throw quicky_exception::quicky_logic_exception("Bad graphical extension packed reserved field value ("+l_reserved_stream.str()+") should be 0",__LINE__,__FILE__);
	}
      if(0 != m_block_terminator)
	{
	  std::stringstream l_block_terminator_stream;
	  l_block_terminator_stream << m_block_terminator;
	  throw quicky_exception::quicky_logic_exception("Bad graphical extension block_terminator value ("+l_block_terminator_stream.str()+") should be 0",__LINE__,__FILE__);
	}
      
    }

    //----------------------------------------------------------------------------
    void gif_graphic_control_extension::write_extension(std::ofstream & p_file)const
    {
      uint8_t l_extension_label = 0xF9;
      p_file.write((char*)&l_extension_label,sizeof(l_extension_label));
      p_file.write((char*)&m_block_size,sizeof(m_block_size));
      p_file.write((char*)&m_packed_fields,sizeof(m_packed_fields));
      p_file.write((char*)&m_delay_time,sizeof(m_delay_time));
      p_file.write((char*)&m_transparent_color_index,sizeof(m_transparent_color_index));
   }

    //----------------------------------------------------------------
    const unsigned int gif_graphic_control_extension::get_reserved_field(void)const
    {
      return (m_packed_fields >> 5 ) & 0x7;
    }
    
    //----------------------------------------------------------------
    void gif_graphic_control_extension::print(std::ostream & p_stream)const
    {
      p_stream << "----------------------------" << std::endl ;
      p_stream << "Graphical Control extension :" << std::endl ;
      p_stream << "----------------------------" << std::endl ;
      p_stream << "Packed fields : " << std::hex << "0x" << (unsigned int)m_packed_fields << std::dec << std::endl ;
      p_stream << "\tDisposal Method : " << get_disposal_method() << std::endl ;
      p_stream << "\tUser input flag : " << get_user_input_flag() << std::endl ;
      p_stream << "\tTransparent color flag : " << get_transparent_color_flag() << std::endl ;
      p_stream << "Delay time : " << m_delay_time << " ( * 1/100 s)" << std::endl;
      p_stream << "Transparent Color index : " << (unsigned int) m_transparent_color_index << std::endl;
    }

    //----------------------------------------------------------------------------
    const uint16_t & gif_graphic_control_extension::get_delay_time(void)const
      {
	return m_delay_time;
      }

    //----------------------------------------------------------------------------
    const uint8_t & gif_graphic_control_extension::get_transparent_color_index(void)const
      {
	return m_transparent_color_index;
      }

    //----------------------------------------------------------------------------
    const unsigned int gif_graphic_control_extension::get_disposal_method(void)const
    {
      return (m_packed_fields >> 2) & 0x7;
    }
    
  //----------------------------------------------------------------------------
    bool gif_graphic_control_extension::get_user_input_flag(void)const
    {
      return (m_packed_fields >> 1 ) & 0x1;
    }

    //----------------------------------------------------------------------------
    bool gif_graphic_control_extension::get_transparent_color_flag(void)const
    {
      return m_packed_fields & 0x1;
    }

    //----------------------------------------------------------------------------
    void gif_graphic_control_extension::set_transparent_color_flag(bool p_flag)
    {
      m_packed_fields |= p_flag;
    }

}

#endif 
//EOF
