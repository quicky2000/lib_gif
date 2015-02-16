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
#ifndef GIF_LZW_ENCODER_H
#define GIF_LZW_ENCODER_H

#include "gif_lzw_base.h"
#include <iostream>
#include <map>

namespace lib_gif
{
  template<typename T>
    class gif_lzw_encoder: public gif_lzw_base<T>
    {
    public:
      inline gif_lzw_encoder(const unsigned int & p_minimum_code_size);
      inline bool encode(const T & p_value_to_code,unsigned int & p_coded_value,unsigned int & p_coded_value_size,unsigned int & p_current_code_size,bool & p_clean);
      inline void encode(unsigned int & p_coded_value);
    private:
    };

  //----------------------------------------------------------------------------
  template<typename T>
  gif_lzw_encoder<T>::gif_lzw_encoder(const unsigned int & p_minimum_code_size):
    gif_lzw_base<T>(p_minimum_code_size)
    {
    }

    //----------------------------------------------------------------------------
  template<typename T>
    bool gif_lzw_encoder<T>::encode(const T & p_value_to_code,unsigned int & p_coded_value,unsigned int & p_coded_value_size,unsigned int & p_current_code_size, bool & p_clean)
    {
      bool l_result = false;
      gif_lzw_dictionnary_entry<T> l_new_entry(gif_lzw_base<T>::get_word());
      l_new_entry = l_new_entry + p_value_to_code;
      if(gif_lzw_base<T>::get_dictionnary().contains(l_new_entry))
        {
          gif_lzw_base<T>::set_word(l_new_entry);
          l_result = false;
        }
      else
        {
          unsigned int l_word_Code_size = p_current_code_size;
          p_coded_value_size = l_word_Code_size;
          gif_lzw_base<T>::get_dictionnary().add(l_new_entry,p_current_code_size);
          p_coded_value = gif_lzw_base<T>::get_dictionnary().get_code(gif_lzw_base<T>::get_word());
#ifdef DEBUG_GIF_LZW_ENCODER
	  std::cout << "Proposed coded value " << p_coded_value << "\t" << p_current_code_size << "\t" << l_word_Code_size<< std::endl ;
#endif //DEBUG_GIF_LZW_ENCODER
          gif_lzw_base<T>::set_word(p_value_to_code);
          l_result = true;
        }
      if(4097 == gif_lzw_base<T>::get_dictionnary().get_nb_entry())
	{
	  gif_lzw_base<T>::get_dictionnary().clear();
	  p_clean = true;
	}
      return l_result;
    }

    //----------------------------------------------------------------------------
  template<typename T>
    void gif_lzw_encoder<T>::encode(unsigned int & p_coded_value)
    {
      p_coded_value = gif_lzw_base<T>::get_dictionnary().get_code(gif_lzw_base<T>::get_word());
    }

#if 0
      lib_gif::gif_lzw_encoder<uint8_t> l_encoder(8);
      std::string l_text_to_encode = "TOBEORNOTTOBEORTOBEORNOT";
      unsigned int l_current_code_size = 9;
      unsigned int l_coded_value;
      for(auto l_iter : l_text_to_encode)
        {
          if(l_encoder.encode(l_iter,l_coded_value,l_current_code_size))
            {
              std::cout << "Coded value : " ;
              if(l_coded_value < 256)
                {
                  std::cout << "'" << (uint8_t) l_coded_value << "'" ;
                }
              else
                {
                  std::cout << l_coded_value ;
                }
              std::cout << " " << std::endl ;
            }
        }
      l_encoder.encode(l_coded_value);
      std::cout << "Coded value : " ;
      if(l_coded_value < 256)
	{
	  std::cout << "'" << (uint8_t) l_coded_value << "'" ;
	}
      else
	{
	  std::cout << l_coded_value ;
	}
      std::cout << std::endl ;
#endif

}
#endif
//EOF
