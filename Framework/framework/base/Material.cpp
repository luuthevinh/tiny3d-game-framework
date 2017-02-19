#include "Material.h"

USING_NS_TINY;

const Material Material::WHITE_MATERIAL = Material("white", Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));

Material::Material()
{
	ZeroMemory(this, sizeof(Material));
	this->name = "default";
	this->ambient = Vec3(0.2f, 0.2f, 0.2f);
	this->diffuse = Vec3(0.8f, 0.8f, 0.8f);
	this->specular = Vec3(1.0f, 1.0f, 1.0f);
	this->emissive = Vec3(0.0f, 0.0f, 0.0f);
	this->shininess = 0;
	this->alpha = 1.0f;
	this->hasSpecular = false;
	this->texture = NULL;
	this->texturePath = "";
}

Material::Material(std::string name, const Vec3& amb, const Vec3& dif, const Vec3& spe)
{
	ZeroMemory(this, sizeof(Material));
	this->name = name;
	this->ambient = amb;
	this->diffuse = dif;
	this->specular = spe;
	this->emissive = Vec3(0.0f, 0.0f, 0.0f);
	this->shininess = 0;
	this->alpha = 1.0f;
	this->hasSpecular = false;
	this->texture = NULL;
	this->texturePath = "";
}
