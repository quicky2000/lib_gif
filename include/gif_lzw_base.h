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
#ifndef GIF_LZW_BASE_H
#define GIF_LZW_BASE_H

#include "gif_lzw_dictionnary.h"
#include <iostream>

namespace lib_gif
{
  template<typename T>
    class gif_lzw_base
    {
    public:
      inline gif_lzw_base(const unsigned int & p_minimum_code_size);
      typedef gif_lzw_dictionnary_entry<T> t_dictionnary_entry;
      inline const unsigned int & get_clear_code(void)const;
      inline const unsigned int & get_end_information_code(void)const;
      inline const gif_lzw_dictionnary_entry<T> & get_word(void)const;
    protected:
      inline const unsigned int & get_minimum_code_size(void)const;
      inline gif_lzw_dictionnary<T> & get_dictionnary(void);
      inline void set_word(const gif_lzw_dictionnary_entry<T> & p_word);
    private:
      unsigned int m_minimum_code_size;
      unsigned int m_clear_code;
      unsigned int m_end_information_code;
      gif_lzw_dictionnary<T> m_dictionnary;
      gif_lzw_dictionnary_entry<T> m_word;
    };

  //----------------------------------------------------------------------------
  template<typename T>
    gif_lzw_base<T>::gif_lzw_base(const unsigned int & p_minimum_code_size):
    m_minimum_code_size(p_minimum_code_size),
    m_clear_code(1 << m_minimum_code_size),
    m_end_information_code(m_clear_code + 1),
    m_dictionnary(m_clear_code)
    {
    }

  //----------------------------------------------------------------------------
  template<typename T>
    const unsigned int & gif_lzw_base<T>::get_minimum_code_size(void)const
    {
      return m_minimum_code_size;
    }

  //----------------------------------------------------------------------------
  template<typename T>
      const unsigned int & gif_lzw_base<T>::get_clear_code(void)const
    {
      return m_clear_code;
    }

  //----------------------------------------------------------------------------
  template<typename T>
      const unsigned int & gif_lzw_base<T>::get_end_information_code(void)const
    {
      return m_end_information_code;
    }

  //----------------------------------------------------------------------------
  template<typename T>
      const gif_lzw_dictionnary_entry<T> & gif_lzw_base<T>::get_word(void)const
    {
      return m_word;
    }

  //----------------------------------------------------------------------------
  template<typename T>
      gif_lzw_dictionnary<T> & gif_lzw_base<T>::get_dictionnary(void)
    {
      return m_dictionnary;
    }

  //----------------------------------------------------------------------------
  template<typename T>
      void gif_lzw_base<T>::set_word(const gif_lzw_dictionnary_entry<T> & p_word)
    {
      m_word = p_word;
    }

}
#endif
//EOF
