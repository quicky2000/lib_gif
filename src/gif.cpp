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

#include "gif.h"
#include "gif_image_descriptor.h"
#include "gif_data_block_factory.h"
#include "gif_data_block.h"
#include <fstream>
#include <cinttypes>
#include <iostream>

namespace lib_gif
{
  //----------------------------------------------------------------------------
  gif::gif(std::ifstream & p_file)
  {
    p_file.read((char*)&m_header,m_header.get_size());
     m_logical_screen.read(p_file);
    gif_data_block * l_last_block = nullptr;
    do
      {
        l_last_block = & gif_data_block_factory::extract_block(p_file);
        m_data_blocks.push_back(l_last_block);
      } while(gif_data_block::t_gif_data_block_type::TRAILER != l_last_block->get_type());
  }

  //----------------------------------------------------------------------------
  gif::gif(const unsigned int & p_width,
	   const unsigned  int & p_height):
    m_logical_screen(p_width,p_height)
  {
  }

  //----------------------------------------------------------------------------
  void gif::write(std::ofstream & p_file)
  {
    if(!m_data_blocks.size() || gif_data_block::t_gif_data_block_type::TRAILER != m_data_blocks.back()->get_type())
      {
        m_data_blocks.push_back(new gif_trailer());
      }
    p_file.write((char*)&m_header,m_header.get_size());
    m_logical_screen.write(p_file);
    for(auto l_iter : m_data_blocks)
      {
        l_iter->write(p_file);
      }
  }
  //----------------------------------------------------------------------------
  gif::~gif(void)
    {
      for(auto l_iter: m_data_blocks)
	{
	  delete l_iter;
	}
    }
  //----------------------------------------------------------------------------
  std::ostream & operator<<(std::ostream & p_stream,const gif & p_gif)
  {
    p_stream << p_gif.m_header;
    p_stream << p_gif.m_logical_screen;
    for(auto l_iter : p_gif.m_data_blocks)
      {
        p_stream << *l_iter;
      }
    p_stream << "Number of data blocks : " << p_gif.m_data_blocks.size() << std::endl ;
    return p_stream;
  }

  //----------------------------------------------------------------------------
  void gif::add_image(gif_image & p_image)
  {
    m_data_blocks.push_back(new lib_gif::gif_graphic_block(p_image));
  }

  //----------------------------------------------------------------------------
  void gif::add_comment(const std::string & p_comment)
  {
    m_data_blocks.push_back(new lib_gif::gif_comment_extension(p_comment));
  }

  //----------------------------------------------------------------------------
  void gif::activate_loop(const unsigned int & p_loop_counter)
  {
    m_data_blocks.push_back(new lib_gif::gif_application_extension(p_loop_counter));
  }

  //----------------------------------------------------------------------------
  void gif::add_graphic_control_extension(const unsigned int & p_delay_time,
					  bool p_transparent_color,
					  const unsigned int & p_transparent_color_index)
  {
    m_data_blocks.push_back(new lib_gif::gif_graphic_control_extension(p_delay_time,p_transparent_color,p_transparent_color_index));
  }

}
