#include "Texture.h"
#include <iostream>
#include <map>
#include "stb_image.h"

static std::map<std::string, Texture> globals;

std::string LowerCase(std::string source) {
	const char* arr = source.c_str();
	std::string result = "";
	for (unsigned int i = 0; i < source.length(); i++) {
		result += tolower(arr[i]);
	}
	return result;
}

GLuint Texture::CreateTexture(const std::string &fileName) {
	int numComp;
	GLuint result;
	int width = 0;
	int height = 0;
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComp, 4);
	dim = glm::vec2(width, height);
	if (imageData == NULL) {
		std::cout << "Texture file doesn't exist: " << fileName << std::endl;
		return 0;
	}
	/* Flip texture vertically to correct it */
	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < width; j++) {
			int invI = height - 1 - i;
			unsigned char buf;

			buf = imageData[4 * (i * width + j)];
			imageData[4 * (i * width + j)] = imageData[4 * (invI * width + j)];
			imageData[4 * (invI * width + j)] = buf;

			buf = imageData[4 * (i * width + j) + 1];
			imageData[4 * (i * width + j) + 1] = imageData[4 * (invI * width + j) + 1];
			imageData[4 * (invI * width + j) + 1] = buf;

			buf = imageData[4 * (i * width + j) + 2];
			imageData[4 * (i * width + j) + 2] = imageData[4 * (invI * width + j) + 2];
			imageData[4 * (invI * width + j) + 2] = buf;

			buf = imageData[4 * (i * width + j) + 3];
			imageData[4 * (i * width + j) + 3] = imageData[4 * (invI * width + j) + 3];
			imageData[4 * (invI * width + j) + 3] = buf;
		}
	}

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//Repeat texture width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//Repeat texture height

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Minify texture
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//Magnify texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//Minify texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//Magnify texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
	return result;
}

Texture& Texture::Load(const std::string& fileName) {
	pathway = LowerCase(fileName);
	if (globals.find(pathway) != globals.end()) {
		(*this) = globals[pathway];
	}
	else {
		m_texture = CreateTexture(pathway);
		globals[pathway] = (*this);
	}
	return *this;
}

void Texture::unloadAll() {
	for (const auto& pair : globals) {
		glDeleteTextures(1, &pair.second.m_texture);
	}
	globals.clear();
}

void Texture::unloadThis() {

	glDeleteTextures(1, &m_texture);

}

void Texture::bind(GLuint id) {
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}