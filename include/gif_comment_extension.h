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
#ifndef GIF_COMMENT_EXTENSION
#define GIF_COMMENT_EXTENSION

#include "gif_extension_block.h"
#include "quicky_exception.h"
#include <fstream>
#include <vector>

namespace lib_gif
{
  class gif_comment_extension: public gif_extension_block
  {
  public:
    inline gif_comment_extension(std::ifstream & p_file);
  private:
  };
  //----------------------------------------------------------------------------
  gif_comment_extension::gif_comment_extension(std::ifstream & p_file):
    gif_extension_block(t_gif_data_block_type::COMMENT_EXTENSION)
    {
      std::cout << "----------------------------" << std::endl ;
      std::cout << "GIF comment extension :" << std::endl ;
      std::cout << "----------------------------" << std::endl ;
      std::cout << "Current position : 0x" << std::hex << p_file.tellg() << std::dec << std::endl ;
      bool l_continu = true;
      std::cout << "###" << std::endl;
      do
	{
	  gif_data_sub_block l_data_sub_block(p_file);
	  for(unsigned int l_index = 0 ; l_index < l_data_sub_block.get_size() ; ++l_index)
	    {
	      std::cout << l_data_sub_block.get_data(l_index);
	    }
	  l_continu = l_data_sub_block.get_size();
	} while(l_continu);
      std::cout << "###" << std::endl ;
    }

}

#endif 
//EOF
