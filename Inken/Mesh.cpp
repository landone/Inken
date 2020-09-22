#include "Mesh.h"
#include <fstream>
#include <map>
#include <iostream>

static const std::string DEFAULT_TEXTURE = "./textures/missing.png";
static const std::vector<Vertex> g_centered{
	Vertex{ glm::vec3(-1,-1,0),glm::vec3(0, 0, 1),glm::vec2(0,0) },
	Vertex{ glm::vec3(1,-1,0),glm::vec3(0, 0, 1),glm::vec2(1,0) },
	Vertex{ glm::vec3(1,1,0),glm::vec3(0, 0, 1),glm::vec2(1,1) },
	Vertex{ glm::vec3(-1,1,0),glm::vec3(0, 0, 1),glm::vec2(0,1) }
};
static const std::vector<Vertex> g_notCentered{
	Vertex{ glm::vec3(0,0,0),glm::vec3(0, 0, 1),glm::vec2(0,0) },
	Vertex{ glm::vec3(1,0,0),glm::vec3(0, 0, 1),glm::vec2(1,0) },
	Vertex{ glm::vec3(1,1,0),glm::vec3(0, 0, 1),glm::vec2(1,1) },
	Vertex{ glm::vec3(0,1,0),glm::vec3(0, 0, 1),glm::vec2(0,1) }
};
static const std::vector<GLuint> g_indices{
	0, 1, 2,
	2, 3, 0
};

Mesh& Mesh::Init(std::vector<Vertex> vertices, std::vector<GLuint> indices) {

	this->vertices = vertices;
	this->indices = indices;
	setupMesh();
	return *this;

}

void Mesh::copy(Mesh& src) {

	vertices = src.vertices;
	indices = src.indices;
	setupMesh();
	objects = src.objects;
	textures = src.textures;
	textureScale = src.textureScale;
	textureOffs = src.textureOffs;

}

void Mesh::createQuad(bool centered) {

	Init(centered ? g_centered : g_notCentered, g_indices);

}

void Mesh::setQuadTextureCoord(glm::vec2 bottomLeft, glm::vec2 topRight) {

	if (vertices.size() != 4) {//Must be quad
		return;
	}

	vertices[0].texCoord = glm::vec2(bottomLeft.x, topRight.y);
	vertices[1].texCoord = topRight;
	vertices[2].texCoord = glm::vec2(topRight.x, bottomLeft.y);
	vertices[3].texCoord = bottomLeft;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
	glBindVertexArray(0);

}

void Mesh::setTextureScale(glm::vec2 scale) {
	textureScale.x = scale.x / textureScale.x;
	textureScale.y = scale.y / textureScale.y;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].texCoord.x -= textureOffs.x;
		vertices[i].texCoord.x *= textureScale.x;
		vertices[i].texCoord.x += textureOffs.x;

		vertices[i].texCoord.y -= textureOffs.y;
		vertices[i].texCoord.y *= textureScale.y;
		vertices[i].texCoord.y += textureOffs.y;
	}
	textureScale = scale;

	updateVertices();
}

void Mesh::setTextureOffset(glm::vec2 offs) {
	textureOffs.x = offs.x - textureOffs.x;
	textureOffs.y = offs.y - textureOffs.y;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].texCoord.x += textureOffs.x;
		vertices[i].texCoord.y += textureOffs.y;
	}
	textureOffs = offs;

	updateVertices();
}

void Mesh::updateVertices() {

	if (VAO == NULL) {
		/* Mesh not created yet */
		return;
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
	glBindVertexArray(0);

}

void Mesh::setupMesh() {
	
	if (VAO == NULL) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texID));

	glBindVertexArray(0);
}

void Mesh::draw() {

	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i].bind(i);
	}
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

}

Mesh::~Mesh() {
	deleteBuffers();
}

void Mesh::deleteBuffers() {
	if (VAO != NULL) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	VAO = NULL;
	VBO = NULL;
	EBO = NULL;
}

