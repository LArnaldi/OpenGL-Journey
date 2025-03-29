#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


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


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "ZZA ROMA", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Cor cazzo che se apre cojone" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "a pise ns'è caricato glad" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Shader ourShader("./Shaders/3.3.shader.vs", "./Shaders/3.3.shader.fs");

	float vertices[] = {
	//positions			colors
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
	0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // top center
	0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom right
	};

	//ogni texture ha coordinate da 0,0 a 1,1. Qua je stamo a quale coordinata della texture 
	//cotrispondono i vertici der triangolo, sta roba se chiama texture sampling
	float textCoords[] = {
		0.0f, 0.0f, // lower-left corner
		1.0f, 0.0f, // lower-right corner
		0.0f, 1.0f  // top-center corner
	};

	//qua je dico come deve gesti le coordinate fuori il dominio 0,0 - 1,1.
	//glTexParameter*
	//1: texture target, 2: je dico qualle asse(x,y,z)->(s,t,r), 3:er modo
	//i modi so:
	//GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	//se mettemo er border, je dovemo pure da er colore:
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		float timeValue = glfwGetTime();

		float alphaColor = (sin(timeValue) / 2.0f) + 0.5f;
		ourShader.setFloat("alphaColor", alphaColor);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwPollEvents();

		glfwSwapBuffers(window);

		
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}