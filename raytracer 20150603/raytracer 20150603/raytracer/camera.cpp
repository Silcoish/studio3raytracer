#include "Camera.h"

Camera::Camera() :m_forward(0, 0, 1), m_right(1, 0, 0), m_up(0, 1, 0), m_orthographic(false), m_orthoSize(5.0f), m_fov(0.5)
{

}

kf::Ray Camera::viewToRay(float x, float y) const
{
	kf::Ray ray;
	if (m_orthographic)
	{
		ray.start(m_position + m_right*x*m_orthoSize + m_up*y*m_orthoSize);
		ray.delta(m_forward);
	}
	else
	{
		float phi = -y*m_fov + 3.14159265 / 2.0;
		float theta = -x*m_fov +3.14159265 / 2.0;
		ray.start(m_position);
		float ax = sin(phi)*cos(theta);
		float ay = cos(phi);
		float az = sin(phi)*sin(theta);
		ray.delta((ax*m_right + ay*m_up + az*m_forward).normalise());
	}
	return ray;
}

void Camera::lookAt(const kf::Vector3 &target)
{
	m_forward = (target - m_position).normalise();
	m_right = kf::Vector3(0, 1, 0).cross(m_forward).normalise();
	m_up = m_forward.cross(m_right).normalise();
}
