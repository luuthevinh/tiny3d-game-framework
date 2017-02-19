#pragma once

#include <string>
#include "..\TinyDefinitions.h"

NS_TINY_BEGIN

class Material
{
public:
	Material();
	Material(std::string name, const Vec3& amb, const Vec3& dif, const Vec3& spe);

	static const Material WHITE_MATERIAL;

	std::string name;

	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 emissive;

	int shininess;
	float alpha;

	bool hasSpecular;

	std::string texturePath;
	IDirect3DTexture9* texture;
};

NS_TINY_END