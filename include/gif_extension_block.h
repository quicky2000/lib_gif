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
      PLAIN_TEXT_EXTENSION=0x1,
        GRAPHIC_CONTROL_EXTENSION=0xF9,
        COMMENT_EXTENSION=0xFE,
	APPLICATION_EXTENSION=0xFF
        }
    t_gif_extension_block_key;

    inline gif_extension_block(const t_gif_data_block_type & p_type);
    static inline const std::string key_to_string(const t_gif_extension_block_key & p_key);
    inline void write(std::ofstream & p_file)const;
  private:
    virtual void write_extension(std::ofstream & p_file)const=0;
    
  };

  //----------------------------------------------------------------------------
  void gif_extension_block::write(std::ofstream & p_file)const
  {
    uint8_t l_extension_introducer = 0x21;
    p_file.write((char*)&l_extension_introducer,sizeof(l_extension_introducer));
    this->write_extension(p_file);
    uint8_t l_block_terminator = 0x0;
    p_file.write((char*)&l_block_terminator,sizeof(l_block_terminator));
  }
  
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
        case t_gif_extension_block_key::PLAIN_TEXT_EXTENSION:
          return "PLAIN_TEXT_EXTENSION";
          break;
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
