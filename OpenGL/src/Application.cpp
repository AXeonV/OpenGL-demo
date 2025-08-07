#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

static void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height);
static void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);
static void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
static void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset);
static void set_texture(GLuint& Tid, const GLchar* filename);

const GLuint WIDTH = 1200, HEIGHT = 900;

Camera Falcon(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
GLboolean firstMouse = true;

int main(void) {
	/* setup GLFW */
	glfwInit();

	std::cout << "using OpenGL 3.3" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, u8"ÃÎ ºË Áú Ã¨", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // fps:60

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* setup GLAD */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* setup OpenGL */
	glEnable(GL_DEPTH_TEST);

	/* setup shaders */
	Shader Xeon("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");

	/* init vertex data */
	GLfloat vertices[] = {
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 1.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f
	};

	/* setup vertex & pointer */
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	/* load texture */
	GLuint texture1, texture2;
	set_texture(texture1, "res/textures/wall.jpg");
	set_texture(texture2, "res/textures/cbc.jpg");

	/* get uniform location */
	GLuint texture1Loc = glGetUniformLocation(Xeon.Program, "Utexture1");
	GLuint texture2Loc = glGetUniformLocation(Xeon.Program, "Utexture2");
	GLuint modelLoc = glGetUniformLocation(Xeon.Program, "model");
	GLuint viewLoc = glGetUniformLocation(Xeon.Program, "view");
	GLuint projectionLoc = glGetUniformLocation(Xeon.Program, "projection");

	/* init cube_position */
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.8f, -1.8f, -2.5f),
		glm::vec3(-5.8f, -1.5f, -12.3f),
		glm::vec3( 3.4f, -0.8f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -1.7f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.8f,  1.0f, -1.5f)
	};

	/* GAME LOOP */
	while (!glfwWindowShouldClose(window)) {
		/* render */

		/* clear */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* activate shader */
		Xeon.Use();

		/* setup texture */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(texture1Loc, 0);
		glUniform1i(texture2Loc, 1);

		/* setup transformation */
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = Falcon.GetViewMatrix();
		projection = glm::perspective(glm::radians(Falcon.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		/* draw */
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i) {
			/* setup transformation */
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = static_cast<GLfloat>(glm::radians((sin(glfwGetTime()) * 180.0f) + 20.0f * i));
			model = glm::rotate(model, angle, glm::vec3(0.8f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		/* update */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* delete buffer */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	/* terminate GLFW */
	glfwTerminate();
	return 0;
}

static void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W)
		Falcon.ProcessKeyboard(FORWARD);
	if (key == GLFW_KEY_S)
		Falcon.ProcessKeyboard(BACKWARD);
	if (key == GLFW_KEY_A)
		Falcon.ProcessKeyboard(LEFT);
	if (key == GLFW_KEY_D)
		Falcon.ProcessKeyboard(RIGHT);
}

static void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height) {
	glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, GLdouble xposIn, GLdouble yposIn) {
	GLfloat xpos = static_cast<GLfloat>(xposIn);
	GLfloat ypos = static_cast<GLfloat>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	Falcon.ProcessMouseMovement(xoffset, yoffset);
}

static void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset) {
	Falcon.ProcessMouseScroll(static_cast<GLfloat>(yoffset));
}

static void set_texture(GLuint& Tid, const GLchar* filename) {
	glGenTextures(1, &Tid);
	glBindTexture(GL_TEXTURE_2D, Tid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint img_width, img_height, nr_channels;
	unsigned char* image = stbi_load(filename, &img_width, &img_height, &nr_channels, 0);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Failed to load image: " << filename << std::endl;

	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}