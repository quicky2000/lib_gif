/*    This file is part of lib_gif
\      Copyright (C) 2020  Julien Thevenon ( julien_thevenon at yahoo.fr )

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
#ifdef LIB_GIF_SELF_TEST

#include "gif.h"

int main(int p_argc, char ** p_argv)
{
    try
    {
        if(p_argc != 2)
        {
            throw quicky_exception::quicky_logic_exception("Usage is lib_gif.exe gif_file", __LINE__, __FILE__);
        }

        std::string l_file_name{p_argv[1]};
        std::ifstream l_input_file;
        l_input_file.open(l_file_name);
        if(!l_input_file.is_open())
        {
            throw quicky_exception::quicky_runtime_exception("Unable to read file \"" + l_file_name + "\"", __LINE__, __FILE__);
        }
        lib_gif::gif l_gif(l_input_file);
        std::cout << l_gif << std::endl;
        l_input_file.close();
    }
    catch(quicky_exception::quicky_logic_exception & p_exception)
    {
        std::cerr << "Logic exception \"" << p_exception.what() << "\" from " << p_exception.get_file() << ":" << std::to_string(p_exception.get_line());
    }
    catch(quicky_exception::quicky_runtime_exception & p_exception)
    {
        std::cerr << "Runtime exception \"" << p_exception.what() << "\" from " << p_exception.get_file() << ":" << std::to_string(p_exception.get_line());
    }

}

#endif // LIB_GIF_SELF_TEST
//EOF
