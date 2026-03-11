#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CollisionShape.h"
#include <cmath>
#include <algorithm>

class CollisionSystem {
public:
    bool checkCollision(CollisionShape& a, CollisionShape& b);
    glm::vec3 resolveCollision(CollisionShape& a, CollisionShape& b);

private:
    bool sphereVsSphere(CollisionShape& a, CollisionShape& b);
    bool sphereVsAABB(CollisionShape& sphere, CollisionShape& aabb);
    bool sphereVsPlane(CollisionShape& sphere, CollisionShape& plane);
    bool aabbVsAABB(CollisionShape& a, CollisionShape& b);
    
    glm::vec3 getClosestPointOnAABB(const glm::vec3& point, CollisionShape& aabb);
};

#endif
