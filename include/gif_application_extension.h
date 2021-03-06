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
#ifndef GIF_APPLICATION_EXTENSION
#define GIF_APPLICATION_EXTENSION

#include "gif_extension_block.h"
#include "quicky_exception.h"
#include <fstream>
#include <iomanip>
#include <vector>

namespace lib_gif
{
    class gif_application_extension: public gif_extension_block
    {

      public:

        inline
        gif_application_extension();

        explicit inline
        gif_application_extension(const unsigned int & p_loop_counter);

        explicit inline
        gif_application_extension(std::ifstream & p_file);

        inline
        bool is_supported() const;

        inline
        void print(std::ostream & p_stream) const override;

        inline
        const unsigned int & get_loop_counter() const;

        inline void write_extension(std::ostream & p_stream) const override;

      private:

        uint8_t m_block_size;

        char m_identifier[8];

        uint8_t m_authentication_code[3];

        unsigned int m_loop_counter;

        bool m_supported;
    };

    //----------------------------------------------------------------------------
    bool gif_application_extension::is_supported() const
    {
        return m_supported;
    }

    //----------------------------------------------------------------------------
    const unsigned int & gif_application_extension::get_loop_counter() const
    {
        return m_loop_counter;
    }

    //----------------------------------------------------------------------------
    void gif_application_extension::write_extension(std::ostream & p_stream) const
    {
        uint8_t l_extension_label = 0xFF;
        p_stream.write((char*)&l_extension_label, sizeof(l_extension_label));
        uint8_t l_block_size = 11;
        p_stream.write((char*)&l_block_size, sizeof(l_block_size));
        p_stream.write((char*)m_identifier, 8 * sizeof(uint8_t));
        p_stream.write((char*)m_authentication_code, 3 * sizeof(uint8_t));
        gif_data_sub_block l_block(3);
        l_block.set_data(0,0x1);
        l_block.set_data(1,m_loop_counter & 0xFFu);
        l_block.set_data(2,m_loop_counter >> 8u);
        l_block.write(p_stream);
    }

    //----------------------------------------------------------------------------
    gif_application_extension::gif_application_extension(std::ifstream & p_file)
    : gif_extension_block(t_gif_data_block_type::APPLICATION_EXTENSION)
    , m_block_size(0)
    , m_identifier("")
    , m_authentication_code("")
    , m_loop_counter(0)
    , m_supported(false)
    {
        p_file.read((char*)&m_block_size,12);
        if((p_file.rdstate() & std::ifstream::failbit) || (p_file.rdstate() & std::ifstream::eofbit))
        {
            throw quicky_exception::quicky_logic_exception("Incomplete application extension", __LINE__, __FILE__);
        }

        if(11 != m_block_size)
        {
            std::stringstream l_size_stream;
            l_size_stream << m_block_size;
            throw quicky_exception::quicky_logic_exception("Bad application extension block size ("+l_size_stream.str()+") should be 11",__LINE__,__FILE__);
        }
        std::string l_identifier = std::string(m_identifier,8);
        std::string l_extended_identifier = l_identifier + std::string((char*)m_authentication_code,3);
        gif_data_sub_block * l_last_data_sub_block = nullptr;
        std::vector<gif_data_sub_block *> l_data_sub_blocks;
        do
        {
            l_last_data_sub_block = new gif_data_sub_block(p_file);
            l_data_sub_blocks.push_back(l_last_data_sub_block);
        }
        while(l_last_data_sub_block->get_size());

        if("NETSCAPE2.0" == l_extended_identifier || "ANIMEXTS1.0" == l_extended_identifier)
        {
            if(2 == l_data_sub_blocks.size())
            {
                if(1 == l_data_sub_blocks[0]->get_data(0))
                {
                    m_loop_counter = l_data_sub_blocks[0]->get_data(1) + ((unsigned int)l_data_sub_blocks[0]->get_data(2)) * 256 ;
                    m_supported = true;
                }
                else
                {
                    std::cout << "Ignoring extension \"" << l_extended_identifier << "\" with Id " <<  l_data_sub_blocks[0]->get_data(0) << std::endl ;
                }
            }
            else
            {
                std::stringstream l_nb_block_stream;
                l_nb_block_stream << l_data_sub_blocks.size();
                throw quicky_exception::quicky_logic_exception("Number of sub data block should be 2 including empty data block instead of "+l_nb_block_stream.str(),__LINE__,__FILE__);
            }
        }
        else
        {
            std::cout << "Ignoring unsupported extension \"" << l_extended_identifier << "\"" << std::endl ;
        }
        for(auto l_iter : l_data_sub_blocks)
        {
            delete l_iter;
        }
    }

    //----------------------------------------------------------------------------
    gif_application_extension::gif_application_extension(const unsigned int & p_loop_counter)
    : gif_extension_block(t_gif_data_block_type::APPLICATION_EXTENSION)
    , m_block_size(11)
    , m_identifier{'N','E','T','S','C','A','P','E'}
    , m_authentication_code{'2','.','0'}
    , m_loop_counter(p_loop_counter)
    , m_supported(true)
    {
        if(p_loop_counter & 0xFFFFu)
        {
            std::stringstream l_stream;
            l_stream << p_loop_counter;
            throw quicky_exception::quicky_logic_exception("Loop counter value should be greater than 65535 : "+l_stream.str(),__LINE__,__FILE__);
        }
    }

    //----------------------------------------------------------------------------
    gif_application_extension::gif_application_extension()
    : gif_application_extension(0)
    {
    }

    //----------------------------------------------------------------------------
    void gif_application_extension::print(std::ostream & p_stream)const
    {
        p_stream << "----------------------------" << std::endl ;
        p_stream << "GIF application extension :" << std::endl ;
        p_stream << "----------------------------" << std::endl ;
        p_stream << "\tIdentifier : \"" << std::string(m_identifier,8) << "\"" << std::endl ;
        char l_prev = p_stream.fill('0');
        size_t l_prev_size = p_stream.width(2);
        p_stream << "\tAuthentication code : 0x" << std::hex << (unsigned int)m_authentication_code[0] <<  (unsigned int)m_authentication_code[1] << (unsigned int)m_authentication_code[2] << std::endl ;
        p_stream.fill(l_prev);
        p_stream.width(l_prev_size);
        p_stream << "\tLoop counter : " << m_loop_counter << std::endl ;
    }

}

#endif 
//EOF
