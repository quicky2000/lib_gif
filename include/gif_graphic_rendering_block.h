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
#ifndef GIF_GRAPHIC_RENDERING_BLOCK_H
#define GIF_GRAPHIC_RENDERING_BLOCK_H
namespace lib_gif
{
  class gif_graphic_rendering_block
  {
   public:
    virtual const uint16_t & get_image_left_position(void)const=0;
    virtual const uint16_t & get_image_top_position(void)const=0;
    virtual const uint16_t & get_image_width(void)const=0;
    virtual const uint16_t & get_image_height(void)const=0;
    virtual inline ~gif_graphic_rendering_block(void){}
    virtual void print(std::ostream & p_stream)const=0;
    virtual inline bool is_image(void)const{return false;}
    virtual void write(std::ofstream & p_file)const=0;
  private:
    
  };
}
#endif
//EOF
