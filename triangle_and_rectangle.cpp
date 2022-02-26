#include <glew.h>
#include <glfw3.h>
#include <iostream>

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLchar* vertex_shader_src =
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
	"}\0";

const GLchar* fragment_shader_src =
	"#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main(int argc, char** argv)
{
	/* GLFW initialization */
	if (!glfwInit())
	{
		cout << "Could not initialize GLFW" << endl;
		return EXIT_FAILURE;
	}

	/* Set the minimum required version of OpenGL (major) */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	/* Set the minimum required version of OpenGL (minor) */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	/* Set the profile for which the created context */
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Disable resize for window */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/* Create window parameters */
	GLuint glfw_window_width = 1000;
	GLuint glfw_window_height = 700;

	/* Create the window */
	GLFWwindow* window = glfwCreateWindow(glfw_window_width, glfw_window_height, "Window", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Could not create the window" << endl;
		/* Terminate the GLFW library */
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	/* Set the required callback functions */
	glfwSetKeyCallback(window, key_callback);

	/* Set GL_TRUE to use the latest techniques to manage OpenGL functionality */
	glewExperimental = GL_TRUE;
	/* GLFW initialization */
	if (glewInit() != GLEW_OK)
	{
		cout << "Could not initialize GLEW" << endl;
		return EXIT_FAILURE;
	}

	int width, height;
	/* Get window size from GLFW */
	glfwGetFramebufferSize(window, &width, &height);
	/* Set window size to OpenGL*/
	glViewport(0, 0, width, height);

	/* Create object of shader and set type of shader */
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	/* Bind the shader source code to the shader object and compile it */
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);

	GLint compile_status;
	GLchar compile_status_log[512];

	/* Checking the success of the shader compile */
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, compile_status_log);
		cout << "Shader compilation failed: " << compile_status_log << endl;
	}

	/* Create object of fragment shader and set type of shader */
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	/* Bind the fragment shader source code to the shader object and compile it */
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);

	/* Checking the success of the fragment shader compile */
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, compile_status_log);
		cout << "Fragment shader compilation failed:" << compile_status_log << endl;
	}

	/* Create shader program for linking our shaders */
	GLuint shader_program = glCreateProgram();
	/* Add our shaders to the shader program */
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	/* Linking shaders */
	glLinkProgram(shader_program);

	/* Checking the success of the linking shaders */
	glGetProgramiv(shader_program, GL_LINK_STATUS, &compile_status);
	if (!compile_status) 
	{
		glGetProgramInfoLog(shader_program, 512, NULL, compile_status_log);
		cout << "Shader linking failed: " << compile_status_log << endl;
	}

	/* Delete shaders after linking */
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	/* Create 2d triangle matrix */
	//GLfloat vertices[] = 
	//{
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f,  0.5f, 0.0f
	//};

	/* Create 2d quad matrix */
	GLfloat vertices[] = 
	{
		0.5f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	GLuint indices[] = 
	{
		0, 1, 3,
		1, 2, 3
	};

	GLuint VBO, VAO, IBO;
	/* Create VBO (vertex buffer objects), VAO (vertex array object) and IBO (index buffer object) */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	/* Bind the vertex array object first, then bindand set vertex buffer(s) and attribute pointer(s) */
	glBindVertexArray(VAO);

	/* Bind an object type to an object buffer */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/* Copy data vertices to buffer */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Bind an object type to an object buffer */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	/* Copy indexes to buffer */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Interpreting vertex data */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	/* Uncomment fot wireframe polygons */
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* Сreate a game loop so that the program works until it is closed */
	while (!glfwWindowShouldClose(window))
	{
		/* Check if any events have been activiated(key pressed, mouse moved etc.) and call corresponding response functions */
		glfwPollEvents();

		/* Set the required color that will clear the screen */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		/* Clear the color buffer */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		/* For triangle */
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		/* For quad */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		/* Swap the screen buffers */
		glfwSwapBuffers(window);
	}

	/* Properly de-allocate all resources once they've outlived their purpose */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	/* Terminate the GLFW library */
	glfwTerminate();

	return EXIT_SUCCESS;
}
