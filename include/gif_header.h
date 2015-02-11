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
#ifndef GIF_HEADER_H
#define GIF_HEADER_H

#include <string>
#include <cstring>
#include <sstream>
#include "quicky_exception.h"

namespace lib_gif
{
  class gif_header
  {
  public:
    inline gif_header(void);
    inline void set_signature(const std::string & p_signature);
    inline const std::string get_signature(void)const;
    inline void set_version(const std::string & p_version);
    inline const std::string get_version(void)const;
    static inline unsigned int get_size(void);
  private:
    uint8_t m_signature[3];
    uint8_t m_version[3];
  };

  //----------------------------------------------------------------------------
  inline std::ostream & operator<<(std::ostream & p_stream,const gif_header & p_header)
    {
      p_stream << "----------------------------" << std::endl ;
      p_stream << "GIF Header :" << std::endl ;
      p_stream << "----------------------------" << std::endl ;
      p_stream << "Signature : " << p_header.get_signature() << std::endl ;
      p_stream << "Version : " << p_header.get_version() << std::endl ;
      return p_stream;
    }

  //----------------------------------------------------------------------------
  gif_header::gif_header(void):
    m_signature{'G','I','F'},
    m_version{'8','9','a'}
  {
  }
    //----------------------------------------------------------------------------
    void gif_header::set_version(const std::string & p_version)
    {
    if(p_version.size() > 3)
      {
        std::stringstream l_stream;
        l_stream << p_version.size();
        throw quicky_exception::quicky_logic_exception("GIF header version is "+l_stream.str()+" bytes instead of 3",__LINE__,__FILE__);
      }
    strncpy((char*)m_version,p_version.c_str(),3);
    }

    //----------------------------------------------------------------------------
    const std::string gif_header::get_version(void)const
      {
        return std::string((char*)m_version,3);
      }
 
    //----------------------------------------------------------------------------
    void gif_header::set_signature(const std::string & p_signature)
    {
      if("GIF" != p_signature)
      {
        throw quicky_exception::quicky_logic_exception("GIF header signature is \""+p_signature+"\" instead of \"GIF\"",__LINE__,__FILE__);
      }
      strncpy((char*)m_signature,p_signature.c_str(),3);
    }

    //----------------------------------------------------------------------------
    const std::string gif_header::get_signature(void)const
      {
        return std::string((char*)m_signature,3);
      }
 
    //----------------------------------------------------------------------------
    unsigned int gif_header::get_size(void)
    {
      return 6;
    }
  
}

#endif // GIF_HEADER_H
//EOF
