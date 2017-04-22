#pragma once
#include <Texture.h>
#include <scene/SceneUtil.h>
#include <Color.h>
#include <geometry/Ray.h>
#include "renderer/vulkan/VulkanImage.h"

class Intersection;


struct PBRMaterial {
	float roughness;
	float metallic;
	float specular;
	float r, g, b;	// Color components as single floats because we use push constants
	std::string name;
	PBRMaterial() {};
	PBRMaterial(std::string n, glm::vec3 c, float r, float m) : name(n), roughness(r), metallic(m), r(c.r), g(c.g), b(c.b) {
		specular = 0.8f;
	};
};

typedef struct MaterialTyp
{
	glm::vec4 diffuse;
	glm::vec4 ambient;
	glm::vec4 emission;
	glm::vec4 specular;
	float shininess;
	float transparency;
	glm::ivec2 _pad;
} MaterialPacked;


class Material {
public:
	Material()
		: m_castShadow(false), m_receiveShadow(false), m_translucent(false), m_shininess(0), m_refracti(0), m_reflectivity(0), m_albedoMap(nullptr) {
	};
	Material(MaterialPacked packed, Texture* texture = nullptr) :
		m_castShadow(false), // Optimize out shadows calculation
		m_receiveShadow(false),
		m_translucent(false),
		m_colorDiffuse(packed.diffuse),
		m_colorAmbient(packed.ambient),
		m_colorEmission(packed.emission),
		m_colorSpecular(packed.specular),
		m_colorReflective{glm::vec3()},
		m_colorTransparent{glm::vec3()},
		m_shininess(packed.shininess),
		m_refracti(packed.transparency),
		m_reflectivity(0.0),
		m_albedoMap(texture)
	{};
	virtual ~Material() {}

	virtual ColorRGB EvaluateEnergy(
		const Intersection& isx,
		const Direction& lightDirection,
		const Ray& in,
		Ray& out,
		bool& shouldTerminate
	) = 0;

	bool m_castShadow;
	bool m_receiveShadow;
	bool m_translucent;

	ColorRGB	m_colorDiffuse;
	ColorRGB	m_colorAmbient;
	ColorRGB	m_colorEmission;
	ColorRGB	m_colorSpecular;
	ColorRGB   m_colorReflective;
	ColorRGB   m_colorTransparent;
	float		m_shininess;
	float		m_refracti;
	float	    m_reflectivity;

	Texture*	m_albedoMap;
	VulkanImage::Image m_vkAlbedoMap;


};