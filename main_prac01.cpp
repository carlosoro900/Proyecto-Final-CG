/*---------------------------------------------------------*/
/* ----------------   Práctica 1 --------------------------*/
/*-----------------    2027-1   ---------------------------*/
/*------------- (Orozco Eleuterio José Carlos) ---------------*/
#include <glew.h>
#include <glfw3.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>


#include <iostream>
//#include "include/glfw3.h"

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO[2], VAO[2], EBO[2];
GLuint shaderProgramYellow, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderYellow = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 1.0f, 0.0f);									\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	float vertices[] = 
	{
		// positions             //Color
		-0.14753f, 0.54295f, 0.0f,   1.0f, 0.6f, 0.0f, //A,0
		-0.14753f, 0.43417f, 0.0f,   1.0f, 0.6f, 0.0f,  //B,1
		-0.233f, 0.47561f, 0.0f,  1.0f, 0.6f, 0.0f,//C,2

		-0.36768f, 0.46784f, 0.0f, 0.7098f, 0.4902f, 0.2824f,  //D,3

		-0.4f, 0.4f, 0.0f,   0.4f, 0.2f, 0.0f,  //E,4
		-0.53344f, 0.42381f, 0.0f,   0.4f, 0.2f, 0.0f,  //F,5
		-0.57747f, 0.34093f, 0.0f, 0.4f, 0.2f, 0.0f, //G,6
		-0.48423f, 0.25028f, 0.0f, 0.4f, 0.2f, 0.0f, //H,7

		-0.39617f, 0.15704f, 0.0f, 0.7098f, 0.4902f, 0.2824f, //I,8
		-0.30293, 0.16222, 0.0f, 0.7098f, 0.4902f, 0.2824f, //J,9

		-0.02839, 0.36165, 0.0f, 0.7098f, 0.4902f, 0.2824f, //K,10
		0.19176, 0.37201, 0.0f, 0.7098f, 0.4902f, 0.2824f, //L,11
		0.36011, 0.27877, 0.0f, 0.7098f, 0.4902f, 0.2824f, //M,12
		0.10888, -0.10973, 0.0f, 0.7098f, 0.4902f, 0.2824f, //N,13

		0.33162, -0.29621, 0.0f,  0.2353f, 0.12157f, 0.004f,  //O,14
		0.45853, 0.05862, 0.0f, 0.7098f, 0.4902f, 0.2824f, //P,15
		0.43781, 0.11301, 0.0f, 0.7098f, 0.4902f, 0.2824f, //Q,16

		0.47666, 0.17776, 0.0f,   0.7098f, 0.4902f, 0.2824f,//R,17
		0.4, 0.2, 0.0f,   0.7098f, 0.4902f, 0.2824f,//S,18
		0.14255, -0.30139, 0.0f,  0.2353f, 0.12157f, 0.004f,//T,19
		0.22543, -0.2211, 0.0f,  0.2353f, 0.12157f, 0.004f,//U,20
		-0.02321, -0.30398,0.0f, 0.2353f, 0.12157f, 0.004f,//V,21
		- 0.08796, -0.1952, 0.0f,  0.2353f, 0.12157f, 0.004f,//W,22
		-0.20451, -0.30657, 0.0f,  0.2353f, 0.12157f, 0.004f//X,23
		//0.0f,  0.0f, 0.0f,  //Y,24
		//0.5f,  0.7f, 0.0f,  //Z,25
	};

	unsigned int indices[] =
	{
		0, 1, 2, //oreja
		4, 5, 6, //boca
		4, 6, 7,
		2, 10, 9, //cuello
		21, 22, 23, //pata delantera
		14, 19, 20, //pata trasera
		16, 17, 18, //cola
		8, 7, 3, 2, 9, //cara
		21, 9, 10, 13, //cuerpo
		11, 10, 13, 12,
		14, 13, 12, 15
	};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);



	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &myFragmentShaderYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderYellow);
	glDeleteShader(fragmentShaderColor);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 1 20271", NULL, NULL); //Bug?
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//My Functions
	//Setup Data to use
	myData();
	//To Setup Shaders
	setupShaders();
    
    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//Display Section
		glUseProgram(shaderProgramColor);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

		glPointSize(10.0);
		glDrawElements(GL_TRIANGLES, 21, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_LINE_LOOP, 18, 3);
		//glDrawArrays(GL_LINE_STRIP, 0, 4);
		//glDrawArrays(GL_POINTS, 0, 24);
		glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void*)(21 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(26 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(30 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(34 * sizeof(float)));


		glBindVertexArray(0);
		glUseProgram(0);

		//End of Display Section

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);  //Close
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

//Erase in order to work