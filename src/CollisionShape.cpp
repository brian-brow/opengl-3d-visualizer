#include "CollisionShape.h"

CollisionShape CollisionShape::CreateSphere(glm::vec3 center, float radius) {
    CollisionShape shape;
    shape.type = SPHERE;
    shape.center = center;
    shape.radius = radius;
    return shape;
}

CollisionShape CollisionShape::CreateAABB(glm::vec3 min, glm::vec3 max) {
    CollisionShape shape;
    shape.type = AABB;
    shape.min = min;
    shape.max = max;
    return shape;
}

CollisionShape CollisionShape::CreatePlane(glm::vec3 normal, float distance) {
    CollisionShape shape;
    shape.type = PLANE;
    shape.planeNormal = glm::normalize(normal);
    shape.planeDistance = distance;
    return shape;
}

// Sphere methods
glm::vec3 CollisionShape::getSphereCenter() {
    return center;
}

float CollisionShape::getSphereRadius() {
    return radius;
}

void CollisionShape::setSphereCenter(glm::vec3 newCenter) {
    center = newCenter;
}

// AABB methods
glm::vec3 CollisionShape::getAABBMin() {
    return min;
}

glm::vec3 CollisionShape::getAABBMax() {
    return max;
}

void CollisionShape::setAABBMin(glm::vec3 newMin) {
    min = newMin;
}

void CollisionShape::setAABBMax(glm::vec3 newMax) {
    max = newMax;
}

// Plane methods
glm::vec3 CollisionShape::getPlaneNormal() {
    return planeNormal;
}

float CollisionShape::getPlaneDistance() {
    return planeDistance;
}

void CollisionShape::setPlaneNormal(glm::vec3 newNormal) {
    planeNormal = glm::normalize(newNormal);
}

void CollisionShape::setPlaneDistance(float newDistance) {
    planeDistance = newDistance;
}
