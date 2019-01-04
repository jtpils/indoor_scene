/*opengl project learn by liminghao*/
//20181222
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_dev/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "model.h"
#include "camera.h"
#include "myroom.h"
//#include "filesystem.h"

//ע��һ���ص�����(Callback Function)����ÿ�δ��ڴ�С��������ʱ�򱻵���
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//ע��һ��������Ƶļ��ɺ���
void processInput(GLFWwindow *window);
//ע��һ������ƶ����Ƶļ��ɺ���
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//ע��һ�����ֿ��Ƶļ��ɺ���
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//ע��һ������������Ƶļ��ɺ���
float textureInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;//���
const unsigned int SCR_HEIGHT = 600;//�߶�

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{	
	/******************************************************************************************************
	���ڳ�ʼ��
	******************************************************************************************************/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif
	//����һ�����ڶ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My room", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_CURSOR_NORMAL);// ���ò������
	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//���ô��ڵ�ά��(Dimension)
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	//������Ȳ���depth test
	glEnable(GL_DEPTH_TEST);
	/****************************************************************************************************
	��ɫ���붥��������Գ�ʼ��
	****************************************************************************************************/

	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");	//��ȡ��ɫ���ļ���������ɫ������
	Shader lampShader("lamp.vs", "lamp.fs");//��Դ��ɫ��
	
	Model ourModel("resources/objects/table/HSM0015.obj");
	Model lampModel("resources/objects/Lamp-bureaucrat/lamp-bureaucrat.obj");
	Sphere ball(9,18);
	//�����嶥��
	float vertices[] = {
		//---- λ�� ----     - �������� -   - �������� -
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	//������λ��
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  -2.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};	
	//ƽ�涥��
	float planeVertices[] = {
		// positions              - �������� -       - �������� -    // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 4.0f, -2.0f,  4.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-4.0f, -2.0f,  4.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-4.0f, -2.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

		 4.0f, -2.0f,  4.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-4.0f, -2.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 4.0f, -2.0f, -4.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f
	};
	glm::vec3 planePositions[] = {
		// positions        
		
		//glm::vec3(-6.0f, 0.0f,  0.0f),
		//glm::vec3(6.0f,  0.0f,  0.0f),
		//glm::vec3(0.0f,  0.0f, -6.0f),
		glm::vec3(2.0f, 0.0f,  0.0f),
		glm::vec3(-2.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f, -2.0f),
		glm::vec3(0.0f,  2.0f, 0.0f)
	};
	glm::vec3 planeRotate[] = {
		// Rotate xis        
		
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(1.0f,  0.0f,  0.0f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(1.9f,  0.2f,  -2.0f),
		glm::vec3(-1.9f, 0.2f, -2.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	//�������㻺�����Vertex Buffer Object��VBO�Ͷ����������Vertex Array Object��VAO
	//�����嶥��
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//�Ѷ���Ķ������ݸ��Ƶ�������ڴ���

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //�Զ�������λ��ֵ'0'��Ϊ���������ö������ԡ���λ�ã�
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); //�Զ�������λ��ֵ'1'��Ϊ���������ö������ԡ����������ꣻ
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); //�Զ�������λ��ֵ'2'��Ϊ���������ö������ԡ����������ꣻ
	//ǽ�涥��
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); 
	//��Դ����
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���õ�������Ķ������ԣ������ǵĵ���˵����ֻ��λ�����ݣ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/******************************************************************************************************
	�����ʼ��
	******************************************************************************************************/
	//unsigned int texture1, texture2;
	unsigned int diffuseMap, specularMap;
	//����1
	glGenTextures(1, &diffuseMap);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// ���ز���������
	int width, height, nrChannels;
	unsigned char *data = stbi_load("resources/images/container2.png", &width, &height, &nrChannels, 0);
	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{	std::cout << "Failed to load container2.png" << std::endl;
	}
	stbi_image_free(data);
	//����2
	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// ���ز���������
	data = stbi_load("resources/images/container2_specular.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load container2_specular.png" << std::endl;
	}
	stbi_image_free(data);
	//�ذ� ����3
	unsigned int floorTexture;
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// ���ز���������
	data = stbi_load("resources/images/floor.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load floor.jpg" << std::endl;
	}
	stbi_image_free(data);
	//ǽ�� ����4
	unsigned int wallTexture;
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// ���ز���������
	data = stbi_load("resources/images/wall.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load wall.jpg" << std::endl;
	}
	stbi_image_free(data);
	//���淴�䳣�� ����5
	unsigned int grayTexture;
	glGenTextures(1, &grayTexture);
	glBindTexture(GL_TEXTURE_2D, grayTexture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// ���ز���������
	data = stbi_load("resources/images/gray.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load gray.png" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use(); // �������ڼ�����ɫ��ǰ������uniform��
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);//ʹ����ɫ������������ÿ������������OpenGL�������ĸ�����Ԫ
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grayTexture);
	/******************************************************************************************************
	��Դ��ʼ��
	******************************************************************************************************/
	ourShader.setInt("material.diffuse", 0);//������ʡ�����������ͼ����������������������ɫ
	ourShader.setInt("material.specular", 1);//������ʡ������淴����ͼ����������ն��������ɫӰ��
	ourShader.setFloat("material.shininess", 32.0f);//����߹��ɢ��/�뾶

	/*
	   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/
	// ƽ�й�
	ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);//��Դ����
	ourShader.setVec3("dirLight.ambient", 0.02f, 0.02f, 0.02f);//��Դ������ǿ��
	ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);//��Դ������ǿ��
	ourShader.setVec3("dirLight.specular", 0.4f, 0.4f, 0.4f);//��Դ�����ǿ��
	// point light 1
	ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);//��Դλ��
	ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	ourShader.setFloat("pointLights[0].constant", 1.0f);//��Դ����ϵ��
	ourShader.setFloat("pointLights[0].linear", 0.09);//��Դ����ϵ��
	ourShader.setFloat("pointLights[0].quadratic", 0.032);//��Դ����ϵ��
	// point light 2
	ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	ourShader.setFloat("pointLights[1].constant", 1.0f);
	ourShader.setFloat("pointLights[1].linear", 0.09);
	ourShader.setFloat("pointLights[1].quadratic", 0.032);
	/******************************************************************************************************
	3D�任����
	******************************************************************************************************/
	glm::mat4 trans;
	glm::mat4 model;//ģ�;��󣬰���λ�ơ���������ת���任��ȫ������ռ�
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 projection;//ͶӰ����
	projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
	ourShader.setMat4("model", model);
	ourShader.setMat4("projection", projection);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//������Ⱦѭ��(Render Loop)
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//std::cout<<std::to_string(int (1.0f/ deltaTime))<<" fps"<<std::endl;

		processInput(window);
		
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.use();//ʹ����ɫ��		
		/*********************************************������*******************************************/
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);//ʹ����ɫ������������ÿ������������OpenGL�������ĸ�����Ԫ
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);//ʹ����ɫ������������ÿ������������OpenGL�������ĸ�����Ԫ		
		
		ourShader.setInt("material.diffuse", 0);//������ʡ�����������ͼ����������������������ɫ
		ourShader.setInt("material.specular", 1);//������ʡ������淴����ͼ����������ն��������ɫӰ��
		ourShader.setFloat("material.shininess", 16.0f);//����߹��ɢ��/�뾶
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 1; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(1.0f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/
		/**********************************************�ذ�*******************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grayTexture);
		ourShader.setInt("material.diffuse", 0);//������ʡ�����������ͼ����������������������ɫ
		ourShader.setInt("material.specular", 1);//������ʡ������淴����ͼ����������ն��������ɫӰ��
		ourShader.setFloat("material.shininess", 32.0f);//����߹��ɢ��/�뾶
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		ourShader.setMat4("model", model); 
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grayTexture);
		ourShader.setInt("material.diffuse", 0);//������ʡ�����������ͼ����������������������ɫ
		ourShader.setInt("material.specular", 1);//������ʡ������淴����ͼ����������ն��������ɫӰ��
		ourShader.setFloat("material.shininess", 64.0f);//����߹��ɢ��/�뾶
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model; float angle;
			model = glm::translate(model, planePositions[i]);
			if(i==3){
				angle = 180.0f; ourShader.setFloat("material.shininess", 32.0f);//����߹��ɢ��/�뾶
			}// �����ϰ� 
			else {angle = 90.0f;} 
			model = glm::rotate(model, glm::radians(angle), planeRotate[i]);
			ourShader.setMat4("model", model);		
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		
		/**********************************************�����ģ��*******************************************/
		{
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
			float angle = 90.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.02f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);
		}
		{
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.2f, -3.25f, 0.1f));
			float angle = 135.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			lampModel.Draw(ourShader);
		}
		
		//�����ͶӰ��ʽ
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		//��ȡ�������view
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
			
		
		/**********************************************��Դ*******************************************/
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		lampShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (unsigned int i = 0; i < 2; i++)
		{
			lampShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 0.1f));
			glm::mat4 model;
			model = glm::translate(model, pointLightPositions[i]);
			
			model = glm::scale(model, glm::vec3(0.4f)); // Make it a smaller obj
			lampShader.setMat4("model", model);
			ball.draw(lampShader);
		}
		
		glBindVertexArray(0);
		// �������岢��ѯIO�¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	// ɾ�������������ͻ������
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &planeVBO);

	glfwTerminate();
	return 0;
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//�˳�����
		glfwSetWindowShouldClose(window, true);
	//���̿���������ƶ�
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

float textureInput(GLFWwindow *window)
{
	//���¼��ı���������͸����
	static float tempMix = 0.1f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		tempMix += 0.0001f;
		if (tempMix >= 1.0)
			tempMix = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{ 
		tempMix -= 0.0001f;
		if (tempMix <= 0.0)
			tempMix = 0.0f;
	}
	return tempMix;
}
/*void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}*/
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS){
		//std::cout << "����" << std::endl;
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;
		camera.ProcessMouseMovement(xoffset, yoffset);
		
	}	
	else {
		if (state == GLFW_RELEASE) {
			//std::cout << "����" << std::endl;
			lastX = xpos;
			lastY = ypos;
		}
	}	
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

