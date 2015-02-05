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
#ifndef GIF_COLOR_TABLE_H
#define GIF_COLOR_TABLE_H

#include "gif_color.h"
#include "quicky_exception.h"
#include <fstream>

namespace lib_gif
{
  class gif_color_table
  {
  public:
    inline gif_color_table(const unsigned int & p_size,std::ifstream & p_file);
    inline ~gif_color_table(void);
    inline void display(void);
    inline const gif_color & operator[](const size_t & p_index)const;
  private:
    unsigned int m_size;
    gif_color *m_colors;
  };

  //----------------------------------------------------------------------------
  gif_color_table::~gif_color_table(void)
    {
      delete[] m_colors;
    }

  //----------------------------------------------------------------------------
  gif_color_table::gif_color_table(const unsigned int & p_size,std::ifstream & p_file):
    m_size(p_size),
    m_colors(new gif_color[p_size])
    {
      if(!p_size)
	{
	  throw quicky_exception::quicky_logic_exception("Trying to create an empty color table",__LINE__,__FILE__);
	}
      p_file.read((char*) m_colors,p_size * sizeof(gif_color));      
    }

  //----------------------------------------------------------------------------
    const gif_color & gif_color_table::operator[](const size_t & p_index)const
      {
        if(p_index < m_size) return m_colors[p_index];
        std::stringstream l_index_stream;
        l_index_stream << p_index;
        std::stringstream l_size_stream;
        l_size_stream << m_size;
        throw quicky_exception::quicky_logic_exception("Requested index "+l_index_stream.str()+" is greater than table size "+l_size_stream.str(),__LINE__,__FILE__);
      }

    //----------------------------------------------------------------------------
    void gif_color_table::display(void)
    {
      for(unsigned int l_index = 0 ; l_index < m_size; ++l_index)
	{ 
	  std::cout << "\tColor[" << l_index << " | 0x" << std::hex << l_index << std::dec << "]  = " ;
	  m_colors[l_index].display();
	  std::cout << std::endl;
	}
    }
}

#endif
//EOF
