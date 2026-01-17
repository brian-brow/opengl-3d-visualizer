#include "FileLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

FileLoader::FileLoader() {}

FileLoader::FileLoader(const std::string& filepath) {
		load(filepath);
}

void FileLoader::load(const std::string& filepath) {
		std::ifstream infile(filepath);

		std::string prefix;
		float x, y, z;
		std::vector<glm::vec3> temp_positions;
		std::vector<glm::vec2> temp_texcoords;
		std::vector<glm::vec3> temp_normalvec;

		std::string line;
		int i = 0;
		while(std::getline(infile, line)) {
				std::istringstream iss(line);
				iss >> prefix;
				if(prefix == "v"){
						iss >> x >> y >> z;
						temp_positions.push_back(glm::vec3(x,y,z));
				}
				if(prefix == "vt") {
						iss >> x >> y >> z;
						temp_texcoords.push_back(glm::vec2(x,y));
				}
				if(prefix == "vn") {
						iss >> x >> y >> z;
						temp_normalvec.push_back(glm::vec3(x,y,z));
				}
				if(prefix == "f") {
					std::vector<std::string> vertices;
						std::string vertex;
						while (iss >> vertex) {
								vertices.push_back(vertex);
						}

						for(size_t i = 1; i < vertices.size() - 1; i++) {
								processFaceVertex(vertices[0], temp_positions, temp_texcoords, temp_normalvec);
								processFaceVertex(vertices[i], temp_positions, temp_texcoords, temp_normalvec);
								processFaceVertex(vertices[i+1], temp_positions, temp_texcoords, temp_normalvec);
						}
				}
		}
		infile.close();
}

std::vector<float> FileLoader::getVertices() {
		std::vector<float> result;
		for(size_t i = 0; i < positions.size(); i++) {
				result.push_back(positions[i].x);
				result.push_back(positions[i].y);
				result.push_back(positions[i].z);
				result.push_back(1.0f);
				result.push_back(1.0f);
				result.push_back(1.0f);
				result.push_back(texturecoords[i].x);
				result.push_back(texturecoords[i].y);
				result.push_back(normalvec[i].x);
				result.push_back(normalvec[i].y);
				result.push_back(normalvec[i].z);
		}
		return result;
}

std::vector<unsigned int> FileLoader::getIndices() {
		return indices;
}

void FileLoader::processFaceVertex(const std::string& vertexStr, std::vector<glm::vec3>& temp_positions, std::vector<glm::vec2>& temp_texcoords, std::vector<glm::vec3>& temp_normalvec) {
    size_t slash1 = vertexStr.find('/');
    size_t slash2 = vertexStr.find('/', slash1 + 1);

		int posIndex = stoi(vertexStr.substr(0, slash1)) - 1;
		int texIndex = -1;
		int norIndex = -1;

		if(slash1 != std::string::npos && slash2 != slash1 + 1) {
				std::string texStr = vertexStr.substr(slash1 + 1, slash2 - slash1 - 1);
				if(!texStr.empty()) {
						texIndex = std::stoi(texStr) - 1;
				}
		}

		if(slash2 != std::string::npos) {
				std::string norStr = vertexStr.substr(slash2 + 1);
				if(!norStr.empty()) {
						norIndex = std::stoi(norStr) - 1;
				}
		}

		if (posIndex < 0 || posIndex >= temp_positions.size())
				return;
		if (texIndex >= temp_texcoords.size())
				texIndex = -1;
		if (norIndex < 0 || norIndex >= temp_normalvec.size())
				norIndex = 0;

		glm::vec3 pos = temp_positions[posIndex];
		glm::vec2 tex = (texIndex >= 0) ? temp_texcoords[texIndex] : glm::vec2(0.0f, 0.0f);
		glm::vec3 nor = (norIndex >= 0) ? temp_normalvec[norIndex] : glm::vec3(0.0f, 1.0f, 0.0f);

		for(size_t i = 0; i < positions.size(); i++) {
				if(positions[i] == pos && texturecoords[i] == tex && normalvec[i] == nor) {
						indices.push_back(i);
						return;
				}
		}

		positions.push_back(pos);
		texturecoords.push_back(tex);
		normalvec.push_back(nor);
		indices.push_back(positions.size() - 1);
}
