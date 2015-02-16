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
#ifndef GIF_EXTENSION_BLOCK_FACTORY
#define GIF_EXTENSION_BLOCK_FACTORY

#include "gif_application_extension.h"
#include "gif_comment_extension.h"
#include "gif_graphic_control_extension.h"
#include "gif_plain_text_extension.h"

namespace lib_gif
{
  class gif_extension_block_factory
  {
  public:
    static inline gif_extension_block & extract_block(std::ifstream & p_file);
  private:
  };

  //----------------------------------------------------------------------------
  gif_extension_block & gif_extension_block_factory::extract_block(std::ifstream & p_file)
    {
      gif_extension_block * l_result = NULL;
      gif_extension_block::t_gif_extension_block_key l_block_type_identifier;
      p_file.read((char*)&l_block_type_identifier,sizeof(gif_extension_block::t_gif_extension_block_key));
#ifdef DEBUG_GIF_EXTENSION_BLOCK_FACTORY
      std::cout << "Extension Block key : 0x" << std::hex << (unsigned int) l_block_type_identifier << std::dec << std::endl ;
#endif // DEBUG_GIF_EXTENSION_BLOCK_FACTORY
      switch(l_block_type_identifier)
        {
        case gif_extension_block::t_gif_extension_block_key::PLAIN_TEXT_EXTENSION:
          l_result = new gif_plain_text_extension(p_file);
          break;
	case gif_extension_block::t_gif_extension_block_key::GRAPHIC_CONTROL_EXTENSION :
	  l_result = new gif_graphic_control_extension(p_file);
	  break;
	case gif_extension_block::t_gif_extension_block_key::COMMENT_EXTENSION :
	  l_result = new gif_comment_extension(p_file);
	  break;
	case gif_extension_block::t_gif_extension_block_key::APPLICATION_EXTENSION :
	  l_result = new gif_application_extension(p_file);
	  break;
	default:
          throw quicky_exception::quicky_logic_exception("Unsupportedextension  block key identifier "+gif_extension_block::key_to_string(l_block_type_identifier),__LINE__,__FILE__);
        }


      return *l_result;
    }
}

#endif 
//EOF
