// GLFW - glad
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// // GLM Mathemtics
// #includes <glm/glm.hpp>
// #includes <glm/gtc/matrix_transform.hpp>
// #includes <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "model.hpp"
#include "object.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

namespace visualisation
{


    class render
    {
    private:
        GLFWwindow *window;
        GLuint cubemapTexture;
        Object *room;
        GLuint cubeVBO, cubeVAO, lightVAO, skyboxVAO, skyboxVBO;
        std::vector<VecMat::vec3> modelPosition;
        std::vector<VecMat::vec3> modelRotationVector;
        std::vector<VecMat::vec3> modelScale;
        std::vector<std::string> modelname;
        std::vector<double> modelAngle;
        std::vector<Model> models;
        std::vector<VecMat::vec3> lampPosition;
        std::vector<VecMat::vec3> lightPosition;

        double w;
        double l;
        double h;

    public:
        render(Object *room);
        ~render();
        void getSize();
        void visualise();
        GLuint loadCubemap(std::vector<std::string> faces);
        void initializeVertex();
        void setLampPosition();
        void setLightPosition();
        void initializeGlfw();
        void getModels();
        void setupPointLight(Shader& shader, int index, const VecMat::vec3& position, 
                            const VecMat::vec3& ambient, const VecMat::vec3& diffuse, 
                            const VecMat::vec3& specular, float constant, float linear, float quadratic);
    };
} // namespace visualisation

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Game constants
const float DOOR_OPEN_TRANSLATE_X = -4.5f;
const float DOOR_OPEN_TRANSLATE_Z = 0.75f;
const float DOOR_OPEN_ANGLE = 80.0f;
const float CARD_DISPLAY_OFFSET = 100.0f;
const float CARD_DISPLAY_ANGLE = 90.0f;
const float CANDLE_OFFSET_Y = -0.02f;
const float CANDLE_SCALE = 0.01f;

bool opendoor = false;

// camera
Camera camera(VecMat::vec3(4.0f, 6.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

visualisation::render::render(Object *room)
{
    this->room = room;
    visualise();
}

visualisation::render::~render()
{
}

void visualisation::render::getModels()
{
    for (int i = 0; i < room->children.size(); ++i)
    {
        modelPosition.push_back(room->children[i]->getPosition());
        modelScale.push_back(room->children[i]->getScale());
        modelAngle.push_back(room->children[i]->getAngle());
        modelRotationVector.push_back(room->children[i]->getRotationVector());
        modelname.push_back(room->children[i]->getName());
        Model model(room->children[i]->getModelName());
        models.push_back(model);
    }
}

//Vertices
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};
std::vector<std::string> faces{
     "../resources/skybox/right.jpg",
     "../resources/skybox/left.jpg",
     "../resources/skybox/top.jpg",
     "../resources/skybox/bottom.jpg",
     "../resources/skybox/front.jpg",
     "../resources/skybox/back.jpg"};

 // propPosition removed - using camera.GetHandPosition() instead
 bool nightmode =  true;
 bool yellowcard = false;
 bool bluecard =  false ;
 bool  redcard =  false ;
 bool  greencard =  false ;
 bool displaycard =true;

void visualisation::render::initializeGlfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create glfw window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Escape Room Demonstration in OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // tell GLFW to capture our mouse
    //once the application has focus, the mouse cursor stays within the center of the window
    //        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    std::cout << "WINDOW CREATED!" << std::endl;
}

void visualisation::render::initializeVertex()
{
    // send vertices as input to the first process of the graphics pipeline:
    //the vertex shader and we manage this memory by VBO and it has an unique ID
    //using the glGenBuffers
    //LINKING VERTEX ATTRIBUTES

    //tell OpenGL how it should interpret the vertex data
    //(per vertex attribute) using glVertexAttribPointer:

    //Core OpenGL requires that we use a VAO so it knows what to do
    //with our vertex inputs. If we fail to bind a VAO, OpenGL
    //will most likely refuse to draw anything.
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the cubeVBO, the container's cubeVBO's data already contains the correct data.
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // set the vertex attributes (only position data for our lamp)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

    // we can make a call to the glBufferData function that copies the
    //previously defined vertex data into the buffer's memory:
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    std::cout << "Successfully initialized vertexes" << std::endl;
}

