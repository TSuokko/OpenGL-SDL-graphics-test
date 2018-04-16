#include "ParticleBatch2D.h"
namespace DevyEngine {

	void Particle2D::update(float deltaTime)
	{
		m_position += m_velocity * deltaTime;
	}


	ParticleBatch2D::ParticleBatch2D()
	{
		//empty
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_particles;
	}


	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture)
	{
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[maxParticles];
		m_decayrate = decayRate;
	}
	

	void ParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i < m_maxParticles; i++)
		{
			//check if active
			if (m_particles[i].m_life > 0.0f)
			{
				m_particles[i].update(deltaTime);
				m_particles[i].m_life -= m_decayrate * deltaTime;
			}
		}
	}
	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.f, 0.f, 1.f, 1.f);
		for (int i = 0; i < m_maxParticles; i++)
		{
			//check if active
			auto& p = m_particles[i];
			if (m_particles[i].m_life > 0.0f)
			{
				glm::vec4 destRect(p.m_position.x, p.m_position.y, p.m_width, p.m_width);

				spriteBatch->draw(destRect, uvRect, m_texture.id,0.f, p.m_color);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position,
									  const glm::vec2 velocity,
									  const Color& color,
									  float width){
		int particleIndex = findFreeParticle();
		auto& p = m_particles[particleIndex];

		p.m_life = 1.f;
		p.m_position = position;
		p.m_velocity = velocity;
		p.m_color = color;
		p.m_width = width;
	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (int i = m_lastFreeParticle; i < m_maxParticles; i++)
		{
			if (m_particles[i].m_life <= 0.f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}
		for (int i = 0; i < m_lastFreeParticle; i++)
		{
			if (m_particles[i].m_life <= 0.f)
			{
				m_lastFreeParticle = i;
				return i;
			}
		}
		//no particles are free, overwrite first particle
		return 0;

	}
}