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
#ifndef GIF_DATA_SUB_BLOCK
#define GIF_DATA_SUB_BLOCK

#include "quicky_exception.h"
#include <cinttypes>
#include <sstream>
#include <cstring>

namespace lib_gif
{
  class gif_data_sub_block
  {
  public:
    inline gif_data_sub_block(std::ifstream & p_file);
    inline gif_data_sub_block(const unsigned int & p_size);
    inline const unsigned int get_size(void)const;
    inline const uint8_t & get_data(const unsigned int & p_index)const;
    inline void set_data(const unsigned int & p_index,const uint8_t & p_data)const;
    inline void copy(uint8_t * p_dest)const;
    inline ~gif_data_sub_block(void);
    inline void write(std::ofstream & p_file)const;
  private:
    uint8_t m_size;
    uint8_t * m_data;
  };

  //----------------------------------------------------------------------------
  gif_data_sub_block::gif_data_sub_block(std::ifstream & p_file):
    m_size(0),
    m_data(nullptr)
      {
#ifdef DEBUG_GIF_SUB_DATA_BLOCK
        std::cout << "----------------------------" << std::endl ;
        std::cout << "GIF Image Data Sub block :" << std::endl ;
        std::cout << "----------------------------" << std::endl ;
        std::cout << "Current position : 0x" << std::hex << p_file.tellg() << std::dec << std::endl ;
#endif
        p_file.read((char*)&m_size,sizeof(m_size));
#ifdef DEBUG_GIF_SUB_DATA_BLOCK
        std::cout << "Size : " << (unsigned int) m_size << std::endl ;
#endif
        m_data = new uint8_t[m_size];
        if(nullptr == m_data)
          {
            throw quicky_exception::quicky_logic_exception("Failed to allocate memory for Image Data Sub Block",__LINE__,__FILE__);
          }
        p_file.read((char*)m_data,m_size);

      }
    //----------------------------------------------------------------------------
    void gif_data_sub_block::write(std::ofstream & p_file)const
    {
#ifdef DEBUG_GIF_DATA_SUB_BLOCK
        std::cout << "----------------------------" << std::endl ;
        std::cout << "GIF Image Data Sub block :" << std::endl ;
        std::cout << "----------------------------" << std::endl ;
        std::cout << "Current position : 0x" << std::hex << p_file.tellp() << std::dec << std::endl ;
	for(unsigned int l_index = 0 ; l_index < m_size ; ++l_index)
	  {
	    std::cout << std::hex  << "[0x" << l_index << "] = 0x" << (unsigned int)m_data[l_index] << std::dec << std::endl ;
	  }
#endif // DEBUG_GIF_DATA_SUB_BLOCK
        p_file.write((char*)&m_size,sizeof(m_size));
        p_file.write((char*)m_data,m_size);      
    }

    //----------------------------------------------------------------------------
    gif_data_sub_block::gif_data_sub_block(const unsigned int & p_size):
      m_size(p_size),
      m_data(new uint8_t[m_size])
      {
      }

    //----------------------------------------------------------------------------
    void gif_data_sub_block::copy(uint8_t * p_dest)const
    {
      memcpy(p_dest,m_data,m_size);
    }

    //----------------------------------------------------------------------------
    const unsigned int gif_data_sub_block::gif_data_sub_block::get_size(void)const
    {
      return m_size;
    }

    //----------------------------------------------------------------------------
    const uint8_t & gif_data_sub_block::get_data(const unsigned int & p_index)const
      {
        if(p_index < m_size)
          {
            return m_data[p_index];
          }
        else
          {
            std::stringstream l_stream;
            l_stream << p_index;
            throw quicky_exception::quicky_logic_exception("Out of bounds read : "+l_stream.str(),__LINE__,__FILE__);
          }
      }
    //----------------------------------------------------------------------------
    void gif_data_sub_block::set_data(const unsigned int & p_index,const uint8_t & p_data)const
    {
         if(p_index < m_size)
          {
            m_data[p_index] = p_data;
          }
        else
          {
            std::stringstream l_stream;
            l_stream << p_index;
            throw quicky_exception::quicky_logic_exception("Out of bounds write : "+l_stream.str(),__LINE__,__FILE__);
          }
   }

    //----------------------------------------------------------------------------
    gif_data_sub_block::~gif_data_sub_block(void)
      {
	delete[] m_data;
      }

}
#endif
//EOF
