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
#ifndef GIF_GRAPHIC_BLOCK_H
#define GIF_GRAPHIC_BLOCK_H

#include "gif_data_block.h"
#include "gif_graphic_rendering_block_factory.h"
namespace lib_gif
{
  /**
     This class is there because according to GIF syntax described in GIF spec
     there are two types of Graphic Rendering Block ( GIF Image and Plain Text )
     that could be prefixed by a Graphical Control Extension.
     My initial plan was to have it as a member of this class but I encountered
     quite a lot of GIF files having Comment extension between Graphical Control
     Extension and Graphic Rendering Block so I decided to move Graphical Control
     Extension outside of this class and consider it as an independant data block
     For sure with this new implementation gif_graphic_block class could be 
     replaced by gif_grpahic_rendering_block
   */
  class gif_graphic_block: public gif_data_block
  {
  public:
    inline gif_graphic_block(gif_image & p_gif_image);
    inline gif_graphic_block(const gif_data_block::t_gif_data_block_key & p_key, std::ifstream & p_file);
    inline ~gif_graphic_block(void);
    inline const uint16_t & get_left_position(void)const;
    inline const uint16_t & get_top_position(void)const;
    inline const uint16_t & get_width(void)const;
    inline const uint16_t & get_height(void)const;
    inline bool is_image(void)const;
    inline void print(std::ostream & p_stream)const;
    inline const gif_image & get_image(void)const;
    inline void write(std::ofstream & p_file)const;
      private:
    gif_graphic_rendering_block * m_graphic_rendering_block;
  };

  //----------------------------------------------------------------------------
  gif_graphic_block::gif_graphic_block(gif_image & p_gif_image):
    gif_data_block(gif_data_block::t_gif_data_block_type::GRAPHIC_BLOCK),
    m_graphic_rendering_block(&p_gif_image)
  {
  }

  //----------------------------------------------------------------------------
  void gif_graphic_block::print(std::ostream & p_stream)const
  {
    assert(m_graphic_rendering_block);
    m_graphic_rendering_block->print(p_stream);
  }

  //----------------------------------------------------------------------------
  gif_graphic_block::gif_graphic_block(const gif_data_block::t_gif_data_block_key & p_key, std::ifstream & p_file):
    gif_data_block(gif_data_block::t_gif_data_block_type::GRAPHIC_BLOCK),
    m_graphic_rendering_block(nullptr)
      {
        m_graphic_rendering_block = & gif_graphic_rendering_block_factory::extract_block(p_key,p_file);
      }

    //----------------------------------------------------------------------------
    void gif_graphic_block::write(std::ofstream & p_file)const
    {
       assert(m_graphic_rendering_block);
      m_graphic_rendering_block->write(p_file);
    }

    //----------------------------------------------------------------------------
    gif_graphic_block::~gif_graphic_block(void)
      {
	delete m_graphic_rendering_block;
      }

    //----------------------------------------------------------------------------
    bool gif_graphic_block::is_image(void)const
    {
      assert(m_graphic_rendering_block);
      return m_graphic_rendering_block->is_image();
    }
    
    //----------------------------------------------------------------------------
    const uint16_t & gif_graphic_block::get_left_position(void)const
      {
        assert(m_graphic_rendering_block);
        return m_graphic_rendering_block->get_image_left_position();
      }

    //----------------------------------------------------------------------------
    const uint16_t & gif_graphic_block::get_top_position(void)const
      {
        assert(m_graphic_rendering_block);
        return m_graphic_rendering_block->get_image_top_position();
      }

    //----------------------------------------------------------------------------
    const uint16_t & gif_graphic_block::get_width(void)const
      {
        assert(m_graphic_rendering_block);
        return m_graphic_rendering_block->get_image_width();
      }
    //----------------------------------------------------------------------------
    const uint16_t & gif_graphic_block::get_height(void)const
      {
        assert(m_graphic_rendering_block);
        return m_graphic_rendering_block->get_image_height();
      }
    //----------------------------------------------------------------------------
    const gif_image & gif_graphic_block::get_image(void)const
      {
        if(this->is_image()) return * static_cast<gif_image*>(m_graphic_rendering_block);
        throw quicky_exception::quicky_logic_exception("Try to access to graphic_rendernign block as an image wheras it is not",__LINE__,__FILE__);
      }

}
#endif
//EOF
