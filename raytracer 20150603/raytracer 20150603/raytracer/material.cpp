#include "material.h"

Material::Material() :m_dielectric(false), m_diffuse(1, 1, 1), m_reflective(0.0, 0.0, 0.0), m_specularPower(10), m_specular(1, 1, 1)
{
}
