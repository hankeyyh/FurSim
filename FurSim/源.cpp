#if defined(NANOGUI_GLAD)
#if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
#define GLAD_GLAPI_EXPORT
#endif


#include "HairFactory.h"
#include "FTLNew.h"
#include "WindField.h"
#include "Program.h"
#include "Window.h"
#include "CollisionHandler.h"
#include "Sphere.h"
//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

//#include <glad/glad.h>
#else
#if defined(__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#else
#define GL_GLEXT_PROTOTYPES
#endif
#endif

//#include <GLFW/glfw3.h>

#include <nanogui/nanogui.h>
#include <SOIL2.h>
#include <iostream>


using namespace std;

void print(const Hair& h)
{
	for (size_t s = 0; s<h.size(); s++)
	{
		cout << endl;
		cout << "Strand no. " << s << endl;
		for (size_t v = 0; v<h[s].size(); v++)
		{
			const vec3& pos = h[s][v].Position;

			cout << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << endl;
		}
	}
}

//collision
bool isApplyCollision = true;
bool isDrawAssistModel = false;
CollisionHandler *collisionHandler;
Vertex object(glm::vec3(0, 0.6, 0), FLT_MAX);
float objectRadius = 3.4;
float ks = 1.0;

Sphere sphere(40, 40);

//nanogui
nanogui::Screen *screen = nullptr;
nanogui::Screen *screen_light = nullptr;
nanogui::ref<nanogui::Window> nanoguiWindow;
nanogui::ref<nanogui::Window> nanoguiWindow_light;


//window
int16_t IMAGE_WIDTH = 1320;
int16_t IMAGE_HEIGHT = 800;
Window *g_pWindow;

//simulation
constexpr int Frames = 10000;
int frameIdx = 0;
ISimulation* sim;
vec3 gravity(0, -1, 0);
bool useFTL = true;

bool isDrawParticle = false;
Hair hair;
std::vector<glm::vec3> hair_pos;
std::vector<glm::vec3> hair_prePos;
std::vector<glm::vec2> hair_uv;

//camera
vec3 cameraPos(0, 0, 10);
vec3 cameraTarg(0, 0, 0);
vec3 cameraUp(0, 1, 0);

//wind
std::vector<vec3> windDiskPoses{ glm::vec3(0, -1, 4), glm::vec3(-1, 0, 2.5), glm::vec3(1, 0, 2.5) };
std::vector<vec3> windDiskDirs{ glm::vec3(0, 0.4, -1), glm::normalize(glm::vec3(1, 0, -2.5)), glm::normalize(glm::vec3(-1, 0, -2.5)) };
std::vector<float> windDiskRadiuses{ 3.4f, 1.0f, 1.0f };
std::vector<float> windDiskIntensities{ 0.0f, 3.0f, 3.0f };
std::vector<float> windDiskDisDampings{ 0.5f, 0.3f, 0.3f };
std::vector<WindField*> windFields;

// Mouse Movement 
int oldMouseX, oldMouseY;
int currentMouseX, currentMouseY;
bool isLeftDown = false, isRightDown = false, isMiddleDown = false;
float xTranslate = 0.0f, yTranslate = 0.0f, zTranslate = 0.0f;
float xRotation = 0.0f, yRotation = 0.0f, zRotation = 0.0f;
float zoom = 1.0f;

//Modern GL
struct GLInfo
{
	static GLuint VAO;
	static GLuint posVBO;
	static GLuint posPreVBO;
	static GLuint uvVBO;
	static GLuint EBO;

	static GLuint headVAO;
	static GLuint headVBO;
	static GLuint headEBO;
};
GLuint GLInfo::EBO = -1;
GLuint GLInfo::VAO = -1;
GLuint GLInfo::posVBO = -1;
GLuint GLInfo::posPreVBO = -1;
GLuint GLInfo::uvVBO = -1;

