#ifndef
#define

#include <glm/glm.hpp>
#include "CollisionShape.h"
#include "Mesh.h"

class PhysicsObject {
private:
		CollisionShape collisionShape;
		Mesh* mesh;

		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float mass;
		float restitution;
		float friction;

		bool isStatic;

public:
		PhysicsObject(CollisionShape shape, Mesh* mesh, glm::vec3 pos, float mass = 1.0f) : collisionShape(shape), mesh(mesh), position(pos), velocity(vec3(0.0f,0.0f,0.0f)), acceleration(-10.0f), mass(0.0f), restitution(0.0f), friction(0.0f), isStatic(false)

		void update(float deltaTime);
		void applyForce(glm::vec3 force);
		void applyImpulse(glm::vec3 impulse);

		glm::vec3 getPosition() const { return position; }
		void setPosition(glm::vec3 pos) { position = pos }

		glm::vec3 getVelocity() const { return velocity; }
		void setVelocity(glm::vec3 vel) { velocity = vel }

		CollisionShape& getCollisionShape() { return collisionShape; }
		Mesh* getMesh() { return mesh; }

		float getMass() const { return mass; }
		float getRestitution() const { return restitution; }

		void setStatic(bool static_) { isStatic = static_; }
		bool getStatic() const { return isStatic; }
};

#endif
