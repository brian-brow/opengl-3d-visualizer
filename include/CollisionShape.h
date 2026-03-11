#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include <glm/glm.hpp>

class CollisionShape {
private:
		glm::vec3 center;
		float radius;
		glm::vec3 min, max;
		glm::vec3 planeNormal;
		float planeDistance;

		CollisionShape() {}
public:
		enum Type { SPHERE, AABB, PLANE };
		Type type;

		static CollisionShape CreateSphere(glm::vec3 center, float radius);
		static CollisionShape CreateAABB(glm::vec3 min, glm::vec3 max);
		static CollisionShape CreatePlane(glm::vec3 normal, float distance);

		glm::vec3 getSphereCenter();
		float getSphereRadius();
		void setSphereCenter(glm::vec3 newCenter);

		glm::vec3 getAABBMin();
		glm::vec3 getAABBMax();
		void setAABBMin(glm::vec3 newMin);
		void setAABBMax(glm::vec3 newMax);

		glm::vec3 getPlaneNormal();
		float getPlaneDistance();
		void setPlaneNormal(glm::vec3 newNormal);
		void setPlaneDistance(float newDistance);
};

#endif
