#include "common_HWComps.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

struct Particle{
    glm::vec3 pos;
	glm::vec3 speed;
	unsigned char r,g,b,a; // Color
    float size;
	float angle;
	float weight;
    float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class Particles {
public:
	~Particles();
    int init(Shader *shader, Texture *texture);

	int updateBuffers();
	int setupDraw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	int draw();

	int simulateParticles(float deltaTime, glm::vec3 cameraPosition);
    int createNewParticle(float deltaTime, glm::vec3 startPos, float yaw);
    int sortParticles();
	int FindUnusedParticle();
	bool _isActive = false;

private:
	int drawBuffer(GLuint buffer, int attribute, int size, int glType,
					int normalized);

    int _maxParticles = 10000;
    Particle _particlesContainer[10000];

    Shader *_shader;
    Texture *_texture;

    GLfloat *_g_particule_position_size_data;
    GLubyte *_g_particule_color_data;

	GLuint _cameraRight_worldspace_ID;
	GLuint _cameraUp_worldspace_ID;
	GLuint _viewProjMatrixID;

	GLuint _billboard_vertex_buffer;
	GLuint _particles_position_buffer;  // Positions + size
	GLuint _particles_color_buffer;  // Colors

    int _lastUsedParticle = 0;


};
