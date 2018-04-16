#pragma once

#include <vector>
namespace DevyEngine
{
	class ParticleBatch2D;
	class SpriteBatch;
	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();
		//after adding a particle Batch, the ParticleEngine2D becomes responsible for deallocation
		void addParticleBatch(ParticleBatch2D* m_particleBatch);
		void update(float deltaTime);
		void draw(SpriteBatch* spriteBatch);
	private:
		std::vector<ParticleBatch2D*>m_batches;
	};
}
