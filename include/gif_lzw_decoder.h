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
#ifndef GIF_LZW_DECODER_H
#define GIF_LZW_DECODER_H

#include "gif_lzw_base.h"
      //TO DELETE#include "gif_lzw_dictionnary.h"
#include <iostream>

namespace lib_gif
{
  template<typename T>
    class gif_lzw_decoder: public gif_lzw_base<T>
    {
    public:
      inline gif_lzw_decoder(const unsigned int & p_minimum_code_size);
      inline const gif_lzw_dictionnary_entry<T> & decode(const unsigned int & p_coded_value,unsigned int & p_current_code_size);
    private:
      gif_lzw_dictionnary_entry<T> m_entry;
    };

  //----------------------------------------------------------------------------
  template<typename T>
    gif_lzw_decoder<T>::gif_lzw_decoder(const unsigned int & p_minimum_code_size):
    gif_lzw_base<T>(p_minimum_code_size)
    {
    }
    
    //----------------------------------------------------------------------------
    template<typename T>
      const gif_lzw_dictionnary_entry<T> & gif_lzw_decoder<T>::decode(const unsigned int & p_coded_value,unsigned int & p_current_code_size)
      {

	if(gif_lzw_base<T>::get_word().size())
	  {
	    if(p_coded_value > gif_lzw_base<T>::get_end_information_code())
	      {
		if(gif_lzw_base<T>::get_dictionnary().contains(p_coded_value))
		  {
		    m_entry = gif_lzw_base<T>::get_dictionnary().get(p_coded_value);
		  }
		else
		  {
		    m_entry = gif_lzw_base<T>::get_word();
		    m_entry = m_entry + gif_lzw_base<T>::get_word()[0];
		  }
	      }
	    else if(p_coded_value < gif_lzw_base<T>::get_clear_code())
	      {
		m_entry = gif_lzw_dictionnary_entry<T>(p_coded_value);
	      }
	    else if(p_coded_value == gif_lzw_base<T>::get_clear_code())
	      {
		p_current_code_size = gif_lzw_base<T>::get_minimum_code_size();
		gif_lzw_base<T>::get_dictionnary().clear();
		m_entry = gif_lzw_dictionnary_entry<T>();
		gif_lzw_base<T>::set_word(gif_lzw_dictionnary_entry<T>());
		return m_entry;
	      }
	    
	    gif_lzw_base<T>::get_dictionnary().add(gif_lzw_dictionnary_entry<T>(gif_lzw_base<T>::get_word()) + m_entry[0],p_current_code_size);
	    gif_lzw_base<T>::set_word(m_entry);
	    return m_entry;
	  }
	else
	  {
	    gif_lzw_base<T>::set_word(gif_lzw_dictionnary_entry<T>(p_coded_value));
	    return gif_lzw_base<T>::get_word();
	  }
      }
}

//-----------------------------------------------------
//- Check of Decoder
//-----------------------------------------------------
#if 0
      std::cout << "TOBEORNOTTOBEORTOBEORNOT" << std::endl ;
      std::vector<unsigned int> l_coded_values = {'T','O','B','E','O','R','N','O','T',256,258,260,265,259,261,263};
      lib_gif::gif_lzw_decoder<uint8_t> l_decoder(8);
      unsigned int l_current_code_size = 9;
      for(auto l_iter : l_coded_values)
        {
          const lib_gif::gif_lzw_decoder<uint8_t>::t_dictionnary_entry & l_decoded_values = l_decoder.decode(l_iter + (l_iter < 256 ? 0 : 2),l_current_code_size);
          for(unsigned int l_index = 0 ; l_index < l_decoded_values.size() ; ++l_index)
            {
              std::cout << l_decoded_values[l_index] ;
            }
        }
      std::cout << std::endl;
      std::cout << "ababcbababaaaaaaa" << std::endl ;
      std::vector<unsigned int> l_coded_values = {0,1,3,2, 4, 7, 0, 9, 10, 0};
      lib_gif::gif_lzw_decoder<uint8_t> l_decoder(2);
      unsigned int l_current_code_size = 2;
      for(auto l_iter : l_coded_values)
        {
          const lib_gif::gif_lzw_decoder<uint8_t>::t_dictionnary_entry & l_decoded_values = l_decoder.decode(l_iter + (l_iter < 3 ? 0 : 3),l_current_code_size);
          for(unsigned int l_index = 0 ; l_index < l_decoded_values.size() ; ++l_index)
            {
              std::cout << (uint8_t)('a'+l_decoded_values[l_index]);
            }
        }
      std::cout << std::endl ;
#endif


#endif
//EOF
