#pragma once
#include "shaders/Shader.h"
#include "textures/Texture.h"

class Application
{
private:
	GLFWwindow* window;
	Shader* litShader;
	Shader* unlitShader;
	Texture* texture;
	GLuint VBO, cubeVAO;
	GLuint uvbuffer;
	int width, height;

	void initOpenGL();
	void prepareCubes();
public:
	Application() = delete;
	Application(const unsigned int& width, const unsigned int& height, const char* title);
	~Application();

	void init();
	void render() const;
};

