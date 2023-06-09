
#include <engine/utils.h>
#include <engine/variables.h>


//:::: CALLBACKS  Y FUNCIONES :::://
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void collidedObject_callback(string nameCollidedObject);
void collidedModel_callback(string nameCollidedObject);
void collidedEgg_callback(string nameCollidedObject);
void collidedEnemy_callback(string nameCollidedObject);
void collidedBuff_callback(string nameCollidedObject);



void joystick_callback(int jid, int event);
void processInput(GLFWwindow *window);
void rotateCamInput(double xoff, double yoff);
void actionKeys(GLFWwindow *window);

void setMultipleLight(Shader *shader, vector<glm::vec3> pointLightPositions);
void setSimpleLight(Shader *shader);
void drawModels(Shader *shader, glm::mat4 view, glm::mat4 projection);
void loadEnviroment(Terrain *terrain, SkyBox *sky, glm::mat4 view, glm::mat4 projection);
void initScene(Shader ourShader);
void collisions();
void drawWater(glm::mat4, glm::mat4);
void drawBillboard();
void drawParticles(glm::mat4, glm::mat4);
void drawGUI();

void collisionSide(CollisionBox, CollisionBox);

// Movimiento de la camara
void foward();
void backward();
void left();
void right();

//JOYSTICK CALLBACK
void joystick_callback(int jid, int event)
{

    if (event == GLFW_CONNECTED)
    {
        isJoyStick = true;
        cout << "Se ha conectado un control" << endl;
    }
    else if (event == GLFW_DISCONNECTED)
    {
        // The joystick was disconnected
        isJoyStick = false;
        cout << "Se ha desconectado el control" << endl;
    }
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    GLFWgamepadstate state;
    // The joystick was connected
    if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
    {
        isJoyStick = true;
        if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
        {

            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] == -1)
            {
                foward();
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] == 1)
            {
                backward();
            }

            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] == -1)
            {
                left();
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] == 1)
            {
                right();
            }

            if (state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS) {
                //Yoshi.isPunching = true;
            }

            if (state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS) {
                gameStarted = true;
            }

            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] == 1)
            {
                xJoy += 50.5;
                rotateCamInput(xJoy, yJoy);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] == -1)
            {
                xJoy -= 50.5;
                rotateCamInput(xJoy, yJoy);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] == 1)
            {
                yJoy += 50.5;
                rotateCamInput(xJoy, yJoy);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] == -1)
            {
                yJoy -= 50.5;
                rotateCamInput(xJoy, yJoy);
            }
        }
    }
    else
    {
        //MENÚS
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        {
            int menu = (int)typemenu;
            if (menu < 4)
                menu += 1;
            else
                menu = 0;
            typemenu = (TypeActionKeyBoard)menu;
            system("cls");
            if (typemenu == GAME)
                cout << "\nMenu GAME seleccionado..." << endl;
            else if (typemenu == OBJECTS)
                cout << "\nMenu OBJECTS seleccionado..." << endl;
            else if (typemenu == COLLISION)
                cout << "\nMenu COLLISION seleccionado..." << endl;
            else if (typemenu == LIGHTING)
                cout << "\nMenu LIGHTING seleccionado..." << endl;

        }

        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            gameStarted = true;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            foward();
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            backward();
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            left();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            right();
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            //Yoshi.isPunching = true;
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            cout << endl;
            //posModel = Yoshi.model.getPosition();
            //cout << "PosModel: \n X:" << posModel.x << "\t Z:" << posModel.z << endl;
            posModel = camera.collbox.getPosition();
            cout << "PosCam: \n X:" << posModel.x << "\t Z:" << posModel.z << endl;
        }

        isJoyStick = false;
        actionKeys(window);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
}
void actionKeys(GLFWwindow *window)
{   
    if (typemenu == OBJECTS || typemenu == COLLISION)
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            if (movement < 0.2)
                movement += 0.01;
            else
                movement = 0.0;
            system("cls");
            cout << "\nEl factor de movimiento es: " << movement << endl;
        }
    }

    if (typemenu == GAME)
    {
       //ACCIONES DE USTEDES
    }
    else if (typemenu == OBJECTS)
    {
        //::::::::::::::MOVER OBJETOS:::::::::::::://

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            if (indexObject < models.size()-1)
            {
                indexObject += 1;
            }
            else
            {
                indexObject = 0;
            }

            system("cls");
            cout << "\nModelo seleccionado: " << models[indexObject].name << endl;
        }

        //TRANSLACIÓN DEL MODELO X
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.x -= movement;
            models[indexObject].setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.x += movement;
            models[indexObject].setPosition(posModel);
        }
        //TRANSLACIÓN DEL MODELO Y
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.y -= movement;
            models[indexObject].setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.y += movement;
            models[indexObject].setPosition(posModel);
        }
        //TRANSLACIÓN DEL MODELO Z
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.z -= movement;
            models[indexObject].setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            posModel = models[indexObject].getPosition();
            posModel.z += movement;
            models[indexObject].setPosition(posModel);
        }
        //ROTACIÓN DEL MODELO X
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.x -= 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.x += 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        //ROTACIÓN DEL MODELO Y
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.y -= 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.y += 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        //ROTACIÓN DEL MODELO Z
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.z -= 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            rotationModel = models[indexObject].getAngles();
            rotationModel.z += 1.0;
            models[indexObject].setAngles(rotationModel);
        }
        //IMPRIMIR EN CONSOLA
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            cout << "\nfloat initScale = 0.2f;";
            for (int i = 0; i < models.size(); i++)
            {
                glm::vec3 mPos = models[i].getPosition();
                glm::vec3 mRot = models[i].getAngles();
                float tx = mPos.x;
                float ty = mPos.y;
                float tz = mPos.z;

                float rx = mRot.x;
                float ry = mRot.y;
                float rz = mRot.z;
                cout << "\nmodels.push_back(Model(\"" << models[i].name << "\",\"" << models[i].path << "\", glm::vec3(" << tx << ", " << ty << "," << tz << "), glm::vec3(" << rx << ", " << ry << "," << rz << "), 0.0f, initScale));" << endl;
            }
        }
    }
    else if (typemenu == LIGHTING)
    {
        //::::::::::::::MOVER LUCES:::::::::::::://
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            const char *lightTypes[] = {"FLAT", "ALL", "DIRECTIONAL", "SPOT", "POINT"};
            int lt = (int)lightType;
            lt += 1;
            if (lt > 4)
                lt = 0;
            lightType = (LightType)lt;
            system("cls");
            cout << "\nEl tipo de luz seleccionada es : " << lightTypes[lt] << endl;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            if (indexLight < pointLightPositions.size() - 1)
                indexLight += 1;
            else
                indexLight = 0;

            system("cls");
            cout << "\nLuz seleccionada: " << lightcubes.at(indexLight).first << endl;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            renderLightingCubes = !renderLightingCubes;
            system("cls");
            if (renderLightingCubes)
                cout << "\nVisibilidad de light cubes activada..." << endl;
            else
                cout << "\nVisibilidad de light cubes desactivada..." << endl;
        }

        //TRANSLACIÓN X
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].x -= 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].x += 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        //TRANSLACIÓN Y
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].y -= 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].y += 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        //TRANSLACIÓN Z
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].z -= 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            pointLightPositions[indexLight].z += 0.5;
            lightcubes.at(indexLight).second.setPosition(pointLightPositions[indexLight]);
        }

        //IMPRIMIR EN CONSOLA
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            for (int i = 0; i < pointLightPositions.size(); i++)
            {
                glm::vec3 mPos = pointLightPositions[i];
                float tx = mPos.x;
                float ty = mPos.y;
                float tz = mPos.z;
                cout << "\nglm::vec3(" << tx << ", " << ty << "," << tz << ");" << endl;
            }
        }
    }
    else if (typemenu == COLLISION)
    {
        int size = isCollBoxModel ? models.size() : collboxes.size();
        glm::vec3 transform(0);
        //::::::::::::::MOVER COLLISION BOX:::::::::::::://
        //SWITCH ENTRE COLLBOXES Y MODELS
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            isCollBoxModel = !isCollBoxModel;
            system("cls");
            if (isCollBoxModel)
            {
                indexCollBox = 0;
                transformObject = MODEL;
                cout << "\nSe ha seleccionado el movimiento de collbox de objetos..." << endl;
            }
            else
            {
                indexCollBox = 0;
                transformObject = COLLBOX;
                cout << "\nSe ha seleccionado el movimiento de collbox individuales..." << endl;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            renderCollBox = !renderCollBox;
            system("cls");
            if (renderCollBox)
                cout << "\nVisibilidad de collbox activada..." << endl;
            else
                cout << "\nVisibilidad de collbox desactivada..." << endl;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {      
            if (indexCollBox < size)
            {
                string name = isCollBoxModel ? models[indexCollBox].name : collboxes.at(indexCollBox).first;
                cb = isCollBoxModel ? &models[indexCollBox].collbox : &collboxes.at(indexCollBox).second;
                system("cls");
                cout << "\nCollision Box seleccionado: " << name << endl;
                indexCollBox += 1;
            }
            else
                indexCollBox = 0;
        }
        //ESCALA DEL COLLBOX X
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.x = (scaleCollBox.x < 0.5) ? 0.5 : scaleCollBox.x - movement;
            cb->setScale(scaleCollBox);
        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.x += movement;
            cb->setScale(scaleCollBox);
        }
        //ESCALA DEL COLLBOX Y
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.y = (scaleCollBox.y < 0.5) ? 0.5 : scaleCollBox.y - movement;
            cb->setScale(scaleCollBox);
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.y += movement;
            cb->setScale(scaleCollBox);
        }
        //ESCALA DEL COLLBOX Z
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.z = (scaleCollBox.z < 0.5) ? 0.5 : scaleCollBox.z - movement;
            cb->setScale(scaleCollBox);
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            scaleCollBox = cb->getScale();
            scaleCollBox.z += movement;
            cb->setScale(scaleCollBox);
        }
        //POSICIÓN DEL COLLBOX X
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.x -= movement;
            cb->setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.x += movement;
            cb->setPosition(posModel);
        }
        //POSICIÓN DEL COLLBOX Y
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.y -= movement;
            cb->setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.y += movement;
            cb->setPosition(posModel);
        }
        //POSICIÓN DEL COLLBOX Z
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.z -= movement;
            cb->setPosition(posModel);
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            posModel = cb->getPosition();
            posModel.z += movement;
            cb->setPosition(posModel);
        }
        cb->defineCollisionBox();
        //IMPRIMIR EN CONSOLA
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            cout << "\nCollisionBox collbox;" << endl;
           
            if (transformObject == COLLBOX)
            {
                int i = 0;
                for (pair<int, pair<string, CollisionBox>> colls : collboxes)
                {
                    string name = colls.second.first;
                    glm::vec3 mPos = colls.second.second.getPosition();
                    glm::vec3 mScale = colls.second.second.getScale();
                    cout << "\ncollbox = CollisionBox(glm::vec3(" << mPos.x << "," << mPos.y << "," << mPos.z << "), glm::vec3(" << mScale.x << "," << mScale.y << "," << mScale.z << "), glm::vec4(0.41f, 0.2f, 0.737f, 0.06f));"
                        << "\ncollboxes.insert(pair<int, pair<string, CollisionBox>>(" << i << ", pair<string, CollisionBox>(\"" << name << "\",collbox)));" << endl;
                    i++;
                }
            }
            else if (transformObject == MODEL)
            {
                for (int i=0; i<models.size(); i++)
                {
                    string name = models[i].name;
                    glm::vec3 mPos = models[i].collbox.getPosition();
                    glm::vec3 mScale = models[i].collbox.getScale();
                    cout << "\nmodels[" << i << "].collbox.setPosition(glm::vec3(" << mPos.x << "," << mPos.y << "," << mPos.z << "));"
                        << "\nmodels[" << i << "].collbox.setScale(glm::vec3(" << mScale.x << "," << mScale.y << "," << mScale.z << "));"
                        << "\nmodels[" << i << "].collbox.defineCollisionBox();" << endl;

                   
                }              
            }
            
           
        }
    }
}



