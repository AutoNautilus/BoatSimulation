#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader
{
public:
	std::string ReadFile(const char* file)
	{
		// Open file
		std::ifstream t(file);
 
		// Read file into buffer
		std::stringstream buffer;
		buffer << t.rdbuf();
 
		// Make a std::string and fill it with the contents of buffer
		std::string fileContent = buffer.str();
 
		return fileContent;
	}
 
	void BindAttributeLocation(int index, const std::string &attribute)
	{
		// Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
		// Attribute locations must be setup before calling glLinkProgram
		glBindAttribLocation(shaderProgram, index, attribute.c_str());
	}
 
	void UseProgram()
	{
		// Load the shader into the rendering pipeline
		glUseProgram(shaderProgram);
	}

	bool Init()
	{
		// Generate our shader. This is similar to glGenBuffers() and glGenVertexArray(), except that this returns the ID
		shaderProgram = glCreateProgram();
		
		// Bind the location of our attributes
		BindAttributeLocation(0, "in_Position");
		BindAttributeLocation(1, "in_Color");

		if (!LoadVertexShader("../res/shaders/tutorial2.vert"))
			return false;

		if (!LoadFragmentShader("../res/shaders/tutorial2.frag"))
			return false;

    if(!LoadGeometryShader("../res/shaders/pointToSquare.geom"))
      return false;

		// All shaders has been create, now we must put them together into one large object
		return LinkShaders();
	}
 
 
	bool LoadVertexShader(const std::string &filename)
	{
		std::cout << "Linking Vertex shader" << std::endl;

		// Read file as std::string 
		std::string str = ReadFile(filename.c_str());
 
		 // c_str() gives us a const char*, but we need a non-const one
		char* src = const_cast<char*>( str.c_str());
		int32_t size = str.length();
		
		// Create an empty vertex shader handle
		vertexshader = glCreateShader(GL_VERTEX_SHADER);
 
		// Send the vertex shader source code to OpenGL
		glShaderSource(vertexshader, 1, &src, &size);
 
		// Compile the vertex shader
		glCompileShader(vertexshader);
 
		int wasCompiled = 0;
		glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &wasCompiled );
 
		if (wasCompiled == 0)
		{
			PrintShaderCompilationErrorInfo(vertexshader);
			return false;
		}

		glAttachShader(shaderProgram, vertexshader);
		return true;
	}

	bool LoadFragmentShader(const std::string &filename)
	{
		std::cout << "Loading Fragment Shader" << std::endl;

		// Read file as std::string 
		std::string str = ReadFile(filename.c_str());
 
		 // c_str() gives us a const char*, but we need a non-const one
		char* src = const_cast<char*>( str.c_str());
		int32_t size = str.length();
		
		// Create an empty vertex shader handle
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 
		// Send the vertex shader source code to OpenGL
		glShaderSource(fragmentShader, 1, &src, &size);
 
		// Compile the vertex shader
		glCompileShader(fragmentShader);
 
		int wasCompiled = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &wasCompiled );
 
		if (wasCompiled == false)
		{
			PrintShaderCompilationErrorInfo(fragmentShader);
			return false;
		}

		glAttachShader(shaderProgram, fragmentShader);
		return true;
	}
  bool LoadGeometryShader(const std::string &filename)
	{
		std::cout << "Loading Geometry Shader" << std::endl;

		// Read file as std::string 
		std::string str = ReadFile(filename.c_str());
 
    // c_str() gives us a const char*, but we need a non-const one
		char* src = const_cast<char*>( str.c_str());
		int32_t size = str.length();
		
		// Create an empty vertex shader handle
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
 
		// Send the vertex shader source code to OpenGL
		glShaderSource(geometryShader, 1, &src, &size);
 
		// Compile the vertex shader
		glCompileShader(geometryShader);
 
		int wasCompiled = 0;
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &wasCompiled );
 
		if (wasCompiled == false)
      {
        PrintShaderCompilationErrorInfo(geometryShader);
        return false;
      }

		glAttachShader(shaderProgram, geometryShader);
		return true;
	}
	bool LinkShaders()
	{
		// Link. At this point, our shaders will be inspected/optized and the binary code generated
		// The binary code will then be uploaded to the GPU
		glLinkProgram(shaderProgram);
 
		// Verify that the linking succeeded
		int isLinked;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

		if (isLinked == false)
			PrintShaderLinkingError(shaderProgram);

		return isLinked != 0;
	}

	void PrintShaderLinkingError(int32_t shaderId)
	{
		std::cout << "=======================================\n";
		std::cout << "Shader linking failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		std::cout << "Info Length : " << maxLength << std::endl;

		// Get shader info log
		char* shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

		std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete shaderProgramInfoLog;
		return;
	}

	// If something went wrong whil compiling the shaders, we'll use this function to find the error
	void PrintShaderCompilationErrorInfo(int32_t shaderId)
	{
		std::cout << "=======================================\n";
		std::cout << "Shader compilation failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		// Get shader info log
		char* shaderInfoLog = new char[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog );
 
		// Print shader info log
		std::cout << "\tError info : " << shaderInfoLog << std::endl;
 
		std::cout << "=======================================\n\n";
		delete shaderInfoLog;
	}

 void print_all() {
   GLuint programme = shaderProgram;
  printf("--------------------\nshader programme %i info:\n", programme);
  int params = -1;
  glGetProgramiv(programme, GL_LINK_STATUS, &params);
  printf("GL_LINK_STATUS = %i\n", params);
  
  glGetProgramiv(programme, GL_ATTACHED_SHADERS, &params);
  printf("GL_ATTACHED_SHADERS = %i\n", params);
  
  glGetProgramiv(programme, GL_ACTIVE_ATTRIBUTES, &params);
  printf("GL_ACTIVE_ATTRIBUTES = %i\n", params);
  for (int i = 0; i < params; i++) {
    char name[64];
    int max_length = 64;
    int actual_length = 0;
    int size = 0;
    GLenum type;
    glGetActiveAttrib (
                       programme,
                       i,
                       max_length,
                       &actual_length,
                       &size,
                       &type,
                       name
                       );
    if (size > 1) {
      for(int j = 0; j < size; j++) {
        char long_name[64];
        sprintf(long_name, "%s[%i]", name, j);
        int location = glGetAttribLocation(programme, long_name);
        printf("  %i) type:%s name:%s location:%i\n",
               i, GL_type_to_string(type), long_name, location);
      }
    } else {
      int location = glGetAttribLocation(programme, name);
      printf("  %i) type:%s name:%s location:%i\n",
             i, GL_type_to_string(type), name, location);
    }
  }
  
  glGetProgramiv(programme, GL_ACTIVE_UNIFORMS, &params);
  printf("GL_ACTIVE_UNIFORMS = %i\n", params);
  for(int i = 0; i < params; i++) {
    char name[64];
    int max_length = 64;
    int actual_length = 0;
    int size = 0;
    GLenum type;
    glGetActiveUniform(
                       programme,
                       i,
                       max_length,
                       &actual_length,
                       &size,
                       &type,
                       name
                       );
    if(size > 1) {
      for(int j = 0; j < size; j++) {
        char long_name[64];
        sprintf(long_name, "%s[%i]", name, j);
        int location = glGetUniformLocation(programme, long_name);
        printf("  %i) type:%s name:%s location:%i\n",
               i, GL_type_to_string(type), long_name, location);
      }
    } else {
      int location = glGetUniformLocation(programme, name);
      printf("  %i) type:%s name:%s location:%i\n",
             i, GL_type_to_string(type), name, location);
    }
  }
  
  _print_programme_info_log(programme);
 }
 void _print_programme_info_log(GLuint programme) {
   int max_length = 2048;
   int actual_length = 0;
   char program_log[2048];
   glGetProgramInfoLog(programme, max_length, &actual_length, program_log);
   printf("program info log for GL index %u:\n%s", programme, program_log);
 }
 const char* GL_type_to_string(GLenum type) {
   switch(type) {
   case GL_BOOL: return "bool";
   case GL_INT: return "int";
   case GL_FLOAT: return "float";
   case GL_FLOAT_VEC2: return "vec2";
   case GL_FLOAT_VEC3: return "vec3";
   case GL_FLOAT_VEC4: return "vec4";
   case GL_FLOAT_MAT2: return "mat2";
   case GL_FLOAT_MAT3: return "mat3";
   case GL_FLOAT_MAT4: return "mat4";
   case GL_SAMPLER_2D: return "sampler2D";
   case GL_SAMPLER_3D: return "sampler3D";
   case GL_SAMPLER_CUBE: return "samplerCube";
   case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
   default: break;
   }
   return "other";
}
	void CleanUp()
	{
		/* Cleanup all the things we bound and allocated */
		glUseProgram(0);
		glDetachShader(shaderProgram, vertexshader);
		glDetachShader(shaderProgram, fragmentShader);

		glDeleteProgram(shaderProgram);


		glDeleteShader(vertexshader);
		glDeleteShader(fragmentShader);
	}
 
	// The handle to our shader program
	GLuint shaderProgram;
 
	// The handles to the induvidual shader
	GLuint vertexshader, fragmentShader, geometryShader;
 
};
#endif // SHADER_H