bool Mesh::Load(std::string path, bool backfaceCull) {

	std::ifstream file(path);
	if (!file.is_open()) {
		return false;
	}
	std::string line;
	std::string mtlPath = path.substr(0, path.find_last_of('.')) + ".mtl";

	std::vector<glm::vec3> pos;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> norm;

	std::vector<Vertex> verts;
	std::vector<GLuint> indices;
	GLuint vertCounter = 0;
	/* Map vertex def. to index */
	std::map<std::string, GLuint> vertMap;

	Object curObj;
	/* Map material name to texture index */
	std::map<std::string, GLuint> materials;

	/* Process OBJ data */
	while (std::getline(file, line)) {

		if (line.size() <= 2) {
			continue;
		}
		if (line[0] == 'v') {

			switch (line[1]) {
			case 't':
				glm::vec2 muv;
				if (sscanf_s(line.c_str(), "vt %f %f", &muv[0], &muv[1]) == 2) {
					uv.push_back(muv);
				}
				else {
					return false;
				}
				break;
			case 'n':
				glm::vec3 mnorm;
				if (sscanf_s(line.c_str(), "vn %f %f %f", &mnorm[0], &mnorm[1], &mnorm[2]) == 3) {
					norm.push_back(mnorm);
				}
				else {
					return false;
				}
				break;
			default:
				glm::vec3 mpos;
				if (sscanf_s(line.c_str(), "v %f %f %f", &mpos[0], &mpos[1], &mpos[2]) == 3) {
					pos.push_back(mpos);
				}
				else {
					return false;
				}
				/* Push object position index lower boundary */
				if (curObj.indices.size() == 0) {
					curObj.indices.push_back((GLuint)pos.size());
				}
			}
		}
		else if (line[0] == 'f') {

			size_t start = 1;
			GLuint pushedInd[3];
			for (int i = 0; i < 4; i++) {

				/* Get string representing vertex */
				start = line.find_first_not_of(' ', start);
				size_t end = line.find_first_of(' ', start);
				if (i < 2 && end == std::string::npos) {
					return false;
				}
				if (start == std::string::npos) {
					/* If TRIANGLE face */
					if (i == 3) {
						/* Draw reverse to flip face orientation */
						if (!backfaceCull) {
							for (int j = 2; j >= 0; j--) {
								indices.push_back(pushedInd[j]);
							}
						}
						break;
					}
					else {
						/* Otherwise must be corrupted */
						return false;
					}
				}
				std::string v = line.substr(start, end - start);
				start = end;

				GLuint index;
				/* Place vertex in map if not found */
				if (vertMap.find(v) == vertMap.end()) {
					index = vertCounter;
					vertMap[v] = vertCounter++;
				}
				else {
					index = vertMap[v];
				}
				/* If QUADRANGLE face*/
				if (i == 3) {
					indices.push_back(pushedInd[2]);
					indices.push_back(index);
					indices.push_back(pushedInd[0]);
					/* Draw reverse to flip face orientation */
					if (!backfaceCull) {
						for (int j = 2; j >= 0; j--) {
							indices.push_back(pushedInd[j]);
						}
						indices.push_back(pushedInd[0]);
						indices.push_back(index);
						indices.push_back(pushedInd[2]);
					}
				}
				else {
					pushedInd[i] = index;
					indices.push_back(index);
				}

			}
		}
		else if (line[0] == 'o') {
			/* If object initialized, save with upper boundary */
			if (curObj.indices.size() > 0) {
				curObj.indices.push_back((GLuint)pos.size());
				objects.push_back(curObj);
			}

			/* Setup blank object */
			curObj.indices.clear();
			curObj.material = DEFAULT_TEXTURE;
			curObj.texIndex = 0;

			size_t nameStart = line.find_first_not_of(' ', 1);
			if (nameStart == std::string::npos) {
				curObj.name = "";
			}
			else {
				curObj.name = line.substr(nameStart, line.size() - nameStart);
			}

		}
		else if (line.substr(0, 6) == "usemtl") {

			std::string name = line.substr(7, line.size() - 7);
			materials[name] = 0;
			curObj.material = name;

		}

	}

	std::vector<std::pair<std::string, std::string>> mtlData = ProcessMTL(mtlPath);
	for (unsigned int i = 0; i < mtlData.size(); i++) {
		if (materials.find(mtlData[i].first) != materials.end()) {
			/* If material found, load the image file and cache */
			materials[mtlData[i].first] = (GLuint)textures.size();
			textures.push_back(Texture(mtlData[i].second));
		}
	}

	/* Account for last object */
	if (curObj.indices.size() > 0) {
		curObj.indices.push_back((GLuint)pos.size());
		objects.push_back(curObj);
	}
	/* Middleman obj. vertex recorder */
	std::vector<std::vector<GLuint>> objectVerts;
	objectVerts.resize(objects.size());
	/* Convert map to array */
	verts.resize(vertCounter);
	std::map<std::string, GLuint>::iterator mapIt;
	for (mapIt = vertMap.begin(); mapIt != vertMap.end(); mapIt++) {
		unsigned int ind[3];
		if (sscanf_s(mapIt->first.c_str(), "%d/%d/%d", &ind[0], &ind[1], &ind[2]) != 3) {
			return false;
		}
		/* Convert object indices from position to vertex */
		for (unsigned int i = 0; i < objects.size(); i++) {
			if (ind[0] >= objects[i].indices[0] && ind[0] <= objects[i].indices[1]) {
				objectVerts[i].push_back(mapIt->second);
				break;
			}
		}
		/* Translate indices to 0-based */
		ind[0]--;
		ind[1]--;
		ind[2]--;
		/* Avoid crash due to bad input */
		if ((ind[0] < 0 || ind[0] >= pos.size()) || (ind[1] < 0 || ind[1] >= uv.size()) || (ind[2] < 0 || ind[2] >= norm.size())) {
			return false;
		}
		/* Place vertex into final vector */
		Vertex v;
		v.position = pos[ind[0]];
		v.texCoord = uv[ind[1]];
		v.normal = norm[ind[2]];
		v.texID = 0;
		verts[mapIt->second] = v;
	}

	/* Overwrite object vertex arrays and set textures */
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects[i].indices = objectVerts[i];
		std::string& matName = objects[i].material;
		if (materials.find(matName) != materials.end()) {
			objects[i].texIndex = materials[matName];
			/* Set object's vertices' texture IDs */
			for (unsigned int j = 0; j < objects[i].indices.size(); j++) {
				verts[objects[i].indices[j]].texID = (float)objects[i].texIndex;
			}
		}
	
	}
	
	Init(verts, indices);
	return true;

}

std::vector<std::pair<std::string, std::string>> Mesh::ProcessMTL(std::string path) {

	std::vector<std::pair<std::string, std::string>> output;

	std::ifstream file(path);
	if (!file.is_open()) {
		return output;
	}
	std::string line;
	/* Current pair being formed */
	std::pair<std::string, std::string> current;

	while (std::getline(file, line)) {

		if (line.substr(0, 6) == "newmtl") {
			current.first = line.substr(7, line.size() - 7);
		}
		else if (line.substr(0, 6) == "map_Kd") {
			current.second = line.substr(7, line.size() - 7);
			output.push_back(current);
			current.first = "";
			current.second = "";
		}

	}

	return output;

}