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
  gif::gif(const std::string & p_file_name)
  {
    std::ifstream l_file;
    l_file.open(p_file_name.c_str(),std::ifstream::binary);
    if(!l_file) throw quicky_exception::quicky_runtime_exception("Unable to read file \""+p_file_name+"\"",__LINE__,__FILE__);

    uint8_t l_signature[4];
    l_file.read((char*)&l_signature,3*sizeof(uint8_t));
    l_signature[3] = '\0';
    uint8_t l_version[4];
    l_file.read((char*)&l_version,3*sizeof(uint8_t));
    l_version[3] = '\0';
    std::cout << "----------------------------" << std::endl ;
    std::cout << "GIF Header :" << std::endl ;
    std::cout << "----------------------------" << std::endl ;
    std::cout << "Signature : " << l_signature << std::endl ;
    std::cout << "Version : " << l_version << std::endl ;
    m_header.set_signature((char*)l_signature);
    m_header.set_version((char*)l_version);

    if("89a" != m_header.get_version())
      {
	throw quicky_exception::quicky_logic_exception("Only 89a GIF are supported : "+m_header.get_version(),__LINE__,__FILE__);
      }

    //    l_file.read((char*) & m_logical_screen,sizeof(m_logical_screen));
    m_logical_screen.read(l_file);

    gif_data_block * l_last_block = nullptr;
    do
      {
        l_last_block = & gif_data_block_factory::extract_block(l_file);
        m_data_blocks.push_back(l_last_block);
      } while(gif_data_block::t_gif_data_block_type::TRAILER !=l_last_block->get_type());
    std::cout << "Number of data blocks : " << m_data_blocks.size() << std::endl ;
    l_file.close();
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
