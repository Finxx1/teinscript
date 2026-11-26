#ifndef TEIN_ENTITY_H_
#define TEIN_ENTITY_H_

#include <vector>

namespace glaiel {

struct Entity;

}

#include "EntityManager.h"
#include "Component.h"

namespace glaiel {

struct Entity {
	int _padding_;
	glaiel::EntityManager* manager;
	bool deleted;
	bool enabled;
	std::vector<glaiel::Component> components;
};

}

#endif