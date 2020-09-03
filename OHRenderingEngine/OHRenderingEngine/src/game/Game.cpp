#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "dependencies/utils/glm_util.h"
#include "renderer/Renderer.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

Game::Game(): m_window_width_(WIDTH), m_window_height_(HEIGHT)
{
	InitializeGLFW(MAJOR, MINOR);
}


Game::~Game()
{
	glfwTerminate();
}

bool Game::GameEnded()
{
	return m_game_ended_;
}


int Game::RunLevel()
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 tex_coord;
		float tex_num;
	};
	Vertex vertices[] = {
		glm::vec3(-0.5f,0.5f,0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	1,	//0
		glm::vec3(-0.5f,-0.5f,0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	1,	//1
		glm::vec3(0.5f,-0.5f,0.0f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	1,	//2
		glm::vec3(0.5f,0.5f,0.0f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	1,	//3
		glm::vec3(2-0.5f,2+0.5f,0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	2,	//4
		glm::vec3(2-0.5f,2-0.5f,0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	2,	//5
		glm::vec3(2+0.5f,2-0.5f,0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	2,	//6
		glm::vec3(2+0.5f,2+0.5f,0.0f),	glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	2	//7
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
		4, 5, 7,
		5, 6, 7
	};
	VertexArray vao;

	Shader simple_shader("res/shaders/simple.vert", "res/shaders/simple.frag");
	stbi_set_flip_vertically_on_load_thread(1);
	Texture tex1("res/textures/1.jpg");
	Texture tex2("res/textures/2.jpg");

	tex1.Activate(0);
	tex1.Bind();
	simple_shader.SetInt("texture0", 0);

	tex2.Activate(1);
	tex2.Bind();
	simple_shader.SetInt("texture1", 1);



	VertexBufferLayout vbl;
	VertexBuffer vb(vertices, 9 * 8 * sizeof(float), GL_STATIC_DRAW);
	
	IndexBuffer ib(indices, 12 * sizeof(unsigned int), GL_STATIC_DRAW);
	vbl.Push<float>(3, false);
	vbl.Push<float>(3, false);
	vbl.Push<float>(2, false);
	vbl.Push<float>(1, false);
	
	vao.AddBuffer(vb, vbl);

	glm::vec3 offset = glm::vec3(0.0f);


	while (!glfwWindowShouldClose(m_current_window_))
	{
		//Start new frame for IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//Render IMGUI GUI
		ImGui::Begin("Editor");
		{
			ImGui::SliderFloat("X offset", &offset.x, -3.0f, 3.0f);            
			ImGui::SliderFloat("Y offset", &offset.y, -3.0f, 3.0f);           
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();

		//Clear screen using openGL
		Renderer::ClearScreen(0.4f, 0.2f, 0.4f, 1.0f);

		//Transformations
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0); //TODO: to be implemented
		glm::mat4 projection = glm::mat4(1.0); //TODO: to be implemented
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, offset);
		simple_shader.SetMat4("model", model);
		simple_shader.SetMat4("view", view);
		simple_shader.SetMat4("projection", projection);

		//Render using openGL
		Renderer::Draw(vao, simple_shader, 12, 0);

		//Render GUI onto screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_current_window_);
		glfwPollEvents();
		ProcessInput();
	}
	m_game_ended_ = true;
	return 0;
}

int Game::Run()
{
	CreateWindow();
	if (m_current_window_ == nullptr)
	{
		std::cout << "ERROR: COULDN'T CREATE WINDOW" << std::endl;
		return -1;
	}
	AssignGLFWCallbacks();
	//Load GLAD function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: FAILED TO INITIALIZE GLAD!" << std::endl;
		return -1;
	}
	SetupIMGUI();
	return RunLevel();
}

void Game::InitializeGLFW(int major = 3, int minor = 3) const
{
	glfwInit();
	glfwInitHint(GL_MINOR_VERSION, major);
	glfwInitHint(GL_MAJOR_VERSION, minor);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Game::CreateWindow()
{
	//If we have an open window
	if (m_current_window_ != nullptr) return;
	m_current_window_= glfwCreateWindow(m_window_width_, m_window_height_, "Demo Window", nullptr, nullptr);
	if (m_current_window_ == nullptr) 
		return;
	glfwMakeContextCurrent(m_current_window_);
}

void Game::ProcessInput()
{
	if (glfwGetKey(m_current_window_, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_current_window_, true);
}



void Game::AssignGLFWCallbacks() const
{
	glfwSetWindowSizeCallback(m_current_window_, window_size_callback);
}

void Game::SetupIMGUI() const
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_current_window_, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

//GLFW Callback functions
void window_size_callback(GLFWwindow *window, int width, int height)
{
	Renderer::ResizeWindow(width, height);
}
