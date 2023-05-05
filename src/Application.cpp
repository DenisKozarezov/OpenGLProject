#include "Application.h"

void Application::initOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Application::prepareCubes()
{ 
    litShader = new Shader("shaders/LitShader.vs", "shaders/LitShader.fs");
    unlitShader = new Shader("shaders/UnlitShader.vs", "shaders/UnlitShader.fs");
    texture = new Texture("uvtemplate.DDS");
    static const GLfloat vertices[] = {
       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,

        1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,

       -1.0f, -1.0f, -1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f, -1.0f,

       -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,

        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
    };

    static const GLfloat uv_vertices[] = {
        0.000059f, 0.000004f,
        0.000103f, 0.336048f,
        0.335973f, 0.335903f,
        1.000023f, 0.000013f,
        0.667979f, 0.335851f,
        0.999958f, 0.336064f,
        0.667979f, 0.335851f,
        0.336024f, 0.671877f,
        0.667969f, 0.671889f,
        1.000023f, 0.000013f,
        0.668104f, 0.000013f,
        0.667979f, 0.335851f,
        0.000059f, 0.000004f,
        0.335973f, 0.335903f,
        0.336098f, 0.000071f,
        0.667979f, 0.335851f,
        0.335973f, 0.335903f,
        0.336024f, 0.671877f,
        1.000004f, 0.671847f,
        0.999958f, 0.336064f,
        0.667979f, 0.335851f,
        0.668104f, 0.000013f,
        0.335973f, 0.335903f,
        0.667979f, 0.335851f,
        0.335973f, 0.335903f,
        0.668104f, 0.000013f,
        0.336098f, 0.000071f,
        0.000103f, 0.336048f,
        0.000004f, 0.671870f,
        0.336024f, 0.671877f,
        0.000103f, 0.336048f,
        0.336024f, 0.671877f,
        0.335973f, 0.335903f,
        0.667969f, 0.671889f,
        1.000004f, 0.671847f,
        0.667979f, 0.335851
    };

    // Создание вершинного буфера
    glGenVertexArrays(1, &VBO);
    glBindVertexArray(VBO);

    // Создание массива для хранения всех 36 вершин куба
    glGenBuffers(1, &cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Создание массива для хранения UV-развертки текстуры
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_vertices), uv_vertices, GL_STATIC_DRAW);

    // Отправлять в шейдер 6 параметров: первые 3 - позиция вершин, вторые 3 - нормали
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Application::Application(const unsigned int& width, const unsigned int& height, const char* title)
    : width(width), height(height)
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}

Application::~Application()
{
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteVertexArrays(1, &VBO);
    delete texture;
    delete unlitShader;
    delete litShader;
    glfwTerminate();
}

void Application::init()
{
    initOpenGL();
    prepareCubes();

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    );

    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
    );
}
void Application::render() const
{  
    const float lightRadius = 10.0f;
    glm::vec3 lightPos(-lightRadius / 2.0f, 0, -lightRadius);

    float deltaTime = 0.01f;
    static float angle = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static int mouseState = 0, xx = 0, yy = 0, xprev = 0, yprev = 0;
        static float dx = glm::radians((float)width / 2.), dy = glm::radians((float)height / 2.);
        
        glfwSetMouseButtonCallback(
            window,
            [](auto* window, int button, int action, int mods)
            {
                if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                    mouseState = 1;
                else
                    mouseState = 0;
            }
        );
        glfwSetCursorPosCallback(
            window,
            [](auto* window, double x, double y)
            {
                if (mouseState) {
                    xx = x;
                    dx += glm::radians((float)(xx - xprev));
                    yy = y;
                    dy += glm::radians((float)(yy - yprev));
                }
                xprev = x; yprev = y;
            }
        );
        glm::mat4 RotXMatrix = glm::mat4(1.0);
        RotXMatrix[1][1] = cos(dy);
        RotXMatrix[1][2] = -sin(dy);
        RotXMatrix[2][1] = sin(dy);
        RotXMatrix[2][2] = cos(dy);

        glm::mat4 RotYMatrix = glm::mat4(1.0);
        RotYMatrix[0][0] = cos(dx);
        RotYMatrix[0][2] = sin(dx);
        RotYMatrix[2][0] = -sin(dx);
        RotYMatrix[2][2] = cos(dx);

        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = glm::mat4(1.0);
        glm::mat4 projection = glm::perspective(45.0f, (float)width / height, 0.1f, 1000.0f);
        model = glm::translate(model, glm::vec3(0, 0, -10));
        model = model * RotXMatrix * RotYMatrix;

        // Отображаем кубик
        litShader->use();
        litShader->setUniform("model", model);
        litShader->setUniform("view", view);
        litShader->setUniform("projection", projection);
        litShader->setUniform("myTextureSampler", 0);
        litShader->setUniform("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        litShader->setUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        litShader->setUniform("lightPos", lightPos);
        litShader->setUniform("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));       

        // Отправляем в шейдер позицию каждой вершины (3 числа)
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);
        glVertexAttribPointer(
            0,          // Номер атрибута
            3,          // Количество входящих параметров
            GL_FLOAT,   // Тип входящих параметров
            GL_FALSE,   // Требуется ли нормализация параметров
            0,          // Длина шага между параметрами (0 - означает, что идут друг за другом)
            (void*)0    // Смещение
        );

        // Отправляем в шейдер нормали в каждой вершине (3 числа)
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);
        glVertexAttribPointer(
            1,          // Номер атрибута
            3,          // Количество входящих параметров
            GL_FLOAT,   // Тип входящих параметров
            GL_FALSE,   // Требуется ли нормализация параметров
            0,          // Длина шага между параметрами (0 - означает, что идут друг за другом)
            (void*)0    // Смещение
        );

        // Отправляем в шейдер развертку для отображения текстуры (2 числа)
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            2,          // Номер атрибута
            2,          // Количество входящих параметров
            GL_FLOAT,   // Тип входящих параметров
            GL_FALSE,   // Требуется ли нормализация параметров
            0,          // Длина шага между параметрами (0 - означает, что идут друг за другом)
            (void*)0    // Смещение
        );

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(cubeVAO);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        // Отображаем источник света
        unlitShader->use();
        unlitShader->setUniform("projection", projection);
        unlitShader->setUniform("view", view);

        lightPos.x += std::sin(angle) * (lightRadius / 2.0f) * deltaTime;
        lightPos.z += std::cos(angle) * (lightRadius / 2.0f) * deltaTime;
        angle += 0.01f;

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.05f));
        unlitShader->setUniform("model", model);   

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);
        glVertexAttribPointer(
            0,       
            3,       
            GL_FLOAT,
            GL_FALSE,
            0,       
            (void*)0 
        );

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(cubeVAO);
        glDisableVertexAttribArray(0);

        // Двойная буферизация
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
