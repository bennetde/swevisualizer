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
#include "../extern/ImGuiFileDialog/ImGuiFileDialog.h"
#include "simulation/netcdf_reader.h"
#include "simulation/simulation.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, double xpos, double ypos);
void processKeyboard (GLFWwindow *window);
void processMouse(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
double previousTime = glfwGetTime();
double currentTime, deltaTime;
// bool firstMouse =true;
double scrollOffset = 0.0;

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
	glfwSetScrollCallback(window, scroll_callback);

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
	Simulation sim{};

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		// input
		// -----
		
		double xpos, ypos;
    	glfwGetCursorPos(window, &xpos, &ypos);
		processInput(window, xpos, ypos);

		// render
		// ------
		// Setup ImGui for next frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glm::mat4 model = glm::mat4(1.0f);
		// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 5000.0f);
		shader.setMat("model", model);
		shader.setMat("view", camera.getViewMatrix());
		shader.setMat("projection", proj);

		sim.update(deltaTime);
		sim.render(shader);

		// Render some example UI stuff
		static bool showDemo = false;
		static bool openmenu = false;
		if (openmenu)
		{
			IGFD::FileDialogConfig config;
			config.path = ".";
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".nc", config);
			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
					sim.loadSimulation(filePathName);
				}

				ImGuiFileDialog::Instance()->Close();
				openmenu = false;
			}
		}
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					openmenu = true;
				}
				ImGui::EndMenu();
			}
#ifdef Debug
			if (ImGui::BeginMenu("Debug"))
			{
				if (ImGui::MenuItem("Toggle Demo Window"))
				{
					showDemo = !showDemo;
				}
				ImGui::EndMenu();
			}
#endif
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
			ImGui::Text("MS: %f", deltaTime);
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

// prcess keyboard iput.
//---------------------
// ESC Key closes the window
// UP/DOWN Key moves up/down
// LEFT/RIGHT Key moves left/right
// O Key zooms Out
// I Key zooms In
void processInput(GLFWwindow *window, double xpos, double ypos) {
	processKeyboard(window);
	processMouse(window, xpos, ypos);
}

void processKeyboard (GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		camera.position += camera.cameraSpeed * camera.front;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		camera.position -= camera.cameraSpeed * camera.front;
	// if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	// 	camera.position += camera.up * camera.cameraSpeed;
	// if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	// 	camera.position -= camera.up * camera.cameraSpeed;
	// if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	// 	camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * camera.cameraSpeed;
	// if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	// 	camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * camera.cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.position -= camera.up * camera.cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.position += camera.up * camera.cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * camera.cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * camera.cameraSpeed;
}

void processMouse(GLFWwindow *window, double xpos, double ypos) {
	ImGuiIO& io = ImGui::GetIO();
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !io.WantCaptureMouse) {
		float sensitivity = 0.07f;

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		// camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * (xOffset);
		// camera.position += camera.up * (yOffset);

		camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * (-xOffset);
		camera.position += camera.up * (-yOffset);
	} else {
        // Handle zooming with mouse scroll
        // Sensitivity to scroll wheel movement for zooming
        float zoomSensitivity = 5.0f;

        // Update camera position based on scroll offset
        camera.position += camera.front * static_cast<float>(scrollOffset) * zoomSensitivity;

        // Reset scroll offset
        scrollOffset = 0.0;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	ImGuiIO& io = ImGui::GetIO();
	if(io.WantCaptureMouse) return;
    scrollOffset = yoffset;
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