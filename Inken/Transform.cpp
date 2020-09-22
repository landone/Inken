#include "Transform.h"

void Transform::SetScale(const glm::vec3& scale) { 
	m_scale = scale;
	scaleMatr = glm::scale(m_scale);
	changed = true;
}

void Transform::SetRot(const glm::vec3& rot) {
	m_rot = rot;
	glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
	glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
	glm::mat4 rotZMatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));
	rotMatr = rotZMatrix * rotYMatrix * rotXMatrix;
	changed = true;
}

void Transform::SetPos(const glm::vec3& pos) {
	m_pos = pos;
	posMatr = glm::translate(m_pos);
	changed = true;
}

glm::mat4 Transform::GetMatrix() {
	if (changed) {
		changed = false;
		matrix = posMatr * rotMatr * scaleMatr;
	}
	return matrix;
}