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

bool person = false;

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

// =====================================================
// CALIBRACIÓN INDEPENDIENTE DE BRAZOS (CALACA PANDERO)
// =====================================================
// Brazo Derecho (Flechas Izq/Der/Arriba/Abajo + I/K)
float brazoDerOffX = 0.0f;
float brazoDerOffY = 0.0f;
float brazoDerOffZ = 0.0f;

// Brazo Izquierdo + Pandero (Teclas J/U/O + F/G/H)
float brazoIzqOffX = 0.0f;
float brazoIzqOffY = 0.0f;
float brazoIzqOffZ = 0.0f;


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
float	posX = 0.0f,
		posY = 2.0f,
		posZ = 0.0f,
		anim1X = 122.0f,
		anim1Y = -20.0f,
		anim1Z = -170.0f,
		anim2X = 20.0f,
		anim2Z = 300.0f,
		anim3Y = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 180.0f,
		rotBrazoDer = 0.0f,
		rotBrazoIzq = 0.0f,
		rotPiernaDer = 0.0f,
		rotPiernaIzq = 0.0f,
		rotCabezaSi = 0.0f,
		rotCabezaNo = 0.0f,
		giroFantasma = 0.0f,
		giroFantasma2 = 0.0f,
		giroFantasma3 = 0.0f;

float tiempoCaminata = 0.0f;
//float velocidadCaminata = 0.02f;
float anguloMaximo = 80.0f;
float anguloCaminata = 0.0f;
float velocidadCaminata = 2.0f;
float direccionCaminata = 1.0f;

// --- Calaca Pandero ---
float	pandPosX = 18.0f,
pandPosY = 22.0f,
pandPosZ = 0.0f,
pandGiroY = 0.0f,
pandInclinaX = 0.0f,
pandBalanceoZ = 0.0f,
pandShake = 0.0f;



//Para las animaciones

float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		incAnim1X = 0.0f,
		incAnim1Y = 0.0f,
		incAnim1Z = 0.0f,
		rotRodIzqInc = 0.0f,
		giroMonitoInc = 0.0f,
		rotBrazoDerInc= 0.0f,
		rotBrazoIzqInc = 0.0f,
		rotPiernaDerInc = 0.0f,
		rotPiernaIzqInc = 0.0f,
		rotCabezaSiInc = 0.0f,
		rotCabezaNoInc = 0.0f,
		giroFantasmaInc = 0.0f;

bool perspect = false;

#define MAX_FRAMES 18
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float anim1X;		//Variable para PosicionX
	float anim1Y;		//Variable para PosicionY
	float anim1Z;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float giroFantasma;
	float rotBrazoDer;
	float rotBrazoIzq;
	float rotPiernaDer;
	float rotPiernaIzq;
	float rotCabezaSi;
	float rotCabezaNo;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 16;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].anim1X = anim1X;
	KeyFrame[FrameIndex].anim1Y = anim1Y;
	KeyFrame[FrameIndex].anim1Z = anim1Z;
	KeyFrame[FrameIndex].giroFantasma = giroFantasma;

	FrameIndex++;
}

void resetElements(void)
{
	anim1X = KeyFrame[0].anim1X;
	anim1Y = KeyFrame[0].anim1Y;
	anim1Z = KeyFrame[0].anim1Z;
	giroFantasma = KeyFrame[0].giroFantasma;

}

