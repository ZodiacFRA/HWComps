#include "Particles.hpp"


int Particles::createNewParticles(float deltaTime)
{
	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(deltaTime  * 10000.0);
	if (newparticles > (int)(0.016f * 10000.0))
		newparticles = (int)(0.016f * 10000.0);

	for (int i=0; i < newparticles; i++) {
		findUnusedParticle();
		_particlesContainer[_lastUsedParticle].life = 5.0f; // This particle will live 5 seconds.
		_particlesContainer[_lastUsedParticle].pos = glm::vec3(0, 2.0, 0.2);

		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 2.0, 1.0);
		// Very bad way to generate a random direction;
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		glm::vec3 randomdir = glm::vec3(
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f
		);

		_particlesContainer[_lastUsedParticle].speed = maindir + randomdir * spread;

		// Very bad way to generate a random color
		_particlesContainer[_lastUsedParticle].r = rand() % 256;
		_particlesContainer[_lastUsedParticle].g = rand() % 256;
		_particlesContainer[_lastUsedParticle].b = rand() % 256;
		_particlesContainer[_lastUsedParticle].a = (rand() % 256) / 3;

		_particlesContainer[_lastUsedParticle].size = (rand()%1000)/2000.0f + 0.1f;
	}
    return SUCCESS;
}

int Particles::simulateParticles(float deltaTime, glm::vec3 cameraPosition)
{
	for (int i = 0; i < _maxParticles; i++){
	    Particle& p = _particlesContainer[i]; // shortcut
		std::cout << p.cameradistance << '\n';
		if (p.life > 0.0f){
	        p.life -= deltaTime;  // Decrease life
            // Simulate simple physics : gravity only, no collisions
            p.speed += glm::vec3(0.0f,-0.5f, 0.0f) * (float)deltaTime * 0.5f;
            p.pos += p.speed * (float)deltaTime;
			// std::cout << glm::to_string(p.pos) << '\n';
            p.cameradistance = glm::length2( p.pos - cameraPosition);
            //_particlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) *
											//(float)deltaTime;
            // Fill the GPU buffer
            _g_particule_position_size_data[4 * i + 0] = p.pos.x;
            _g_particule_position_size_data[4 * i + 1] = p.pos.y;
            _g_particule_position_size_data[4 * i + 2] = p.pos.z;

            _g_particule_position_size_data[4 * i + 3] = p.size;

            _g_particule_color_data[4 * i + 0] = p.r;
            _g_particule_color_data[4 * i + 1] = p.g;
            _g_particule_color_data[4 * i + 2] = p.b;
            _g_particule_color_data[4 * i + 3] = p.a;
        } else {
			p.cameradistance = -1.0f;
            // Particles that just died will be put at the end of the buffer
			// in SortParticles();
        }
	}
    return SUCCESS;
}
