#pragma once

#include "Transform.h"
#include "Texture.h"
#include "Mesh.h"

class Entity {
public:

	static Entity* getEntity(int index);
	static int getEntityCount();

	int getIndex() { return id; }

	virtual glm::vec3 getPos() { return trans.GetPos(); }
	glm::vec3 getRot() { return trans.GetRot(); }
	glm::vec3 getScale() { return trans.GetScale(); }
	Transform& getTransform() { return trans; }

	void setPos(double x, double y, double z) { setPos(glm::vec3(x, y, z)); }
	virtual void setPos(glm::vec3 pos) { trans.SetPos(pos); }

	void move(double x, double y, double z) { move(glm::vec3(x, y, z)); }
	virtual void move(glm::vec3 amt) { setPos(trans.GetPos() + amt); }

	void setRot(double x, double y, double z) { setRot(glm::vec3(x, y, z)); }
	virtual void setRot(glm::vec3 rot) { trans.SetRot(rot); }

	void rotate(double x, double y, double z) { rotate(glm::vec3(x, y, z)); }
	virtual void rotate(glm::vec3 amt) { setRot(trans.GetRot() + amt); }

	void setScale(double factor) {
		setScale(glm::vec3((float)factor));
	}
	void setScale(double x, double y, double z) {
		setScale(glm::vec3(x, y, z));
	}
	void setScale(glm::vec3 factor) {
		trans.SetScale(factor);
	}
	void scale(double amount) {
		scale(glm::vec3((float)amount));
	}
	void scale(double x, double y, double z) {
		scale(glm::vec3(x, y, z));
	}
	void scale(glm::vec3 amount) {
		trans.SetScale(trans.GetScale() * amount);
	}

protected:

	Entity();

private:

	int id;
	Transform trans;

};