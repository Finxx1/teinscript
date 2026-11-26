#ifndef TEIN_COMPONENT_H_
#define TEIN_COMPONENT_H_

namespace glaiel {

struct Component;

}

#include "Entity.h"

namespace glaiel {

struct Component {
	int _padding_[2];
	glaiel::Entity* entity;
	bool deleted;
	bool enabled;
	bool started;
	double deltaTime;
};

}

#endif