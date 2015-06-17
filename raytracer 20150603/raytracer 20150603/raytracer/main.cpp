#include <string>
#include "CImg.h"
#include "kf/kf_colour.h"
#include "kf/kf_vector3.h"
#include "scene.h"
#include "renderable.h"
#include "kf/kf_ray.h"
#include "light.h"
#include "windows.h"
#include "omp.h"
#include "Logger.h"

// This controls if the rendering displays progressively. When timing the results, turn this off (otherwise updating the window repeatedly will affect the timing)
// progressiveCount is how many lines are rendered before updating the window.
bool progressiveDisplay = true;
int  progressiveCount   = 10;
unsigned int nProcessors = omp_get_max_threads();

using namespace cimg_library;

int main()
{
	srand(0);

	Silcoish::Utils::Logger logger("raytracer.log");
	logger << "Processors: " << nProcessors;

	//set threads to 4 * amount of processors
	omp_set_num_threads(nProcessors * 4);

	// The resolution of the window and the output of the ray tracer.
	int windowWidth = 1024;
	int windowHeight = 1024;

	// The floating point image target that the scene is rendered into.
	CImg<float> image(windowWidth, windowHeight, 1, 3, 0);
	// The display object used to show the image.
	CImgDisplay main_disp(image, "Raytrace");
	main_disp.set_normalization(0);	// Normalisation 0 disables auto normalisation of the image (scales to make the darkest to brightest colour fit 0 to 1 range.

	// The scene object.
	Scene scene;

	// Configure the camera.
	// The default camera is spherical perspective projection with a 0.5 radian fov.
	scene.m_camera.m_position.set(0, 0, -10);
	scene.m_camera.lookAt(kf::Vector3(0, 0, 0));

	Sphere *s;

	//#pragma omp parallel for
	for (int i = 0; i < 1080; i+=20)
	{
		s = new Sphere(0.8f);
		s->m_position.set(cos(i / 57.29) * 2, i / 170.0f-4, sin(i / 57.29) * 2);
		s->m_material.m_diffuse.set(0.3f, 0.3f, 0.3f);
		s->m_material.m_reflective.set(0.7f, 0.7f, 0.7f);
		scene.m_renderables.push_back(s);
	}

	//#pragma omp parallel for
	for (int y = 0; y < 30; y += 1)
	{
		//#pragma omp parallel for
		for (int x = 0; x < 30; x += 1)
		{
			s = new Sphere(0.4f);
			s->m_position.set((x - 15)*0.8, -3, (y - 15)*0.8);// rand() / 8192.0f - 2, rand() / 8192.0f - 2, rand() / 8192.0f - 2);
			s->m_material.m_diffuse.set(0.3f, 0.8f, 0.3f);
			s->m_material.m_reflective.set(0.05f, 0.05f, 0.05f);
			s->m_material.m_specularPower = 4;
			s->m_material.m_specular.set(0.2, 0.2, 0.2);
			scene.m_renderables.push_back(s);
		}
	}

	Light *light;

	// Add a white light above and a little to the right.
	light = new Light();
	light->m_position = kf::Vector3(4, 15, 0);
	light->m_colour = kf::Colour(1, 1, 1);
	scene.m_lights.push_back(light);

	// Add a red light mostly to the left and a little up and back.
	light = new Light();
	light->m_position = kf::Vector3(-10, 3, -3);
	light->m_colour = kf::Colour(0.7f, 0.01f, 0.01f);
	scene.m_lights.push_back(light);

	// Record the starting time.
	DWORD startTime = timeGetTime();

	// Primary loop through all screen pixels.
	#pragma omp parallel for
	for (int y = 0; y < windowHeight; y++)
	{
		#pragma omp parallel for
		for (int x = 0; x < windowWidth; x++)
		{
			// Retrieve the colour of the specified pixel. The math below converts pixel coordinates (0 to width) into camera coordinates (-1 to 1).
			kf::Colour output = scene.trace(float(x - windowWidth / 2) / (windowWidth / 2), -float(y - windowHeight / 2) / (windowHeight / 2)*((float(windowHeight) / windowWidth)));
			
			// Clamp the output colour to 0-1 range before conversion.
			output = output.saturate();
			// Convert from linear space to sRGB.
			output.toSRGB();
			// Write the colour to the image (scaling up by 255).
			*image.data(x, y, 0, 0) = output.r*255;
			*image.data(x, y, 0, 1) = output.g*255;
			*image.data(x, y, 0, 2) = output.b*255;
		}

		// Perform progressive display if enabled.
		if (progressiveDisplay)
		{
			if (y % progressiveCount == 0)
				main_disp.display(image);
		}

		// Check for Escape key.
		//if (main_disp.is_keyESC())
			//return 0;
	}

	// Record ending time.
	DWORD endTime = timeGetTime();

	// Display elapsed time in the window title bar.
	main_disp.set_title("Render time: %dms", endTime - startTime);
	std::string timeTaken = std::to_string(endTime - startTime);
	logger << "Time taken: " << timeTaken.c_str();
	main_disp.display(image);

	// Save the output to a bmp file.
	image.save_bmp("output.bmp");
	// Keep refreshing the window until it is closed or escape is hit.
	while (!main_disp.is_closed())
	{
		if (main_disp.is_keyESC())
			return 0;
		main_disp.wait();
	}

	return 0;

}
