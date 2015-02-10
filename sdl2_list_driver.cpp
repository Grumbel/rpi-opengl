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

#include <SDL2/SDL.h>
#include <iostream>

/** List audio and video drivers supported by the local SDL2
    installation */
int main()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
    return 1;
  }
  else
  {
    std::cout << "Video Driver" << std::endl;
    std::cout << "============" << std::endl;
    int num_video_drivers = SDL_GetNumVideoDrivers();
    for(int i = 0; i < num_video_drivers; ++i)
    {
      std::cout << "  * " << SDL_GetVideoDriver(i) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Audio Driver" << std::endl;
    std::cout << "============" << std::endl;
    int num_audio_drivers = SDL_GetNumAudioDrivers();
    for(int i = 0; i < num_audio_drivers; ++i)
    {
      std::cout << "  * " << SDL_GetAudioDriver(i) << std::endl;
    }

    SDL_Quit();
    return 0;
  }
}

/* EOF */
