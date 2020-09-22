#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm\gtx\transform.hpp"

class Transform {
public:

	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {}

	glm::mat4 GetMatrix();
	inline glm::mat4 GetRotMatrix() { return rotMatr; }
	inline glm::mat4 GetScaleMatrix() { return scaleMatr; }
	inline glm::vec3 GetPos() { return m_pos; }
	inline glm::vec3 GetRot() { return m_rot; }
	inline glm::vec3 GetScale() { return m_scale; }

	void SetPos(const glm::vec3&);
	void SetRot(const glm::vec3&);
	void rotate(const glm::vec3& r) { SetRot(m_rot + r); }
	void SetScale(const glm::vec3&);

protected:
private:
	bool changed = false;
	glm::vec3 m_pos = glm::vec3(0,0,0);
	glm::vec3 m_rot = glm::vec3(0, 0, 0);
	glm::vec3 m_scale = glm::vec3(1, 1, 1);
	glm::mat4 matrix = glm::mat4(1);
	glm::mat4 posMatr = glm::mat4(1);
	glm::mat4 rotMatr = glm::mat4(1);
	glm::mat4 scaleMatr = glm::mat4(1);
};