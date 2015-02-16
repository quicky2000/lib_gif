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
#ifndef GIF_IMAGE_H
#define GIF_IMAGE_H

#include "gif_image_descriptor.h"
#include "gif_graphic_rendering_block.h"
#include "gif_color_table.h"
#include "gif_data_sub_block.h"
#include "gif_lzw_decoder.h"
#include "quicky_bitfield.h"
#include <iostream>
#include <vector>
#include <sstream>

namespace lib_gif
{

  class gif_image: public gif_graphic_rendering_block
  {
  public:
    inline gif_image(std::ifstream & p_file);
    inline ~gif_image(void);
    inline const uint16_t & get_image_left_position(void)const;
    inline const uint16_t & get_image_top_position(void)const;
    inline const uint16_t & get_image_width(void)const;
    inline const uint16_t & get_image_height(void)const;
    inline bool get_local_color_table_flag(void)const;
    inline bool get_interlace_flag(void)const;
    inline const gif_color_table & get_local_color_table(void)const;
    inline bool is_image(void)const;
    inline const unsigned int & get_color_index(const unsigned int & p_x, const unsigned int & p_y)const;
    inline void print(std::ostream & p_stream)const;
    inline unsigned int deinterlace(const unsigned int & p_y)const;
  private:
    gif_image_descriptor m_descriptor;
    gif_color_table * m_color_table;
    uint8_t m_lzw_minimum_code_size;
    typedef unsigned int t_content;
    t_content * m_content;
  };
 
  //----------------------------------------------------------------------------
  gif_image::gif_image(std::ifstream & p_file):
    m_color_table(nullptr),
    m_lzw_minimum_code_size(0),
    m_content(nullptr)
      {
	p_file.read((char*) & m_descriptor,m_descriptor.get_size());
	m_descriptor.check();
	if(m_descriptor.get_local_color_table_flag())
	  {
	    m_color_table = new gif_color_table(m_descriptor.get_decoded_size_of_local_color_table(),p_file);
	  }
	p_file.read((char*) &m_lzw_minimum_code_size,sizeof(uint8_t));
        uint8_t * l_compressed_data = NULL;
        size_t l_compressed_data_size = 0;
        bool l_continu = false;
        unsigned int l_nb_block = 0;
	do
	  {
	    gif_data_sub_block l_data_sub_block(p_file);
            l_continu = l_data_sub_block.get_size();
            unsigned int l_new_data_size = l_compressed_data_size + l_data_sub_block.get_size();
            l_compressed_data = (uint8_t*)realloc(l_compressed_data,l_new_data_size);
            l_data_sub_block.copy(&(l_compressed_data[l_compressed_data_size]));
            l_compressed_data_size = l_new_data_size;
            ++l_nb_block;
            
	  } while(l_continu);
#ifdef DEBUG_GIF_IMAGE
	std::cout << "Number of Image Data sub blocks : " << l_nb_block << std::endl ;
	std::cout << "Number of Data bytes in sub blocks : " << l_compressed_data_size << std::endl ;
#endif // DEBUG_GIF_IMAGE
        m_content = new t_content[m_descriptor.get_image_width() * m_descriptor.get_image_height()];
        quicky_utils::quicky_bitfield l_bitfield(l_compressed_data_size*8);
        unsigned int l_first_code = 0;
        for(unsigned int l_index = 0 ; l_index < l_compressed_data_size ; ++l_index)
          {
            l_bitfield.set(l_compressed_data[l_index],8,8 * (l_compressed_data_size - l_index - 1));
          }
        free(l_compressed_data);

        l_bitfield.get(l_first_code,(unsigned int)(m_lzw_minimum_code_size + 1),(unsigned int)(l_compressed_data_size * 8 - m_lzw_minimum_code_size - 1));
	

#ifdef DEBUG_GIF_IMAGE
        std::cout << "First code = 0x" << std::hex << l_first_code << std::dec << std::endl ;
#endif // DEBUG_GIF_IMAGE
        gif_lzw_decoder<t_content> l_decoder(m_lzw_minimum_code_size);

        if(l_first_code != l_decoder.get_clear_code()) 
          {
            std::stringstream l_clear_code_stream;
            l_clear_code_stream << "0x" << std::hex << l_decoder.get_clear_code();
            std::stringstream l_first_code_stream;
            l_first_code_stream << "0x" << std::hex << l_first_code;
            throw quicky_exception::quicky_logic_exception("First LZW code ("+l_first_code_stream.str()+") should be clear code ("+l_clear_code_stream.str()+")",__LINE__,__FILE__);
          }

        unsigned int l_current_lzw_minimum_code_size = m_lzw_minimum_code_size;
        unsigned int l_extracted_value = 0x0;
        unsigned int l_remaining_size = l_compressed_data_size * 8 - l_current_lzw_minimum_code_size - 1;
        unsigned int l_content_index = 0;


        l_bitfield.get(l_extracted_value,l_current_lzw_minimum_code_size + 1,l_remaining_size - l_current_lzw_minimum_code_size - 1);
        l_remaining_size -= l_current_lzw_minimum_code_size + 1;
	do
	  {
	    gif_lzw_dictionnary_entry<t_content> l_decoded_value = l_decoder.decode(l_extracted_value,l_current_lzw_minimum_code_size);
#ifdef DEBUG_GIF_IMAGE
	    std::cout << std::hex << " Decoded value 0x" << (unsigned int)l_extracted_value << std::dec << "\tWidth : " << l_current_lzw_minimum_code_size + 1 << std::endl ;
#endif // DEBUG_GIF_IMAGE
	    // Output entry
	    for(unsigned int l_index = 0 ; l_index < l_decoded_value.size() ; ++l_index)
	      {
		m_content[l_content_index + l_index] = l_decoded_value[l_index];
	      }
	    l_content_index += l_decoded_value.size();

            if(l_remaining_size >= l_current_lzw_minimum_code_size + 1)
              {
                l_bitfield.get(l_extracted_value,l_current_lzw_minimum_code_size + 1,l_remaining_size - l_current_lzw_minimum_code_size - 1);
                l_remaining_size -= l_current_lzw_minimum_code_size + 1;
              }
            else
              {
                std::cout << "Warning : Missing end of information code in Image data : 0x" << std::hex << l_extracted_value << std::dec << std::endl ;
                break;
              }
          }
	while(l_decoder.get_end_information_code() != l_extracted_value);
      }

