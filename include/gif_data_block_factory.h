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
#ifndef GIF_DATA_BLOCK_FACTORY
#define GIF_DATA_BLOCK_FACTORY

#include "gif_graphic_block.h"
#include "gif_extension_block_factory.h"
#include "gif_trailer.h"

namespace lib_gif
{
  class gif_data_block_factory
  {
  public:
    static inline gif_data_block & extract_block(std::ifstream & p_file);
  private:
  };

  //----------------------------------------------------------------------------
  gif_data_block & gif_data_block_factory::extract_block(std::ifstream & p_file)
    {
      gif_data_block * l_result = NULL;
      gif_data_block::t_gif_data_block_key l_block_type_identifier;
      p_file.read((char*)&l_block_type_identifier,sizeof(gif_data_block::t_gif_data_block_key));

#ifdef DEBUG_GIF_DATA_BLOCK_FACTORY
      std::cout << "Block key : 0x" << std::hex << (unsigned int) l_block_type_identifier << " @ 0x" << p_file.tellg() << std::dec << std::endl ;
#endif // DEBUG_GIF_DATA_BLOCK_FACTORY

      switch(l_block_type_identifier)
        {
        case gif_data_block::t_gif_data_block_key::IMAGE :
	  l_result = new gif_graphic_block((gif_data_block::t_gif_data_block_key)l_block_type_identifier,p_file);
          break;
	case gif_data_block::t_gif_data_block_key::EXTENSION :
	  l_result = & gif_extension_block_factory::extract_block(p_file);
	  break;
	case gif_data_block::t_gif_data_block_key::TRAILER :
	  l_result = new gif_trailer();
	  break;
	default:
          throw quicky_exception::quicky_logic_exception("Unsupported block key identifier "+gif_data_block::key_to_string(l_block_type_identifier),__LINE__,__FILE__);
        }


      return *l_result;
    }
}

#endif 
//EOF
