#pragma once

#include <btBulletDynamicsCommon.h>

#include "Larp/LarpPrerequisites.hpp"
#include "Larp/Node.hpp"
#include "PhysicsWorld.hpp"

class PhysicsObject
{
private:
btRigidBody* _rigid_body;
public:
PhysicsObject(glm::quat orientation, glm::vec3 position,
                  GLfloat mass, glm::vec3 local_inertia, GLfloat restitution,
                  Larp::NodePtr user_pointer);
    btRigidBody* get_rigid_body() const;
};

typedef PhysicsObject* PhysicsObjectPtr;