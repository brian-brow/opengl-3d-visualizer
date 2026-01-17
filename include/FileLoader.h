#ifndef FILELOADER_H
#define FILELOADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

class FileLoader {
private:
		std::string filepath;
		std::vector<glm::vec3> positions;
		std::vector<unsigned int> indices;
		std::vector<glm::vec2> texturecoords;
		std::vector<glm::vec3> normalvec;

public:
		FileLoader();
		FileLoader(const std::string& filepath);
		void load(const std::string& filepath);
		std::vector<float> getVertices();
		std::vector<unsigned int> getIndices();
		void processFaceVertex(const std::string& vertexStr, std::vector<glm::vec3>& temp_positions, std::vector<glm::vec2>& temp_texcoords, std::vector<glm::vec3>& temp_normalvec);
};

#endif
