#include "Particles.hpp"


int Particles::init(Shader *shader, Texture *texture)
{
    _shader = shader;
    _texture = texture;

    _g_particule_position_size_data = new GLfloat[_maxParticles * 4];
    _g_particule_color_data = new GLubyte[_maxParticles * 4];

    for(int i=0; i<_maxParticles; i++){
        _particlesContainer[i].life = -1.0f;
        _particlesContainer[i].cameradistance = -1.0f;
    }
    static const GLfloat g_vertex_buffer_data[] = {
         -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
         -0.5f,  0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
    };

    glGenBuffers(1, &_billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    glGenBuffers(1, &_particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    glGenBuffers(1, &_particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    return SUCCESS;
}

int Particles::updateBuffers()
{
    // Update the buffers that OpenGL uses for rendering.
    glBindBuffer(GL_ARRAY_BUFFER, _particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, _maxParticles * sizeof(GLfloat) * 4, _g_particule_position_size_data);

    glBindBuffer(GL_ARRAY_BUFFER, _particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, _maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, _maxParticles * sizeof(GLubyte) * 4, _g_particule_color_data);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return SUCCESS;
}

int Particles::setupDraw()
{

    // glUseProgram(_shader->getProgramID());
    // _texture->setupDraw(_shader->getProgramID());

    // 1rst attribute buffer : vertices
    drawBuffer(_billboard_vertex_buffer, 0, 3, GL_FLOAT, GL_FALSE);
    // 2nd attribute buffer : positions of particles' centers
    drawBuffer(_particles_position_buffer, 1, 4, GL_FLOAT, GL_FALSE);
    // 3rd attribute buffer : particles' colors
    drawBuffer(_particles_color_buffer, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE);

    return SUCCESS;
}

int Particles::drawBuffer(GLuint buffer, int attribute, int size, int glType,
                            int normalized)
{
	glEnableVertexAttribArray(attribute);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		attribute,          // attribute
		size,               // size
		glType,           // type
		normalized,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	return SUCCESS;
}

int Particles::draw()
{
    // These functions are specific to glDrawArrays*Instanced*.
    // The first parameter is the attribute buffer we're talking about.
    // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
    // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml

    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1

    // Draw the particules !
    // This draws many times a small triangle_strip (which looks like a quad).
    // This is equivalent to :
    // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
    // but faster.

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _maxParticles);
    //
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    return SUCCESS;
}

int Particles::sortParticles()
{
	std::sort(&_particlesContainer[0], &_particlesContainer[_maxParticles]);
    return SUCCESS;
}

Particles::~Particles()
{
	// Clear VBOs
	glDeleteBuffers(1, &_billboard_vertex_buffer);
	glDeleteBuffers(1, &_particles_position_buffer);
	glDeleteBuffers(1, &_particles_color_buffer);
}
