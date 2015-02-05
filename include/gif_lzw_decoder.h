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

#include "gif_lzw_dictionnary.h"
#include <iostream>

namespace lib_gif
{
  template<typename T>
    class gif_lzw_decoder
    {
    public:
      inline gif_lzw_decoder(const unsigned int & p_minimum_code_size);
      inline const gif_lzw_dictionnary_entry<T> & decode(const unsigned int & p_coded_value,unsigned int & p_current_code_size);
      typedef gif_lzw_dictionnary_entry<T> t_dictionnary_entry;
      const unsigned int & get_clear_code(void)const;
      const unsigned int & get_end_information_code(void)const;
    private:
      unsigned int m_minimum_code_size;
      unsigned int m_clear_code;
      unsigned int m_end_information_code;
      gif_lzw_dictionnary<T> m_dictionnary;
      gif_lzw_dictionnary_entry<T> m_word;
      gif_lzw_dictionnary_entry<T> m_entry;
    };

  //----------------------------------------------------------------------------
  template<typename T>
    gif_lzw_decoder<T>::gif_lzw_decoder(const unsigned int & p_minimum_code_size):
    m_minimum_code_size(p_minimum_code_size),
    m_clear_code(1 << m_minimum_code_size),
    m_end_information_code(m_clear_code + 1),
    m_dictionnary(m_clear_code)
      {
      }

  //----------------------------------------------------------------------------
  template<typename T>
    const unsigned int & gif_lzw_decoder<T>::get_clear_code(void)const
    {
      return m_clear_code;
    }
  
  //----------------------------------------------------------------------------
  template<typename T>
    const unsigned int & gif_lzw_decoder<T>::get_end_information_code(void)const
    {
      return m_end_information_code;
    }


    //----------------------------------------------------------------------------
    template<typename T>
      const gif_lzw_dictionnary_entry<T> & gif_lzw_decoder<T>::decode(const unsigned int & p_coded_value,unsigned int & p_current_code_size)
      {
	 //TO DELETE std::cout << "Word = ";
        //TO DELETE        	m_word.display();
	 //TO DELETE std::cout << std::endl ;
	 //TO DELETE std::cout << "Entry = ";
        //TO DELETE        	m_entry.display();
	 //TO DELETE std::cout << std::endl ;

	 //TO DELETE std::cout << "Value to decode =0x" << std::hex << (unsigned int) p_coded_value << std::dec << " ";
	if(m_word.size())
	  {
	    if(p_coded_value > m_end_information_code)
	      {
		 //TO DELETE std::cout << "Compressed code";
		if(m_dictionnary.contains(p_coded_value))
		  {
		    m_entry = m_dictionnary.get(p_coded_value);
		  }
		else
		  {
		    m_entry = m_word;
		    m_entry = m_entry + m_word[0];
		  }
	      }
	    else if(p_coded_value < m_clear_code)
	      {
		 //TO DELETE std::cout << "Basic code";
		m_entry = gif_lzw_dictionnary_entry<T>(p_coded_value);
	      }
	    else if(p_coded_value == m_clear_code)
	      {
		 //TO DELETE std::cout << "Clean code ";
		p_current_code_size = m_minimum_code_size;
		m_dictionnary.clear();
		m_entry = gif_lzw_dictionnary_entry<T>();
		m_word = gif_lzw_dictionnary_entry<T>();
		return m_entry;
	      }
	     //TO DELETE std::cout << std::endl ;
	    m_dictionnary.add(m_word + m_entry[0]);
             //TO DELETE std::cout << "Dictionnary size : " << m_dictionnary.size() << std::endl ;
	    m_word = m_entry;
             //TO DELETE std::cout << (unsigned int)((2 << p_current_code_size ) - 1) << std::endl ;
	    if(p_current_code_size < 11 && (unsigned int)((2 << p_current_code_size )) == m_dictionnary.size())
	      {
		++p_current_code_size;
                 //TO DELETE std::cout << "Increase code size to " <<  p_current_code_size << std::endl ;
	      }
	    return m_entry;
	  }
	else
	  {
	    m_word = gif_lzw_dictionnary_entry<T>(p_coded_value);
	    return m_word;
	  }
      }
}
#endif
//EOF
