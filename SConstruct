import os
import platform

env = Environment(ENV=os.environ,
                  CXXFLAGS=["-std=c++11",
                            "-g", "-O3",
                            "-Wall", "-Werror", "-Wextra"])

sdl_env = env.Clone()
sdl_env.ParseConfig("pkg-config --libs --cflags sdl2")
sdl_env.Append(LIBS=["GLESv2"])
sdl_env.Program("sdl2_list_driver", ["sdl2_list_driver.cpp"])

if platform.machine() == 'armv6l':
    egl_env = env.Clone()
    egl_env.Append(CPPPATH=["/opt/vc/include",
                            "/opt/vc/include/interface/vcos/pthreads",
                            "/opt/vc/include/interface/vmcs_host/linux"])
    egl_env.Append(LIBPATH=["/opt/vc/lib"])
    egl_env.Append(LIBS=["GLESv2", "EGL", "openmaxil", "bcm_host", "SOIL"])
    egl_env.Program("egl_opengl", ["egl_opengl.cpp"])

    sdl_env.Append(CPPDEFINES=[('HAVE_OPENGL_ES2')])
    sdl_env.Program("sdl2_opengl", ["sdl2_opengl.cpp"])
else:
    # egl_env = env.Clone()
    # egl_env.Program("egl_opengl", ["egl_opengl.cpp"])
    sdl_env.Program("sdl2_opengl", ["sdl2_opengl.cpp"])

# EOF #
