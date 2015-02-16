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
#ifndef GIF
#define GIF

#include "gif_header.h"
#include "gif_logical_screen.h"
#include <vector>
#include <sstream>

namespace lib_gif
{
  class gif_data_block;

  class gif
  {
    friend std::ostream & operator<<(std::ostream & p_stream,const gif & p_gif);
  public:
    gif(std::ifstream & p_file);
    ~gif(void);
    inline const uint16_t & get_width(void)const;
    inline const uint16_t & get_height(void)const;
    inline bool get_global_color_table_flag(void)const;
    inline const gif_color_table & get_global_color_table(void)const;
    inline unsigned int get_nb_data_block(void)const;
    inline const gif_data_block & get_data_block(const unsigned int & p_index)const;
    inline const uint8_t  & get_background_index(void)const;
    void write(std::ofstream & p_file);
  private:
    gif_header m_header;
    gif_logical_screen m_logical_screen;
    std::vector<gif_data_block*> m_data_blocks;
  };

  //----------------------------------------------------------------------------
  std::ostream & operator<<(std::ostream & p_stream,const gif & p_gif);

  //----------------------------------------------------------------------------
  const uint16_t & gif::get_width(void)const
    {
      return m_logical_screen.get_width();
    }

  //----------------------------------------------------------------------------
  const uint16_t & gif::get_height(void)const
    {
      return m_logical_screen.get_height();
    }
  //----------------------------------------------------------------------------
  bool gif::get_global_color_table_flag(void)const
  {
    return m_logical_screen.get_global_color_table_flag();
  }
  //----------------------------------------------------------------------------
  const gif_color_table & gif::get_global_color_table(void)const
    {
      return m_logical_screen.get_global_color_table();
    }
  
  //----------------------------------------------------------------------------
  const uint8_t  & gif::get_background_index(void)const
    {
      return m_logical_screen.get_background_index();
    }
  
  //----------------------------------------------------------------------------
  unsigned int gif::get_nb_data_block(void)const
  {
    return m_data_blocks.size();
  }

  //----------------------------------------------------------------------------
  const gif_data_block & gif::get_data_block(const unsigned int & p_index)const
    {
      if(p_index < m_data_blocks.size())
	{
	  return *(m_data_blocks[p_index]);
	}
      std::stringstream l_index_stream;
      l_index_stream << p_index;
      std::stringstream l_size_stream;
      l_index_stream << m_data_blocks.size();
      throw quicky_exception::quicky_logic_exception("Requested index ("+l_index_stream.str()+") is greater than number of data blocks ("+l_size_stream.str(),__LINE__,__FILE__);
    }
}

#endif // GIF
//EOF
