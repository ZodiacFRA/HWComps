#include "Particles.hpp"


int Particles::startParticleSystem()
{
	for (int i = 0; i < _maxParticles; i++) {
		if ((rand() % 2) == 1) {
			createNewParticle(i, (rand() % 20) / 10.0);
		} else {
			createNewParticle(i, -1.0);
		}
	}
	return SUCCESS;
}


int Particles::createNewParticle(int pos, float life)
{
	_particlesContainer[pos].life = life;
	_particlesContainer[pos].pos = glm::vec3(0, 1.0, 0);
	_particlesContainer[pos].cameradistance = 1.0;

	glm::vec3 maindir = glm::vec3(0.0, 15.0, 0.0);
	float spread = 5.0;
	glm::vec3 randomdir = glm::vec3(
		(rand()%2000 - 1000.0)/1000.0,
		0,
		(rand()%2000 - 1000.0)/1000.0
	);

	_particlesContainer[pos].speed = maindir + (randomdir * spread);

	// // Very bad way to generate a random color
	_particlesContainer[pos].r = rand() % 256;
	_particlesContainer[pos].g = rand() % 256;
	_particlesContainer[pos].b = rand() % 256;
	_particlesContainer[pos].a = (rand() % 256) * 0.8;

	_particlesContainer[pos].size = (rand()%1)/2.0 + 0.2f;
	return SUCCESS;
}

int Particles::simulateParticles(float deltaTime, glm::vec3 cameraPosition)
{
	for (int i = 0; i < _maxParticles; i++){
	    Particle& p = _particlesContainer[i]; // shortcut
		// std::cout << "life: " << p.life << '\n';
		if (p.life > 0.0f){
			p.life -= deltaTime;  // Decrease life
            p.speed += glm::vec3(0.0f, -0.7, 0.0f);
            p.pos += p.speed * ((float)deltaTime);
            p.cameradistance = glm::length2(p.pos - cameraPosition);

            // Fill the GPU buffer
            _g_particule_position_size_data[4 * i + 0] = p.pos.x;
            _g_particule_position_size_data[4 * i + 1] = p.pos.y;
            _g_particule_position_size_data[4 * i + 2] = p.pos.z;

            _g_particule_position_size_data[4 * i + 3] = p.size;

            _g_particule_color_data[4 * i + 0] = p.r;
            _g_particule_color_data[4 * i + 1] = p.g;
            _g_particule_color_data[4 * i + 2] = p.b;
            _g_particule_color_data[4 * i + 3] = p.a;
        } else if ((rand() % 2) == 1) {
			createNewParticle(i, 1.5);
        }
	}
    return SUCCESS;
}