GLuint GLInfo::headVBO = -1;
GLuint GLInfo::headVAO = -1;
GLuint GLInfo::headEBO = -1;

//shader
Program *g_pHairProgram;
Program *g_pHeadProgram;
Program *g_pSphereProgram;

//hair material
typedef struct _HairMat
{
	nanogui::Color hairSurfaceColor = nanogui::Color(1.0f, 0.84f, 0.0f, 1.0f);
	float sdamping = 0.98;
} HairMat;
HairMat hairMat;

//model material
typedef struct _ModelMat
{
	nanogui::Color modelSurfaceColor = nanogui::Color(1.0f, 0.84f, 0.0f, 1.0f);
	glm::vec3 materialSpecular = glm::vec3(0.1f);
	float materialShininess = 32.0f;
} ModelMat;
ModelMat headMat;

//texture
typedef struct _Texture
{
	GLuint id;
	string type;
	int width, height;
} Texture;
Texture texture_diffuse;
Texture texture_specShift;

//vertex
typedef struct _Vert
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
} Vert;
std::vector<Vert> headVertices;
std::vector<int> headIndices;
bool isDrawModel = true;

//obj SOA
typedef struct _ObjInfo
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<int> vIndices;
	std::vector<int> nIndices;
	std::vector<int> tIndices;
} ObjInfo;

//light
typedef struct _Light
{
	glm::vec3 lightPos = glm::vec3(0.0f);
	glm::vec3 lightAmbient = glm::vec3(0.2f);
	glm::vec3 lightDiffuse = glm::vec3(0.5f);
	glm::vec3 lightSpecular = glm::vec3(1.0f);
	float lightConstant = 1.0f;
	float lightLinear = 0.0f;
	float lightQuadratic = 0.0f;
} Light;
Light light;

unsigned TextureFromFile(const char* filename, int &width, int &height, bool isRGBA = true);
Texture LoadTexture(string filename, string typeName, bool isRGBA = true);

void drawHair();
void drawHead();
void updateHair();
void setUpGLHairMesh();
void setUpGLHeadMesh();
void deleteMesh();
void readObj(char *filename, ObjInfo& objInfo);
void readObj(char *filename, std::vector<Vert>& vertices, std::vector<int>& indices);
void processNode(aiNode* node, const aiScene* scene, std::vector<Vert>& vertices, std::vector<int>& indices);
void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<Vert>& vertices, std::vector<int>& indices);


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double x, double y);
void windowsize_callback(GLFWwindow* window, int width, int height); 
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);