void rotateCamInput(double xoff, double yoff)
{
    if (firstMouse)
    {
        lastX = xJoy;
        lastY = yJoy;
        firstMouse = false;
    }

    float xoffset = (xJoy - lastX) * 0.2;
    float yoffset = (lastY - yJoy) * 0.2; // reversed since y-coordinates go from bottom to top

    lastX = xJoy;
    lastY = yJoy;

    //Rotar
    //rotationModel = Yoshi.model.getAngles();
    //camYaw = (camera.Yaw * -1) + 90;
    //rotationModel.y = camYaw;
    //Yoshi.updateAngles(rotationModel);

    camera.ProcessMouseMovement(xoffset, yoffset);
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
    if (!isJoyStick)
    {

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;                                                          // RIGHT
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top    // LEFT

        //Rotar
        //rotationModel = Yoshi.model.getAngles();
        //camYaw = (camera.Yaw * -1) + 90;
        //rotationModel.y = camYaw;
        //Yoshi.updateAngles(rotationModel);
        
        lastX = xpos;
        lastY = ypos;
        
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    auto pos = camera.Position;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
       //ACCIÓN
        //Yoshi.isPunching = true;
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}





void collidedObject_callback(string nameCollidedObject) // Camara - collboxes
{
    //SI OCURRE UNA COLISIÓN HACEMOS ALGO
   
    // Collboxes
    int i = 0;
    for (pair<int, pair<string, CollisionBox>> colls : collboxes)
    {
        string name = colls.second.first;
        if (name == nameCollidedObject) {

            collisionSide(colls.second.second, camera.collbox);
            
        }

    }
 
}


void collidedBuff_callback(string nameCollidedObject) {

    //if (nameCollidedObject == "Star") {
    //    playerAtk = Yoshi.getAtk();
    //    playerAtk = actualStar->getBuff(playerAtk);
    //    Yoshi.setAtk(playerAtk);
    //    actualStar->isPicked = true;
    //    actualStar->bDraw = false;
    //    maxBuff--;
    //}

    //if (nameCollidedObject == "Heart") {
    //    int playerHp = Yoshi.getHP();
    //    playerHp = actualHeart->getBuff(playerHp);
    //    Yoshi.setHP(playerHp);
    //    actualHeart->isPicked = true;
    //    actualHeart->bDraw = false;
    //    maxBuff--;
    //}

    //if (nameCollidedObject == "Coin") {
    //    int playerScore = Yoshi.getScore();
    //    playerScore = actualCoin->getBuff(playerScore);
    //    Yoshi.setScore(playerScore);
    //    actualCoin->isPicked = true;
    //    actualCoin->bDraw = false;
    //    maxBuff--;
    //}

} 

void collidedEgg_callback(string nameCollidedObject) {

    if (nameCollidedObject == "shyGuy") {
        actualSG->isMoving = false;
        actualSG->itArrived = true;
        if (!actualSG->enemCollPlayer) {
            enemyAtk = actualSG->attackObjective();
            Egg.getDMG(enemyAtk);
        }
    }

    if (nameCollidedObject == "billBalla") {
        actualBB->isMoving = false;
        actualBB->itArrived = true;
        if (!actualBB->enemCollPlayer) {
            enemyAtk = actualBB->attackObjective();
            Egg.getDMG(enemyAtk);
        }

    }

    if (nameCollidedObject == "Blooper") {
         actualBL->isMoving = false;
         actualBL->itArrived = true;
         if (!actualBL->enemCollPlayer) {
             enemyAtk = actualBL->attackObjective();
             Egg.getDMG(enemyAtk);
         }
    }

}


void collidedEnemy_callback(string nameCollidedObject) {


    //if (nameCollidedObject == "shyGuy") {
    //    collisionSide(actualSG->cb, Yoshi.cb);
    //    actualSG->isMoving = false;

    //    // Si Yoshi no esta golpeando, entonces ataca el huevo
    //    // Tendria que poner un timer que lo haga cada 20 segundos...
    //    // Como el de la animacion
    //    if (Yoshi.isPunching) {
    //       playerAtk = Yoshi.attackEnemy();
    //       actualSG->getDMG(playerAtk);
    //    }
    //    else {
    //        enemyAtk = actualSG->attackObjective();
    //        Yoshi.getDMG(enemyAtk);
    //    }
    //}

    //if (nameCollidedObject == "billBalla") {
    //    collisionSide(actualBB->cb, Yoshi.cb);
    //    actualBB->isMoving = false;

    //    if (Yoshi.isPunching) {
    //        playerAtk = Yoshi.attackEnemy();
    //        actualBB->getDMG(playerAtk);
    //    }
    //    else {
    //        enemyAtk = actualBB->attackObjective();
    //        Yoshi.getDMG(enemyAtk);
    //    }

    //}

    //if (nameCollidedObject == "Blooper") {
    //    collisionSide(actualBL->cb, Yoshi.cb);
    //    actualBL->isMoving = false;
    //
    //    if (Yoshi.isPunching) {
    //        playerAtk = Yoshi.attackEnemy();
    //        actualBL->getDMG(playerAtk);
    //    }
    //    else {
    //        enemyAtk = actualBL->attackObjective();
    //        Yoshi.getDMG(enemyAtk);
    //    }
    //
    //}

}



void collidedModel_callback(string nameCollidedObject) {

    // Modelos
    for (int i = 0; i < models.size(); i++)
    {
        string name = models[i].name;
        if (name == nameCollidedObject) {

            if (nameCollidedObject == "krustyKrab") {
                return;
            }

            //Para bloquear el paso con los modelos
            //collisionSide(models[i].collbox, Yoshi.cb);

        }

    }

}


void collisionSide(CollisionBox modelCb, CollisionBox collidedObj) {

    //Para bloquear el paso con los modelos
    // Modelos  
    glm::vec3 minCB = modelCb.min;
    glm::vec3 maxCB = modelCb.max;

    glm::vec3 minCam = collidedObj.min;
    glm::vec3 maxCam = collidedObj.max;


    //Arriba
    if (maxCB.x > minCam.x && maxCB.x < maxCam.x) {
        stop.Back = true;
        stop.Front = false;
    }
    // Abajo
    else if (minCB.x < maxCam.x && minCam.x < minCB.x) {
        stop.Back = false;
        stop.Front = true;
    }

    //Derecha
    if (minCam.z < maxCB.z && maxCB.z < maxCam.z) {
        stop.Left = true;
        stop.Right = false;
    }
    //Izquierda
    else if (minCB.z < maxCam.z && minCam.z < minCB.z) {
        stop.Left = false;
        stop.Right = true;
    }

}

#pragma region camMovements

// Procesa el movimiento de la camara
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, stopCam& stop)
{
    //if (Yoshi.isPunching)
    //    return;

    // Creamos una variable que guarde la posicion de la camara antes de modificarse
    glm::vec3 antPos = Position;
    // Luego igualamos la Posicion a la futura posicion de la camara (los valores se obtuvieron al presionar una tecla)
    Position = camPos;

    float velocity = MovementSpeed * deltaTime;

    // Ejecutamos el movimiento
    if (direction == FORWARD && notFrontMove == false) {
        Position += Front * velocity;
    }
    if (direction == BACKWARD) {
        Position -= Front * velocity;
    }
    if (direction == LEFT) {
        Position -= Right * velocity;
    }
    if (direction == RIGHT) {
        Position += Right * velocity;
    }
    if (Position.y > 1.4f or Position.y < 1.0f) // Para que no suba ni baje
        return;

    // Checamos si no colisiono con algun objeto
    //collisions();
    // Si no, entonces liberar cualquier movimiento
    if (!isCollision) {
        stop.Back = false;
        stop.Front = false;
        stop.Left = false;
        stop.Right = false;
    }

    // En caso de haber colisionado, si el valor del lado con el que chocamos de una collbox 
    // es igual hacia donde nos queremos mover entonces no
    // ejecutamos el cambio. 
    if (stop.Back && direction == BACKWARD) {
        Position = antPos; // Regresamos a la posicion anterior
        return;
    }
    if (stop.Front && direction == FORWARD) {
        Position = antPos;
        return;
    }
    if (stop.Right && direction == RIGHT) {
        Position = antPos;
        return;
    }
    if (stop.Left && direction == LEFT) {
        Position = antPos;
        return;
    }

    // De no ser asi, seteamos la nueva posicion de nuestra camara.
    //Position = camPos;
    //Yoshi.updatePosition(posModel);
    collbox.setPosition(Position);
    collbox2.setPosition(Position);
    collbox.defineCollisionBox();
    collbox2.defineCollisionBox();

}


