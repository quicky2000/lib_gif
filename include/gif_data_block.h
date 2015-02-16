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
#ifndef GIF_DATA_BLOCK
#define GIF_DATA_BLOCK

namespace lib_gif
{

  /**
     Extensions are treated all treated as data blocks including Graphical 
     Control extension due to some GIF files like the one found on Wikipedia
     that do not respect GIF grammar spec by having a comment extension between
     Grahpical Control Extension and Image extension
  */
  class gif_data_block
  {
  public:
    typedef enum class gif_data_block_key : uint8_t {
	EXTENSION=0x21,
        IMAGE=0x2C,
        TRAILER=0x3B
        }
    t_gif_data_block_key;

    typedef enum class gif_data_block_type {
      GRAPHIC_BLOCK=0,
        GRAPHICAL_CONTROL_EXTENSION,
	APPLICATION_EXTENSION,
	COMMENT_EXTENSION,
	PLAIN_TEXT_EXTENSION,
	TRAILER
        }
    t_gif_data_block_type;
    static inline const std::string type_to_string(const t_gif_data_block_type & p_type);
    static inline const std::string key_to_string(const t_gif_data_block_key & p_key);
    inline const t_gif_data_block_type & get_type(void)const;
    virtual void write(std::ofstream & p_file)const=0;
    inline virtual ~gif_data_block(void){}
    virtual void print(std::ostream & p_stream)const=0;
  protected:
    inline gif_data_block(const t_gif_data_block_type & p_type);
  private:
    t_gif_data_block_type m_type;
  };
  //----------------------------------------------------------------------------
  inline std::ostream & operator<<(std::ostream & p_stream,const gif_data_block & p_data_block)
    {
      p_data_block.print(p_stream);
      return p_stream;
    }

  //----------------------------------------------------------------------------
  const std::string gif_data_block::type_to_string(const t_gif_data_block_type & p_type)
    {
      switch(p_type)
        {
        case t_gif_data_block_type::GRAPHIC_BLOCK:
          return "GRAPHIC_BLOCK";
          break;
        case t_gif_data_block_type::GRAPHICAL_CONTROL_EXTENSION:
          return "GRAPHICAL_CONTROL_EXTENSION";
          break;
        case t_gif_data_block_type::APPLICATION_EXTENSION:
          return "APPLICATION_EXTENSION";
          break;
        case t_gif_data_block_type::COMMENT_EXTENSION:
          return "COMMENT_EXTENSION";
          break;
        case t_gif_data_block_type::PLAIN_TEXT_EXTENSION:
          return "PLAIN_TEXT_EXTENSION";
          break;
        case t_gif_data_block_type::TRAILER:
          return "TRAILER";
          break;
        default:
          std::stringstream l_type_stream;
          l_type_stream << "0x" << std::hex << (unsigned int)p_type;
          throw quicky_exception::quicky_logic_exception("Unknow data block type "+l_type_stream.str(),__LINE__,__FILE__);
        }
    }

  //----------------------------------------------------------------------------
  const std::string gif_data_block::key_to_string(const t_gif_data_block_key & p_key)
    {
      switch(p_key)
        {
        case t_gif_data_block_key::EXTENSION:
          return "EXTENSION";
          break;
        case t_gif_data_block_key::IMAGE:
          return "IMAGE";
          break;
        case t_gif_data_block_key::TRAILER:
          return "TRAILER";
          break;
        default:
          std::stringstream l_key_stream;
          l_key_stream << "0x" << std::hex << (unsigned int)p_key;
          throw quicky_exception::quicky_logic_exception("Unknow data block key "+l_key_stream.str(),__LINE__,__FILE__);
        }
    }

  //----------------------------------------------------------------------------
  gif_data_block::gif_data_block(const t_gif_data_block_type & p_type):
    m_type(p_type)
    {
    }

    //----------------------------------------------------------------------------
    const gif_data_block::t_gif_data_block_type & gif_data_block::get_type(void)const
      {
        return m_type;
      }
}

#endif 
//EOF
