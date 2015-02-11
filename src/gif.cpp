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
    std::cout << m_header;
    m_logical_screen.read(p_file);

    gif_data_block * l_last_block = nullptr;
    do
      {
        l_last_block = & gif_data_block_factory::extract_block(p_file);
        m_data_blocks.push_back(l_last_block);
      } while(gif_data_block::t_gif_data_block_type::TRAILER != l_last_block->get_type());
    std::cout << "Number of data blocks : " << m_data_blocks.size() << std::endl ;
  }

  //----------------------------------------------------------------------------
  gif::~gif(void)
    {
      for(auto l_iter: m_data_blocks)
	{
	  delete l_iter;
	}
    }

}
