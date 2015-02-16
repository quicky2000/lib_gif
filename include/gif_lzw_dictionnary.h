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
#ifndef GIF_LZW_DICTIONNARY_H
#define GIF_LZW_DICTIONNARY_H

#include "gif_lzw_dictionnary_entry.h"
#include <vector>
#include <map>
#include <sstream>

namespace lib_gif
{
  template <typename T>
  class gif_lzw_dictionnary
  {
  public:
    inline gif_lzw_dictionnary(const unsigned int & p_size);
    inline ~gif_lzw_dictionnary(void);
    inline bool contains(const unsigned int & p_index);
    inline const gif_lzw_dictionnary_entry<T> & get(const unsigned int & p_index)const;
    inline void add(const gif_lzw_dictionnary_entry<T> & p_entry,unsigned int & p_current_code_size);
    inline void clear(void);
    inline bool contains(const gif_lzw_dictionnary_entry<T> & p_entry)const;
    inline const unsigned int & get_code(const gif_lzw_dictionnary_entry<T> & p_entry)const;
    inline const unsigned int get_nb_entry(void)const;
  private:
    unsigned int m_size;
    std::vector<gif_lzw_dictionnary_entry<T> *> m_content;
    std::map<gif_lzw_dictionnary_entry<T>,unsigned int> m_content2index;
  };

  //----------------------------------------------------------------------------  
  template <typename T>
  const unsigned int gif_lzw_dictionnary<T>::get_nb_entry(void)const
    {
      return m_content.size();
    }

  //----------------------------------------------------------------------------
  template <typename T>
    gif_lzw_dictionnary<T>::gif_lzw_dictionnary(const unsigned int & p_size):
    m_size(p_size)
    {
      for(unsigned int l_index = 0 ; l_index < p_size ; ++l_index)
	{
          gif_lzw_dictionnary_entry<T> * l_new_entry = new gif_lzw_dictionnary_entry<T>((T)l_index);
          m_content2index.insert(typename std::map<gif_lzw_dictionnary_entry<T>,unsigned int>::value_type(*l_new_entry,l_index));
	  m_content.push_back(l_new_entry);
	}
      m_content.push_back(nullptr); // Index of clear code
      m_content.push_back(nullptr); // Index of end of information code
    }

  //----------------------------------------------------------------------------  
  template <typename T>
    gif_lzw_dictionnary<T>::~gif_lzw_dictionnary(void)
    {
      for(auto l_iter:m_content)
	{
	  delete l_iter;
	}
    }

  //----------------------------------------------------------------------------  
  template <typename T>
  const gif_lzw_dictionnary_entry<T> & gif_lzw_dictionnary<T>::get(const unsigned int & p_index)const
    {
      if(p_index < m_content.size())
	{
	  if(m_content[p_index])
	    {
	      return *(m_content[p_index]);
	    }
	  else
	    {
	      std::stringstream l_index_stream;
	      l_index_stream << p_index;
	      throw quicky_exception::quicky_logic_exception("Index "+l_index_stream.str()+" is empty",__LINE__,__FILE__);
	    }
	}
      std::stringstream l_index_stream;
      l_index_stream << p_index;
      std::stringstream l_size_stream;
      l_size_stream << m_content.size();
      throw quicky_exception::quicky_logic_exception("Index "+l_index_stream.str()+" is greater than size "+l_size_stream.str(),__LINE__,__FILE__);
    }

  //----------------------------------------------------------------------------  
  template <typename T>
  bool gif_lzw_dictionnary<T>::contains(const unsigned int & p_index)
    {
      return p_index < m_content.size();
    }

  //----------------------------------------------------------------------------  
  template <typename T>
    bool gif_lzw_dictionnary<T>::contains(const gif_lzw_dictionnary_entry<T> & p_entry)const
    {
      typename std::map<gif_lzw_dictionnary_entry<T>,unsigned int>::const_iterator l_iter = m_content2index.find(p_entry);
      return m_content2index.end() != l_iter;
    }

  //----------------------------------------------------------------------------  
  template <typename T>
  const unsigned int & gif_lzw_dictionnary<T>::get_code(const gif_lzw_dictionnary_entry<T> & p_entry)const
    {
      typename std::map<gif_lzw_dictionnary_entry<T>,unsigned int>::const_iterator l_iter = m_content2index.find(p_entry);
      assert(m_content2index.end() != l_iter);
      return l_iter->second;
   }

  //----------------------------------------------------------------------------  
  template <typename T>
    void gif_lzw_dictionnary<T>::add(const gif_lzw_dictionnary_entry<T> & p_entry,unsigned int & p_current_code_size)
    {
#ifdef DEBUG_GIF_LZW_DICTIONNARY
      std::cout << "Add " << m_content.size() << std::endl ;
#endif // DEBUG_GIF_LZW_DICTIONNARY
      m_content2index.insert(typename std::map<gif_lzw_dictionnary_entry<T>,unsigned int>::value_type(p_entry,m_content.size()));
      m_content.push_back(new gif_lzw_dictionnary_entry<T>(p_entry));


      if(p_current_code_size < 11 && (unsigned int)((2 << p_current_code_size )) == m_content.size())
	{
	  ++p_current_code_size;
	}

    }
  //----------------------------------------------------------------------------  
  template <typename T>
    void gif_lzw_dictionnary<T>::clear(void)
    {
#ifdef DEBUG_GIF_LZW_DICTIONNARY
      std::cout << "Clear " << m_content.size() << std::endl ;
#endif // DEBUG_GIF_LZW_DICTIONNARY
      for(unsigned int l_index = m_size + 2; l_index < m_content.size() ; ++l_index)
        {
          delete m_content[l_index];
        }
      m_content.resize(m_size + 2);

      gif_lzw_dictionnary_entry<T>(m_size - 1);
      typename std::map<gif_lzw_dictionnary_entry<T>,unsigned int>::iterator l_erase_iter = m_content2index.find(gif_lzw_dictionnary_entry<T>(m_size - 1));
      assert(m_content2index.end() != l_erase_iter);
      ++l_erase_iter;
      m_content2index.erase(l_erase_iter,m_content2index.end());
    }

}
#endif
//EOF
