#include "CollisionSystem.h"
//#include "CollisionShape.cpp"
#include <iostream>

bool CollisionSystem::checkCollision(CollisionShape& a, CollisionShape& b) {
		std::cout << a.type << ' ' << b.type << '\n';
		if(a.type == b.type) {
				if(glm::distance(a.getSphereCenter(), b.getSphereCenter()) < (a.getSphereRadius() + b.getSphereRadius())) {
						return true;
				}
		}
		return false;
}

glm::vec3 resolveCollision(CollisionShape& a, CollisionShape& b) {
		return glm::vec3(0.0f, 0.0f, 0.0f);
}
