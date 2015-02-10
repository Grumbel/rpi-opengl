#include <string>
#include <sstream>
#include <fstream>

void check_gl_error()
{
  GLenum err = glGetError( );
  switch(err)
  {
    case GL_NO_ERROR:
      // ok
      break;

    case GL_INVALID_ENUM :
      std::cerr << "OpenGL error: GL_INVALID_ENUM" << std::endl;
      break;

    case GL_INVALID_VALUE :
      std::cerr << "OpenGL error: GL_INVALID_VALUE" << std::endl;
      break;

    case GL_INVALID_OPERATION :
      std::cerr << "OpenGL error: GL_INVALID_OPERATION" << std::endl;
      break;

    case GL_INVALID_FRAMEBUFFER_OPERATION :
      std::cerr << "OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
      break;

    case GL_OUT_OF_MEMORY :
      std::cerr << "OpenGL error: GL_OUT_OF_MEMORY" << std::endl;
      break;

    default:
      std::cerr << "unknown OpenGL error" << std::endl;
      break;
  };
}

std::string read_text_file(const std::string& filename)
{
  std::ifstream fin(filename);
  if (!fin)
  {
    int err = errno;
    std::cerr << "failed to open: " << filename << std::endl;
    std::cerr << strerror(err) << std::endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    std::ostringstream buffer;
    buffer << fin.rdbuf();
    return buffer.str();
  }
}

GLuint compile_shader(GLenum shader_type, const std::string& filename)
{
  std::string source = read_text_file(filename);
  
  std::cout << ">>> compiling '" << filename << "':\n" << source << std::endl;
  
  GLuint shader = glCreateShader(shader_type);
  check_gl_error();
  const GLchar* source_lst[] = { source.c_str() };
  GLint length_lst[] = { static_cast<GLint>(source.size()) };
  glShaderSource(shader, 1, source_lst, length_lst);
  check_gl_error();
  glCompileShader(shader);
  check_gl_error();
  
  GLint status = 45454545;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  check_gl_error();
  if (!status)
  {
    GLchar infolog[4096];
    GLsizei len;
    glGetShaderInfoLog(shader, sizeof(infolog), &len, infolog);
    std::cerr << "Compile error in '" << filename << "': status = " << status << std::endl;
    std::cerr.write(infolog, len);
    exit(EXIT_FAILURE);
  }
  else
  {
    std::cout << ">>> compiling ok: status = " << status << std::endl;
    return shader;
  }
}

GLuint create_program(const std::string& vert_shader_filename,
                      const std::string& frag_shader_filename)
{
  GLuint vert_shader = compile_shader(GL_VERTEX_SHADER, vert_shader_filename);
  GLuint frag_shader = compile_shader(GL_FRAGMENT_SHADER, frag_shader_filename);

  GLuint program = glCreateProgram();

  glAttachShader(program, frag_shader);
  glAttachShader(program, vert_shader);

  glLinkProgram(program);

  GLint link_status = 45454545;
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);
  check_gl_error();
  if (!link_status)
  {
    GLchar infolog[4096];
    GLsizei len;
    glGetProgramInfoLog(program, sizeof(infolog), &len, infolog);
    std::cerr << "Link error: status = " << link_status << std::endl;
    std::cerr.write(infolog, len);
    exit(EXIT_FAILURE);
  }
  else
  {
    std::cout << "link ok: status = " << link_status << std::endl;
  }
  
  return program;
}

void render_opengl()
{
  //glViewport(0, 0, 720, 576);

  check_gl_error();

  glClearColor(1, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  check_gl_error();
        
  GLuint program = create_program("shader.vert", "shader.frag");

  check_gl_error();
    
  glUseProgram(program);
  GLint attr_coord = glGetAttribLocation(program, "coord");
  if (attr_coord == -1)
  {
    std::cerr << "Could not bind attribute 'coord'" << std::endl;
    exit(EXIT_FAILURE);
  }

  check_gl_error();
    
  GLfloat verts[] = {
    0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
  };

  glEnableVertexAttribArray(attr_coord);
  glVertexAttribPointer(attr_coord,
                        4, // number of elements
                        GL_FLOAT, // type
                        GL_FALSE, // normalized
                        0, // stride
                        verts);

  glDrawArrays(GL_TRIANGLES, 0, 3);
  check_gl_error();
}

/* EOF */
