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
    inline gif_application_extension(std::ifstream & p_file);
    inline bool is_supported(void)const;
    inline const unsigned int & get_loop_counter(void)const;
  private:
    uint8_t m_block_size;
    char m_identifier[8];
    uint8_t m_authentication_code[3];
    unsigned int m_loop_counter;
    bool m_supported;
  };
  //----------------------------------------------------------------------------
  bool gif_application_extension::is_supported(void)const
  {
    return m_supported;
  }

  //----------------------------------------------------------------------------
  const unsigned int & gif_application_extension::get_loop_counter(void)const
    {
      return m_loop_counter;
    }

  //----------------------------------------------------------------------------
  gif_application_extension::gif_application_extension(std::ifstream & p_file):
    gif_extension_block(t_gif_data_block_type::APPLICATION_EXTENSION),
    m_loop_counter(0),
    m_supported(false)
    {
      std::cout << "----------------------------" << std::endl ;
      std::cout << "GIF application extension :" << std::endl ;
      std::cout << "----------------------------" << std::endl ;
      std::cout << "Current position : 0x" << std::hex << p_file.tellg() << std::dec << std::endl ;
      p_file.read((char*)&m_block_size,12);
      if(11 != m_block_size) 
	{
	  std::stringstream l_size_stream;
	  l_size_stream << m_block_size;
	  throw quicky_exception::quicky_logic_exception("Bad application extension block size ("+l_size_stream.str()+") should be 11",__LINE__,__FILE__);
	}
      std::string l_identifier = std::string(m_identifier,8);
      std::cout << "\tIdentifier : \"" << l_identifier << "\"" << std::endl ;
      char l_prev = std::cout.fill('0');
      size_t l_prev_size = std::cout.width(2);
      std::cout << "\tAuthentication code : 0x" << std::hex << (unsigned int)m_authentication_code[0] <<  (unsigned int)m_authentication_code[1] << (unsigned int)m_authentication_code[2] << std::endl ;
      std::cout.fill(l_prev);
      std::cout.width(l_prev_size);
      std::string l_extended_identifier = l_identifier + std::string((char*)m_authentication_code,3);
      std::cout << "\tExtended identifier : \"" << l_extended_identifier << "\"" << std::endl ;
      gif_data_sub_block * l_last_data_sub_block = nullptr;
      std::vector<gif_data_sub_block *> l_data_sub_blocks;
      do
	{
	  l_last_data_sub_block = new gif_data_sub_block(p_file);
	  l_data_sub_blocks.push_back(l_last_data_sub_block);
	} while(l_last_data_sub_block->get_size());
      std::cout << "Number of data sub_blocks : " << l_data_sub_blocks.size() << std::endl ;
      if("NETSCAPE2.0" == l_extended_identifier || "ANIMEXTS1.0" == l_extended_identifier)
	{
	  if(2 == l_data_sub_blocks.size())
	    {
	      if(1 == l_data_sub_blocks[0]->get_data(0))
		{
		  m_loop_counter = l_data_sub_blocks[0]->get_data(1) + ((unsigned int)l_data_sub_blocks[0]->get_data(2)) * 256 ;
		  std::cout << "\tLoop counter : " << m_loop_counter << std::endl ;
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

}

#endif 
//EOF
