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
#ifndef GIF_LZW_DICTIONNARY_ENTRY_H
#define GIF_LZW_DICTIONNARY_ENTRY_H

#include <vector>
#include <sstream>

namespace lib_gif
{
  template <typename T>
  class gif_lzw_dictionnary_entry
  {
  public:
    inline gif_lzw_dictionnary_entry(const T & p_char);
    inline gif_lzw_dictionnary_entry(void);
    inline gif_lzw_dictionnary_entry<T> & operator+(const T & p_char);
    inline const T & operator[](size_t p_index)const;
    inline bool operator<(const gif_lzw_dictionnary_entry<T> & p_entry)const;
    inline size_t size(void)const;
    inline void display(void)const;
  private:
    std::vector<T> m_word;
  };

  //----------------------------------------------------------------------------
    template <typename T>
      gif_lzw_dictionnary_entry<T>::gif_lzw_dictionnary_entry(const T & p_char):
      m_word(1,p_char)
    {
    }

  //----------------------------------------------------------------------------
    template <typename T>
      gif_lzw_dictionnary_entry<T>::gif_lzw_dictionnary_entry(void)
    {
    }

  //----------------------------------------------------------------------------
    template <typename T>
      gif_lzw_dictionnary_entry<T> & gif_lzw_dictionnary_entry<T>::operator+(const T & p_char)
      {
	m_word.push_back(p_char);
	return *this;
      }

  //----------------------------------------------------------------------------
    template <typename T>
      bool gif_lzw_dictionnary_entry<T>::operator<(const gif_lzw_dictionnary_entry<T> & p_entry)const
      {
	if(m_word.size() != p_entry.m_word.size()) return m_word.size() < p_entry.m_word.size();
        return m_word < p_entry.m_word;
      }

    //----------------------------------------------------------------------------
    template <typename T>
      const T & gif_lzw_dictionnary_entry<T>::operator[](size_t p_index)const
      {
	if(p_index < m_word.size())
	  {
	    return m_word[p_index];
	  }
	std::stringstream l_index_stream;
	l_index_stream << p_index;
	std::stringstream l_size_stream;
	l_size_stream << m_word.size();
	throw quicky_exception::quicky_logic_exception("Index "+l_index_stream.str()+" is greater than size "+l_size_stream.str(),__LINE__,__FILE__);
      }

    //----------------------------------------------------------------------------
    template <typename T>
      size_t gif_lzw_dictionnary_entry<T>::size(void)const
      {
	return m_word.size();
      }

    //----------------------------------------------------------------------------
    template <typename T>
      void gif_lzw_dictionnary_entry<T>::display(void)const
      {
	std::cout << "{" << std::hex;
	for(auto l_iter:m_word)
	  {
	    std::cout << " 0x" << (unsigned int) l_iter ;
	  }
	std::cout << "}" << std::dec << std::endl ;
      }    

}
#endif
//EOF
