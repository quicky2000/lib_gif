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
#ifndef GIF_PLAIN_TEXT_EXTENSION
#define GIF_PLAIN_TEXT_EXTENSION

#include "gif_extension_block.h"
#include <fstream>

namespace lib_gif
{
    class gif_plain_text_extension: public gif_extension_block
    {

      public:

        inline
        gif_plain_text_extension(std::ifstream & p_file);

        inline
        const uint16_t & get_grid_left_position() const;

        inline
        const uint16_t & get_grid_top_position() const;

        inline
        const uint16_t & get_grid_width() const;

        inline
        const uint16_t & get_grid_height() const;

        inline
        const uint8_t & get_character_cell_width() const;

        inline
        const uint8_t & get_character_cell_height() const;

        inline
        const uint8_t & get_foreground_color_index() const;

        inline
        const uint8_t & get_background_color_index() const;

        inline
        void print(std::ostream & p_stream) const override;

        inline
        const std::string & get_content() const;

        inline
        void write_extension(std::ostream & p_stream) const override;

      private:

        uint8_t m_block_size;

        uint16_t m_grid_left_position;

        uint16_t m_grid_top_position;

        uint16_t m_grid_width;

        uint16_t m_grid_height;

        uint8_t m_character_cell_width;

        uint8_t m_character_cell_height;

        uint8_t m_foreground_color_index;

        uint8_t m_background_color_index;

        std::string m_content;
    };

    //----------------------------------------------------------------------------
    void gif_plain_text_extension::print(std::ostream & p_stream) const
    {
        p_stream << "----------------------------" << std::endl ;
        p_stream << "Plain text extension :" << std::endl ;
        p_stream << "----------------------------" << std::endl ;
        p_stream << "Grid left position : " << m_grid_left_position << std::endl ;
        p_stream << "Grid top position : " << m_grid_top_position << std::endl ;
        p_stream << "Grid width : " << m_grid_width << std::endl ;
        p_stream << "Grid height : " << m_grid_height << std::endl ;
        p_stream << "Character cell width : " << (unsigned int) m_character_cell_width << std::endl ;
        p_stream << "Character cell height : " << (unsigned int) m_character_cell_height << std::endl ;
        p_stream << "Foreground color index : " << (unsigned int) m_foreground_color_index << std::endl ;
        p_stream << "Background color index : " << (unsigned int) m_background_color_index << std::endl ;
        p_stream << "###" << std::endl ;
        for(unsigned int l_y = 0 ; l_y < m_grid_height ; ++l_y)
        {
            for(unsigned int l_x = 0 ; l_x < m_grid_width;++l_x)
            {
                unsigned int l_index = l_y * m_grid_width + l_x ;
                p_stream << (l_index < m_content.size() ? m_content[l_index] : ' ' );
            }
            p_stream << std::endl ;
        }
        p_stream << "###" << std::endl ;
    }

    //----------------------------------------------------------------------------
    gif_plain_text_extension::gif_plain_text_extension(std::ifstream & p_file)
    : gif_extension_block(t_gif_data_block_type::PLAIN_TEXT_EXTENSION)
    {
#ifdef DEBUG_GIF_PLAIN_TEXT_EXTENSION
        std::cout << "----------------------------" << std::endl ;
      std::cout << "Plain text extension :" << std::endl ;
      std::cout << "----------------------------" << std::endl ;
      std::cout << "Current position : 0x" << std::hex << p_file.tellg() << std::dec << std::endl ;
#endif //DEBUG_GIF_PLAIN_TEXT_EXTENSION
        p_file.read((char*)&m_block_size,13);
        if(12 != m_block_size)
        {
            std::stringstream l_size_stream;
            l_size_stream << m_block_size;
            throw quicky_exception::quicky_logic_exception("Bad graphical extension block size ("+l_size_stream.str()+") should be 12",__LINE__,__FILE__);
        }
#ifdef DEBUG_GIF_PLAIN_TEXT_EXTENSION
        std::cout << "Grid left position : " << m_grid_left_position << std::endl ;
        std::cout << "Grid top position : " << m_grid_top_position << std::endl ;
        std::cout << "Grid width : " << m_grid_width << std::endl ;
        std::cout << "Grid height : " << m_grid_height << std::endl ;
        std::cout << "Character cell width : " << (unsigned int) m_character_cell_width << std::endl ;
        std::cout << "Character cell height : " << (unsigned int) m_character_cell_height << std::endl ;
        std::cout << "Foreground color index : " << (unsigned int) m_foreground_color_index << std::endl ;
        std::cout << "Background color index : " << (unsigned int) m_background_color_index << std::endl ;
#endif //DEBUG_GIF_PLAIN_TEXT_EXTENSION
        bool l_continu = true;
        do
        {
            gif_data_sub_block l_data_sub_block(p_file);
            for(unsigned int l_index = 0 ; l_index < l_data_sub_block.get_size() ; ++l_index)
            {
                m_content += l_data_sub_block.get_data(l_index);
            }
            l_continu = l_data_sub_block.get_size();
        }
        while(l_continu);

        if(m_content.size() && '\0' != m_content[m_content.size()-1])
        {
            m_content += '\0';
        }
    }
    
