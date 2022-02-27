#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "load_shader.hpp"

using namespace std;

GLuint load_shader(const char* vertex_shader_path, const char* fragment_shader_path)
{
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    string vertex_shader_code;
    ifstream vertex_shader_stream(vertex_shader_path, ios::in);
    if (vertex_shader_stream.is_open()) 
    {
        string line = "";
        while (getline(vertex_shader_stream, line))
        {
            vertex_shader_code += "\n" + line;
        }
        vertex_shader_stream.close();
    }
    else 
    {
        cout << "Could not open the vertex shader file: " << vertex_shader_path << endl;
    }

    string fragment_shader_code;
    ifstream fragment_shader_stream(fragment_shader_path, ios::in);
    if (fragment_shader_stream.is_open()) 
    {
        string line = "";
        while (getline(fragment_shader_stream, line))
        {
            fragment_shader_code += "\n" + line;
        }
        fragment_shader_stream.close();
    }
    else
    {
        cout << "Could not open the fragment shader file: " << fragment_shader_path << endl;
    }

    GLint result = GL_FALSE;
    int info_log_len;

    cout << "Compiling vertex shader: " << vertex_shader_path << endl;
    const char* vertex_source_ptr = vertex_shader_code.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_source_ptr, NULL);
    glCompileShader(vertex_shader_id);

    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_len);

    if (info_log_len > 0) 
    {
        vector<char> vertex_shader_error(info_log_len + 1);
        glGetShaderInfoLog(vertex_shader_id, info_log_len, NULL, &vertex_shader_error[0]);
        cout << &vertex_shader_error[0] << endl;
    }

    cout << "Compiling fragment shader: " << fragment_shader_path << endl;
    const char* fragment_source_ptr = fragment_shader_code.c_str();
    glShaderSource(fragment_shader_id, 1, &fragment_source_ptr, NULL);
    glCompileShader(fragment_shader_id);

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_len);

    if (info_log_len > 0) 
    {
        vector<char> fragment_shader_error(info_log_len + 1);
        glGetShaderInfoLog(fragment_shader_id, info_log_len, NULL, &fragment_shader_error[0]);
        cout << &fragment_shader_error[0] << endl;
    }

    cout << "Linking the program with our shaders" << endl;
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_len);

    if (info_log_len > 0) 
    {
        vector<char> program_error(info_log_len + 1);
        glGetProgramInfoLog(program_id, info_log_len, NULL, &program_error[0]);
        cout << &program_error[0] << endl;
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}
