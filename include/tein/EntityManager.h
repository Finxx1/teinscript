#ifndef TEIN_ENTITY_MANAGER_H_
#define TEIN_ENTITY_MANAGER_H_

#include <stdint.h>

#include <vector>

namespace glaiel {

struct EntityManager;

}

#include "Entity.h"
#include "Component.h"

namespace glaiel {

struct EntityManager {
	std::vector<Entity> Entities;
	std::vector<int> NullEntities;
	std::vector<Component> Components;
	std::vector<int> NullComponents;
	void* CachedActiveComponentsList;
	double SortDepth;
	std::vector<glaiel::Component> deletion_component_buffer;
	std::vector<glaiel::Entity> deletion_entity_buffer;
	std::vector<glaiel::Component> deletion_component_backbuffer;
	std::vector<glaiel::Entity> deletion_entity_backbuffer;
	bool prevent_object_creation;
	uint8_t taskpool[0x298];
};

}

#endif