#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <complex>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

std::complex<float> center {-2,-2};
float pixelToDistance = 0.0066667;
float zooms = 0;
float scalar = 0;


void checkCompileErrors(unsigned int ID, std::string type)
{
    using namespace std;
	int success;
	char	infolog[512];
	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infolog);
			cout << "shader compile error:" << infolog << endl;
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infolog);
			cout << "program link error:" << infolog << endl;
		}
	}
}


string readFile(const char *filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint createShaderProgram() {
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vfprogram = glCreateProgram();

	string vertShaderStr = readFile("vertShader.glsl");
	string fragShaderStr = readFile("fragShader.glsl");
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	checkCompileErrors(vShader, "VERTEX");
	checkCompileErrors(fShader, "FRAGMENT");

	glAttachShader(vfprogram, vShader);
	glAttachShader(vfprogram, fShader);
	glLinkProgram(vfprogram);

	return vfprogram;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float temp;
    switch (key)
    {
    case GLFW_KEY_LEFT:
        temp = center.real() - 0.25*pow(10, -zooms);
        center.real(temp);
        break;
    case GLFW_KEY_RIGHT:
        temp = center.real() + 0.25*pow(10, -zooms);
        center.real(temp);
        break;
    case GLFW_KEY_UP:
        temp = center.imag() + 0.25*pow(10, -zooms);
        center.imag(temp);
        break;
    case GLFW_KEY_DOWN:
        temp = center.imag() - 0.25*pow(10, -zooms);
        center.imag(temp);
        break;
    case GLFW_KEY_MINUS:
        zooms -= 0.1;
        break;
    case GLFW_KEY_EQUAL:
        zooms += 0.1;
        break;

    }
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
    glfwSetKeyCallback(window, key_callback);
}


void display(GLFWwindow* window, double currentTime) {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
    auto mgcenterR = glGetUniformLocation(renderingProgram, "center.real");
	auto mgcenterI = glGetUniformLocation(renderingProgram, "center.imag");
	auto mgscalar = glGetUniformLocation(renderingProgram, "scalar");
	glUniform1f(mgcenterR, center.real());
	glUniform1f(mgcenterI, center.imag());
	scalar = pixelToDistance*pow(10,-zooms);
	glUniform1f(mgscalar, scalar);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "mandelbrot", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}






























