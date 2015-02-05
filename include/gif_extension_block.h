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
#ifndef GIF_EXTENSION_BLOCK
#define GIF_EXTENSION_BLOCK

#include "gif_data_block.h"

namespace lib_gif
{
  class gif_extension_block: public gif_data_block
  {
  public:
    typedef enum class gif_extension_block_key : uint8_t {
      GRAPHIC_CONTROL_EXTENSION=0xF9,
        COMMENT_EXTENSION=0xFE,
	APPLICATION_EXTENSION=0xFF
        }
    t_gif_extension_block_key;

    inline gif_extension_block(const t_gif_data_block_type & p_type);
    static inline const std::string key_to_string(const t_gif_extension_block_key & p_key);
  private:
    
  };
  
  //----------------------------------------------------------------------------
  gif_extension_block::gif_extension_block(const t_gif_data_block_type & p_type):
    gif_data_block(p_type)
    {
    }

  //----------------------------------------------------------------------------
  const std::string gif_extension_block::key_to_string(const t_gif_extension_block_key & p_key)
    {
      switch(p_key)
        {
        case t_gif_extension_block_key::GRAPHIC_CONTROL_EXTENSION:
          return "GRAPHIC_CONTROL_EXTENSION";
          break;
        case t_gif_extension_block_key::COMMENT_EXTENSION:
          return "COMMENT_EXTENSION";
          break;
        case t_gif_extension_block_key::APPLICATION_EXTENSION :
          return "APPLICATION_EXTENSION";
          break;
        default:
          std::stringstream l_key_stream;
          l_key_stream << "0x" << std::hex << (unsigned int)p_key;
          throw quicky_exception::quicky_logic_exception("Unknow extension block key "+l_key_stream.str(),__LINE__,__FILE__);
        }
    }


}
#endif
//EOF