void visualisation::render::setLightPosition()
{
    if(nightmode){
        VecMat::vec3 candlePos=camera.Position+VecMat::normalize(camera.Front)*0.1;
        lightPosition.emplace_back(50, 6, -5.1);
        lightPosition.emplace_back(-50, 6, -5.1);

        lightPosition.push_back(candlePos);
        lightPosition.emplace_back(55, 40, 0);
    }
    else {
        lightPosition.emplace_back(0.8, 6, -5.1);
        lightPosition.emplace_back(-0.8, 6, -5.1);

        lightPosition.emplace_back(3.55, 2.1, -4.6);
        lightPosition.emplace_back(0, 40, 0);
    }

    // Note: propPosition functionality removed - using GetHandPosition() instead
}
void visualisation::render::visualise()
{

    initializeGlfw();

    // build and compile shaders
    // -------------------------
    Shader ourShader("../resources/shaders/mainvertex.vs", "../resources/shaders/mainfragment.fs");    //Lightning Shader
    Shader lampShader("../resources/shaders/lightvertex.vs", "../resources/shaders/lightfragment.fs"); //Light Shader
    Shader skyboxShader("../resources/shaders/skybox.vs", "../resources/shaders/skybox.fs");           //CubeMap Shader

    //initiliaze vertex
    initializeVertex();

    // setLampPosition();
    setLightPosition();

    //Get the models
    getModels();

    // Load cubemap faces
    cubemapTexture = Texture::loadCubemap(faces);

    ourShader.Bind();
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
    skyboxShader.Bind();
    skyboxShader.setInt("skybox", 0);

    Model model("../resources/models/Room/candle.obj");


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = 2 * (currentFrame - lastFrame);
        lastFrame = currentFrame;
        
        // Cache time for lighting calculations (only update once per frame)
        float currentTime = static_cast<float>(time(nullptr));

        // input
        // -----
        glfwPollEvents();
        processInput(window);


        // render
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.Bind();
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setFloat("material.shininess", 32.0f);
        // light properties
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.00001f, 0.00001f, 0.001f);
        ourShader.setVec3("dirLight.diffuse", 0.0f, 0.0f, 0.0f);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        if (nightmode) {
            // Night mode: dimmer, animated lights
            VecMat::vec3 candlePos = camera.Position + camera.Front * 0.1f;
            float cosTime = cos(currentTime);
            float sinTime = sin(currentTime);
            
            setupPointLight(ourShader, 0, lightPosition[0], 
                          VecMat::vec3(0.05f, 0.05f, 0.05f),
                          VecMat::vec3(cosTime, 0.8f, sinTime),
                          VecMat::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
            
            setupPointLight(ourShader, 1, VecMat::vec3(-2.30034f, 5.45702f, -4.67766f),
                          VecMat::vec3(0.05f, 0.05f, 0.05f),
                          VecMat::vec3(sinTime, 0.8f, cosTime),
                          VecMat::vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f, 0.42f);
            
            setupPointLight(ourShader, 2, VecMat::vec3(candlePos.x, candlePos.y, candlePos.z),
                          VecMat::vec3(0.00005f, 0.00005f, 0.00005f),
                          VecMat::vec3(1.0f, 1.0f, 0.5f),
                          VecMat::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.9f, 0.32f);
            
            setupPointLight(ourShader, 3, VecMat::vec3(0.0f, 40.0f, 0.0f),
                          VecMat::vec3(0.15f, 0.15f, 0.15f),
                          VecMat::vec3(0.8f, 0.8f, 0.8f),
                          VecMat::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.04f, 0.0032f);
        }
        else {
            // Day mode: brighter, static lights
            setupPointLight(ourShader, 0, VecMat::vec3(0.8f, 6.0f, -5.1f),
                          VecMat::vec3(0.05f, 0.05f, 0.05f),
                          VecMat::vec3(0.8f, 0.8f, 0.8f),
                          VecMat::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
            
            setupPointLight(ourShader, 1, VecMat::vec3(-0.8f, 6.0f, -5.1f),
                          VecMat::vec3(0.05f, 0.05f, 0.05f),
                          VecMat::vec3(0.8f, 0.8f, 0.8f),
                          VecMat::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.042f);
            
            setupPointLight(ourShader, 2, VecMat::vec3(3.55f, 2.1f, -4.6f),
                          VecMat::vec3(0.05f, 0.05f, 0.05f),
                          VecMat::vec3(1.0f, 1.0f, 0.5f),
                          VecMat::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.0032f);
            
            setupPointLight(ourShader, 3, VecMat::vec3(0.0f, 40.0f, 0.0f),
                          VecMat::vec3(0.15f, 0.15f, 0.15f),
                          VecMat::vec3(0.8f, 0.8f, 0.8f),
                          VecMat::vec3(1.0f, 1.0f, 1.0f), 0.01f, 0.0004f, 0.0013f);
        }

        ourShader.Bind();

        // Transformation matrices
        VecMat::mat4 projection = VecMat::perspective(camera.Zoom, static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT));
        VecMat::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Draw the models
        for (int i = 0; i < models.size(); ++i)
        {
            VecMat::mat4 modelObject = VecMat::mat4(1.0f);
            if (modelname[i] == "door" && opendoor)
            {
                modelObject = VecMat::translate(modelObject, VecMat::vec3(DOOR_OPEN_TRANSLATE_X, 0.0f, DOOR_OPEN_TRANSLATE_Z));
                modelObject = VecMat::rotate(modelObject, to_radians(DOOR_OPEN_ANGLE), VecMat::vec3(0.0f, 1.0f, 0.0f));
                modelObject = VecMat::scale(modelObject, modelScale[i]);
            }
            else
            {
                if (modelname[i] == "redCard" && redcard)
                {
                    modelPosition[i] = VecMat::vec3(-4.32f, 3.66f, 3.29f);
                    modelAngle[i] = CARD_DISPLAY_ANGLE;
                    if (!displaycard) {
                        modelPosition[i] = VecMat::vec3(CARD_DISPLAY_OFFSET, CARD_DISPLAY_OFFSET, CARD_DISPLAY_OFFSET);
                    }
                }
                if (modelname[i] == "yellowCard" && yellowcard)
                {
                    modelPosition[i] = VecMat::vec3(-4.32f, 3.11f, 3.29f);
                    modelAngle[i] = CARD_DISPLAY_ANGLE;
                    if (!displaycard) {
                        modelPosition[i] = VecMat::vec3(CARD_DISPLAY_OFFSET, CARD_DISPLAY_OFFSET, CARD_DISPLAY_OFFSET);
                    }
                }
                if (modelname[i] == "blueCard" && bluecard)
                {
                    modelPosition[i] = VecMat::vec3(-4.32f, 3.70f, 4.42f);
                    modelAngle[i] = CARD_DISPLAY_ANGLE;
                    if (!displaycard) {
                        modelPosition[i] = VecMat::vec3(CARD_DISPLAY_OFFSET, CARD_DISPLAY_OFFSET, CARD_DISPLAY_OFFSET);
                    }
                }
                if (modelname[i] == "greenCard" && greencard)
                {
                    modelPosition[i] = VecMat::vec3(-4.32f, 3.09f, 4.40f);
                    modelAngle[i] = CARD_DISPLAY_ANGLE;
                    if (!displaycard) {
                        modelPosition[i] = VecMat::vec3(CARD_DISPLAY_OFFSET, CARD_DISPLAY_OFFSET, CARD_DISPLAY_OFFSET);
                    }
                }

                modelObject = VecMat::translate(modelObject, modelPosition[i]);
                modelObject = VecMat::rotate(modelObject, to_radians(static_cast<float>(modelAngle[i])), VecMat::vec3(0.0f, 1.0f, 0.0f));
                modelObject = VecMat::scale(modelObject, modelScale[i]);



            }

            ourShader.setMat4("model", modelObject);
            models[i].Draw(ourShader);
        }

        VecMat::mat4 candle(1.0f);
        VecMat::vec3 candlePos = camera.Position + camera.Front * 0.1f;
        candle = VecMat::translate(candle, VecMat::vec3(candlePos.x, candlePos.y + CANDLE_OFFSET_Y, candlePos.z));
        candle = VecMat::scale(candle, VecMat::vec3(CANDLE_SCALE, CANDLE_SCALE, CANDLE_SCALE));

        ourShader.setMat4("model", candle);
        if(displaycard)
        {
            model.Draw(ourShader);
        }


        // Note: propPosition functionality removed - camera now uses GetHandPosition()

        // Light objects (lamps)
        lampShader.Bind();
        for (unsigned int i = 0; i < 2; i++)
        {
            VecMat::mat4 lampModel(1.0f);
            lampModel = VecMat::translate(lampModel, lightPosition[i]);
            lampModel = VecMat::rotate(lampModel, to_radians(90.0f), VecMat::vec3(0.0f, 1.0f, 0.0f));
            lampModel = VecMat::scale(lampModel, VecMat::vec3(0.03f, 0.05f, 1.8f));
            lampShader.setMat4("model", lampModel);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for (unsigned int i = 2; i < 4; i++)
        {
            VecMat::mat4 lampModel(1.0f);
            lampModel = VecMat::translate(lampModel, lightPosition[i]);
            lampModel = VecMat::rotate(lampModel, to_radians(90.0f), VecMat::vec3(1.0f, 0.0f, 0.0f));
            lampModel = VecMat::scale(lampModel, VecMat::vec3(0.1f, 0.15f, 0.15f));
            lampShader.setMat4("model", lampModel);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Draw skybox as last
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Bind();
        VecMat::mat4 skyboxView = VecMat::mat4(VecMat::mat3(camera.GetViewMatrix())); // Remove translation
        skyboxShader.setMat4("view", skyboxView);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //All the buffers are deleted in destructor
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &cubeVBO);
    std::cout << "Successfully deleted Buffers" << std::endl;

    glfwTerminate();
}

// Helper function to setup point lights
void visualisation::render::setupPointLight(Shader& shader, int index, const VecMat::vec3& position, 
                                            const VecMat::vec3& ambient, const VecMat::vec3& diffuse, 
                                            const VecMat::vec3& specular, float constant, float linear, float quadratic)
{
    std::string baseName = "pointLights[" + std::to_string(index) + "].";
    shader.setVec3(baseName + "position", position);
    shader.setVec3(baseName + "ambient", ambient);
    shader.setVec3(baseName + "diffuse", diffuse);
    shader.setVec3(baseName + "specular", specular);
    shader.setFloat(baseName + "constant", constant);
    shader.setFloat(baseName + "linear", linear);
    shader.setFloat(baseName + "quadratic", quadratic);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        opendoor=true;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE)
        opendoor = false;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        //greencard = !greencard;

        VecMat::vec3 candlePos = camera.Position + camera.Front*0.1;

        if ((-3.9 < candlePos.x && candlePos.x < -3.2) && ((0.01 < (candlePos.y - 0.02)) && ((candlePos.y - 0.02) < 0.10)) && (-3.56 < candlePos.z) && (candlePos.z < -3.0))
        {
            std::cout << "Blue card found!" << std::endl;
            bluecard = true;
        }
        if ((3.3 < candlePos.x && candlePos.x < 3.9) && ((0.9 < (candlePos.y - 0.02)) && ((candlePos.y - 0.02) < 1.2)) && (-4.4 < candlePos.z) && (candlePos.z < -3.9))
        {
            std::cout << "Red card found!" << std::endl;
            redcard = true;
        }
        if ((-3.6 < candlePos.x && candlePos.x < -3.0) && ((4.4 < (candlePos.y - 0.02)) && ((candlePos.y - 0.02) < 5.2)) && (-5.2 < candlePos.z && candlePos.z < -4.4))
        {
            std::cout << "Green card found!" << std::endl;
            greencard = true;
        }
        if ((2.0 < candlePos.x && candlePos.x < 2.6) && ((0.4 < (candlePos.y - 0.02)) && ((candlePos.y - 0.02) < 1.2)) && (2.5 < candlePos.z && candlePos.z < 3.2))
        {
            std::cout << "Yellow card found!" << std::endl;
            yellowcard = true;
        }
//        if(bluecard && redcard && greencard && yellowcard)
//        {
//            nightmode=false;
//            opendoor = true;
//
//        }
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE){
        if(bluecard && redcard && greencard && yellowcard)
        {
            std:: cout<<"All cards are found. Lights turned on! \n Door Opened!";
            nightmode=false;
            camera.freeCamMode = true;  // Enable free camera mode after escaping
            opendoor = true;
            displaycard =false;
//            camera.Front=VecMat::vec3(2.40302,3.35561,2.98072);

        }
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
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
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double mouse_x, mouse_y;
        //getting cursor position
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        // Debug: Cursor position (commented out for performance)
        // std::cout << "Cursor Position at (" << mouse_x << " : " << mouse_y << ")" << std::endl;

        //3D normalized device coordinates
        float x = (2.0f * mouse_x) / SCR_WIDTH - 1.0f;
        float y = 1.0f - (2.0f * mouse_y) / SCR_HEIGHT;
        float z = 1.0f;
        VecMat::vec3 ray_nds = VecMat::vec3(x, y, z);

        //4d Homogeneous Clip Coordinates
//        vec4 ray_clip = vec4(ray_nds.xy, -1.0, 1.0);
    }
}