    //----------------------------------------------------------------------------
    void gif_plain_text_extension::write_extension(std::ostream & p_stream) const
    {
        uint8_t l_extension_label = 0x1;
        p_stream.write((char*)&l_extension_label, sizeof(l_extension_label));
        p_stream.write((char*)&m_block_size, sizeof(m_block_size));
        p_stream.write((char*)&m_grid_left_position, sizeof(m_grid_left_position));
        p_stream.write((char*)&m_grid_top_position, sizeof(m_grid_top_position));
        p_stream.write((char*)&m_grid_width, sizeof(m_grid_width));
        p_stream.write((char*)&m_grid_height, sizeof(m_grid_height));
        p_stream.write((char*)&m_character_cell_width, sizeof(m_character_cell_width));
        p_stream.write((char*)&m_character_cell_height, sizeof(m_character_cell_height));
        p_stream.write((char*)&m_foreground_color_index, sizeof(m_foreground_color_index));
        p_stream.write((char*)&m_background_color_index, sizeof(m_background_color_index));
        unsigned int l_nb_data_sub_block = m_content.size() / 255;
        for(unsigned int l_index = 0 ; l_index < l_nb_data_sub_block ; ++l_index)
        {
            gif_data_sub_block l_block(255);
            for(unsigned int l_index2 = 0 ;l_index2 < 255 ; ++l_index2)
            {
                l_block.set_data(l_index2,m_content[255 * l_index + l_index2]);
            }
            l_block.write(p_stream);
        }
        unsigned int l_remaining_size = m_content.size() % 255;
        if(l_remaining_size)
        {
            gif_data_sub_block l_block(l_remaining_size);
            for(unsigned int l_index = 0 ; l_index < l_remaining_size ; ++l_index)
            {
                l_block.set_data(l_index,m_content[255 * l_nb_data_sub_block + l_index]);
            }
            l_block.write(p_stream);
        }
    }

    //--------------------------------------------------------------------------
    const uint16_t & gif_plain_text_extension::get_grid_left_position() const
    {
        return m_grid_left_position;
    }

    //--------------------------------------------------------------------------
    const uint16_t & gif_plain_text_extension::get_grid_top_position() const
    {
        return m_grid_top_position;
    }

    //--------------------------------------------------------------------------
    const uint16_t & gif_plain_text_extension::get_grid_width() const
    {
        return m_grid_width;
    }

    //--------------------------------------------------------------------------
    const uint16_t & gif_plain_text_extension::get_grid_height() const
    {
        return m_grid_height;
    }

    //--------------------------------------------------------------------------
    const uint8_t & gif_plain_text_extension::get_character_cell_width() const
    {
        return m_character_cell_width;
    }

    //--------------------------------------------------------------------------
    const uint8_t & gif_plain_text_extension::get_character_cell_height() const
    {
        return m_character_cell_height;
    }

    //--------------------------------------------------------------------------
    const uint8_t & gif_plain_text_extension::get_foreground_color_index() const
    {
        return m_foreground_color_index;
    }

    //--------------------------------------------------------------------------
    const uint8_t & gif_plain_text_extension::get_background_color_index() const
    {
        return m_background_color_index;
    }

    //--------------------------------------------------------------------------
    const std::string & gif_plain_text_extension::get_content() const
    {
        return m_content;
    }

}

#endif 
//EOF
