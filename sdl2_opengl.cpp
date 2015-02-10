// Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received m_a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <SDL.h>
#include <SDL_opengles2.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <errno.h>

#include "util.hpp"

class SDLLibrary
{
private:
  SDL_Window* m_window;
  SDL_GLContext m_gl_context;
  
public:
  SDLLibrary() :
    m_window(),
    m_gl_context()
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
      exit(EXIT_FAILURE);
    }
    else
    {
      SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
      SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
      SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
      //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
      //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

      m_window = SDL_CreateWindow("Title", 0, 0, 720, 576, SDL_WINDOW_OPENGL);
      if (m_window == NULL)
      {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
      }

      m_gl_context = SDL_GL_CreateContext(m_window);
      std::cerr << "Context ok" << std::endl;

      int r, g, b;
      SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &r);
      SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &g);
      SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &b);
    
      std::cout << "Red size:   " << r
                << "\nGreen size: " << g
                << "\nBlue size:  " << b << std::endl;
    }
  }

  ~SDLLibrary()
  { 
    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
  }

  void swap()
  {
    SDL_GL_SwapWindow(m_window);
  }
};

int main()
{
  SDLLibrary sdl_library;
  
  render_opengl();
  sdl_library.swap();
  SDL_Delay(5000);
 
  return 0;
}

/* EOF */
