#pragma once

#include"HitTable.h"
#include<vector>

struct World
{
	std::vector<Sphere> world;

	std::vector<Material> materials;

	void Add_Object(Sphere object) {
		world.push_back(object);
	}

	void Add_Material(Material material) {
		materials.push_back(material);
	}

	void Add_Material(const glm::vec3& abd, float rough, float metal) {
		Material temp_material(abd, rough, metal);
		materials.push_back(temp_material);
	}

	void Clear() {
		world.clear();
		materials.clear();
	}

	int Size() {
		return world.size();
	}

};

