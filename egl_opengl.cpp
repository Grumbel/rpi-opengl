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

// This file is somewhat based on EGLWindow.cpp, Copyright 2012 Jon Macey
// <https://github.com/dff180/pishadertoy>

#include <bcm_host.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>

#include "util.hpp"

int main()
{
  bcm_host_init();
  atexit(bcm_host_deinit);

  uint32_t width;
  uint32_t height;
  int32_t ret = graphics_get_display_size(0, &width, &height);
  if (ret < 0)
  {
    std::cerr << "graphics_get_display_size() failed with: " << ret << std::endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    std::cout << "Display size: " << width << "x" << height << std::endl;
  }

  static EGL_DISPMANX_WINDOW_T nativeWindow;

  static const EGLint context_attributes[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
  };

  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if(display == EGL_NO_DISPLAY)
  {
    std::cerr << " error getting display" << std::endl;
    exit(EXIT_FAILURE);
  }

  int major, minor;
  EGLBoolean init_result = eglInitialize(display, &major, &minor);
  if(init_result == EGL_FALSE)
  {
    std::cerr<<"error initialising display\n";
    exit(EXIT_FAILURE);
  }
  else
  {
    std::cout << "EGL init version " << major << "." << minor << std::endl;
  }
  
  EGLint attribs[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_DEPTH_SIZE, 16,
    // EGL_SURFACE_TYPE
    EGL_NONE,
  };

  EGLint num_config;
  EGLConfig config;
  EGLBoolean config_result = eglChooseConfig(display, &attribs[0], &config, 1, &num_config);
  if (config_result == EGL_FALSE)
  {
    std::cerr << "error chosing config API" << std::endl;
    exit(EXIT_FAILURE);
  }

  EGLBoolean bind_result = eglBindAPI(EGL_OPENGL_ES_API);
  if (bind_result == EGL_FALSE)
  {
    std::cerr << "error binding API" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
  if (context == EGL_NO_CONTEXT)
  {
    std::cerr << "couldn't get a valid context" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  // create an EGL window surface the way this works is we set the dimensions of the srec
  // and destination rectangles.
  // if these are the same size there is no scaling, else the window will auto scale

  VC_RECT_T dstRect;
  dstRect.x = 0;
  dstRect.y = 0;
  dstRect.width = width;
  dstRect.height = height;

  VC_RECT_T srcRect;
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.width = width << 16;
  srcRect.height = height << 16;

  // whilst this is mostly taken from demos I will try to explain what it does
  // there are very few documents on this ;-0
  // open our display with 0 being the first display, there are also some other versions
  // of this function where we can pass in a mode however the mode is not documented as
  // far as I can see
  DISPMANX_DISPLAY_HANDLE_T m_dispmanDisplay = vc_dispmanx_display_open(0);
  
  // now we signal to the video core we are going to start updating the config
  DISPMANX_UPDATE_HANDLE_T m_dispmanUpdate = vc_dispmanx_update_start(0);
  
  // this is the main setup function where we add an element to the display, this is filled in
  // to the src / dst rectangles
  DISPMANX_ELEMENT_HANDLE_T m_dispmanElement =
    vc_dispmanx_element_add(m_dispmanUpdate, m_dispmanDisplay,
                            0, &dstRect,
                            0, &srcRect,
                            DISPMANX_PROTECTION_NONE, 0, 0,
                            DISPMANX_NO_ROTATE);
  
  // now we have created this element we pass it to the native window structure ready
  // no create our new EGL surface
  nativeWindow.element = m_dispmanElement;
  nativeWindow.width = width;
  nativeWindow.height = height;
  // we now tell the vc we have finished our update
  vc_dispmanx_update_submit_sync(m_dispmanUpdate);

  // finally we can create a new surface using this config and window
  EGLSurface surface = eglCreateWindowSurface(display, config, &nativeWindow, NULL);
  assert(surface != EGL_NO_SURFACE);
  
  // connect the context to the surface
  EGLBoolean make_current_result = eglMakeCurrent(display, surface, surface, context);
  assert(EGL_FALSE != make_current_result);

  glClearColor(1.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  eglSwapBuffers(display, surface);

  render_opengl();
  eglSwapBuffers(display, surface);
  
  std::cout << "sleeping for 5 seconds" << std::endl;
  sleep(5);
  
  return 0;
}

/* EOF */
