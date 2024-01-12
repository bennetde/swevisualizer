#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "renderer/shader.h"
#include "mesh/plane.h"
#include "renderer/camera.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void processKeyboard(GLFWwindow *window);
void processMouse(GLFWwindow *window, double xpos, double ypos);

int render();

// settings
unsigned int SCR_WIDTH = 1000;
unsigned int SCR_HEIGHT = 800;
const std::filesystem::path vertexShaderPath(std::filesystem::current_path() / "resources/shaders/test.vs");
const std::filesystem::path fragmentShaderPath(std::filesystem::current_path() / "resources/shaders/test.fs");

Camera camera{};
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
float sensitivity = 0.1f;
bool firstMouse = true;

int main()
{
	try
	{
		return render();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}

int render()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SWE Visualizer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, processMouse);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Intialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsClassic();

	// Initialize shader and plane for rendering
	Shader shader{vertexShaderPath, fragmentShaderPath};
	Plane plane{};

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		// Setup ImGui for next frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glm::mat4 model = glm::mat4(1.0f);
		// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat("model", model);
		shader.setMat("view", camera.getViewMatrix());
		shader.setMat("projection", proj);

		plane.render(shader);

		// Render some example UI stuff
		static bool showDemo = false;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Debug"))
			{
				if (ImGui::MenuItem("Toggle Demo Window"))
				{
					showDemo = !showDemo;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Camera"))
			{
				if (ImGui::MenuItem("Re-center camera"))
				{
					camera.position = camera.originalPosition;
					camera.front = camera.originalFront;
					camera.up = camera.originalUp;
				}
				ImGui::EndMenu();
			}
			ImGui::Button("Play");
			ImGui::Button("Pause");
			ImGui::EndMainMenuBar();
		}
		if (showDemo)
			ImGui::ShowDemoWindow(&showDemo);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

// process all input
// ------------------
void processInput(GLFWwindow *window)
{
	processKeyboard(window);
	processMouse(window, lastX, lastY);
}

void processMouse(GLFWwindow *window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera.yaw += xoffset;
		camera.pitch += yoffset;

		if (camera.pitch > 89.0f)
			camera.pitch = 89.0f;
		if (camera.pitch < -89.0f)
			camera.pitch = -89.0f;

		camera.direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.direction.y = sin(glm::radians(camera.pitch));
		camera.direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.front = glm::normalize(camera.direction);
	}
}

void processKeyboard(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// todo: eventuell cameraspeed noch mit deltaTime anpassen
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.position += camera.cameraSpeed * camera.front;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.position -= camera.cameraSpeed * camera.front;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * camera.cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * camera.cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}