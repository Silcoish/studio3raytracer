#include "scene.h"

Scene::Scene() :m_ambient(0.0, 0.0, 0.0), m_sky(0.1, 0.3, 0.8), m_shadows(true), m_recurseLimit(4)
{
	m_skytexture.load_bmp("KKZ5V.bmp");
}

Scene::~Scene()
{
	for (int i = 0; i < m_lights.size(); ++i)
	{
		delete m_lights[i];
	}
	for (int i = 0; i < m_renderables.size(); ++i)
	{
		delete m_renderables[i];
	}
}

HitPoint Scene::intersect(const kf::Ray &ray)
{
	HitPoint hp;

	for (int i = 0; i < m_renderables.size(); ++i)
	{
		if (m_renderables[i]->m_active)
		{
			// Find the nearest intersect point.
			hp.nearest(m_renderables[i]->intersect(ray));
			
		}
	}
	return hp;
}

HitPoint Scene::intersectShadow(const kf::Ray &ray)
{
	HitPoint shadowHp;

	for (int i = 0; i < m_renderables.size(); ++i)
	{
		if (m_renderables[i]->m_active)
		{
			// Find the nearest intersect point.
			shadowHp.nearest(m_renderables[i]->intersect(ray));

			if (shadowHp.m_hit)
				return shadowHp;
		}
	}
	return shadowHp;
}

kf::Colour Scene::trace(float x, float y)
{
	kf::Colour output;
	kf::Ray ray = m_camera.viewToRay(x, y);
	output = trace(ray, 0);

	return output;
}

kf::Colour Scene::trace(const kf::Ray &ray, int recurseDepth)
{
	kf::Colour output = m_sky;

	if (recurseDepth > m_recurseLimit)
	{
		output.set(0, 0, 0);
		return output;
	}

	// Get the colour of the skybox that this ray would hit.
	int tx = kf::remap<float>(-3.14159265, 3.14159265, 0, m_skytexture.width() - 1, atan2(ray.delta().x, ray.delta().z));
	int ty = kf::remap<float>(1, -1, 0, m_skytexture.height() - 1, ray.delta().y);
	output.r = *m_skytexture.data(tx, ty, 0, 0) / 255.0f;
	output.g = *m_skytexture.data(tx, ty, 0, 1) / 255.0f;
	output.b = *m_skytexture.data(tx, ty, 0, 2) / 255.0f;
	output.toLinear();

	// Find the intersect with the scene.
	HitPoint hp = intersect(ray);
	// If true, we hit something. Otherwise return the skybox.
	if (hp.m_hit)
	{
		kf::Colour specColour(0, 0, 0);
		kf::Colour diffuseColour(0, 0, 0);

		// Loop through every light in the scene.
		for (int i = 0; i < m_lights.size(); ++i)
		{
			// Direction and distance from the intersect point to the current light.
			kf::Vector3 lightDirection = m_lights[i]->m_position - hp.m_position;
			float lightDistance = lightDirection.length();
			lightDirection = lightDirection.normalise();

			kf::Colour lightColour = m_lights[i]->m_colour;
			if (m_shadows)
			{
				// Cast a ray from the intersect point to each light. If anything is in the way, a shadow is being cast on the intersect point.
				kf::Ray shadowRay;
				shadowRay.start(hp.m_position + lightDirection*0.001); // The lightDirection*0.001 bit is to stop floating point error making the ray collide with it's starting point.
				shadowRay.delta(lightDirection);
				HitPoint shadowhp = intersectShadow(shadowRay);

				// If a shadow is detected, the light's contribution is zero.
				if (shadowhp.m_hit && shadowhp.m_distance < lightDistance)
				{
					lightColour.set(0, 0, 0);
				}
			}

			// Calculate standard lambert diffuse lighting
			float lambert = hp.m_normal.dot(lightDirection);
			if (lambert < 0)
				lambert = 0;

			// Accumulate diffuse lighting.
			diffuseColour = diffuseColour + hp.m_renderable->m_material.m_diffuse * lightColour * lambert;

			// Accumulate specular lighting.
			kf::Vector3 rl = lightDirection.reflect(hp.m_normal);
			float specular = rl.dot(ray.delta());
			if (specular > 0)
			{
				specColour += hp.m_renderable->m_material.m_specular * m_lights[i]->m_colour *pow(specular, hp.m_renderable->m_material.m_specularPower);
			}
		}

		kf::Colour reflectionColour(0,0,0);
		kf::Ray reflectionRay;
		// If the hit object's material has reflection, cast a new ray recursively.
		if (hp.m_renderable->m_material.m_reflective.length_squared() > 0)
		{
			reflectionRay.delta(ray.delta().reflect(hp.m_normal).normalise());
			reflectionRay.start(hp.m_position + reflectionRay.delta()*0.001);
			reflectionColour = trace(reflectionRay, recurseDepth + 1) * hp.m_renderable->m_material.m_reflective;
		}

		// Final output is diffuse + specular + reflection.
		output = m_ambient + diffuseColour + specColour + reflectionColour;
	}

	return output;
}

