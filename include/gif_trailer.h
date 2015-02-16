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
#ifndef GIF_TRAILER
#define GIF_TRAILER

#include "gif_data_block.h"

namespace lib_gif
{
  class gif_trailer: public gif_data_block
  {
  public:
    inline void print(std::ostream & p_stream)const;
    inline gif_trailer(void);
    inline void write(std::ofstream & p_file)const;
  };

    //----------------------------------------------------------------------------
    void gif_trailer::print(std::ostream & p_stream)const
    {
      p_stream << "----------------------------" << std::endl ;
      p_stream << "GIF Trailer :" << std::endl ;
      p_stream << "----------------------------" << std::endl ;
    }
  //----------------------------------------------------------------------------
  gif_trailer::gif_trailer(void):
    gif_data_block(gif_data_block::t_gif_data_block_type::TRAILER)
    {
    }
    //----------------------------------------------------------------------------
    void gif_trailer::write(std::ofstream & p_file)const
    {
      uint8_t l_trailer = 0x3B;
      p_file.write((char*)&l_trailer,sizeof(l_trailer));
    }

}
#endif
//EOF


