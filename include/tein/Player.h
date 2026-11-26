#ifndef TEIN_PLAYER_H_
#define TEIN_PLAYER_H_

#include <stdint.h>

#include <vector>

namespace glaiel {

struct Player;

}

#include "EntityManager.h"
#include "Component.h"
#include "Math.h"

namespace glaiel {

struct Player {
	int _padding_;
	glaiel::EntityManager* manager;
	bool deleted;
	bool enabled;
	std::vector<glaiel::Component> components;
	void* transform;
	void* renderer;
	void* input;
	void* animator;
	glaiel::CollisionBoundingBox spikebox;
	void* air_gibs;
	void* drowning_gibs;
	std::vector<void*> trail_air;
	std::vector<void*> trail_ground;
	std::vector<void*> trail_ducking;
	std::vector<Vec2D> pastpos;
	void* collider;
	void* grounded_map;
	glaiel::Vec2D speed;
	glaiel::Vec2D prev_pos;
	bool dead;
	bool underwater;
	int debugKeys;
	double deadtimer;
	bool shouldPlayPickupAnimation;
	int pickupFrame;
	double switchdirtimer;
	double stuntimer;
	uint8_t phys[0x158];
	uint8_t main_phys[0x158];
	uint8_t water_phys[0x158];
	uint8_t lowgrav_phys[0x158];
	bool canCancelJump;
	bool canFastFall;
	bool canFastFallJerkDown;
	bool isFastFalling;
	bool canFloat;
	bool canDoubleFloat;
	double floatTimer;
	bool floating;
	bool HasMoved;
	bool canControlRunningAccel;
	bool already_died;
	int jumpcouter;
	bool running;
	bool grounded;
	double groudned_timer;
	double cairfriction;
	double cmaxspeed;
	double cgravity;
	double cmaxfallspeed;
	double phasetimer;
	double declining_timer;
	bool phase_possible;
	bool ledgehugging;
	double ledgedir;
	double ledgetimer;
	double current_flip;
	double jump_buffer_timer;
	double swim__buffer_timer;
	double jumped_dir;
	double toxic_timer;
	double MoveX;
	double dir_explode;
	double bouncetimer;
	bool bounced;
	bool highbounced;
	bool mushbounced;
	bool canSwim;
	bool controls_enabled;
	bool visible;
	bool p_isFastFall;
	bool p_isFastFallThreshold;
	double warp_enable_timer;
	int water_type;
	double toxic_flash_timer;
	uint8_t prev_collision[0x18];
};

}

#endif