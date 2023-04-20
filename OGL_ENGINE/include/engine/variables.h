#include <GLFW/glfw3.h>
#include <engine/Billboard.h>
#include <engine/CollisionBox.h>
#include <engine/Objectives.h>
#include <engine/Particles.h>
#include <engine/Plane.h>
#include <engine/QuadTexture.h>
#include <engine/RigidModel.h>
#include <engine/Terrain.h>
#include <engine/shader_m.h>
#include <engine/skybox.h>
#include <engine/textrenderer.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "gameMap.h"
#include "Puertas.h"

enum LightType
{
    FlatColor,
    AllLights,
    DirectionalLight,
    SpotLight,
    PointLight
};

enum Axis
{
    X,
    Y,
    Z
};
enum TypeActionKeyBoard
{
    GAME,
    OBJECTS,
    COLLISION,
    LIGHTING,
};

enum TransformObject
{
    MODEL,
    COLLBOX,
};

//:::: ENUMS :::://
LightType lightType = FlatColor;
TypeActionKeyBoard typemenu = GAME;
TransformObject transformObject = COLLBOX;

//:::: VARIABLES GLOBALES :::://
int moveCount = 0;

//:::: BANDERAS :::://


// 
//VENTANA
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//CONTROL
double xJoy = 0.0;
double yJoy = 0.0;
double yLeftJoy = 0.0;
bool isJoyStick = false;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//TIMING
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//TEXTURAS DEL TERRENO
const char **texturePaths;

//PARTICULAS
float respawnCount = 0.0f;
Particles particles;
ParticleProps m_Particle;
bool updateParticles = false;


//:::: OBJETOS :::://
Camera camera(glm::vec3(1.7f, 1.2f, 2.5f));
//3, -0.1, 3.7
glm::vec3 camPos(0);
stopCam stop;
float camYaw;


//AGUA
Plane agua;
float waterX, waterY;
bool waterOut = false;

// Billbord
Billboard smoke;
float smokeX, smokeY;

//:::: VECTORES :::://
glm::vec3 force(0);
glm::vec3 posModel(0);
glm::vec3 rotationModel(0);
glm::vec3 scaleCollBox(0);
glm::vec3 skyPos(0);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDir(0.0f, 1.0f, 0.0f);
vec3 originPoint(0.0f, 1.0f, 0.0f);
vec3 rotationPoint(0.0, 1.0, 0.0);
vec3 mainLight(0.5);

//::::::::::::::MOVER OBJETOS:::::::::::::://
float initScale = 0.2f;
int indexObject = 0;
int indexCollBox = 0;
int indexLight = 0;

bool isCollBoxModel = false;
bool renderCollBox = false;
bool renderLightingCubes = false;
float angleObjects = 0.0f;
float movement = 0.01;

//::::::::::::::ARREGLOS DE OBJETOS:::::::::::::://
vector<glm::vec3> pointLightPositions;
vector<glm::vec3> physicsObjectsPositions;

vector<RigidModel> rigidModels;
vector<RigidModel> rbmodels = rigidModels;

vector<Model> models;
bool callbackCalled = false;

map<int, pair<string, CollisionBox>> collboxes;
map<int, pair<string, CollisionBox>> lightcubes;
CollisionBox* cb = new CollisionBox();

//::::::::::::::FISICAS:::::::::::::://
rbEnvironment physicsEnviroment;
rbRigidBody piso, pared, barrera;
int indexRigidModel = 0;
float colDetection = 0;

// ::::::::::::::: GUI ::::::::::::::::::: //
QuadTexture yoshiIcon, eggIcon, coinIcon, yoshiHP, eggHP;
QuadTexture dialogBox, coinBox;
QuadTexture rainSprite;
float rainX = 0.0, rainY = 0.0;

const char* text = "";
TextRenderer* initGameTxt;
TextRenderer* gameOverTxt;
TextRenderer* coinCount;

//::::::::::::::::::::: VARIABLES CAMBIO SKYBOX :::::::::::::::::::::::
int changeSkyBoxTexture = 0;
float cambioSky = 0.5;
float contadorSky = 0;
int minuteCount = 0;
bool isNight = false, isDay = false;
bool isRaining = false;

// HP
int indiHpYoshi, indiHpEgg;
int initHpYoshi, initHpEgg;

// Puertas
puertaL doorL;
puertaR doorR;