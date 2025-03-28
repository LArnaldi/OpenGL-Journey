#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* Be sure to include GLAD before GLFW. 
	The include file for GLAD includes the required OpenGL headers behind the scenes (like GL/gl.h) 
	so be sure to include GLAD before other header files that require OpenGL (like GLFW).
*/

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

bool checkShaderCompiling(unsigned int shader) {
	//controllo compilazione shader
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return success;
}

bool checkProgramCompiling(unsigned int program) {
	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return success;
}

unsigned int createVertexShader()
{
	//creo come it il vertex shader e je dico a opengl che tipo de shader è
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//attacco il source allo shader e lo compilo
	//1: shader, 2: quante stringhe passamo come sourcecode, 3: er source code, 4: array de lunghezze bo
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	if (!checkShaderCompiling(vertexShader))
		return NULL;
	return vertexShader;
}

unsigned int createFragmentShader() {

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	if (!checkShaderCompiling(fragmentShader))
		return NULL;
	return fragmentShader;
}

unsigned int linkShaders(unsigned int vertexShader, unsigned int fragmentShader) {

	//creo un program che ce da un'id
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//attaccamo gli shader ar program e poi famo er link
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//controllamo se er link va bene
	if (!checkProgramCompiling(shaderProgram))
		return NULL;

	//uso er program
	//ogni shader e rendering call dopo l'uso del program, userà sto program e gli shader dentrolo
	glUseProgram(shaderProgram);
	
	return shaderProgram;
}

int main()
{
	//initialize glfw
	glfwInit();

	/*glfw configuration*/
	//tell to gflw we're using opengl 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//tell to glfw to use the core profile of opengl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//creo la finestra, je do larghezza, altezza, titolo. monitor e share le lascio null ma che cazzo so
	GLFWwindow* window = glfwCreateWindow(800, 600, "ZZA ROMA", NULL, NULL);

	//se la finestra nse crea te dico cor cazzo e basta
	if (window == NULL)
	{
		std::cout << "Cor cazzo che se apre cojone" << std::endl;
		glfwTerminate();
		return -1;
	}

	//mo che ho creato er contesto finestra, lo setto come corrente
	glfwMakeContextCurrent(window);


	//vedemo se glad s'è caricato mo, ner caso a pise ns'è caricato glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "a pise ns'è caricato glad" << std::endl;
		return -1;
	}

	//setto er viewport, i parametri so x e y del bottom left corner, larghezza e altezza gang
	glViewport(0, 0, 800, 600);

	//setto er callback de quando l'utente resiza la finestra, 
	// e je do la window e er callback che se pia de base la finestra, la larghezza e l'altezza
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//creo il vertex shader
	unsigned int vertexShader = createVertexShader();
	//creo il fragment shader
	unsigned int fragmentShader = createFragmentShader();

	//combino i due shaderzzzz
	if (vertexShader == NULL || fragmentShader == NULL)
		return -1;

	//linkamo gli shaderz
	unsigned int shaderProgram = linkShaders(vertexShader, fragmentShader);
	if (shaderProgram == NULL)
		return -1;
	//mo cancellamo li shader che nce serveno più visto che l'avemo linkatis
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* TRIANGOLO */
	//er triangolo deve esse normalizzato, quindi x y z min -1.0f max +1.0f
	//es:
	//float vertices[] = { //mettemo le z a 0 cosi famo un triangolo con depth a 0 e quindi tipo in 2d dai
	//-0.5f, -0.5f, 0.0f,
	// 0.5f, -0.5f, 0.0f,
	// 0.0f,  0.5f, 0.0f
	//};
	/* RETTANGOLO */
	//usamo l'index rendering
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,	//first triangle
		1, 2, 3		//second triangle
	};
	//genero il vertex buffer object a cui passo tutti i dati possibili cosi che la gpu cha accesso a tutto velocemente
	//creo er vertex array object, lo bindamo e ogni chiamata seguente sugli attributi vertex starà dentro sto VAO.
	//quando configuriamo i pointer degli attributi vertex dovemo fa le chiamate solo na volta e quando volemo disegnalli
	//questo rende lo switch tra vertex data e config di attributi semplice come bindare un nuovo VAO
	//se er VAO non viè bindato bene, opengl disegna er cazzo
	//EBO sarebbe nbuffer che cha indici che opengl usa pe decide quali vertici disegnare
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//mo bindo il buffer, posso farlo con diversi tipi, e posso utilizza quanti buffer me pare a patto che siano de tipi differenti
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	//qua sto a mette i vertici nel buffer. je specifico er tipo de buffer, la grandezza der dato in byte, er dato, e il metodo de disegno
	//metodi de disegno:
	//1. GL_STREAM_DRAW: er dato è settato solo na vorta e vie usato dalla gpu npar de vorte
	//2. GL_STATIC_DRAW: er dato è settato solo na vorta ma usato nbotto de vorte
	//3. GL_DYNAMIC_DRAW: er dato cambia nbotto e vie usato nbotto de vorte
	//all'element je passo l'indici perche ebo serve a punta ai dati de vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//sapemo che nell'array de buffer del vertice ce stanno x y z, e so 3 vertici
	//ogni coordinata so 32bit (4Byte), e i vertici nell'array so attaccati de seguito
	//quindi er Byte 0-4 è ad esempio la x del primo vertice
	//dovemo di ar puntatore de opengl come movese
	//1: vertice co location 0, 2: size del vertex attribute (so 3 punti quindi 3), 3: So float, 4: lo stride, ovvero lo lo spazio tra tra attributi consecutivi
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//pulimo er buffer mo che c'avemo tutto
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//questo serve per i wireframe se li volemo
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//finche' l'utente nun chiude 
	while (!glfwWindowShouldClose(window))
	{

		//funzione custom per l'inpuuuuts
		processInput(window);
		
		//se renderizzaaaa
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//usamo er programma
		glUseProgram(shaderProgram);

		//ribindamo vao cosi lo disegnamo
		glBindVertexArray(VAO);

		//se disegna
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//vedemo npo se ce stanno cose che se triggerano e le famo partiii
		glfwPollEvents();
		//se swappano i bufferz
		glfwSwapBuffers(window);

		
	}

	//se volemo deallocamo tutto
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	//fermite
	glfwTerminate();
	return 0;
}