int main(int argc, char** argv)
{
	std::srand(time(NULL));

	//create window
	g_pWindow = new Window("Fur Simulation", IMAGE_WIDTH, IMAGE_HEIGHT);
	g_pWindow->SetKeyCallback(key_callback);
	g_pWindow->SetCursorPosCallback(mouse_callback);
	g_pWindow->SetWindowSizeCallback(windowsize_callback);
	g_pWindow->SetMouseButtonCallback(mouseButton_callback);
	g_pWindow->SetCharCallback([](GLFWwindow *, unsigned int codepoint) {
		screen->charCallbackEvent(codepoint);
		screen_light->charCallbackEvent(codepoint);
	});
	g_pWindow->SetDropCallback([](GLFWwindow *, int count, const char **filenames) {
		screen->dropCallbackEvent(count, filenames);
		screen_light->dropCallbackEvent(count, filenames);
	});
	g_pWindow->SetScrollCallback([](GLFWwindow *, double x, double y) {
		screen->scrollCallbackEvent(x, y);
		screen_light->scrollCallbackEvent(x, y);
	});
	g_pWindow->SetFrameBufferSizeCallback([](GLFWwindow *, int width, int height) {
		screen->resizeCallbackEvent(width, height);
		screen_light->resizeCallbackEvent(width, height);
	});

	//nanogui
	{
		// Create a nanogui screen and pass the glfw pointer to initialize
		bool enabled = true;
		screen = new nanogui::Screen();
		screen->initialize(g_pWindow->GetGLFWWindow(), true);
		nanogui::FormHelper *gui = new nanogui::FormHelper(screen);
		nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Dashboard");
		gui->addGroup("Hair Config");
		gui->addVariable("Hair Color", hairMat.hairSurfaceColor);
		nanogui::detail::FormWidget<float> *sdamp = gui->addVariable("Sdamping", hairMat.sdamping); sdamp->setSpinnable(true); sdamp->setMinMaxValues(0.0f, 1.0f);
		sdamp->setValueIncrement(0.01f);

		gui->addGroup("Collide Config");
		gui->addVariable("Collide", isApplyCollision);
		gui->addVariable("Collide Object", isDrawAssistModel);
		gui->addVariable("Collide Position X", object.Position.x)->setSpinnable(true);
		gui->addVariable("Collide Position Y", object.Position.y)->setSpinnable(true);
		gui->addVariable("Collide Position Z", object.Position.z)->setSpinnable(true);
		gui->addVariable("Collide Radius", objectRadius)->setSpinnable(true);
		nanogui::detail::FormWidget<float> * pKs = gui->addVariable("Ks", ks); pKs->setSpinnable(true); pKs->setMinValue(0.1f);

		gui->addGroup("Model Config");
		gui->addVariable("Draw Model", isDrawModel);
		gui->addVariable("Model Color", headMat.modelSurfaceColor);
		gui->addVariable("Material Specular X", headMat.materialSpecular.x)->setSpinnable(true);
		gui->addVariable("Material Specular Y", headMat.materialSpecular.y)->setSpinnable(true);
		gui->addVariable("Material Specular Z", headMat.materialSpecular.z)->setSpinnable(true);
		gui->addVariable("Material Shininess", headMat.materialShininess)->setSpinnable(true);

		gui->addGroup("Wind0");
		gui->addVariable("positionX", windDiskPoses[0].x)->setSpinnable(true);
		gui->addVariable("positionY", windDiskPoses[0].y)->setSpinnable(true);
		gui->addVariable("positionZ", windDiskPoses[0].z)->setSpinnable(true);

		nanogui::detail::FormWidget<float>* dirX = gui->addVariable("DirectionX", windDiskDirs[0].x); dirX->setSpinnable(true); dirX->setMinMaxValues(-1.0f, 1.0f);
		nanogui::detail::FormWidget<float>* dirY = gui->addVariable("DirectionY", windDiskDirs[0].y); dirY->setSpinnable(true); dirY->setMinMaxValues(-1.0f, 1.0f);
		nanogui::detail::FormWidget<float>* dirZ = gui->addVariable("DirectionZ", windDiskDirs[0].z); dirZ->setSpinnable(true); dirZ->setMinMaxValues(-1.0f, 1.0f);

		gui->addVariable("Intensity", windDiskIntensities[0])->setSpinnable(true);
		gui->addVariable("Radius", windDiskRadiuses[0])->setSpinnable(true);
		gui->addVariable("Distance Damping", windDiskDisDampings[0])->setSpinnable(true);

		screen->setVisible(true);
		screen->performLayout();
	}
	{
		screen_light = new nanogui::Screen();
		screen_light->initialize(g_pWindow->GetGLFWWindow(), true);
		nanogui::FormHelper *gui = new nanogui::FormHelper(screen_light);
		nanoguiWindow_light = gui->addWindow(Eigen::Vector2i(IMAGE_WIDTH - 210, 10), "Light");
		
		gui->addGroup("Light Config");
		gui->addVariable("Light Position X", light.lightPos.x)->setSpinnable(true);
		gui->addVariable("Light Position Y", light.lightPos.y)->setSpinnable(true);
		gui->addVariable("Light Position Z", light.lightPos.z)->setSpinnable(true);
		gui->addVariable("Light Ambient R", light.lightAmbient.r)->setSpinnable(true);
		gui->addVariable("Light Ambient G", light.lightAmbient.g)->setSpinnable(true);
		gui->addVariable("Light Ambient B", light.lightAmbient.b)->setSpinnable(true);
		gui->addVariable("Light Diffuse R", light.lightDiffuse.r)->setSpinnable(true);
		gui->addVariable("Light Diffuse G", light.lightDiffuse.g)->setSpinnable(true);
		gui->addVariable("Light Diffuse B", light.lightDiffuse.b)->setSpinnable(true);
		gui->addVariable("Light Specular R", light.lightSpecular.r)->setSpinnable(true);
		gui->addVariable("Light Specular G", light.lightSpecular.g)->setSpinnable(true);
		gui->addVariable("Light Specular B", light.lightSpecular.b)->setSpinnable(true);
		gui->addVariable("Constant Damping", light.lightConstant)->setSpinnable(true);
		gui->addVariable("Linear Damping", light.lightLinear)->setSpinnable(true);
		gui->addVariable("Quadratic Damping", light.lightQuadratic)->setSpinnable(true);

		screen_light->setVisible(true);
		screen_light->performLayout();
	}

	//create hair
	ObjInfo scalpInfo;
	readObj("../resources/scalp_0x.obj", scalpInfo);
	hair = HairFactory::GrowHairOnDefinedPos(100, 0.06, scalpInfo.positions);
	//hair = HairFactory::GrowHairOnRow(600, 100, 0.06);
	readObj("../resources/MaleHead_0x.obj", headVertices, headIndices);

	//create wind
	windFields.push_back(new WindField_Disk(windDiskPoses[0], windDiskDirs[0], windDiskRadiuses[0], windDiskIntensities[0], windDiskDisDampings[0]));
	windFields.push_back(new WindField_Disk(windDiskPoses[1], windDiskDirs[1], windDiskRadiuses[1], windDiskIntensities[1], windDiskDisDampings[1]));
	windFields.push_back(new WindField_Disk(windDiskPoses[2], windDiskDirs[2], windDiskRadiuses[2], windDiskIntensities[2], windDiskDisDampings[2]));

	//texture
	texture_diffuse = LoadTexture("../resources/Hair_diffuse2.jpg", "texture_diffuse", true);
	texture_specShift = LoadTexture("../resources/Hair_specShift2.jpg", "texture_specShift", true);

	if (useFTL)
	{
		//sim = new FTL(h);
		sim = new FTLNew(hair, hairMat.sdamping);
	}
	isDrawParticle = true;

	//collision
	if (isApplyCollision)
	{
		collisionHandler = new CollisionHandler();
	}

	std::vector<ShaderYH> shaders;
	shaders.push_back(ShaderYH::ShaderYHFromFile("../resources/HairShader.vs", GL_VERTEX_SHADER));
	shaders.push_back(ShaderYH::ShaderYHFromFile("../resources/HairShader.frag", GL_FRAGMENT_SHADER));
	g_pHairProgram = new Program(shaders);

	shaders.clear();
	shaders.push_back(ShaderYH::ShaderYHFromFile("../resources/HeadShader.vs", GL_VERTEX_SHADER));
	shaders.push_back(ShaderYH::ShaderYHFromFile("../resources/HeadShader.frag", GL_FRAGMENT_SHADER));
	g_pHeadProgram = new Program(shaders);

	shaders.clear();
	shaders.push_back(ShaderYH::ShaderYHFromFile("../resources/SphereShader.vs", GL_VERTEX_SHADER));
	shaders.push_back(ShaderYH::ShaderYHFromFile("../resources/SphereShader.frag", GL_FRAGMENT_SHADER));
	g_pSphereProgram = new Program(shaders);

	setUpGLHairMesh();
	setUpGLHeadMesh();
	sphere.Init();//collid sphere

	// Game loop
	while (!g_pWindow->ShouldClose())
	{
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
	
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		int pixelWidth, pixelHeight;
		glfwGetFramebufferSize(/*window*/g_pWindow->GetGLFWWindow(), &pixelWidth, &pixelHeight);
		glViewport(0, 0, pixelWidth, pixelHeight);

		glm::mat4 projMat = glm::perspective(45.0f, (float)IMAGE_WIDTH / IMAGE_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 modelView = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10.0f));
		modelView = glm::translate(modelView, glm::vec3(xTranslate, yTranslate, 0.0f));
		
		modelView = glm::rotate(modelView, yRotation, glm::vec3(1, 0, 0));
		modelView = glm::rotate(modelView, xRotation, glm::vec3(0, 1, 0));
		modelView = glm::scale(modelView, glm::vec3(zoom, zoom, zoom));

		//hair
		g_pHairProgram->Use();
		{
			g_pHairProgram->SetUniform("projection", projMat);
			g_pHairProgram->SetUniform("modelView", modelView);
			g_pHairProgram->SetUniform("lightPosition", glm::vec3(0));
			g_pHairProgram->SetUniform("surfColor", glm::vec4(hairMat.hairSurfaceColor.r(), hairMat.hairSurfaceColor.g(), hairMat.hairSurfaceColor.b(), hairMat.hairSurfaceColor.w()));
			

			drawHair();
			updateHair();
		}
		g_pHairProgram->End();

		//画球
		if (isDrawAssistModel)
		{
			g_pSphereProgram->Use();
			{
				glm::mat4 tmpModelView = modelView * glm::translate(glm::mat4(1.0f), object.Position);
				g_pSphereProgram->SetUniform("projMatrix", projMat);
				g_pSphereProgram->SetUniform("mvMatrix", tmpModelView);
				sphere.Draw();
			}
			g_pSphereProgram->End();
		}

		//model
		if (isDrawModel)
		{
			g_pHeadProgram->Use();
			{
				g_pHeadProgram->SetUniform("projection", projMat);
				g_pHeadProgram->SetUniform("modelView", modelView);

				g_pHeadProgram->SetUniform("lightPosition", light.lightPos);
				g_pHeadProgram->SetUniform("lightAmbient", light.lightAmbient);
				g_pHeadProgram->SetUniform("lightDiffuse", light.lightDiffuse);
				g_pHeadProgram->SetUniform("lightSpecular", light.lightSpecular);

				g_pHeadProgram->SetUniform("lightConstant", light.lightConstant);
				g_pHeadProgram->SetUniform("lightLinear", /*0.04f*/light.lightLinear);
				g_pHeadProgram->SetUniform("lightQuadratic", /*0.012f*/light.lightQuadratic);

				g_pHeadProgram->SetUniform("surfaceColor", glm::vec3(headMat.modelSurfaceColor.r(), headMat.modelSurfaceColor.g(), headMat.modelSurfaceColor.b()));
				g_pHeadProgram->SetUniform("materialSpecular", headMat.materialSpecular);
				g_pHeadProgram->SetUniform("materialShininess", headMat.materialShininess);

				drawHead();
			}
			g_pHeadProgram->End();
		}
	
		// Draw nanogui
		screen->drawContents();
		screen->drawWidgets();
		screen_light->drawContents();
		screen_light->drawWidgets();
	
		g_pWindow->SwapBuffers();
	}
	
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	delete g_pWindow;
	delete g_pHairProgram;
	delete g_pHeadProgram;

	return 0;
}



