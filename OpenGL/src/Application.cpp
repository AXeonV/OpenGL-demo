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

GLuint WIDTH = 1200, HEIGHT = 1200;

Camera Cam(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
GLboolean firstMouse = true;
GLboolean cursorEnabled = true;

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

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, u8"Èü²©ÃÎºËÐÇ", nullptr, nullptr);
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
	Shader Cube("res/shaders/cube/vertex.glsl", "res/shaders/cube/fragment.glsl");
	Shader Lamp("res/shaders/light/vertex.glsl", "res/shaders/light/fragment.glsl");

	/* init vertex data */
	GLfloat vertices[] = {
		    // Position           // TexCoord  // Color           // Normal
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,

				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
				 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,

				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f
	};

	/* setup vertex & pointer */
	GLuint VBO, cube_VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &cube_VAO);
	glBindVertexArray(cube_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	GLuint light_VAO;
	glGenVertexArrays(1, &light_VAO);
	glBindVertexArray(light_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	/* load texture */
	GLuint texture1, texture2;
	set_texture(texture1, "res/textures/wall.jpg");
	set_texture(texture2, "res/textures/cbc.jpg");

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
	/* init Lamp_position */
	glm::vec3 LampPosition(-4.0f, 2.0f, -4.0f);

	/* GAME LOOP */
	while (!glfwWindowShouldClose(window)) {
		/* render */

		/* clear */
		glClearColor(0.28f, 0.32f, 0.48f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* setup texture */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		Cube.setInt("Utexture1", 0);
		Cube.setInt("Utexture2", 1);

		/* setup transformation */
		glm::mat4 model = glm::mat4(1.0f); 
		glm::mat4 view = Cam.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(Cam.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		/* draw lamp */
		Lamp.Use();
		GLfloat timeVal = (GLfloat)glfwGetTime();
		LampPosition.x = -4.0f * sin(timeVal * 0.5f);
		LampPosition.y = 2.0f * sin(timeVal * 0.5f);
		LampPosition.z = -4.0f * cos(timeVal * 0.5f);
		model = glm::translate(model, LampPosition);
		model = glm::scale(model, glm::vec3(0.2f));
		Lamp.setMat4("model", glm::value_ptr(model));
		Lamp.setMat4("view", glm::value_ptr(view));
		Lamp.setMat4("projection", glm::value_ptr(projection));

		glBindVertexArray(light_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		/* draw cubes */
		Cube.Use();
		Cube.setVec3("Lcolor", 1.0f, 1.0f, 1.0f);
		Cube.setVec3("light", 1.0f, 1.0f, 1.0f);
		Cube.setVec4("material", 0.1f, 0.5f, 3.0f, 32.0f);
		Cube.setVec3("Lpos", LampPosition.x, LampPosition.y, LampPosition.z);
		Cube.setVec3("Vpos", Cam.Position.x, Cam.Position.y, Cam.Position.z);

		for (int i = 0; i < 10; ++i) {
			/* setup transformation */
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			Cube.setMat4("model", glm::value_ptr(model));
			Cube.setMat4("view", glm::value_ptr(view));
			Cube.setMat4("projection", glm::value_ptr(projection));
			
			GLfloat startOffset = (i + 1) * 20.0f;
			Cube.setVec2("u_resolution", (GLfloat)WIDTH, (GLfloat)HEIGHT);
			Cube.setFloat("u_time", timeVal + startOffset);

			glBindVertexArray(cube_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		/* update */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* delete buffer */
	glDeleteVertexArrays(1, &cube_VAO);
	glDeleteBuffers(1, &VBO);

	/* terminate GLFW */
	glfwTerminate();
	return 0;
}

static void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		cursorEnabled ^= 1;
		if (cursorEnabled) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			firstMouse = false;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstMouse = true;
		}
	}

	if (key == GLFW_KEY_W)
		Cam.ProcessKeyboard(FORWARD);
	if (key == GLFW_KEY_S)
		Cam.ProcessKeyboard(BACKWARD);
	if (key == GLFW_KEY_A)
		Cam.ProcessKeyboard(LEFT);
	if (key == GLFW_KEY_D)
		Cam.ProcessKeyboard(RIGHT);
}

static void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height) {
	glViewport(0, 0, WIDTH = width, HEIGHT = height);
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

	Cam.ProcessMouseMovement(xoffset, yoffset);
}

static void scroll_callback(GLFWwindow* window, GLdouble xoffset, GLdouble yoffset) {
	Cam.ProcessMouseScroll(static_cast<GLfloat>(yoffset));
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