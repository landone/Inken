#include "Entity.h"
#include <vector>

using namespace std;

static vector<Entity*> entities;

Entity::Entity() {

	id = (int)entities.size();
	entities.push_back(this);

}

Entity* Entity::getEntity(int index) {

	if (index < 0 || index >= getEntityCount()) {
		return nullptr;
	}

	return entities.at(index);

}

int Entity::getEntityCount() {

	return (int)entities.size();

}