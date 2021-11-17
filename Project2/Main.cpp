// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <control.h>
using namespace glm;
using namespace std;


GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {



	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// If Q in Caps Lock is pressed close the window 
	if (GetKeyState(VK_CAPITAL) & 0x8000 != GLFW_RELEASE) {
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		if (mods == GLFW_MOD_SHIFT) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
}

// Generates a number between 2.0 and 10.0
INT randomNum() {
	srand((unsigned)time(0));
	float random;
	random = rand() % (10 - 2 + 1) + 2;
	if (random >10) {
		randomNum();
	}
	return random;
}

void move(GLFWwindow* window, glm::vec3& rotation, glm::vec3& scale) {
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		scale +=0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		scale -= 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		rotation.x += 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		rotation.x -= 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rotation.y += 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotation.y -= 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		rotation.z += 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		rotation.z -= 0.5f;
	}
}

int main(void)
{

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 800, "Pyramid", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ModelID = glGetUniformLocation(programID, "Model");

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);


	Projection = glm::perspective(
		glm::radians(18.0f),
		4.0f / 4.0f,
		0.1f,
		100.0f
	);

	// Camera View
	View = glm::lookAt(
		glm::vec3(10.0f, 10.0f, 30.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	glm::mat4 MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));
	glm::vec3 rotation(0.0f);
	glm::vec3 scale(0.0f);

	Model = glm::rotate(Model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	Model = glm::rotate(Model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	Model = glm::rotate(Model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	Model = glm::scale(Model, scale);
	
	move(window, rotation, scale);
	

	int rn;
	rn = randomNum();

	static const GLfloat g_vertex_buffer_data[] = {
		-1.5f,  1.5f, 0.0f,// First Triangle of the square(base)
		 1.5f,  1.5f, 0.0f,
		-1.5f, -1.5f, 0.0f,
		 1.5f, -1.5f, 0.0f,// Second Triangle of the square(base)
		-1.5f, -1.5f, 0.0f,
		 1.5f,  1.5f, 0.0f,
		-1.5f, -1.5f, 0.0f,//Back Triangle
		-1.5f,  1.5f, 0.0f,
		 0.0f,  0.0f, rn,
		 1.5f, -1.5f, 0.0f,	//Front Triangle
		 1.5f,  1.5f, 0.0f,
		 0.0f,  0.0f, rn,
		-1.5f, -1.5f, 0.0f,// Other
		 1.5f, -1.5f, 0.0f,
		 0.0f,  0.0f, rn,
		-1.5f,  1.5f, 0.0f,
		 1.5f,  1.5f, 0.0f,
		 0.0f,  0.0f, rn,
	};

	float  pyramidColors[54];
	

	//Create an array with random pyramid colors
	for (int i = 0; i < 54; i++) {
		pyramidColors[i] = rand() / float(RAND_MAX);
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidColors), pyramidColors,GL_STATIC_DRAW);

	glfwSetKeyCallback(window, key_callback);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glUniformMatrix4fv(MatrixID,1,GL_FALSE, &MVP[0][0]);

		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
		

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 18); // 18 indices starting at 0 -> 1 triangle


		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::mat4 Projection = glm::mat4(1.0f);

		// Camera View
		View = glm::lookAt(
			glm::vec3(10.0f, 10.0f, 30.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

		Projection = glm::perspective(
			glm::radians(18.0f),
			4.0f / 4.0f,
			0.1f,
			100.0f
		);

		glm::mat4 MVP = Projection * View * Model;

		glm::vec3 rotation(0.0f);
		glm::vec3 scale(0.0f);

		Model = glm::rotate(Model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		Model = glm::scale(Model, scale);

		move(window, rotation, scale);


		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		// 2nd attribute buffer : color
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

	}

	while (glfwWindowShouldClose(window) == 0);
	
	

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
	