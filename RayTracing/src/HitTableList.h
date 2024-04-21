#pragma once

#include"HitTable.h"
#include<vector>

struct World
{
	std::vector<Sphere> world;

	void Add(Sphere object) {
		world.push_back(object);
	}

	void Clear() {
		world.clear();
	}

	int Size() {
		return world.size();
	}

};

