#ifndef MATERIAL_H
#define MATERIAL_H

#include "kf/kf_quaternion.h"
#include "kf/kf_vector.h"
#include "kf/kf_colour.h"
#include "kf/kf_ray.h"


class Material
{
public:
	Material();
	kf::Colour m_diffuse;
	kf::Colour m_specular;
	float m_specularPower;
	kf::Colour m_reflective;
	bool m_dielectric;
};



#endif
