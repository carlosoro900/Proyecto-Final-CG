/*---------------------------------------------------------*/
/* ----------------   Proyecto Final  --------------------------*/

/*-----Alumnos:     Orozco Eleuterio José Carlos  , Gustavo  ,  Hanny              ---------------*/
/*----No. Cuenta:    316050717   ,         ,                      ---------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>						//main
#include <stdlib.h>		
#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					//Texture

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <mmsystem.h>
#include <fstream>
//#include <iostream>
#include <string>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);
void moverCamaraCenital(GLFWwindow* window);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

GLuint VBO[3], VAO[3], EBO[3];

//Camera
Camera camera(glm::vec3(0.0f, 10.0f, 3.0f));

float cameraDistance = 30.0f;
float cameraHeight = 10.0f;
float cameraYaw = 0.0f;
float cameraPitch = 10.0f;

float mouseSensitivity = 0.4f;

float MovementSpeed = 0.1f;
GLfloat lastX = SCR_WIDTH / 2.0f,
		lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Vista cenital (plano visto desde arriba)
bool vistaCenital = false;					// Se activa/desactiva con la tecla V
float zoomCenital = 200.0f;					// Mitad de la altura visible, en unidades del mundo
glm::vec2 centroCenital(200.0f, -10.0f);	// Punto (X, Z) del escenario que mira la vista cenital

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

void getResolution(void);
void myData(void);							// De la practica 4
void LoadTextures(void);					// De la práctica 6
unsigned int generateTextures(char*, bool, bool);	// De la práctica 6

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

//Texture
unsigned int	t_smile,
t_toalla,
t_unam,
t_white,
t_ladrillos;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

glm::vec3 lightPosition01(-10.0f, -0.2f, -5.0f);
glm::vec3 lightPosition02(8.0f, 0.0f, 0.0f);
float myTime = 0.0f;
glm::vec3 colorDeLaLuz = glm::vec3(1.0f, 1.0f, 1.0f);

glm::vec3 colores[] = {
	glm::vec3(1.0f, 1.0f, 1.0f), // Blanco
	glm::vec3(0.0f, 0.0f, 1.0f), // Azul
	glm::vec3(1.0f, 0.0f, 0.0f), // Rojo
	glm::vec3(1.0f, 1.0f, 0.0f), // Amarillo
	glm::vec3(0.0f, 1.0f, 0.0f)  // Verde
};

int contador = 0;
int colorActual = 0;


//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 90.0f;
bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posX = -200.0f,
		posY = 2.0f,
		posZ = 100.0f,
		rotRodIzq = 0.0f,
		giroMonito = 180.0f,
		rotBrazoDer = 0.0f,
		rotBrazoIzq = 0.0f,
		rotPiernaDer = 0.0f,
		rotPiernaIzq = 0.0f,
		rotCabezaSi = 0.0f,
		rotCabezaNo = 0.0f;

float tiempoCaminata = 0.0f;
//float velocidadCaminata = 0.02f;
float anguloMaximo = 80.0f;
float anguloCaminata = 0.0f;
float velocidadCaminata = 2.0f;
float direccionCaminata = 1.0f;

float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotRodIzqInc = 0.0f,
		giroMonitoInc = 0.0f,
		rotBrazoDerInc= 0.0f,
		rotBrazoIzqInc = 0.0f,
		rotPiernaDerInc = 0.0f,
		rotPiernaIzqInc = 0.0f,
		rotCabezaSiInc = 0.0f,
		rotCabezaNoInc = 0.0f;

bool perspect = false;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float rotBrazoDer;
	float rotBrazoIzq;
	float rotPiernaDer;
	float rotPiernaIzq;
	float rotCabezaSi;
	float rotCabezaNo;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotRodIzqInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

unsigned int generateTextures(const char* filename, bool alfa, bool isPrimitive)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	
	if(isPrimitive)
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	else
		stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.


	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{

	t_smile = generateTextures("Texturas/awesomeface.png", 1, true);
	t_toalla = generateTextures("Texturas/toalla.tga", 0, true);
	t_unam = generateTextures("Texturas/escudo_unam.jpg", 0, true);
	t_ladrillos = generateTextures("Texturas/bricks.jpg", 0, true);
	//This must be the last
	t_white = generateTextures("Texturas/white.jpg", 0, false);
}



void animate(void) 
{

	lightPosition01.x = 200.0f * cos(myTime);
	lightPosition01.z = 200.0f * sin(myTime);
	myTime += 0.005f;

	lightPosition02.x = 200.0f * cos(myTime);
	lightPosition02.z = 200.0f * sin(myTime);
	myTime += 0.005f;

	contador++;
	if (contador >= 60)
	{
		contador = 0;
		colorActual = (colorActual + 1) % 5;
		colorDeLaLuz = colores[colorActual];
	}

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotRodIzqInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;

			//camera.Position = glm::vec3(posX - 3, posY + 3, posZ - 3);
			
		}
		
	}

	

	if (perspect == true) {
		//float i, 
		//	g = 0;
		//bool permiso_giro = true;
		

		// Convertir el giro del personaje a radianes
		float angle = glm::radians(giroMonito);

		// Calcular la posición detrás del personaje
		camera.Position.x = posX - sin(angle) * cameraDistance;
		camera.Position.z = posZ - cos(angle) * cameraDistance;

		// Aplicar altura de la cámara
		camera.Position.y = posY + cameraHeight;

		// Calcular hacia dónde mira la cámara
		glm::vec3 target(
			posX,
			posY + cameraPitch * 0.1f,
			posZ
		);

		// Orientar la cámara hacia el objetivo
		camera.Front = glm::normalize(target - camera.Position);

		// ANIMACIÓN DE CAMINAR

		// Actualizar ángulo
		anguloCaminata += direccionCaminata * velocidadCaminata * deltaTime;

		// Limitar el movimiento entre -60 y 60
		if (anguloCaminata >= 60.0f) {
			anguloCaminata = 60.0f;
			direccionCaminata = -1.0f;
		}
		else if (anguloCaminata <= -60.0f) {
			anguloCaminata = -60.0f;
			direccionCaminata = 1.0f;
		}

		// Alternar brazos
		rotBrazoDer = anguloCaminata;
		rotBrazoIzq = -anguloCaminata;

		// Alternar piernas
		rotPiernaDer = -anguloCaminata;
		rotPiernaIzq = anguloCaminata;

		perspect = false;
	}
	else {

		/*int i,
			g = 0;
		bool permiso_giro = true;*/

		

		// Convertir el giro del personaje a radianes
		float angle = glm::radians(giroMonito);

		// Calcular la posición detrás del personaje
		camera.Position.x = posX - sin(angle) * cameraDistance;
		camera.Position.z = posZ - cos(angle) * cameraDistance;

		// Aplicar altura de la cámara
		camera.Position.y = posY + cameraHeight;

		// Calcular hacia dónde mira la cámara
		glm::vec3 target(
			posX,
			posY + cameraPitch * 0.1f,
			posZ
		);

		// Orientar la cámara hacia el objetivo
		camera.Front = glm::normalize(target - camera.Position);


	}
	//Vehículo
	if (animacion)
	{
		movAuto_x += 3.0f;
	}


}