unsigned TextureFromFile(const char* filename, int &width, int &height, bool isRGBA)
{
	GLuint textureID;
	glGenTextures(1, &textureID);


	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == 0)
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	else
	{
		//assign texture to id
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, isRGBA ? GL_RGBA : GL_RGB, width, height, 0, isRGBA ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	SOIL_free_image_data(image);

	return textureID;
}

Texture LoadTexture(string filename, string typeName, bool isRGBA)
{
	Texture tex;
	int imgWidth, imgHeight;
	tex.id = TextureFromFile(filename.c_str(), imgWidth, imgHeight, isRGBA);
	tex.type = typeName;
	tex.width = imgWidth;
	tex.height = imgHeight;

	return tex;
}

void drawHair()
{
	glBindVertexArray(GLInfo::VAO);
	glDrawArrays(GL_LINES, 0, hair_pos.size());
	glBindVertexArray(0);
}

void drawHead()
{
	glBindVertexArray(GLInfo::headVAO);
	glDrawElements(GL_TRIANGLES, headIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void updateHair()
{
	sim->addForce(gravity);

	std::vector<glm::vec3> disturbances;
	disturbances.resize(windFields.size());

	for (unsigned wId = 0; wId < windFields.size(); wId++)
	{
		WindField_Disk* pWindDisk = dynamic_cast<WindField_Disk*>(windFields[wId]);
		pWindDisk->SetIntensity(windDiskIntensities[wId]);
		pWindDisk->SetDirection(windDiskDirs[wId]);
		pWindDisk->SetPointInPlane(windDiskPoses[wId]);
		pWindDisk->SetRadius(windDiskRadiuses[wId]);
		pWindDisk->SetDisDamping(windDiskDisDampings[wId]);

		if (wId == 0)
		{
			disturbances[wId] = pWindDisk->GetDirection() * 0.3f * cosf(0.1f * frameIdx);
		}
		else
		{
			disturbances[wId] = pWindDisk->GetDirection() * 0.4f * cosf(0.05f * frameIdx);
		}
	}

	for (int s = 0; s < hair.size(); s++)
	{
		for (int v = 0; v < hair[s].size(); v++)
		{
			if (hair[s][v].enabled)
			{
				//for (unsigned wId = 0; wId < windFields.size(); wId++)
				{
					glm::vec3 tmpForce = windFields[0]->GetForce(hair[s][v].Position);
					if (tmpForce.x != 0 || tmpForce.y != 0 || tmpForce.z != 0)
					{
						tmpForce += disturbances[0];
					}
					hair[s][v].Force += tmpForce;
				}
			}
		}
	}

	if (useFTL)
	{
		FTLNew *ftl = dynamic_cast<FTLNew*>(sim);
		ftl->SetSDamping(hairMat.sdamping);
	}
	sim->update();

	//处理碰撞
	if (isApplyCollision)
	{
		collisionHandler->Update(hair, object, objectRadius, ks >= 0.1 ? ks : 0.1);
	}
	

	frameIdx++;

	//更新pos buffer
	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::posVBO);
	glm::vec3 *verts = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	int k = 0;
	for (size_t s = 0; s < hair.size(); s++)
	{
		for (size_t v = 0; v < hair[s].size() - 1; v++)
		{
			verts[k++] = hair[s][v].Position;
			verts[k++] = hair[s][v + 1].Position;
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//更新pre-pos buffer
	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::posPreVBO);
	glm::vec3 *vertsPre = (glm::vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	k = 0;
	for (int s = 0; s < hair.size(); s++)
	{
		for (int v = 0; v < hair[s].size() - 1; v++)
		{
			vertsPre[k++] = hair[s][(v - 1) >= 0 ? (v - 1) : 0].Position;
			vertsPre[k++] = hair[s][v].Position;
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void setUpGLHairMesh()
{
	glGenVertexArrays(1, &GLInfo::VAO);
	glGenBuffers(1, &GLInfo::posVBO);
	glGenBuffers(1, &GLInfo::posPreVBO);
	glGenBuffers(1, &GLInfo::uvVBO);
	glGenBuffers(1, &GLInfo::EBO);

	// Load data into vertex buffers/index buffer
	for (size_t s = 0; s < hair.size(); s++)
	{
		for (size_t v = 0; v < hair[s].size() - 1; v++)
		{
			hair_pos.push_back(hair[s][v].Position);
			hair_pos.push_back(hair[s][v + 1].Position);
		}
	}

	for (int s = 0; s < hair.size(); s++)
	{
		for (int v = 0; v < hair[s].size() - 1; v++)
		{
			hair_prePos.push_back(hair[s][(v - 1) >= 0 ? (v - 1) : 0].Position);
			hair_prePos.push_back(hair[s][v].Position);
		}
	}

	for (unsigned s = 0; s < hair.size(); s++)
	{
		for (unsigned v = 0; v < hair[s].size(); v++)
		{
			glm::vec2 uv;
			uv.x = s / (float)texture_diffuse.width;
			uv.y = v / (float)texture_diffuse.height;

			hair_uv.push_back(uv);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::posVBO);
	glBufferData(GL_ARRAY_BUFFER, hair_pos.size() * sizeof(glm::vec3), hair_pos.data(), GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::posPreVBO);
	glBufferData(GL_ARRAY_BUFFER, hair_prePos.size() * sizeof(glm::vec3), hair_prePos.data(), GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::uvVBO);
	glBufferData(GL_ARRAY_BUFFER, hair_uv.size() * sizeof(glm::vec2), hair_uv.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(GLInfo::VAO);
	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::posVBO);
	{
		//Vertex pos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::posPreVBO);
	{
		//pre vertex pos
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::uvVBO);
	{
		//uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void setUpGLHeadMesh()
{
	glGenVertexArrays(1, &GLInfo::headVAO);
	glGenBuffers(1, &GLInfo::headVBO);
	glGenBuffers(1, &GLInfo::headEBO);

	glBindBuffer(GL_ARRAY_BUFFER, GLInfo::headVBO);
	glBufferData(GL_ARRAY_BUFFER, headVertices.size() * sizeof(Vert), headVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLInfo::headEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, headIndices.size() * sizeof(int), headIndices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(GLInfo::headVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, GLInfo::headVBO);
		{
			//position
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)0);

			//normal
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)offsetof(Vert, normal));

			//uv
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)offsetof(Vert, uv));
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLInfo::headEBO);
	}
	glBindVertexArray(0);
}

void deleteMesh()
{
	glDeleteVertexArrays(1, &GLInfo::VAO);
	glDeleteBuffers(1, &GLInfo::posVBO);
	glDeleteBuffers(1, &GLInfo::posPreVBO);
	glDeleteBuffers(1, &GLInfo::EBO);
}

void readObj(char *filename, ObjInfo& objInfo)
{
	FILE *file;

	file = fopen(filename, "r");
	if (!file)
	{
		fprintf(stderr, "readSurface() failed: can't open data file \"%s\".\n",
			filename);
		getchar();
		exit(1);
	}

	char buf[128];
	float vx, vy, vz;
	while (fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case '#':
			fgets(buf, sizeof(buf), file);
			break;
		case 'v':
			switch (buf[1])
			{
			case '\0':
				fscanf(file, "%f %f %f", &vx, &vy, &vz);
				objInfo.positions.push_back(glm::vec3(vx, vy, vz));
				break;
			case 'n':
				fscanf(file, "%f %f %f", &vx, &vy, &vz);
				objInfo.normals.push_back(glm::vec3(vx, vy, vz));
				break;
			case 't':
				fscanf(file, "%f %f", &vx, &vy);
				objInfo.uvs.push_back(glm::vec2(vx, vy));
				break;
			default:
				break;
			}
			break;
		case 'f':
			switch (buf[1])
			{
			case '\0':
				int v1, t1, n1;
				int v2, t2, n2;
				int v3, t3, n3;
				int v4, t4, n4;
				fscanf(file, "%d/%d/%d", &v1, &t1, &n1);
				fscanf(file, "%d/%d/%d", &v2, &t2, &n2);
				fscanf(file, "%d/%d/%d", &v3, &t3, &n3);
				objInfo.vIndices.push_back(v1); objInfo.tIndices.push_back(t1); objInfo.nIndices.push_back(n1);
				objInfo.vIndices.push_back(v2); objInfo.tIndices.push_back(t2); objInfo.nIndices.push_back(n2);
				objInfo.vIndices.push_back(v3); objInfo.tIndices.push_back(t3); objInfo.nIndices.push_back(n3);

				if (fscanf(file, "%d/%d/%d", &v4, &t4, &n4) == 3)
				{
					objInfo.vIndices.push_back(v2); objInfo.tIndices.push_back(t2); objInfo.nIndices.push_back(n2);
					objInfo.vIndices.push_back(v3); objInfo.tIndices.push_back(t3); objInfo.nIndices.push_back(n3);
					objInfo.vIndices.push_back(v4); objInfo.tIndices.push_back(t4); objInfo.nIndices.push_back(n4);
				}
				break;
			default:
				break;
			}
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
}

void readObj(char *filename, std::vector<Vert>& vertices, std::vector<int>& indices)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(string(filename), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	// Process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene, vertices, indices);
}

void processNode(aiNode* node, const aiScene* scene, std::vector<Vert>& vertices, std::vector<int>& indices)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene, vertices, indices);
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, vertices, indices);
	}
}

void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<Vert>& vertices, std::vector<int>& indices)
{
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vert vertex;
		glm::vec3 vector;

		//position
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;

		// Normals
		if (mesh->mNormals)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		else
		{
			vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else
		{
			vertex.uv = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	screen->keyCallbackEvent(key, scancode, action, mode);
	screen_light->keyCallbackEvent(key, scancode, action, mode);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
			object.Position.z -= 0.5f;
			std::cout << "Object pos: " << object.Position.x << " " << object.Position.y << " " << object.Position.z << std::endl;
			break;
		case GLFW_KEY_DOWN:
			object.Position.z += 0.5f;
			std::cout << "Object pos: " << object.Position.x << " " << object.Position.y << " " << object.Position.z << std::endl;
			break;
		case GLFW_KEY_LEFT:
			object.Position.x -= 0.5f;
			std::cout << "Object pos: " << object.Position.x << " " << object.Position.y << " " << object.Position.z << std::endl;
			break;
		case GLFW_KEY_RIGHT:
			object.Position.x += 0.5f;
			std::cout << "Object pos: " << object.Position.x << " " << object.Position.y << " " << object.Position.z << std::endl;
			break;
		}
	}
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
	screen->cursorPosCallbackEvent(x, y);
	screen_light->cursorPosCallbackEvent(x, y);

	currentMouseX = x;
	currentMouseY = y;

	if (isLeftDown)
	{
		xRotation += (currentMouseX - oldMouseX) / 125.0f;
		yRotation += (currentMouseY - oldMouseY) / 125.0f;
	}

	if (isMiddleDown)
	{
		xTranslate += (currentMouseX - oldMouseX) / 125.0f;
		yTranslate -= (currentMouseY - oldMouseY) / 125.0f;
	}

	if (isRightDown)
	{
		zoom -= (currentMouseY - oldMouseY) / 125.0f;
	}

	oldMouseX = currentMouseX;
	oldMouseY = currentMouseY;
}

void windowsize_callback(GLFWwindow* window, int width, int height)
{
	IMAGE_WIDTH = width;
	IMAGE_HEIGHT = height;

	nanoguiWindow->setPosition(Eigen::Vector2i(10, 10));
	nanoguiWindow_light->setPosition(Eigen::Vector2i(IMAGE_WIDTH - 210, 10));
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
	screen->mouseButtonCallbackEvent(button, action, mods);
	screen_light->mouseButtonCallbackEvent(button, action, mods);

	if (action == GLFW_RELEASE)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			isLeftDown = false;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			isRightDown = false;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			isMiddleDown = false;
			break;
		}
	}

	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			isLeftDown = true;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			isRightDown = true;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			isMiddleDown = true;
			break;
		}
	}
}