void interpolation(void)
{
	incAnim1X = (KeyFrame[playIndex + 1].anim1X - KeyFrame[playIndex].anim1X) / i_max_steps;
	incAnim1Y = (KeyFrame[playIndex + 1].anim1Y - KeyFrame[playIndex].anim1Y) / i_max_steps;
	incAnim1Z = (KeyFrame[playIndex + 1].anim1Z - KeyFrame[playIndex].anim1Z) / i_max_steps;
	giroFantasmaInc = (KeyFrame[playIndex + 1].giroFantasma - KeyFrame[playIndex].giroFantasma) / i_max_steps;

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
			anim1X += incAnim1X;
			anim1Y += incAnim1Y;
			anim1Z += incAnim1Z;

			rotRodIzq += rotRodIzqInc;
			giroMonito += giroMonitoInc;
			giroFantasma += giroFantasmaInc;

			i_curr_steps++;

			//camera.Position = glm::vec3(posX - 3, posY + 3, posZ - 3);
			
		}
		
	}

	

	if (person == true) {
		
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


		// Mantener la cámara siempre vertical
		glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

		camera.Right = glm::normalize(
			glm::cross(camera.Front, worldUp)
		);

		camera.Up = glm::normalize(
			glm::cross(camera.Right, camera.Front)
		);


		// ANIMACIÓN DE CAMINAR
		
		if (perspect) {
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

void animations() {

	static int paso = 0;
	float velocidad = 0.9f;

	/*float velocidad2 = 3.0f;
	static int paso2 = 0;

	switch (paso2) {

		// 1. SALTO
	case 0:
		anim3Y += velocidad2;
		if (anim3Y >= 10.0f) paso++;
		break;

		// 2. CAÍDA
	case 1:
		anim3Y -= velocidad2;
		if (anim3Y <= 0.0f) {
			anim3Y = 0.0f;
			paso2++;
		}
		break;

		// 3. GIRO SOBRE SU PROPIO EJE
	case 2:
		giroFantasma3 += 2.0f;
		if (giroFantasma3 >= 360.0f) {
			giroFantasma3 = 360.0f;
			paso2++;
		}
		break;

		// 4. QUIETO
	case 3:

		for (int x = 0; x <= 20; x++) {
			anim3Y = 0.0f;
		}
		paso2 = 0;
		break;
	}*/

	switch (paso) {
	case 0:
		giroFantasma2 = 90;
		anim2Z -= velocidad;
		if (anim2Z <= 150) paso++;
		break;

	case 1:
		giroFantasma2 = 0;
		anim2X += velocidad;
		if (anim2X >= 190) paso++;
		break;

	case 2:
		velocidad = 2.5;
		giroFantasma2 = 90;
		anim2Z -= velocidad;
		if (anim2Z <= -300) paso++;
		break;

	case 3:
		velocidad = 2.5;
		giroFantasma2 = 180;
		anim2X -= velocidad;
		if (anim2X <= -190) paso++;
		break;

	case 4:
		velocidad = 2.5;
		giroFantasma2 = 270;
		anim2Z += velocidad;
		if (anim2Z >= 150) paso++;
		break;

	case 5:
		velocidad = 0.9;
		giroFantasma2 = 0;
		anim2X += velocidad;
		if (anim2X >= -30) paso++;
		break;

	case 6:
		giroFantasma2 = -90;
		anim2Z += velocidad;
		if (anim2Z >= 300) paso++;
		break;

	case 7:
		giroFantasma2 = 0;
		anim2X += velocidad;
		if (anim2X >= 20) paso = 0;
		break;
	}

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
	Model cesped("resources/objects/ModelosGustavo/cesped/cesped.obj");
	comprobarArchivo("resources/objects/ModelosGustavo/cesped/cesped.obj");

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
	Model Fantasma("resources/objects/ModelosCarlos/mujer_fantasma/mujer_fantasma.obj");
	comprobarArchivo("resources/objects/ModelosCarlos/mujer_fantasma/mujer_fantasma.obj");
	Model SinCabeza("resources/objects/ModelosCarlos/sin_cabeza/sin_cabeza.obj");
	comprobarArchivo("resources/objects/ModelosCarlos/sin_cabeza/sin_cabeza.obj");
	Model Mujer("resources/objects/ModelosCarlos/woman/woman.obj");
	comprobarArchivo("resources/objects/ModelosCarlos/woman/woman.obj");
	//catrin.initShaders(animShader.ID);

	comprobarArchivo("resources/objects/ModelosHanny/calacaPandero/cabeza.obj");
	Model CaraPandero("resources/objects/ModelosHanny/calacaPandero/cabeza.obj");
	comprobarArchivo("resources/objects/ModelosHanny/calacaPandero/cuerpo.obj");
	Model CuerpoPandero("resources/objects/ModelosHanny/calacaPandero/cuerpo.obj");
	Model PderPandero("resources/objects/ModelosHanny/calacaPandero/pierDer.obj");
	Model PizqAbajoPandero("resources/objects/ModelosHanny/calacaPandero/pierIzqAbajo.obj");
	Model PizqArribaPandero("resources/objects/ModelosHanny/calacaPandero/pierIzqArriba.obj");
	Model BrazoDerPandero("resources/objects/ModelosHanny/calacaPandero/brazoDer.obj");
	Model BrazoArribaPandero("resources/objects/ModelosHanny/calacaPandero/brazoIzq.obj");
	Model Pandero("resources/objects/ModelosHanny/calacaPandero/pandero.obj");

	ModelAnim animacionPersonaje("resources/objects/Personaje1/Arm.dae");
	animacionPersonaje.initShaders(animShader.ID);



	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].anim1X = 0;
		KeyFrame[i].anim1Y = 0;
		KeyFrame[i].anim1Z = 0;

	}

	KeyFrame[0].anim1X = 122.0f;
	KeyFrame[0].anim1Y = -15.0f;
	KeyFrame[0].anim1Z = -170.0f;

	KeyFrame[1].anim1X = 122.0f;
	KeyFrame[1].anim1Y = -10.0f;
	KeyFrame[1].anim1Z = -170.0f;

	KeyFrame[2].anim1X = 122.0f;
	KeyFrame[2].anim1Y = -5.0f;
	KeyFrame[2].anim1Z = -170.0f;

	KeyFrame[3].anim1X = 122.0f;
	KeyFrame[3].anim1Y = 0.0f;
	KeyFrame[3].anim1Z = -130.0f;

	KeyFrame[4].anim1X = 122.0f;
	KeyFrame[4].anim1Y = 0.0f;
	KeyFrame[4].anim1Z = -90.0f;

	//KeyFrame[5].giroFantasma = -90.0f;
	KeyFrame[5].anim1X = 80.0f;
	KeyFrame[5].anim1Y = 0.0f;
	KeyFrame[5].anim1Z = -90.0f;

	//KeyFrame[6].giroFantasma = -90.0f;
	KeyFrame[6].anim1X = 40.0f;
	KeyFrame[6].anim1Y = 0.0f;
	KeyFrame[6].anim1Z = -90.0f;

	//KeyFrame[7].giroFantasma = -90.0f;
	KeyFrame[7].anim1X = -4.0f;
	KeyFrame[7].anim1Y = 0.0f;
	KeyFrame[7].anim1Z = -90.0f;

	//KeyFrame[8].giroFantasma = 0.0f;
	KeyFrame[8].anim1X = -4.0f;
	KeyFrame[8].anim1Y = 0.0f;
	KeyFrame[8].anim1Z = -40.0f;

	KeyFrame[9].anim1X = -4.0f;
	KeyFrame[9].anim1Y = 0.0f;
	KeyFrame[9].anim1Z = 10.0f;

	KeyFrame[10].anim1X = -4.0f;
	KeyFrame[10].anim1Y = 0.0f;
	KeyFrame[10].anim1Z = 60.0f;

	KeyFrame[11].anim1X = -4.0f;
	KeyFrame[11].anim1Y = 0.0f;
	KeyFrame[11].anim1Z = 110.0f;

	KeyFrame[12].anim1X = -4.0f;
	KeyFrame[12].anim1Y = 0.0f;
	KeyFrame[12].anim1Z = 150.0f;

	KeyFrame[13].anim1X = -4.0f;
	KeyFrame[13].anim1Y = 0.0f;
	KeyFrame[13].anim1Z = 190.0f;

	KeyFrame[14].anim1X = -4.0f;
	KeyFrame[14].anim1Y = 0.0f;
	KeyFrame[14].anim1Z = 230.0f;

	KeyFrame[15].anim1X = 122.0f;
	KeyFrame[15].anim1Y = -20.0f;
	KeyFrame[15].anim1Z = -170.0f;

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
		animations();

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

			// El aspecto se toma del tamaño real del framebuffer para que el mapa no se deforme
			int anchoFB = 0, altoFB = 0;
			glfwGetFramebufferSize(window, &anchoFB, &altoFB);
			if (altoFB == 0) altoFB = 1;
			float aspecto = (float)anchoFB / (float)altoFB;

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
		// Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------
		myShader.use();

		//glBindVertexArray(0);
		// ------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		glm::mat4 refPiso = glm::mat4(1.0f);
		glm::mat4 refCementerio = glm::mat4(1.0f);
		glm::mat4 refHacienda = glm::mat4(1.0f);

		refPiso = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.25f,0.2f, 0.35f));
		staticShader.setMat4("model", modelOp);
		piso.Draw(staticShader);

		refHacienda = modelOp = glm::translate(refPiso, glm::vec3(0.0f, -10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 3.4f, 4.0f));
		staticShader.setMat4("model", modelOp);
		Hacienda.Draw(staticShader);

		modelOp = glm::translate(refHacienda, glm::vec3(150.0f, 10.5f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(6.0f, 2.0f, 3.0f));
		staticShader.setMat4("model", modelOp);
		cesped.Draw(staticShader);

		refCementerio = modelOp = glm::translate(refPiso, glm::vec3(0.0f, -2.0f, -500.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.8f,1.2f,1.5f));
		staticShader.setMat4("model", modelOp);
		Cementerio.Draw(staticShader);

		modelOp = glm::translate(refCementerio, glm::vec3(0.0f, 0.0f, -40.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.4f));
		staticShader.setMat4("model", modelOp);
		catrina.Draw(staticShader);


		//glEnable(GL_BLEND); // Activa la mezcla de colores para permitir que los objetos puedan tener transparencia.
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Establece cómo se combinará el color del objeto con el fondo, utilizando el canal alfa para controlar la transparencia.
		modelOp = glm::translate(refCementerio, glm::vec3(anim1X, anim1Y, anim1Z));
		modelOp = glm::rotate(modelOp, glm::radians(giroFantasma), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.3f));
		staticShader.setMat4("model", modelOp);
		Fantasma.Draw(staticShader);
		//glDisable(GL_BLEND);
		// Desactiva la mezcla de colores para evitar que los siguientes objetos dibujados hereden el efecto de transparencia.


		float tumbas=-120;

		for (int i = 0; i < 4; i++) {
			tumbas += 60.5;
			modelOp = glm::translate(refCementerio, glm::vec3(tumbas, -1.0f, -170.0f));
			modelOp = glm::scale(modelOp, glm::vec3(0.55f));
			modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			staticShader.setMat4("model", modelOp);
			Tumbas.Draw(staticShader);
		}

		//Hombre sin cabeza
		modelOp = glm::translate(refCementerio, glm::vec3(anim2X, 1.0f, anim2Z));
		modelOp = glm::rotate(modelOp, glm::radians(giroFantasma2), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.3f));
		staticShader.setMat4("model", modelOp);
		SinCabeza.Draw(staticShader);

		//Mujer
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, anim3Y, 5.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giroFantasma3), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.2f));
		staticShader.setMat4("model", modelOp);
		Mujer.Draw(staticShader);
		
		

		// =====================================================
		// CALACA 2: PANDERO 
		// =====================================================
		glm::mat4 refCuerpoCalacaPandero = glm::translate(
			refPiso,
			glm::vec3(0.0f, 10.5f, 100.0f)
		);
		refCuerpoCalacaPandero = glm::rotate(
			refCuerpoCalacaPandero,
			glm::radians(pandGiroY),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		refCuerpoCalacaPandero = glm::rotate(
			refCuerpoCalacaPandero,
			glm::radians(pandInclinaX),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);
		refCuerpoCalacaPandero = glm::rotate(
			refCuerpoCalacaPandero,
			glm::radians(pandBalanceoZ),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		refCuerpoCalacaPandero = glm::scale(
			refCuerpoCalacaPandero,
			glm::vec3(8.0f)
		);

		// Cuerpo, cabeza y piernas unidos
		staticShader.setMat4("model", refCuerpoCalacaPandero);
		CuerpoPandero.Draw(staticShader);
		CaraPandero.Draw(staticShader);
		PderPandero.Draw(staticShader);
		PizqArribaPandero.Draw(staticShader);
		PizqAbajoPandero.Draw(staticShader);

		// Brazo Derecho
		glm::mat4 refBrazoDer = glm::translate(
			refCuerpoCalacaPandero,
			glm::vec3(brazoDerOffX, brazoDerOffY + pandShake, brazoDerOffZ)
		);
		staticShader.setMat4("model", refBrazoDer);
		BrazoDerPandero.Draw(staticShader);

		// Brazo Izquierdo + Pandero 
		glm::mat4 refBrazoIzq = glm::translate(
			refCuerpoCalacaPandero,
			glm::vec3(brazoIzqOffX, brazoIzqOffY + pandShake, brazoIzqOffZ)
		);
		staticShader.setMat4("model", refBrazoIzq);
		BrazoArribaPandero.Draw(staticShader);
		Pandero.Draw(staticShader);





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
		
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Marcador del personaje (solo en la vista cenital)
		// Cuadro rojo = posición del personaje, cuadro amarillo = hacia dónde mira.
		// Su tamaño es proporcional al zoom para que se vea igual de grande en pantalla.
		// -------------------------------------------------------------------------------------------------------------------------
		if (vistaCenital)
		{
			myShader.use();
			glBindVertexArray(VAO[0]);
			glBindTexture(GL_TEXTURE_2D, t_white);

			float tam = zoomCenital * 0.03f;

			glm::mat4 refMarcador = glm::translate(glm::mat4(1.0f), glm::vec3(posX, 400.0f, posZ));
			refMarcador = glm::rotate(refMarcador, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0f));

			// Cuerpo (acostado sobre el plano XZ)
			modelOp = glm::rotate(refMarcador, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(tam, tam, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", 1.0f, 0.0f, 0.0f);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// Punta hacia el frente del personaje (+Z local)
			modelOp = glm::translate(refMarcador, glm::vec3(0.0f, 1.0f, tam * 0.9f));
			modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(tam * 0.5f, tam * 0.5f, 1.0f));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", 1.0f, 1.0f, 0.0f);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}


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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && person==false)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && person == false)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && person == false)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && person == false)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	
	

	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && person == true)
	{
		posX += direccion.x * velocidad;
		posZ += direccion.z * velocidad;
		perspect = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && person == true)
	{
		posX -= direccion.x * velocidad;
		posZ -= direccion.z * velocidad;
		perspect = true;
	}
		//posZ -= 5.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && person == true)
	{
		posX -= direccion.z * velocidad;
		posZ += direccion.x * velocidad;
		perspect = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && person == true)
	{
		posX += direccion.z * velocidad;
		posZ -= direccion.x * velocidad;
		perspect = true;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		person = false;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		person = true;

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

	

	if (person) {
		//giroMonito += (float)xoffset * 0.1;
		giroMonito -= (float)xoffset * mouseSensitivity;

		// El movimiento vertical inclina solamente la cámara
		cameraPitch += yoffset * mouseSensitivity * 3;

		// Limitar la inclinación para evitar que la cámara dé vueltas
		cameraPitch = glm::clamp(cameraPitch, -10.0f, 120.0f);
	}
	else {
		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}


