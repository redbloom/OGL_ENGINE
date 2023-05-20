
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

    //firstTime = false;

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


        //:::: RENDER DE MODELOS:::://
        drawModels(&ourShader, view, projection);

        //:::: SKYBOX Y TERRENO:::://
        loadEnviroment(&terrain, &sky, view, projection);

        //:::: AGUA :::://
        drawWater(view, projection);

        //:::: PARTICLES :::://
        drawParticles(view, projection);

        //:::: BILLBOARDS :::://
        //drawBillboard();

        // ::::::::::::::: GUI ::::::::::::::::::: //
        drawGUI();

        //:::: COLISIONES :::://
        collisions();

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

    // GUI
    dialogBox.Release();
    digitBar.Release();

    glfwTerminate();

    return 0;
}

void initScene(Shader ourShader)
{

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
    scaleCollBox.x += 0.3;
    scaleCollBox.z += 0.3;
    scaleCollBox.y += 2.1;
    camera.collbox.setScale(scaleCollBox);
    camera.Position.y = 2.1f;
    camPos = camera.Position;
    camera.collbox.setPosition(camPos);
    camera.collbox2.setPosition(camPos);
    camera.collbox.defineCollisionBox();
    camera.collbox2.defineCollisionBox();


    //:::: CARGAMOS LOS SHADERS :::://
    ourShader.use();

    //:::: INICIALIZAMOS NUESTROS MODELOS :::://   
    models.push_back(Model("mcDonalds", "models/mcDonalds/mcDonalds2.obj", glm::vec3(22, -0.78, 2.7), glm::vec3(0, -90, 0), 0.0f, initScale));

    //Muebles 
    models.push_back(Model("Counter", "models/Counter/Counter.obj", glm::vec3(23, 0.2, 1.5), glm::vec3(0, -90, 0), 0.0f, 0.13f));

    models.push_back(Model("Seat01", "models/Counter/Seat.obj", glm::vec3(20.7, 0.2, -0.2), glm::vec3(0, -90, 0), 0.0f, 0.125f));
    models.push_back(Model("Seat02", "models/Counter/Seat.obj", glm::vec3(20.7, 0.2, 0.8), glm::vec3(0, -90, 0), 0.0f, 0.125f));
    models.push_back(Model("Seat03", "models/Counter/Seat.obj", glm::vec3(20.7, 0.2, 1.8), glm::vec3(0, -90, 0), 0.0f, 0.125f));
    //models.push_back(Model("Seat04", "models/Counter/Seat.obj", glm::vec3(20.7, 0.2, 2.8), glm::vec3(0, -90, 0), 0.0f, 0.125f));
    //models.push_back(Model("Seat05", "models/Counter/Seat.obj", glm::vec3(21.1, 0.2, 3.8), glm::vec3(0, -90, 0), 0.0f, 0.125f));

    models.push_back(Model("Fridge", "models/Counter/Fridge.obj", glm::vec3(27.8, 0.2, -0.8), glm::vec3(0, -90, 0), 0.0f, 0.16f));
    models.push_back(Model("Fryer", "models/Counter/Fryer.obj", glm::vec3(27.8, 0.2, 0.9), glm::vec3(0, -90, 0), 0.0f, 0.145f));
    models.push_back(Model("Stove", "models/Counter/Stove.obj", glm::vec3(27.8, 0.2, 2.5), glm::vec3(0, -90, 0), 0.0f, 0.145f));

    models.push_back(Model("sodaMachine", "models/sodaMachine/sodaMachine.obj", glm::vec3(27.8, 0.2, 6.8), glm::vec3(0, -90, 0), 0.0f, 0.16f));
    models.push_back(Model("Arcade", "models/Arcade/Arcade.obj", glm::vec3(27.8, 0.2, 9.0), glm::vec3(0, -90, 0), 0.0f, 0.16f));


    models.push_back(Model("Bin01", "models/Bin/Bin.obj", glm::vec3(24.3, 0.2, 9.5), glm::vec3(0, -90, 0), 0.0f, 0.145f));
    models.push_back(Model("Bin02", "models/Bin/Bin.obj", glm::vec3(23.3, 0.2, 9.5), glm::vec3(0, -90, 0), 0.0f, 0.145f));

    //z: 10.2

    models.push_back(Model("Table01", "models/Table/Table.obj", glm::vec3(15.45, 0.2, 1.0), glm::vec3(0, -90, 0), 0.0f, 0.16f));
    models.push_back(Model("Table02", "models/Table/Table.obj", glm::vec3(15.46, 0.2, 8.6), glm::vec3(0, 0, 0), 0.0f, 0.16f));
    models.push_back(Model("Table03", "models/Table/Table.obj", glm::vec3(19.75, 0.2, 8.6), glm::vec3(0, 0, 0), 0.0f, 0.16f));

    //// Objetos en mesa Y: 0.8
    models.push_back(Model("botesTable", "models/Table/Botes.obj", glm::vec3(16.15, 1.4, 1.25), glm::vec3(0, -75, 0), 0.0f, 0.16f));
    models.push_back(Model("burgerTable", "models/Table/Burger.obj", glm::vec3(15.4, 1.35, 7.8), glm::vec3(0, -45, 0), 0.0f, 0.16f));
    models.push_back(Model("Notebook", "models/Notebook/Notebook.obj", glm::vec3(19.8, 1.3, 7.8), glm::vec3(0, -45, 0), 0.0f, 0.12f));

    // Objetos en el mostrador
    models.push_back(Model("burgerThing", "models/Counter/burgerThing.obj", glm::vec3(22.65, 1.5, 3.1), glm::vec3(0, -90, 0), 0.0f, 0.13f));
    models.push_back(Model("botesCounter", "models/Table/Botes.obj", glm::vec3(22.6, 1.5, 1.65), glm::vec3(0, -75, 0), 0.0f, 0.16f));
    models.push_back(Model("burgerCounter", "models/Table/Burger.obj", glm::vec3(22.45, 1.5, -0.65), glm::vec3(0, -90, 0), 0.0f, 0.16f));

    // Fuera
    models.push_back(Model("fireHydr", "models/fireHydr/fireHydr.obj", glm::vec3(5.2, -0.1, 9.47), glm::vec3(0, -90, 0), 0.0f, 0.18f));
    models.push_back(Model("volkswagenGolf", "models/volkswagenGolf/volkswagenGolf.obj", glm::vec3(19.89, -0.15, 18.3), glm::vec3(0, -90, 0), 0.0f, 0.073f));
    models.push_back(Model("Trash", "models/Trash/Trash.obj", glm::vec3(21.78, -0.1, 14.4), glm::vec3(0, 0, 0), 0.0f, 0.35f));

    modifyModels();

    // Puertas
    door.setInitModel();


    //CREAMOS TODAS  LAS CAJAS DE COLISION INDIVIDUALES
    CollisionBox collbox;
    glm::vec4 colorCollbox(0.41f, 0.2f, 0.737f, 0.06f);
    // Lugar                  //Escala
    collbox = CollisionBox(glm::vec3(12.37, -0.2, -1.05), glm::vec3(0.5, 4, 3.325), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(0, pair<string, CollisionBox>("Pared1", collbox)));
    collbox = CollisionBox(glm::vec3(12.37, -0.2, 9.6), glm::vec3(0.5, 4, 3.275), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(1, pair<string, CollisionBox>("Pared2", collbox)));
    collbox = CollisionBox(glm::vec3(22.39, -0.2, 12.1), glm::vec3(8.07, 4, 0.7), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(2, pair<string, CollisionBox>("Pared3", collbox)));
    collbox = CollisionBox(glm::vec3(22.39, -0.2, -3.1), glm::vec3(8.07, 4, 0.71), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(3, pair<string, CollisionBox>("Pared4", collbox)));
    collbox = CollisionBox(glm::vec3(29.79, 0.0, 3.35), glm::vec3(1.095, 4, 5.659), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(4, pair<string, CollisionBox>("Pared5", collbox)));

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
    //rainSprite = QuadTexture("GUI/Rain.png", 85.0f, 73.0f, 0.0, 0);

    dialogBox = QuadTexture("GUI/DialogueBox.png", 540.5f, 240.5f, 0.0, 0);
    digitBar = QuadTexture("GUI/EmptyScoreBar.png", 260.0f, 75.0f, 0.0, 0);
    pauseBtn = QuadTexture("GUI/MediumGreenButton.png", 125.0f, 92.0f, 0.0, 0);
    acceptBtn = QuadTexture("GUI/GreenButton.png", 130.0f, 88.0f, 0.0, 0);


    // ::::::::::::::: TEXT RENDERER ::::::::::::::::::: //
    acceptTxt = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    acceptTxt->Load("fonts/Dreamy.TTF", 40);

    digitRegTxt = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    digitRegTxt->Load("fonts/Dreamy.TTF", 40);

    interStatusTxt = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    interStatusTxt->Load("fonts/Dreamy.TTF", 55);

    objectInfoTxt = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    objectInfoTxt->Load("fonts/Dreamy.TTF", 45);

    gameInfoTxt = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    gameInfoTxt->Load("fonts/Dreamy.TTF", 45);
    
    gameInfoTxt2 = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
    gameInfoTxt2->Load("fonts/Dreamy.TTF", 45);

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

     //Cambio de skybox (amanecer)
    contadorSky = (float)glfwGetTime();
    int totalSec;
    if (contadorSky > 60) {// Sacar el residuo
        totalSec = (int)contadorSky % 60;
    }
    else {
        totalSec = (int)contadorSky;
    }



     if (/*minuteCount % 2 == 0*/  totalSec >= 0 && totalSec <= 30) {
         if (cambioSky > 0.2) {
             mainLight = vec3(cambioSky);
             cambioSky -= 0.001;
         }
         else {
             if (changeSkyBoxTexture == 0)
             {
                 isNight = true;
                 isDay = false;
                 sky->reloadTexture("10");
                 changeSkyBoxTexture++;
             }
         }
     }
     else {
         if (cambioSky < 0.5) {
             mainLight = vec3(cambioSky);
             cambioSky += 0.001;
         }
         else {
             if (changeSkyBoxTexture == 1)
             {
                 isDay = true;
                 isNight = false;
                 sky->reloadTexture("9");
                 changeSkyBoxTexture--;
             }
         }
     }
    

}
void drawModels(Shader *shader, glm::mat4 view, glm::mat4 projection)
{

    //DEFINIMOS EL BRILLO  DEL MATERIAL
    shader->setFloat("material.shininess", 60.0f);
    setMultipleLight(shader, pointLightPositions);  

    // Puertas
    door.Slide(*shader);

    // Arquitectura
    for (int i = 0; i < models.size(); i++)
    {
        //SI SE RECOGIO EL OBJETO
        shader->use();

        models[i].Draw(*shader);
        
        (&models[i].collbox, models[i].name, &camera, renderCollBox, collidedObject_callback);
    }

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

    if (!gameStarted && startMessage) {
        // Texto de arriba    
        text = "Minijuego";
        interStatusTxt->RenderText(text, -0.665f, 0.32f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
        //Descripcion
        text = "Objetivo: encontrar los 4 digitos";
        gameInfoTxt->RenderText(text, -0.67f, 0.52f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
        text = " escondidos en el lugar";
        gameInfoTxt2->RenderText(text, -0.67f, 0.62f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
        //Cerrar
        text = "Presione K para jugar o L para cancelar";
        acceptTxt->RenderText(text, -0.47f, 0.87f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
        dialogBox.Draw(glm::vec2(0.0f, -0.6f), glm::vec3(1.7f, 0.8f, 0.5));
        return;
    }


    if (collidedObject && interactingWithObject) {
        
        if (digitFound) {

            if (!models[lastObjectInt].digitTaken) {
                // Texto de arriba    
                text = "Digito encontrado";
                interStatusTxt->RenderText(text, -0.665f, 0.32f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
                //Descripcion
                string digitFoundTxt = "Encontraste el digito " + to_string(models[lastObjectInt].digitCode);
                text = digitFoundTxt.c_str();
                objectInfoTxt->RenderText(text, -0.67f, 0.52f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
                //Cerrar
                text = "Presione ENTER para cerrar";
                acceptTxt->RenderText(text, -0.07f, 0.87f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
                dialogBox.Draw(glm::vec2(0.0f, -0.6f), glm::vec3(1.7f, 0.8f, 0.5));
            }
            else {
                // Texto de arriba    
                text = "Digito ya tomado";
                interStatusTxt->RenderText(text, -0.665f, 0.32f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
                //Descripcion
                text = "Siga interactuando con mas objetos";
                objectInfoTxt->RenderText(text, -0.69f, 0.52f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
                //Cerrar
                text = "Presione ENTER para cerrar";
                acceptTxt->RenderText(text, -0.07f, 0.87f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
                dialogBox.Draw(glm::vec2(0.0f, -0.6f), glm::vec3(1.7f, 0.8f, 0.5));
            }
        }
        else {
            // Texto de arriba    
            text = "No hay nada";
            interStatusTxt->RenderText(text, -0.665f, 0.32f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
            //Descripcion
            text = "Siga interactuando con mas objetos";
            objectInfoTxt->RenderText(text, -0.69f, 0.52f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
            //Cerrar
            text = "Presione ENTER para cerrar";
            acceptTxt->RenderText(text, -0.07f, 0.87f, 0.0017, glm::vec3(0.1f, 0.0f, 0.0f));
            dialogBox.Draw(glm::vec2(0.0f, -0.6f), glm::vec3(1.7f, 0.8f, 0.5));
        }
    }
   

    // Digit register
    string digitStr = digitsToText();
    text = digitStr.c_str();
    digitRegTxt->RenderText(text, -0.67f, -0.868f, 0.0027f, glm::vec3(0.08f, 0.0f, 0.0f));
    digitBar.Draw(glm::vec2(-0.6f, 0.83f), glm::vec3(0.6f, 0.23f, 0.5));

   
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


void collisions()
{
    //TODO LO DE LAS COLISIONES VA AQUÍ

    // Colisiones con la camara / Collboxes (Para que no tope)
    detectColls(collboxes, &camera, renderCollBox, collidedObject_callback);

    // Colisiones con la camara y el Vector Models
    detectColls(models, &camera, renderCollBox, collidedModel_callback);

    detectColls(door.cb, &camera, door.collDetected, collidedDoor_callback);

    if (door.collDetected)
        collidedWithDoor = true;
    else
        collidedWithDoor = false;
   
}


void modifyModels() {
    // Guardando los nombres de cada modelo
    string modelNames[30];
    for (int i = 0; i < models.size(); i++)
        modelNames[i] = models[i].name;

    // Se los paso para generar la contraseña
    generatePass(modelNames, models.size());
    
    // Asignar los digitos escogidos
    int codeCounter = 0;
    for (int i = 0; i < models.size(); i++)
    {
        if (code[codeCounter].assignedPlace == i) {
            cout << models[i].name << endl;
            models[i].setDigit(code[codeCounter].digit);
            codeCounter++;
        }
    }

    // Modificar ciertas collboxes
    for (int i = 0; i < models.size(); i++)
    {
        if (models[i].name == "Counter") {
            scaleCollBox = models[i].collbox.getScale();
            scaleCollBox.x -= 0.48;
            scaleCollBox.z += 0.8;
            models[i].collbox.setScale(scaleCollBox);
            models[i].collbox.defineCollisionBox();
        }

        if (models[i].name == "Bin01" || models[i].name == "Bin02") {
            scaleCollBox = models[i].collbox.getScale();
            scaleCollBox.x -= 0.48;
            scaleCollBox.z -= 0.1;
            models[i].collbox.setScale(scaleCollBox);
            models[i].collbox.defineCollisionBox();
        }

        if (models[i].name == "Table01" || models[i].name == "Table02"
           || models[i].name == "Table03") {
            scaleCollBox = models[i].collbox.getScale();
            scaleCollBox.x += 0.01;
            scaleCollBox.z += 0.01;
            models[i].collbox.setScale(scaleCollBox);
            models[i].collbox.defineCollisionBox();
        }

        if (models[i].name == "botesTable"
            || models[i].name == "burgerTable" || models[i].name == "Notebook") {
            scaleCollBox = models[i].collbox.getScale();
            scaleCollBox.x += 0.2;
            scaleCollBox.z += 0.2;
            models[i].collbox.setScale(scaleCollBox);
            models[i].collbox.defineCollisionBox();
        }

        if (models[i].name == "botesCounter" || models[i].name == "burgerCounter") {
            scaleCollBox = models[i].collbox.getScale();
            scaleCollBox.x += 0.5;
            scaleCollBox.z -= 0.02;
            models[i].collbox.setScale(scaleCollBox);
            models[i].collbox.defineCollisionBox();
        }

        if (models[i].name == "burgerThing") {
            scaleCollBox = models[i].collbox.getScale();
            scaleCollBox.z += 0.2;
            scaleCollBox.x -= 0.25;
            models[i].collbox.setScale(scaleCollBox);
            models[i].collbox.defineCollisionBox();
        }

    }
}