    //----------------------------------------------------------------------------
    gif_image::~gif_image(void)
      {
        delete[] m_content;
	delete m_color_table;
      }

    //--------------------------------------------------------------------------
    const uint16_t & gif_image::get_image_left_position(void)const
      {
        return m_descriptor.get_image_left_position();
      }

    //--------------------------------------------------------------------------
    const uint16_t & gif_image::get_image_top_position(void)const
      {
        return m_descriptor.get_image_top_position();
      }

    //--------------------------------------------------------------------------
    const uint16_t & gif_image::get_image_width(void)const
      {
        return m_descriptor.get_image_width();
      }

    //--------------------------------------------------------------------------
    const uint16_t & gif_image::get_image_height(void)const
      {
        return m_descriptor.get_image_height();
      }
    //--------------------------------------------------------------------------
    bool gif_image::is_image(void)const
    {
      return true;
    }

    //--------------------------------------------------------------------------
    bool gif_image::get_local_color_table_flag(void)const
    {
      return m_descriptor.get_local_color_table_flag();
    }
    //--------------------------------------------------------------------------
    bool gif_image::get_interlace_flag(void)const
    {
      return m_descriptor.get_interlace_flag();
    }

    //--------------------------------------------------------------------------
    const gif_color_table & gif_image::get_local_color_table(void)const
      {
	if(m_color_table) return *m_color_table;
	throw quicky_exception::quicky_logic_exception("Try to access to non existing local color table",__LINE__,__FILE__);
      }

    //--------------------------------------------------------------------------
    const unsigned int & gif_image::get_color_index(const unsigned int & p_x, const unsigned int & p_y)const
      {
        if(p_x < get_image_width() && p_y < get_image_height())
          {
            return m_content[p_x + p_y * get_image_width()];
          }
        std::stringstream l_x_stream;
        l_x_stream << p_x;
        std::stringstream l_y_stream;
        l_y_stream << p_y;
        std::stringstream l_width_stream;
        l_width_stream << get_image_width();
        std::stringstream l_height_stream;
        l_height_stream << get_image_height();        
        throw quicky_exception::quicky_logic_exception("Requested coordinates ("+l_x_stream.str()+","+l_y_stream.str()+" are outside of picture dimensions ("+l_width_stream.str()+","+l_height_stream.str()+")",__LINE__,__FILE__);
      }

    //--------------------------------------------------------------------------
    unsigned int gif_image::deinterlace(const unsigned int & p_y)const
    {
      if(! (p_y % 8))
        {
          return p_y / 8;
        }
      if(! ((p_y - 4) % 8))
        {
          return ( p_y - 4) / 8 + m_descriptor.get_image_height() / 8; 
        }
      if(! ((p_y - 2) % 4))
        {
          return (p_y - 2) / 4 + m_descriptor.get_image_height() / 4;
        }
      if(! ((p_y - 1) % 2))
        {
          return (p_y - 1) / 2 + m_descriptor.get_image_height() / 2;
        }
      
      std::stringstream l_y_stream;
      l_y_stream << p_y;
      throw quicky_exception::quicky_logic_exception("Unknown y "+l_y_stream.str(),__LINE__,__FILE__);
    }

    //----------------------------------------------------------------------------
    void gif_image::print(std::ostream & p_stream)const
    {
      p_stream << "----------------------------" << std::endl ;
      p_stream << "GIF Image :" << std::endl ;
      p_stream << "----------------------------" << std::endl ;
      p_stream << "Image left position : " << m_descriptor.get_image_left_position() << std::endl ;
      p_stream << "Image top position : " << m_descriptor.get_image_top_position() << std::endl ;
      p_stream << "Image width : " << m_descriptor.get_image_width() << std::endl ;
      p_stream << "Image height : " << m_descriptor.get_image_height() << std::endl ;
      p_stream << "Packed fields : " << std::hex << "0x" << (unsigned int)m_descriptor.get_packed_fields() << std::dec << std::endl ;
      p_stream << "\tLocal Color Table Flag : " << m_descriptor.get_local_color_table_flag() << std::endl ;
      p_stream << "\tInterlace Flag         : " << m_descriptor.get_interlace_flag() << std::endl;
      p_stream << "\tSorted Flag            : " << m_descriptor.get_sort_flag() << std::endl ;
      p_stream << "\tSize of local table    : " << m_descriptor.get_decoded_size_of_local_color_table() << " entries" << std::endl ;
      if(m_descriptor.get_local_color_table_flag())
	{
	  p_stream << * m_color_table;
	}
      p_stream << "----------------------------" << std::endl ;
      p_stream << "GIF Image Data :" << std::endl ;
      p_stream << "----------------------------" << std::endl ;
      p_stream << "LZW minimum code size : " << (unsigned int) m_lzw_minimum_code_size << std::endl ;
    }

}
#endif
//EOF
