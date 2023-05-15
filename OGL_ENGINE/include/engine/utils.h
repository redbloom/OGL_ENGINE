
#include <RigidBox/RigidBox.h>
#include <engine/CollisionBox.h>
#include <engine/camera.h>
#include <engine/model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>



static bool intersect(CollisionBox a, CollisionBox b)
{
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

void detectColls(CollisionBox *coll, string name, Camera *cam, bool renderCollBox, void (*callback)(string))
{
    bool itCollided = false;
    glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)800 / 600, 0.1f, 100.0f);
    glm::mat4 view = cam->GetViewMatrix();
    if (intersect(*coll, cam->collbox)) {
        callback(name);
        itCollided = true;
    }
    if (intersect(*coll, cam->collbox2))
        cam->notFrontMove = true;
    if (renderCollBox)
        coll->draw(view, projection);

}

void detectColls(vector<Model> models, Camera* cam, bool renderCollBox, void (*callback)(string)) {
    bool itCollided = false;
    glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)800 / 600, 0.1f, 100.0f);
    glm::mat4 view = cam->GetViewMatrix();
    for (int i = 0; i < models.size(); i++)
    {
        string name = models[i].name;
        if (intersect(models[i].collbox, cam->collbox)) {
            callback(name);
            // Si llego a colisionar con algo, entonces
            itCollided = true;
        }
    }

    if (!itCollided)
        isCollision = false;
    else
        isCollision = true;
}

void detectColls(CollisionBox cb, Camera* cam, bool& doorBool, void(*callback)) {
    bool itCollided = false;

    if (intersect(cb, cam->collbox)) {
        // Si llego a colisionar con algo, entonces
        callback;
        itCollided = true;
    }

    if (!itCollided) {
        doorBool = false;
    }
    else {
        doorBool = true;
    }
}



void detectColls(CollisionBox* coll, string name, Model* model, bool renderCollBox, void (*callback)(string))
{
    if (intersect(*coll, model->collbox)) {
        isCollision = true;
        callback(name);
    }
}


void detectColls(CollisionBox* coll, string name, CollisionBox* model, bool renderCollBox, void (*callback)(string))
{
    if (intersect(*coll, *model)) {
        if (name == "Yoshi")
            isCollision = true;

        callback(name);
    }
}

void detectColls(CollisionBox* coll, string name, CollisionBox* model, string nameCollided, bool renderCollBox, void (*callback)(string))
{
    bool itCollided = false;
    if (intersect(*coll, *model)) {
        if (nameCollided == "Cam") {
            callback(name);
            // Si llego a colisionar con algo, entonces
            itCollided = true;
        }
    }
    
    if (itCollided)
        isCollision = true;
    else 
        isCollision = false;

}

void detectColls(map<int, pair<string, CollisionBox>> collboxes, Camera *cam, bool renderCollBox, void (*callback)(string))
{
    bool itCollided = false; 
    glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)800 / 600, 0.1f, 100.0f);
    glm::mat4 view = cam->GetViewMatrix();
    for (pair<int, pair<string, CollisionBox>> colls : collboxes)
    {

        if (intersect(colls.second.second, cam->collbox)) {
            callback(colls.second.first);
            // Si llego a colisionar con algo, entonces
            itCollided = true;
        }
        if (intersect(colls.second.second, cam->collbox2))
            cam->notFrontMove = true;
        if (renderCollBox)
            colls.second.second.draw(view, projection);
    }

    if (!itCollided)
        isCollbox = false;
    else
        isCollbox = true;
}



void detectCam(CollisionBox* coll, string name, Camera* cam, bool renderCollBox, void (*callback)(string)) {
    bool itCollided = false;
    glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)800 / 600, 0.1f, 100.0f);
    glm::mat4 view = cam->GetViewMatrix();

    if (name == "KrustyKrab")
        return;

    if (intersect(*coll, cam->collbox)) {
        callback(name);
        itCollided = true; // Si llego a colisionar con algo, entonces
    }
    if (renderCollBox)
        coll->draw(view, projection);

    if (!itCollided)
        isCollision = false;
    else
        isCollision = true;
}


void detectColls(rbEnvironment *env, void (*callback)(string, string))
{
    for (rbContact c : env->Contacts())
    {
        if (c.Body[0]->IsNotFixed() && c.Body[1]->IsNotFixed())
        {
            string name1 = c.Body[0]->getName();
            string name2 = c.Body[1]->getName();
            callback(name1, name2);
        }
    }
}