
#include <GLFW/glfw3.h>
#include <engine/Billboard.h>
#include <engine/CollisionBox.h>
#include <engine/Objectives.h>
#include <engine/Particles.h>
#include <engine/Plane.h>
#include <engine/QuadTexture.h>
#include <engine/RigidModel.h>
#include <engine/Terrain.h>
#include <engine/functions.h>
#include <engine/shader_m.h>
#include <engine/skybox.h>
#include <engine/textrenderer.h>
#include <glad/glad.h>
#include <iostream>



int main()
{
   

    //:::: INICIALIZAMOS GLFW CON LA VERSIÓN 3.3 :::://
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //:::: CREAMOS LA VENTANA:::://
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Yoshi Defense", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //:::: OBTENEMOS INFORMACIÓN DE TODOS LOS EVENTOS DE LA VENTANA:::://
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetJoystickCallback(joystick_callback);

    //:::: DESHABILITAMOS EL CURSOR VISUALMENTE EN LA PANTALLA :::://
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //:::: INICIALIZAMOS GLAD CON LAS CARACTERÍSTICAS DE OPENGL ESCENCIALES :::://
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //INICIALIZAMOS LA ESCENA
    Shader ourShader("shaders/multiple_lighting.vs", "shaders/multiple_lighting.fs");
    Shader selectShader("shaders/selectedShader.vs", "shaders/lighting_maps.fs");
    initScene(ourShader);
    Terrain terrain("textures/heightmap3.jpg", texturePaths);
    SkyBox sky(1.0f, "9");
    cb = isCollBoxModel ? &models[indexCollBox].collbox : &collboxes.at(indexCollBox).second;
    //enemiesMap();
    //buffsMap();
    //hpCheck();

    firstTime = false;

    //:::: RENDER:::://
    while (!glfwWindowShouldClose(window))
    {

        //::::TIMING:::://Ayuda a crear animaciones fluidas
        float currentFrame = glfwGetTime();
        deltaTime = (currentFrame - lastFrame);
        lastFrame = currentFrame;
        respawnCount += 0.1;

        //::::ENTRADA CONTROL:::://
        processInput(window);
        //:::: LIMPIAMOS BUFFERS:::://
        glClearColor(0.933f, 0.811f, 0.647f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //:::: PASAMOS INFORMACIÓN AL SHADER:::://
        ourShader.use();

        //:::: DEFINICIÓN DE MATRICES::::// La multiplicaciónd e model*view*projection crea nuestro entorno 3D
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

 
        // Update map
        //enemiesMap();
        //buffsMap();

        //:::: RENDER DE MODELOS:::://
        drawModels(&ourShader, view, projection);
        //loadEnemyAnim(&ourShader);
        //loadBuffAnim(&ourShader);

        //:::: SKYBOX Y TERRENO:::://
        loadEnviroment(&terrain, &sky, view, projection);

        //:::: AGUA :::://
        drawWater(view, projection);

        //:::: PARTICLES :::://
        drawParticles(view, projection);

        //:::: BILLBOARDS :::://
        //drawBillboard();

        // ::::::::::::::: GUI ::::::::::::::::::: //
        //drawGUI();

        //:::: COLISIONES :::://
        //collisions();

        // HP check
        //hpCheck();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //:::: LIBERACIÓN DE MEMORIA:::://   
    delete[] texturePaths;
    sky.Release();
    terrain.Release();
    for (int i = 0; i < lightcubes.size(); i++)
        lightcubes[i].second.Release();
    for (int i = 0; i < collboxes.size(); i++)
        collboxes[i].second.Release();
    for (int i = 0; i < models.size(); i++)
        models[i].Release();
    for (int i = 0; i < rigidModels.size(); i++)
    {
        physicsEnviroment.Unregister(rigidModels[i].getRigidbox());
    }
    physicsEnviroment.Unregister(&piso);
    physicsEnviroment.Unregister(&pared);
    agua.Release();
    smoke.Release();
    particles.Release();
    //yoshiIcon.Release();
    //eggIcon.Release();
    //coinIcon.Release();
    //yoshiHP.Release();
    //eggHP.Release();
    //rainSprite.Release();
    //dialogBox.Release();
    glfwTerminate();

    return 0;
}

void initScene(Shader ourShader)
{

    // Hp
    //initHpYoshi = Yoshi.getHP();
    //initHpEgg = Egg.getHp();

    //AGUA
    agua = Plane("textures/agua3.jpg", 500.0, 500.0, 0, 0);
    agua.setPosition(glm::vec3(3, -0.2, 4.79));
    agua.setAngles(glm::vec3(90.0, 0, 0));
    agua.setScale(glm::vec3(30.0));

    // Smoke
    //smoke = Billboard("textures/smokeText1.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 20.0f, 36.0f);
    //smoke.setPosition(glm::vec3(19.87, 0.9, 17.33));
    //smoke.setScale(1.5f);

    particles = Particles("textures/smoke.png");
    //:::: CONFIGURAMOS LOS VALORES DE MOVIMIENTO DE LAS PARTICULAS :::://
    m_Particle.ColorBegin = { 254 / 255.0f,212 / 255.0f,123 / 255.0f,1.0f };
    m_Particle.ColorEnd = { 254 / 255.0f,109 / 255.0f,41 / 255.0f,1.0f };
    m_Particle.SizeVariation = 0.3f;
    m_Particle.SizeBegin = 1.0f;
    m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = { 0.2f,1.0f,0.5f };
    m_Particle.VelocityVariation = { 0.2f,1.0f,1.0f };
    m_Particle.Position = { 0.0f,0.0f,0.0f };

    //:::: DEFINIMOS LAS TEXTURAS DE LA MULTITEXTURA DEL TERRENO :::://
    texturePaths = new const char* [4];
    texturePaths[0] = "textures/multitexture_colors3.jpg";
    texturePaths[1] = "textures/terrain7.png"; // ROJO
    texturePaths[2] = "textures/terrain5.png"; // VERDE
    texturePaths[3] = "textures/terrain8.png"; // AZUL

    //:::: POSICIONES DE TODAS LAS LUCES :::://
    pointLightPositions.push_back(glm::vec3(2.3f, 5.2f, 2.0f));
    pointLightPositions.push_back(glm::vec3(2.3f, 10.3f, -4.0f));
    pointLightPositions.push_back(glm::vec3(1.0f, 9.3f, -7.0f));
    pointLightPositions.push_back(glm::vec3(0.0f, 10.0f, -3.0f));

    //:::: POSICIONES DE TODOS LOS OBJETOS CON FISICAS :::://
    physicsObjectsPositions.push_back(glm::vec3(0.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(2.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(1.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(-2.0, 10.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(-2.0, 10.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(15.0, 1.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(15.0, 1.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(25.0, 10.0, -2.0));


    //:::: ESTADO GLOBAL DE OPENGL :::://
    glEnable(GL_DEPTH_TEST);

    //Definimos los collbox de la camara
    camera.setCollBox();
    scaleCollBox = camera.collbox.getScale();
    scaleCollBox.x -= 0.8;
    scaleCollBox.z -= 0.8;
    scaleCollBox.y += 1.0;
    camera.collbox.setScale(scaleCollBox);
    camPos = camera.Position;

    //:::: CARGAMOS LOS SHADERS :::://
    ourShader.use();

    //:::: INICIALIZAMOS NUESTROS MODELOS :::://   
    // Crustaceo
    models.push_back(Model("KrustyKrab", "models/KrustyKrab/KrustyKrab.obj", glm::vec3(18, 0.0, 2.7), glm::vec3(0, -90, 0), 0.0f, initScale));
    models.push_back(Model("KrustySign", "models/KrustySign/KrustySign.obj", glm::vec3(16, -0.1, 14.2), glm::vec3(0, -90, 0), 0.0f, initScale));
    models.push_back(Model("hourSign", "models/24hoursign/24hoursign.obj", glm::vec3(12.5, 1.6, 6.4), glm::vec3(0, -90, 0), 0.0f, initScale));

    // Muebles 
    models.push_back(Model("Boat", "models/Boat/Boat.obj", glm::vec3(22.4, 0.1, 2.7), glm::vec3(0, -90, 0), 0.0f, initScale));
    models.push_back(Model("Menu", "models/Menu/Menu.obj", glm::vec3(22.4, 1.6, 2.7), glm::vec3(0, -90, 0), 0.0f, initScale));
    models.push_back(Model("Table01", "models/Seats/Seats.obj", glm::vec3(18.5, -0.1, -1.1), glm::vec3(0, -180, 0), 0.0f, 0.18f));
    models.push_back(Model("Table02", "models/Seats/Seats.obj", glm::vec3(15.0, -0.1, -1.1), glm::vec3(0, -180, 0), 0.0f, 0.18f));
    models.push_back(Model("Table03", "models/Seats/Seats.obj", glm::vec3(18.5, -0.1, 7.06), glm::vec3(0, 0, 0), 0.0f, 0.18f));
    models.push_back(Model("Table04", "models/Seats/Seats.obj", glm::vec3(15.0, -0.1, 7.06), glm::vec3(0, 0, 0), 0.0f, 0.18f));
    models.push_back(Model("vendingMachine", "models/vendingMachine/vendingMachine.obj", glm::vec3(24.3, -0.1, -1.5), glm::vec3(0, -90, 0), 0.0f, 0.18f));
    models.push_back(Model("Arcade", "models/Arcade/Arcade.obj", glm::vec3(24.3, -0.1, 0.3), glm::vec3(0, -90, 0), 0.0f, 0.18f));
    models.push_back(Model("Bin", "models/Bin/Bin.obj", glm::vec3(24.3, -0.1, 5.4), glm::vec3(0, -90, 0), 0.0f, 0.18f));
    models.push_back(Model("Jukebox", "models/Jukebox/Jukebox.obj", glm::vec3(24.3, -0.1, 6.9), glm::vec3(0, -90, 0), 0.0f, 0.18f));

    // Objetos
    // En mesa 1
    models.push_back(Model("Burger", "models/Burger/Burger.obj", glm::vec3(18.5, 0.8, -0.7), glm::vec3(0, 0, 0), 0.0f, 0.18f));
    // En mesa 2
    models.push_back(Model("Soda", "models/Soda/Soda.obj", glm::vec3(15.0, 0.8, -0.9), glm::vec3(0, 0, 0), 0.0f, 0.18f));
    // En mesa 3
    models.push_back(Model("Notebook", "models/Notebook/Notebook.obj", glm::vec3(18.5, 0.8, 6.4), glm::vec3(0, -45, 0), 0.0f, 0.12f));

    // Fuera
    models.push_back(Model("fireHydr", "models/fireHydr/fireHydr.obj", glm::vec3(5.2, -0.1, 9.47), glm::vec3(0, -90, 0), 0.0f, 0.18f));
    models.push_back(Model("volkswagenGolf", "models/volkswagenGolf/volkswagenGolf.obj", glm::vec3(19.89, -0.15, 18.3), glm::vec3(0, -90, 0), 0.0f, 0.063f));
    models.push_back(Model("sportsCar", "models/sportsCar/sportsCar.obj", glm::vec3(20.19, -0.1, 20.9), glm::vec3(0, -90, 0), 0.0f, 0.08f));
    models.push_back(Model("Trash", "models/Trash/Trash.obj", glm::vec3(21.78, -0.1, 12.24), glm::vec3(0, 0, 0), 0.0f, 0.35f));



    // Puertas
    doorL.setInitModel();
    doorR.setInitModel();

    // Yoshi
    //Yoshi.setInitModel();
    //Yoshi.setAnimation();


    //CREAMOS TODAS  LAS CAJAS DE COLISION INDIVIDUALES
    CollisionBox collbox;
    glm::vec4 colorCollbox(0.41f, 0.2f, 0.737f, 0.06f);
                                        // Lugar                  //Escala
    collbox = CollisionBox(glm::vec3(-2.1f, -0.2f, 3.0f), glm::vec3(2.27, 3, 12.6), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(0, pair<string, CollisionBox>("cbBarrier1", collbox)));
    //collbox = CollisionBox(glm::vec3(0.0f, -0.2f, 11.6f), glm::vec3(4.3, 5, 4.4), colorCollbox);
    //collboxes.insert(pair<int, pair<string, CollisionBox>>(1, pair<string, CollisionBox>("cbBarrier2", collbox)));
    //collbox = CollisionBox(glm::vec3(1.0f, -0.2f, -3.9f), glm::vec3(4.0, 5, 4.6), colorCollbox);
    //collboxes.insert(pair<int, pair<string, CollisionBox>>(2, pair<string, CollisionBox>("cbBarrier3", collbox)));
    //collbox = CollisionBox(glm::vec3(24.4, 0.1, 1.5), glm::vec3(1.0, 3, 0.8), colorCollbox);
    //collboxes.insert(pair<int, pair<string, CollisionBox>>(3, pair<string, CollisionBox>("cbPared1", collbox)));
    //collbox = CollisionBox(glm::vec3(24.4, 0.1, 4.5), glm::vec3(1.0, 3, 0.8), colorCollbox);
    //collboxes.insert(pair<int, pair<string, CollisionBox>>(4, pair<string, CollisionBox>("cbPared2", collbox)));
    //collbox = CollisionBox(glm::vec3(27.2, 0.1, 5.5), glm::vec3(4.5, 3, 0.8), colorCollbox);
    //collboxes.insert(pair<int, pair<string, CollisionBox>>(5, pair<string, CollisionBox>("cbPared3", collbox)));
    //collbox = CollisionBox(glm::vec3(30.6, 0.1, 3.0), glm::vec3(1.1, 3, 3.4), colorCollbox);
    //collboxes.insert(pair<int, pair<string, CollisionBox>>(6, pair<string, CollisionBox>("cbPared4", collbox)));
    //collbox = CollisionBox(glm::vec3(27.2, 0.1, 0.5), glm::vec3(4.5, 3, 0.8), colorCollbox);
    //collboxes.insert(pair<int, pair<string, CollisionBox>>(7, pair<string, CollisionBox>("cbPared5", collbox)));


    //CREAMOS LOS LIGHTCUBES QUE ENREALIDAD SON COLLISION BOXES QUE NOS AYUDARAN A IDENTIFICAR LA POSICIÓN DE DONDE ESTAN
    glm::vec3 lScale(0.5);
    colorCollbox = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    collbox = CollisionBox(pointLightPositions[0], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(0, pair<string, CollisionBox>("LUZ1", collbox)));
    collbox = CollisionBox(pointLightPositions[1], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(1, pair<string, CollisionBox>("LUZ2", collbox)));
    collbox = CollisionBox(pointLightPositions[2], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(2, pair<string, CollisionBox>("LUZ3", collbox)));
    collbox = CollisionBox(pointLightPositions[3], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(3, pair<string, CollisionBox>("LUZ4", collbox)));

    // ::::::::::::::: GUI ::::::::::::::::::: //
    //yoshiIcon = QuadTexture("GUI/yoshiIcon.png", 49.0f, 50.0f, 0, 0);
    //eggIcon = QuadTexture("GUI/eggIcon.png", 60.0f, 70.0f, 0, 0);
    //coinIcon = QuadTexture("GUI/Coin.png", 325.0f, 395.0f, 0, 0);
    //yoshiHP = QuadTexture("GUI/hpBar.png", 105.0f, 7.0f, 0.5, 0); 
    //eggHP = QuadTexture("GUI/hpBar.png", 105.0f, 7.0f, 0.5, 0);
    //dialogBox = QuadTexture("GUI/UI-01.png", 395.0f, 103.0f, 0.0, 0);
    //rainSprite = QuadTexture("GUI/Rain.png", 85.0f, 73.0f, 0.0, 0);
     
    // ::::::::::::::: TEXT RENDERER ::::::::::::::::::: //
    //text = "Presione ENTER para comenzar";
    //initGameTxt = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    //initGameTxt->Load("fonts/LycheeSoda.TTF", 60);

    //gameOverTxt = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    //gameOverTxt->Load("fonts/LycheeSoda.TTF", 60);

    //coinCount = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    //coinCount->Load("fonts/LycheeSoda.TTF", 60);
}




//:::: CONFIGURACIONES :::://

void loadEnviroment(Terrain* terrain, SkyBox* sky, glm::mat4 view, glm::mat4 projection)
{
    glm::mat4 model = mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, -2.5f, 0.0f));   // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(100.5f, 100.0f, 100.5f)); // it's a bit too big for our scene, so scale it down

    terrain->draw(model, view, projection);
    terrain->getShader()->setFloat("shininess", 100.0f);
    setMultipleLight(terrain->getShader(), pointLightPositions);

    glm::mat4 skyModel = mat4(1.0f);
    skyModel = glm::translate(skyModel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    skyModel = glm::scale(skyModel, glm::vec3(40.0f, 40.0f, 40.0f));  // it's a bit too big for our scene, so scale it down
    sky->draw(skyModel, view, projection, skyPos);
    sky->getShader()->setFloat("shininess", 10.0f);
    setMultipleLight(sky->getShader(), pointLightPositions);

    //RENDER DE LIGHT CUBES
    if (renderLightingCubes)
        for (pair<int, pair<string, CollisionBox>> lights : lightcubes)
            lights.second.second.draw(view, projection);

    // Cambio de skybox (amanecer)
    //contadorSky = (float)glfwGetTime();
    //int totalSec;
    //if (contadorSky > 60) {// Sacar el residuo
    //    totalSec = (int)contadorSky % 60;
    //    //if (totalSec == 0)
    //    //    minuteCount++;
    //}
    //else {
    //    totalSec = (int)contadorSky;
    //    //if (totalSec >= 60)
    //    //    minuteCount++;
    //}
    //

    //if (/*minuteCount % 2 == 0*/  totalSec >= 0 && totalSec <= 30) {
    //    if (cambioSky > 0.2) {
    //        mainLight = vec3(cambioSky);
    //        cambioSky -= 0.001;
    //    }
    //    else {
    //        if (changeSkyBoxTexture == 0)
    //        {
    //            isNight = true;
    //            isDay = false;
    //            sky->reloadTexture("7");
    //            changeSkyBoxTexture++;
    //        }
    //    }
    //}
    //else {
    //    if (cambioSky < 0.5) {
    //        mainLight = vec3(cambioSky);
    //        cambioSky += 0.001;
    //    }
    //    else {
    //        if (changeSkyBoxTexture == 1)
    //        {
    //            isDay = true;
    //            isNight = false;
    //            sky->reloadTexture("6");
    //            changeSkyBoxTexture--;
    //        }
    //    }
    //}


}
void drawModels(Shader *shader, glm::mat4 view, glm::mat4 projection)
{

    //DEFINIMOS EL BRILLO  DEL MATERIAL
    shader->setFloat("material.shininess", 60.0f);
    setMultipleLight(shader, pointLightPositions);  

    // Puertas
    doorL.Slide(*shader);
    doorR.Slide(*shader);

    // Arquitectura
    for (int i = 0; i < models.size(); i++)
    {
        //SI SE RECOGIO EL OBJETO
        shader->use();

        models[i].Draw(*shader);
        detectColls(&models[i].collbox, models[i].name, &camera, renderCollBox, collidedObject_callback);
    }

    // Personaje principal
    //if (!Yoshi.isPunching)
    //    Yoshi.model.Draw(*shader);
    //else
    //    Yoshi.Punch(*shader);

    // Huevito
    //Egg.model.Draw(*shader);

}


void drawBillboard() {

    //if (smokeY <= 4.0) {
        if (smokeX <= 4.0f)
            smokeX += 0.05f;
        else // {
            smokeX = 0.0f;
            //smokeY += 1.0f;
        //}
    //}
    //else {
    //    smokeX = 0.0f;
    //    smokeY = 0.0f;
    //}

    smoke.Draw(camera, round(smokeX), round(smokeY));
}

void drawParticles(glm::mat4 view, glm::mat4 projection) {
    particles.OnUpdate(0.01);// velocidad
    particles.Draw(glm::vec3(19.87, 0.9, 17.33), view, projection);
    if (respawnCount >= 1.5)//tiempo que tarda en volver el spawn
    {
        for (int i = 0; i < 12; i++)//el valor es la cantidad de planos
        {
            particles.Emit(m_Particle);
            updateParticles = false;
            respawnCount = 0;
        }
    }
}

void drawWater(glm::mat4 view, glm::mat4 projection) {
    if (waterX <= 2.0 && waterY <= 2.0 && !waterOut) {
        waterX += 0.001;
        waterY += 0.001;
    }
    else {
        waterOut = true;
        if (waterY > 0.0f && waterX > 0.0f && waterOut) {
            waterX -= 0.001;
            waterY -= 0.001;
        }
        else
            waterOut = false;
    }
    agua.draw(waterX, waterY, view, projection);

}


void drawGUI() {

    // Icons
    //yoshiIcon.Draw(glm::vec2(-0.8f, 0.8f), 0.19);
    //eggIcon.Draw(glm::vec2(0.2f, 0.8f), 0.18);
    //coinIcon.Draw(glm::vec2(-0.8f, 0.55f), glm::vec3(0.15f, 0.2f, 0.0));

    //// HP BARS
    //float yoshiX = 0.0, yoshiY;
    ////Yoshi
    //if (indiHpYoshi == 100) 
    //    yoshiY = 0.0;
    //if (indiHpYoshi == 75)
    //    yoshiY = 1.1;
    //if (indiHpYoshi == 50)
    //    yoshiY = 2.2;
    //if (indiHpYoshi == 25)
    //    yoshiY = 3.3;
    //if (indiHpYoshi == 0)
    //    yoshiY = 4.4;

    //yoshiHP.changeSprite(yoshiX, yoshiY);
    //yoshiHP.Draw(glm::vec2(-0.4f, 0.8f), glm::vec3(0.5f, 0.15f, 0.0));

    //// Egg
    //float eggX = 0.0, eggY;
    //if (indiHpEgg == 100)
    //    eggY = 0.0;
    //if (indiHpEgg == 75)
    //    eggY = 1.1;
    //if (indiHpEgg == 50)
    //    eggY = 2.2;
    //if (indiHpEgg == 25)
    //    eggY = 3.3;
    //if (indiHpEgg == 0)
    //    eggY = 4.4;

    //eggHP.changeSprite(eggX, eggY);
    //eggHP.Draw(glm::vec2(0.6f, 0.8f), glm::vec3(0.5f, 0.15f, 0.0));
   

    //// Coin amount
    //int playerScore = Yoshi.getScore();
    //string coinAmount;
    //if (playerScore < 10)
    //    coinAmount = "0" + to_string(playerScore);
    //else
    //    coinAmount = to_string(playerScore);
    //text = coinAmount.c_str();
    //coinCount->RenderText(text, -0.68f, -0.58f, 0.0027f, glm::vec3(0.1f, 0.0f, 0.0f));

    //if (!gameStarted) {
    //    // Instrucciones inicio
    //    text = "¡Protege el objeto de los enemigos.";
    //    initGameTxt->RenderText(text, -0.67f, 0.5f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
    //    text = "Presione M para comenzar.";
    //    initGameTxt->RenderText(text, -0.56f, 0.6f, 0.0018f, glm::vec3(0.1f, 0.0f, 0.0f));
    //    dialogBox.Draw(glm::vec2(0.0f, -0.6f), glm::vec3(1.5f, 0.5f, 0.5));
    //}
   
    ////Game Over
    //if (gameOver) {
    //    text = "GAME OVER";
    //    gameOverTxt->RenderText(text, -0.35f, 0.55f, 0.0025f, glm::vec3(0.1f, 0.0f, 0.0f));
    //    dialogBox.Draw(glm::vec2(0.0f, -0.6f), glm::vec3(1.5f, 0.5f, 0.5));
    //}

    //// Juego completado
    //if (gameFinished) {
    //    text = "Juego completado";
    //    gameOverTxt->RenderText(text, -0.5f, 0.55f, 0.0025f, glm::vec3(0.1f, 0.0f, 0.0f));
    //    dialogBox.Draw(glm::vec2(0.0f, -0.6f), glm::vec3(1.5f, 0.5f, 0.5));
    //}

        // Lluvia
    //if (isNight) {
    //    if (rainY >= -4)
    //        rainY -= 0.4;
    //    
    //    else 
    //        rainY = 0.0;
    //    
    //    rainSprite.changeSprite(round(rainX), rainY);
    //    rainSprite.Draw(glm::vec2(0.0f, 0.0f), glm::vec3(1.9f, 1.9f, 0.0f));
    //}
}

void setSimpleLight(Shader *shader)
{
    shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader->setInt("lightType", (int)lightType);
    shader->setVec3("light.position", lightPos);
    shader->setVec3("light.direction", lightDir);
    shader->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader->setVec3("viewPos", camera.Position);
    shader->setFloat("light.constant", 0.2f);
    shader->setFloat("light.linear", 0.02f);
    shader->setFloat("light.quadratic", 0.032f);
    shader->setFloat("material.shininess", 60.0f);
}
void setMultipleLight(Shader *shader, vector<glm::vec3> pointLightPositions)
{
    shader->setVec3("viewPos", camera.Position);

    shader->setVec3("dirLights[0].direction", pointLightPositions[0]);
    shader->setVec3("dirLights[0].ambient", mainLight.x, mainLight.y, mainLight.z);
    shader->setVec3("dirLights[0].diffuse", mainLight.x, mainLight.y, mainLight.z);
    shader->setVec3("dirLights[0].specular", mainLight.x, mainLight.y, mainLight.z);

    shader->setVec3("dirLights[1].direction", pointLightPositions[1]);
    shader->setVec3("dirLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("dirLights[1].diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLights[1].specular", 0.5f, 0.5f, 0.5f);

    shader->setVec3("dirLights[2].direction", pointLightPositions[2]);
    shader->setVec3("dirLights[2].ambient", 0.3f, 0.5f, 0.5f);
    shader->setVec3("dirLights[2].diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLights[2].specular", 0.5f, 0.5f, 0.5f);

    shader->setVec3("dirLights[3].direction", pointLightPositions[3]);
    shader->setVec3("dirLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("dirLights[3].diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLights[3].specular", 0.5f, 0.5f, 0.5f);

    shader->setVec3("pointLights[0].position", pointLightPositions[0]);
    shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[0].constant", 1.0f);
    shader->setFloat("pointLights[0].linear", 0.09);
    shader->setFloat("pointLights[0].quadratic", 0.032);

    shader->setVec3("pointLights[1].position", pointLightPositions[1]);
    shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[1].constant", 1.0f);
    shader->setFloat("pointLights[1].linear", 0.09);
    shader->setFloat("pointLights[1].quadratic", 0.032);

    shader->setVec3("pointLights[2].position", pointLightPositions[2]);
    shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[2].constant", 1.0f);
    shader->setFloat("pointLights[2].linear", 0.09);
    shader->setFloat("pointLights[2].quadratic", 0.032);

    shader->setVec3("pointLights[3].position", pointLightPositions[3]);
    shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[3].constant", 1.0f);
    shader->setFloat("pointLights[3].linear", 0.09);
    shader->setFloat("pointLights[3].quadratic", 0.032);

    shader->setVec3("spotLights[0].position", pointLightPositions[0]);
    shader->setVec3("spotLights[0].direction", glm::vec3(0.2, 0.8, 0.2));
    shader->setVec3("spotLights[0].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[0].constant", 1.0f);
    shader->setFloat("spotLights[0].linear", 0.09);
    shader->setFloat("spotLights[0].quadratic", 0.032);
    shader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));

    // spotLight
    shader->setVec3("spotLights[1].position", pointLightPositions[1]);
    shader->setVec3("spotLights[1].direction", camera.Front);
    shader->setVec3("spotLights[1].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[1].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[1].constant", 1.0f);
    shader->setFloat("spotLights[1].linear", 0.09);
    shader->setFloat("spotLights[1].quadratic", 0.032);
    shader->setFloat("spotLights[1].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setVec3("spotLights[2].position", pointLightPositions[2]);
    shader->setVec3("spotLights[2].direction", camera.Front);
    shader->setVec3("spotLights[2].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[2].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[2].constant", 1.0f);
    shader->setFloat("spotLights[2].linear", 0.09);
    shader->setFloat("spotLights[2].quadratic", 0.032);
    shader->setFloat("spotLights[2].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[2].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setVec3("spotLights[3].position", pointLightPositions[3]);
    shader->setVec3("spotLights[3].direction", camera.Front);
    shader->setVec3("spotLights[3].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[3].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[3].constant", 1.0f);
    shader->setFloat("spotLights[3].linear", 0.09);
    shader->setFloat("spotLights[3].quadratic", 0.032);
    shader->setFloat("spotLights[3].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[3].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setInt("lightType", (int)lightType);
    shader->setInt("maxRenderLights", 1);
}


void enemyColls() {
    // shyGuy
    //for (int i = 0; i < sizeSG; i++)
    //{
    //    if (sgList[i].isDefeated or !sgList[i].bDraw)
    //        continue;
    //    actualSG = &sgList[i];
    //    // yoshiEgg
    //    detectColls(&Egg.model.collbox, actualSG->name, &actualSG->cb, renderCollBox, collidedEgg_callback);
    //    // Yoshi
    //    detectColls(&actualSG->cb, actualSG->name, &Yoshi.cb, Yoshi.name, renderCollBox, collidedEnemy_callback);
    //    if (!enemyCollidedPlayer) {
    //        actualSG->enemCollPlayer = false;
    //        if (!actualSG->itArrived)
    //            actualSG->isMoving = true;  // Si no colisiono con Yoshi y no ha llegado al destino, entonces seguira su camino
    //    }
    //    else
    //        actualSG->enemCollPlayer = true;

    //}

    //// billBala
    //for (int i = 0; i < sizeBB; i++)
    //{
    //    if (bbList[i].isDefeated or !bbList[i].bDraw)
    //        continue;
    //    actualBB = &bbList[i];
    //    // yoshiEgg
    //    detectColls(&Egg.model.collbox, actualBB->name, &actualBB->cb, renderCollBox, collidedEgg_callback);
    //    // Yoshi
    //    detectColls(&actualBB->cb, actualBB->name, &Yoshi.cb, Yoshi.name, renderCollBox, collidedEnemy_callback);
    //    if (!enemyCollidedPlayer) {
    //        actualBB->enemCollPlayer = false;
    //        if (!actualBB->itArrived)
    //            actualBB->isMoving = true;  // Si no colisiono con Yoshi y no ha llegado al destino, entonces seguira su camino
    //    }
    //    else
    //        actualBB->enemCollPlayer = true;

    //}

    //// Blooper
    //for (int i = 0; i < sizeBL; i++)
    //{
    //    if (blList[i].isDefeated or !blList[i].bDraw)
    //        continue;
    //    actualBL = &blList[i];
    //    // yoshiEgg
    //    detectColls(&Egg.model.collbox, actualBL->name, &actualBL->cb, renderCollBox, collidedEgg_callback);
    //    // Yoshi
    //    detectColls(&actualBL->cb, actualBL->name, &Yoshi.cb, Yoshi.name, renderCollBox, collidedEnemy_callback);
    //    if (!enemyCollidedPlayer) {
    //        actualBL->enemCollPlayer = false;
    //        if (!actualBL->itArrived)
    //            actualBL->isMoving = true;  // Si no colisiono con Yoshi y no ha llegado al destino, entonces seguira su camino
    //    }
    //    else
    //        actualBL->enemCollPlayer = true;
    //}

}

void buffColls() {

    //// Star
    //for (int i = 0; i < sizeStar; i++)
    //{
    //    if (starList[i].isPicked or !starList[i].bDraw)
    //        continue;
    //    actualStar = &starList[i];
    //    detectColls(&actualStar->cb, actualStar->name, &Yoshi.cb, Yoshi.name, renderCollBox, collidedBuff_callback);

    //}

    //// Coin
    //for (int i = 0; i < sizeCoin; i++)
    //{
    //    if (coinList[i].isPicked or !coinList[i].bDraw)
    //        continue;
    //    actualCoin = &coinList[i];
    //    detectColls(&actualCoin->cb, actualCoin->name, &Yoshi.cb, Yoshi.name, renderCollBox, collidedBuff_callback);

    //}

    //// Heart
    //for (int i = 0; i < sizeHeart; i++)
    //{
    //    if (heartList[i].isPicked or !heartList[i].bDraw)
    //        continue;
    //    actualHeart = &heartList[i];
    //    detectColls(&actualHeart->cb, actualHeart->name, &Yoshi.cb, Yoshi.name, renderCollBox, collidedBuff_callback);

    //}
}

void collisions()
{
    //TODO LO DE LAS COLISIONES VA AQUÍ

    // Colisiones con la camara (Para que no tope)
    //detectColls(collboxes, &camera, renderCollBox, collidedObject_callback);

    // Colisiones con Yoshi y los modelos (Arquitectura)
  /*  for (int i = 0; i < models.size(); i++) 
        detectColls(&models[i].collbox, models[i].name, &Yoshi.cb, Yoshi.name, renderCollBox, collidedModel_callback); 
    */

    //Colisiones de los enemigos
    //enemyColls();

    // Coliisiones con los buffs
    //buffColls();

}


void hpCheck() {

    //int enemID;
    //int totalHp;
    //float Porcentaje;

    //// Yoshi
    //totalHp = Yoshi.getHP();
    //Porcentaje = (totalHp * 100) / initHpYoshi;
    //if (Porcentaje > 75)
    //    indiHpYoshi = 100; // 100%
    //if (Porcentaje > 50 && Porcentaje <= 75)
    //    indiHpYoshi = 75; // 75%
    //if (Porcentaje > 25 && Porcentaje <= 50)
    //    indiHpYoshi = 50; // 50%
    //if (Porcentaje > 0 && Porcentaje <= 25)
    //    indiHpYoshi = 25; // 25%
    //if (Porcentaje <= 0) {
    //    indiHpYoshi = 0;
    //    gameOver = true;
    //}


    //// Egg
    //totalHp = Egg.getHp();
    //Porcentaje = (totalHp * 100) / initHpEgg;
    //if (Porcentaje > 75)
    //    indiHpEgg = 100; // 100%
    //if (Porcentaje > 50 && Porcentaje <= 75)
    //    indiHpEgg = 75; // 75%
    //if (Porcentaje > 25 && Porcentaje <= 50)
    //    indiHpEgg = 50; // 50%
    //if (Porcentaje > 0 && Porcentaje <= 25)
    //    indiHpEgg = 25; // 25%
    //if (Porcentaje <= 0) {
    //    indiHpEgg = 0;
    //    gameOver = true;
    //}


    //// Enemies
    //bool allEnemiesDefeated = true;
    //// shyGuy
    //for (int i = 0; i < sizeSG; i++)
    //{
    //    if (sgList[i].isDefeated or !sgList[i].bDraw)
    //        continue;
    //    allEnemiesDefeated = false;
    //    actualSG = &sgList[i];
    //    totalHp = actualSG->getHP();
    //    if (totalHp <= 0) {
    //        actualSG->isDefeated = true;
    //        actualSG->bDraw = false;
    //        maxEnemy--;
    //    }

    //}

    //// billBala
    //for (int i = 0; i < sizeBB; i++)
    //{
    //    if (bbList[i].isDefeated or !bbList[i].bDraw)
    //        continue;
    //    allEnemiesDefeated = false;
    //    actualBB = &bbList[i];
    //    totalHp = actualBB->getHP();
    //    if (totalHp <= 0) {
    //        actualBB->isDefeated = true;
    //        actualBB->bDraw = false;
    //        maxEnemy--;
    //    }

    //}

    //// Blooper
    //for (int i = 0; i < sizeBL; i++)
    //{
    //    if (blList[i].isDefeated or !blList[i].bDraw)
    //        continue;
    //    allEnemiesDefeated = false;
    //    actualBL = &blList[i];
    //    totalHp = actualBL->getHP();
    //    if (totalHp <= 0) {
    //        actualBL->isDefeated = true;
    //        actualBL->bDraw = false;
    //        maxEnemy--;
    //    }
    //}

    //if (allEnemiesDefeated and eMapFinished)
    //    gameFinished = true;
   
}