// MOVIMIENTOS DE LA CAMARA

void foward() {
        //Tomo la posicion del modelo
        //posModel = Yoshi.model.getPosition();

        //// Muevo el modelo
        //posModel = Yoshi.model.getPosition();
        //posModel.x += movement;

        ////Le doy los valores a la camara de mi modelo
        //camPos.x = posModel.x - 1.3;
        //camPos.z = posModel.z;
        
        camPos.x += movement;
        camera.ProcessKeyboard(FORWARD, deltaTime, stop);
        skyPos += camera.Front * glm::vec3(2);

}

void backward() {

    //Tomo la posicion del modelo
    //posModel = Yoshi.model.getPosition();

    // Muevo el modelo
    //posModel = Yoshi.model.getPosition();
    //posModel.x -= movement;

    //Le doy los valores a la camara de mi modelo
    //camPos.x = posModel.x - 1.3;
    //camPos.z = posModel.z;

    camPos.x -= movement;
    camera.ProcessKeyboard(BACKWARD, deltaTime, stop);
    skyPos -= camera.Front * glm::vec3(2);

}

void left() {

        ////Tomo la posicion del modelo
        //posModel = Yoshi.model.getPosition();

        //// Muevo el modelo
        //posModel = Yoshi.model.getPosition();
        //posModel.z -= movement;

        ////Le doy los valores a la camara de mi modelo
        //camPos.x = posModel.x - 1.3;
        //camPos.z = posModel.z;

        camPos.z -= movement;
        camera.ProcessKeyboard(LEFT, deltaTime, stop);
        skyPos -= camera.Right * glm::vec3(2);

    
}

void right() {

        ////Tomo la posicion del modelo
        //posModel = Yoshi.model.getPosition();

        //// Muevo el modelo
        //posModel = Yoshi.model.getPosition();
        //posModel.z += movement;

        ////Le doy los valores a la camara de mi modelo
        //camPos.x = posModel.x - 1.3;
        //camPos.z = posModel.z;

        camPos.z += movement;
        camera.ProcessKeyboard(RIGHT, deltaTime, stop);
        skyPos -= camera.Right * glm::vec3(2);
    
}

#pragma endregion