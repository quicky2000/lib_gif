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
#ifndef GIF_GRAPHIC_RENDERING_BLOCK_FACTORY
#define GIF_GRAPHIC_RENDERING_BLOCK_FACTORY

#include "gif_graphic_rendering_block.h"
#include "gif_image.h"

namespace lib_gif
{
  class gif_graphic_rendering_block_factory
  {
  public:
    static inline gif_graphic_rendering_block & extract_block(const gif_data_block::t_gif_data_block_key & p_key, std::ifstream & p_file);
  private:
  };

  //----------------------------------------------------------------------------
  gif_graphic_rendering_block & gif_graphic_rendering_block_factory::extract_block(const gif_data_block::t_gif_data_block_key & p_key, std::ifstream & p_file)
    {
      gif_graphic_rendering_block * l_result = NULL;
      switch(p_key)
        {
        case gif_data_block::t_gif_data_block_key::IMAGE :
	  l_result = new gif_image(p_file);
          break;
        default:
          std::stringstream l_stream;
          l_stream << "0x" << std::hex << (unsigned int) p_key ;
          throw quicky_exception::quicky_logic_exception("Unsupported Graphic rendering block type identifier "+l_stream.str(),__LINE__,__FILE__);
        }


      return *l_result;
    }
}

#endif 
//EOF