// Movimiento de la cámara en la vista cenital (se llama una vez por frame)
void moverCamaraCenital(GLFWwindow* window)
{
	float paso = zoomCenital * 0.02f;	// más lento cuando el plano está más cerca

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		centroCenital.y -= paso;		// arriba en pantalla = -Z
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		centroCenital.y += paso;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		centroCenital.x += paso;		// derecha en pantalla = +X
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		centroCenital.x -= paso;
}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData() {
	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	float verticesPiso[] = {
		// positions          // texture coords
		 10.5f,  10.5f, 0.0f,   4.0f, 4.0f, // top right
		 10.5f, -10.5f, 0.0f,   4.0f, 0.0f, // bottom right
		-10.5f, -10.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-10.5f,  10.5f, 0.0f,   0.0f, 4.0f  // top left 
	};
	unsigned int indicesPiso[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLfloat verticesCubo[] = {
		//Position				//texture coords
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V5
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	//V0
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	//V5

		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V7
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,	//V7

		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	//V3
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	//V3
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	//V2
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f,	//V6
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,	//V2

		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,	//V4
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V7
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f,	//V6

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V0
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f,	//V1
	};

	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(3, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para Piso
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPiso), verticesPiso, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPiso), indicesPiso, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//PARA CUBO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool comprobarArchivo(const std::string& ruta)
{
	std::ifstream archivo(ruta, std::ios::binary);

	if (!archivo.is_open())
	{
		std::cout << "[ERROR] No se pudo abrir: "
			<< ruta << std::endl;

		return false;
	}

	std::cout << "[OK] Archivo encontrado y abierto: "
		<< ruta << std::endl;

	archivo.close();

	return true;
}

int main() {
	// Inicializar GLFW
	if (!glfwInit())
	{
		std::cout << "Error al inicializar GLFW" << std::endl;
		return -1;
	}

	// Obtener resolución del monitor
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	// Obtener modo de video
	const GLFWvidmode* mode = glfwGetVideoMode(monitors);

	// Crear ventana en pantalla completa
	GLFWwindow* window = glfwCreateWindow(
		mode->width,
		mode->height,
		"Proyecto Final - OpenGL",
		monitors,
		nullptr
	);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Registrar callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// Capturar el mouse como Minecraft
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Capturar el mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::cout << "Ancho: " << SCR_WIDTH << std::endl;
	std::cout << "Alto: " << SCR_HEIGHT << std::endl;

	// configure global opengl state
	// -----------------------------
	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);

	

	// build and compile shaders
	// -------------------------
	Shader myShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs"); //To use with primitives
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");	//To use with static models
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");	//To use with skybox
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");	//To use with animated models 
	
	
	vector<std::string> faces{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};


	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------



	Model piso("resources/objects/ModelosGustavo/Piso3.obj");
	comprobarArchivo("resources/objects/ModelosGustavo/Piso3.obj");

	Model CabezaAquaman("resources/objects/Aquaman/Cabeza.obj");
	Model CuerpoAquaman("resources/objects/Aquaman/Cuerpo.obj");
	Model BrazoIzqAquaman("resources/objects/Aquaman/BrazoIzq.obj");
	Model BrazoDerAquaman("resources/objects/Aquaman/BrazoDer.obj");
	Model PiernaIzqAquaman("resources/objects/Aquaman/PiernaIzq.obj");
	Model PiernaDerAquaman("resources/objects/Aquaman/PiernaDer.obj");
	Model BotaIzqAquaman("resources/objects/Aquaman/BotaIzq.obj");
	Model BotaDerAquaman("resources/objects/Aquaman/BotaDer.obj");
	Model catrina("resources/objects/DiaDeMuertos/catrina/catrina.obj");
	Model Hacienda("resources/objects/ModelosHanny/Hacienda/exHacienda2.obj");
	comprobarArchivo("resources/objects/ModelosHanny/Hacienda/exHacienda2.obj");
	Model Pasillo("resources/objects/ModelosCarlos/pasillo_cementerio/pasillo.obj");
	comprobarArchivo("resources/objects/ModelosCarlos/pasillo_cementerio/pasillo.obj");
	Model Paredes("resources/objects/ModelosCarlos/pasillo_cementerio/paredes.obj");
	comprobarArchivo("resources/objects/ModelosCarlos/pasillo_cementerio/paredes.obj");
	Model Cementerio("resources/objects/ModelosCarlos/pasillo_cementerio/cementerio.obj");
	comprobarArchivo("resources/objects/ModelosCarlos/pasillo_cementerio/cementerio.obj");
	Model Tumbas("resources/objects/ModelosCarlos/cementerio/cementerio.obj");
	comprobarArchivo("resources/objects/ModelosCarlos/cementerio/cementerio.obj");
	//catrin.initShaders(animShader.ID);

	ModelAnim animacionPersonaje("resources/objects/Personaje1/Arm.dae");
	animacionPersonaje.initShaders(animShader.ID);



	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}


	// create transformations and Projection
	glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 viewOp = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render
		// Selección de cámara: vista normal (tercera persona) o vista cenital (ortográfica)
		glm::vec3 posCamara;
		if (vistaCenital)
		{
			moverCamaraCenital(window);
			float aspecto = (float)SCR_WIDTH / (float)SCR_HEIGHT;
			posCamara = glm::vec3(centroCenital.x, 500.0f, centroCenital.y);
			// up = -Z: el norte (-Z) queda hacia arriba en pantalla y +X hacia la derecha
			viewOp = glm::lookAt(posCamara, glm::vec3(centroCenital.x, 0.0f, centroCenital.y), glm::vec3(0.0f, 0.0f, -1.0f));
			projectionOp = glm::ortho(-zoomCenital * aspecto, zoomCenital * aspecto, -zoomCenital, zoomCenital, 0.1f, 2000.0f);
			glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
		}
		else
		{
			posCamara = camera.Position;
			viewOp = camera.GetViewMatrix();
			projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//Setup shader for static models
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.45f, 0.44f, 0.30f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.15f, 0.8f, 0.4f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", lightPosition01);
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00000032f);

		staticShader.setVec3("pointLight[2].position", lightPosition02);
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00000032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		//glm::mat4 model = glm::mat4(1.0f);
		//glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		//projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		//viewOp = camera.GetViewMatrix();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		
		//Setup shader for primitives
		myShader.use();
		// view/projection transformations
		//projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 400.0f);
		viewOp = camera.GetViewMatrix();
		// pass them to the shaders
		//myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		myShader.setMat4("projection", projectionOp);
		/**********/

		skyboxShader.use();
		skybox.Draw(skyboxShader, viewOp, projectionOp, camera);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		/*animShader.use();
		animShader.setMat4("projection", projectionOp);
		animShader.setMat4("view", viewOp);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));	// it's a bit too big for our scene, so scale it down
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", modelOp);
		animacionPersonaje.Draw(animShader);*/

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		/*modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));	// it's a bit too big for our scene, so scale it down
		//modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", modelOp);
		AmyBailando.Draw(animShader);*/


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------
		myShader.use();

		//Tener Piso como referencia
		glBindVertexArray(VAO[2]);
		//Colocar código aquí
		modelOp = glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 2.0f, 40.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_ladrillos);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO[0]);
		//Colocar código aquí
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 5.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_unam);
		//glDrawArrays(GL_TRIANGLES, 0, 36); //A lonely cube :(
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/***   Segundo objeto  **/
		/*
		glBindVertexArray(VAO[1]);
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_unam);
		glDrawArrays(GL_TRIANGLES, 0, 36); //A lonely cube :(
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		*/
		glBindVertexArray(0);
		// ------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		/*modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, 25.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f));
		staticShader.setMat4("model", modelOp);
		Iglesia.Draw(staticShader);*/

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 0.0f, 100.0f));
		modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.4f));
		staticShader.setMat4("model", modelOp);
		catrina.Draw(staticShader);

		

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.2f));
		staticShader.setMat4("model", modelOp);
		piso.Draw(staticShader);

		/*modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.0f, 50.0f));
		//modelOp = glm::scale(modelOp, glm::vec3(0.2f));
		staticShader.setMat4("model", modelOp);
		Pasillo.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(110.0f, -1.0f, 0.0f));
		//modelOp = glm::scale(modelOp, glm::vec3(0.2f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		Paredes.Draw(staticShader);*/

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-350.0f, -10.0f, 100.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", modelOp);
		Hacienda.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, -2.0f, 100.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.8f,1.2f,1.5f));
		staticShader.setMat4("model", modelOp);
		Cementerio.Draw(staticShader);

		float tumbas=0;

		for (int i = 0; i < 4; i++) {
			tumbas = i * 60.5;
			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(tumbas, -1.0f, 10.0f));
			modelOp = glm::scale(modelOp, glm::vec3(0.6f));
			modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			staticShader.setMat4("model", modelOp);
			Tumbas.Draw(staticShader);
		}


		
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		
		glm::mat4 refCuerpoAquaman = glm::mat4(1.0f);

		refCuerpoAquaman = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
		refCuerpoAquaman = modelOp = glm::rotate(modelOp, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0f));
		refCuerpoAquaman = modelOp = glm::scale(modelOp, glm::vec3(0.8f));
		staticShader.setMat4("model", modelOp);
		CuerpoAquaman.Draw(staticShader);

		modelOp = glm::translate(refCuerpoAquaman, glm::vec3(0.0f, 1.5f, 0.0f));
		staticShader.setMat4("model", modelOp);
		CabezaAquaman.Draw(staticShader);

		modelOp = glm::translate(refCuerpoAquaman, glm::vec3(-0.75f, 1.5f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(rotBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		BrazoDerAquaman.Draw(staticShader);

		modelOp = glm::translate(refCuerpoAquaman, glm::vec3(0.75f, 1.5f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(rotBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		BrazoIzqAquaman.Draw(staticShader);

		//modelOp = glm::translate(refCuerpoAquaman, glm::vec3(-0.5f, 0.0f, -0.1f));
		glm::mat4 refPiernaDer = modelOp = glm::translate(refCuerpoAquaman, glm::vec3(-0.5f, 0.0f, -0.1f));
		refPiernaDer = modelOp = glm::rotate(modelOp, glm::radians(rotPiernaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		PiernaDerAquaman.Draw(staticShader);

		modelOp = glm::translate(refCuerpoAquaman, glm::vec3(0.5f, 0.0f, -0.1f));
		glm::mat4 refPiernaIzq = modelOp = glm::translate(refCuerpoAquaman, glm::vec3(0.5f, 0.0f, -0.1f));
		refPiernaIzq = modelOp = glm::rotate(modelOp, glm::radians(rotPiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		PiernaIzqAquaman.Draw(staticShader);

		//modelOp = glm::translate(refCuerpoAquaman, glm::vec3(-0.5f, -0.9f, 0.0f));
		modelOp = glm::translate(refPiernaDer, glm::vec3(0.0f, -0.9f, 0.0f));
		//modelOp = glm::rotate(modelOp, glm::radians(rotPiernaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setMat4("model", modelOp);
		BotaDerAquaman.Draw(staticShader);

		//modelOp = glm::translate(refCuerpoAquaman, glm::vec3(0.5f, -0.9f, 0.0f));
		modelOp = glm::translate(refPiernaIzq, glm::vec3(0.0f, -0.9f, 0.0f));
		//modelOp = glm::rotate(modelOp, glm::radians(rotPiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setMat4("model", modelOp);
		BotaIzqAquaman.Draw(staticShader);
		
		
		


		//------------------------------------------------------------------------------------
		//IMPORTANTE
		//------------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------
		
		/*glEnable(GL_BLEND); // Activa la mezcla de colores para permitir que los objetos puedan tener transparencia.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		// Establece cómo se combinará el color del objeto con el fondo, utilizando el canal alfa para controlar la transparencia.
		
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 70.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		BoxF1.Draw(staticShader);
		glDisable(GL_BLEND);
		// Desactiva la mezcla de colores para evitar que los siguientes objetos dibujados hereden el efecto de transparencia.
		//------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------
		*/

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		/*skyboxShader.use();
		skybox.Draw(skyboxShader, viewOp, projectionOp, camera);*/

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//skybox.Terminate();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode) 
{

	float velocidad = 0.5f * (float)deltaTime;

	// Obtener el ángulo de giro del personaje
	float angulo = glm::radians(giroMonito);

	// Calcular la dirección según la rotación
	glm::vec3 direccion(
		sin(angulo),
		0.0f,
		cos(angulo)
	);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	*/
	

	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		posX += direccion.x * velocidad;
		posZ += direccion.z * velocidad;
		perspect = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		posX -= direccion.x * velocidad;
		posZ -= direccion.z * velocidad;
		perspect = true;
	}
		//posZ -= 5.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		posX -= direccion.z * velocidad;
		posZ += direccion.x * velocidad;
		perspect = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		posX += direccion.z * velocidad;
		posZ -= direccion.x * velocidad;
		perspect = true;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	/*if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;*/
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//perspectiva
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		perspect = !perspect;
		
	}

	// Alternar la vista cenital (plano visto desde arriba) con la tecla 'V'
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		vistaCenital = !vistaCenital;
	}

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	//camera.ProcessMouseMovement(xoffset, yoffset);

	//giroMonito += (float)xoffset * 0.1;
	giroMonito -= (float)xoffset * mouseSensitivity;

	// El movimiento vertical inclina solamente la cámara
	cameraPitch += yoffset * mouseSensitivity*3;

	// Limitar la inclinación para evitar que la cámara dé vueltas
	cameraPitch = glm::clamp(cameraPitch, -10.0f, 120.0f);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}


