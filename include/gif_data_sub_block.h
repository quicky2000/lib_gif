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

        explicit inline
        gif_data_sub_block(std::ifstream & p_file);

        explicit inline
        gif_data_sub_block(const unsigned int & p_size);

        inline
        unsigned int get_size()const;

        inline
        const uint8_t & get_data(const unsigned int & p_index)const;

        inline
        void set_data(const unsigned int & p_index,const uint8_t & p_data)const;

        inline
        void copy(uint8_t * p_dest)const;

        inline
        ~gif_data_sub_block();

        inline
        void write(std::ostream & p_stream)const;

      private:

        uint8_t m_size;

        uint8_t * m_data;
    };

    //----------------------------------------------------------------------------
    gif_data_sub_block::gif_data_sub_block(std::ifstream & p_file)
    : m_size(0)
    , m_data(nullptr)
    {
#ifdef DEBUG_GIF_SUB_DATA_BLOCK
        std::cout << "----------------------------" << std::endl ;
        std::cout << "GIF Image Data Sub block :" << std::endl ;
        std::cout << "----------------------------" << std::endl ;
        std::cout << "Current position : 0x" << std::hex << p_file.tellg() << std::dec << std::endl ;
#endif
        p_file.read((char*)&m_size,sizeof(m_size));
        if((p_file.rdstate() & std::ifstream::failbit) || (p_file.rdstate() & std::ifstream::eofbit))
        {
            throw quicky_exception::quicky_logic_exception("Incomplete data sub block size", __LINE__, __FILE__);
        }

#ifdef DEBUG_GIF_SUB_DATA_BLOCK
        std::cout << "Size : " << (unsigned int) m_size << std::endl ;
#endif
        m_data = new uint8_t[m_size];
        if(nullptr == m_data)
        {
            throw quicky_exception::quicky_logic_exception("Failed to allocate memory for Image Data Sub Block",__LINE__,__FILE__);
        }
        p_file.read((char*)m_data,m_size);
        if((p_file.rdstate() & std::ifstream::failbit) || (p_file.rdstate() & std::ifstream::eofbit))
        {
            throw quicky_exception::quicky_logic_exception("Incomplete data sub block", __LINE__, __FILE__);
        }

    }

    //----------------------------------------------------------------------------
    void gif_data_sub_block::write(std::ostream & p_stream) const
    {
#ifdef DEBUG_GIF_DATA_SUB_BLOCK
        std::cout << "----------------------------" << std::endl ;
        std::cout << "GIF Image Data Sub block :" << std::endl ;
        std::cout << "----------------------------" << std::endl ;
        std::cout << "Current position : 0x" << std::hex << p_stream.tellp() << std::dec << std::endl ;
	for(unsigned int l_index = 0 ; l_index < m_size ; ++l_index)
	  {
	    std::cout << std::hex  << "[0x" << l_index << "] = 0x" << (unsigned int)m_data[l_index] << std::dec << std::endl ;
	  }
#endif // DEBUG_GIF_DATA_SUB_BLOCK
        p_stream.write((char*)&m_size,sizeof(m_size));
        p_stream.write((char*)m_data,m_size);
    }

    //----------------------------------------------------------------------------
    gif_data_sub_block::gif_data_sub_block(const unsigned int & p_size)
    : m_size(p_size)
    , m_data(new uint8_t[m_size])
    {
    }

    //----------------------------------------------------------------------------
    void gif_data_sub_block::copy(uint8_t * p_dest) const
    {
        memcpy(p_dest,m_data,m_size);
    }

    //----------------------------------------------------------------------------
    unsigned int gif_data_sub_block::gif_data_sub_block::get_size() const
    {
        return m_size;
    }

    //----------------------------------------------------------------------------
    const uint8_t & gif_data_sub_block::get_data(const unsigned int & p_index) const
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
    void gif_data_sub_block::set_data(const unsigned int & p_index,const uint8_t & p_data) const
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
    gif_data_sub_block::~gif_data_sub_block()
    {
        delete[] m_data;
    }

}
#endif
//EOF
