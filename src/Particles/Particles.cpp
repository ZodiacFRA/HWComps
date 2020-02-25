#include "Particles.hpp"


int Particles::FindUnusedParticle()
{
	for(int i=_lastUsedParticle; i<_maxParticles; i++){
		if (_particlesContainer[i].life < 0){
			_lastUsedParticle = i;
			return i;
		}
	}

	for(int i=0; i<_lastUsedParticle; i++){
		if (_particlesContainer[i].life < 0){
			_lastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}


int Particles::createNewParticle(float deltaTime, glm::vec3 startPos, float yaw, float pitch)
{
	if (!_isActive)
		return SUCCESS;
	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(deltaTime*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);

	for(int i=0; i<newparticles; i++){
		FindUnusedParticle();
		_particlesContainer[_lastUsedParticle].life = 0.5 + ((float)(rand() % 10) / 20.0) - 0.2;
		_particlesContainer[_lastUsedParticle].pos = startPos + glm::vec3(0, 1, 0);

		int flameSpeed = 40;
		float tmpX = flameSpeed * sin(yaw);
		float tmpZ = flameSpeed * cos(yaw);
		float height = (pitch - 2.5) * 20.0;
		glm::vec3 maindir = glm::vec3(tmpX, height, tmpZ);

		float spread = 3.0;
		glm::vec3 randomdir = glm::vec3(
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f
		);

		_particlesContainer[_lastUsedParticle].speed = maindir + randomdir*spread;


		// Very bad way to generate a random color
		_particlesContainer[_lastUsedParticle].r = 255; //rand() % 256;
		_particlesContainer[_lastUsedParticle].g = 200 + rand() % 80;
		_particlesContainer[_lastUsedParticle].b = 150 + rand() % 80;
		_particlesContainer[_lastUsedParticle].a = (rand() % 256);

		_particlesContainer[_lastUsedParticle].size = 0.1f + (rand()%1000)/5000.0f;

	}
	return SUCCESS;
}

int Particles::simulateParticles(float deltaTime, glm::vec3 cameraPosition)
{
	// Simulate all particles
	int ParticlesCount = 0;
	for (int i=0; i < _maxParticles; i++) {
		Particle& p = _particlesContainer[i]; // shortcut
		p.life -= deltaTime;
		// Decrease life
		if (p.life > 0.0f) {
			// Simulate simple physics : gravity only, no collisions
			p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)deltaTime * 2.0f;
			p.pos += (p.speed * (float)deltaTime * (float)(p.life * p.life * 100.0));
			p.cameradistance = glm::length2(p.pos - cameraPosition);

			// Fill the GPU buffer
			_g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
			_g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
			_g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

			_g_particule_position_size_data[4*ParticlesCount+3] = p.size;// + exp(0.5 - p.life) - 1;

			_g_particule_color_data[4*ParticlesCount+0] = p.r;
			_g_particule_color_data[4*ParticlesCount+1] = p.g - ((1.0 - p.life) * 200);
			_g_particule_color_data[4*ParticlesCount+2] = p.b - ((1.0 - p.life) * 150);
			_g_particule_color_data[4*ParticlesCount+3] = p.a;

		}else{
			// Particles that just died will be put at the end of the buffer in SortParticles();
			p.cameradistance = -1.0f;
			_g_particule_position_size_data[4*ParticlesCount+3] = 0;
		}
		ParticlesCount++;
	}
    return SUCCESS;
}
