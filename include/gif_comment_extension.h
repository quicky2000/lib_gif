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
    inline void print(std::ostream & p_stream)const;
    inline gif_comment_extension(std::ifstream & p_file);
    inline void write_extension(std::ofstream & p_file)const;
  private:
    std::string m_comment;
  };

    //----------------------------------------------------------------------------
    void gif_comment_extension::write_extension(std::ofstream & p_file)const
    {
      uint8_t l_extension_label = 0xFE;
      p_file.write((char*)&l_extension_label,sizeof(l_extension_label));
      unsigned int l_nb_data_sub_block = m_comment.size() / 255;
      for(unsigned int l_index = 0 ; l_index < l_nb_data_sub_block ; ++l_index)
        {
          gif_data_sub_block l_block(255);
          for(unsigned int l_index2 = 0 ;l_index2 < 255 ; ++l_index2)
            {
              l_block.set_data(l_index2,m_comment[255 * l_index + l_index2]);
            }
          l_block.write(p_file);
        }
      unsigned int l_remaining_size = m_comment.size() % 255;
      if(l_remaining_size)
        {
          gif_data_sub_block l_block(l_remaining_size);
          for(unsigned int l_index = 0 ; l_index < l_remaining_size ; ++l_index)
            {
              l_block.set_data(l_index,m_comment[255 * l_nb_data_sub_block + l_index]);
            }
          l_block.write(p_file);
        }
    }

  //----------------------------------------------------------------------------
  gif_comment_extension::gif_comment_extension(std::ifstream & p_file):
    gif_extension_block(t_gif_data_block_type::COMMENT_EXTENSION)
    {
      bool l_continu = true;
      do
	{
	  gif_data_sub_block l_data_sub_block(p_file);
	  for(unsigned int l_index = 0 ; l_index < l_data_sub_block.get_size() ; ++l_index)
	    {
	      m_comment+= l_data_sub_block.get_data(l_index);
	    }
	  l_continu = l_data_sub_block.get_size();
	} while(l_continu);
    }

    //----------------------------------------------------------------------------
    void gif_comment_extension::print(std::ostream & p_stream)const
    {
      p_stream << "----------------------------" << std::endl ;
      p_stream << "GIF comment extension :" << std::endl ;
      p_stream << "----------------------------" << std::endl ;
      p_stream << "###" << std::endl;
      std::string l_comment = m_comment;
      if(m_comment.size() && '\0' != m_comment[m_comment.size()-1])
        {
          l_comment += '\0';
        }
      p_stream << l_comment << std::endl;
      p_stream << "###" << std::endl ;
    }
}

#endif 
//EOF
