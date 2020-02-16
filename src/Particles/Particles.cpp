#include "Particles.hpp"


int Particles::startParticleSystem()
{
	for (int i = 0; i < _maxParticles; i++)
		createNewParticle(i);
	std::cout << "INIT PARTICLE SYSTEM START" << '\n';
	for (int i = 0; i < _maxParticles; i++) {
		printf("p%d -> life: %f\n\tpos: ", i, _particlesContainer[i].life);
		std::cout << glm::to_string(_particlesContainer[i].pos) << '\n';
	}
	std::cout << "INIT PARTICLE SYSTEM DONE" << '\n';
	return SUCCESS;
}


int Particles::createNewParticle(int pos)
{
	std::cout << "creating new particle " << pos << '\n';
	_particlesContainer[pos].life = 1.0;
	_particlesContainer[pos].pos = glm::vec3(0, 1.0, 0);
	//
	// float spread = 1.5f;
	// glm::vec3 maindir = glm::vec3(0.0, 0.0, 0.0);
	// // Very bad way to generate a random direction;
	// // See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
	// // combined with some user-controlled parameters (main direction, spread, etc)
	// glm::vec3 randomdir = glm::vec3(
	// 	(rand()%2000 - 1000.0)/1000.0,
	// 	(rand()%2000 - 1000.0)/1000.0,
	// 	(rand()%2000 - 1000.0)/1000.0
	// );
	//
	// _particlesContainer[pos].speed = maindir;// + randomdir * spread;
	//
	// // Very bad way to generate a random color
	// _particlesContainer[pos].r = 0;//rand() % 256;
	// _particlesContainer[pos].g = 0;//rand() % 256;
	// _particlesContainer[pos].b = 255;//rand() % 256;
	// _particlesContainer[pos].a = 255;//(rand() % 256) / 3;
	//
	// _particlesContainer[pos].size = (rand()%1000)/2000.0f + 0.1f;


	// TMP buffer fill, to be removed!
	_particlesContainer[pos].cameradistance = 1.0;
	_g_particule_position_size_data[4 * 0 + 0] = 0;
	_g_particule_position_size_data[4 * 0 + 1] = 1;
	_g_particule_position_size_data[4 * 0 + 2] = 0;
	_g_particule_position_size_data[4 * 0 + 3] = 1;

	_g_particule_color_data[4 * 0 + 0] = 0;
	_g_particule_color_data[4 * 0 + 1] = 255;
	_g_particule_color_data[4 * 0 + 2] = 0;
	_g_particule_color_data[4 * 0 + 3] = 255;
	return SUCCESS;
}

int Particles::simulateParticles(float deltaTime, glm::vec3 cameraPosition)
{
	printf("--------------------------Dt = %f \n", deltaTime);
	for (int i = 0; i < _maxParticles; i++){
	    Particle& p = _particlesContainer[i]; // shortcut
		// std::cout << p.cameradistance << '\n';
		p.life -= deltaTime;  // Decrease life
		printf("p%d -> life: %f\n\tpos: ", i, _particlesContainer[i].life);
		std::cout << glm::to_string(_particlesContainer[i].pos) << '\n';
		if (p.life > 0.0f){
            // Simulate simple physics : gravity only, no collisions
            // p.speed += glm::vec3(0.0f,-9.8f, 0.0f) * (float)deltaTime * 0.5f;

            p.pos += p.speed * (float)deltaTime;

            p.cameradistance = glm::length2(p.pos - cameraPosition);
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
			createNewParticle(i);
        }
	}
    return SUCCESS;
}
