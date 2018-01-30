#include "SpriteBatch.h"
#include <algorithm>
namespace DevyEngine {

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType/* = GlyphSortType::TEXTURE*/)
	{
		_sortType = sortType;
		_renderBatches.clear();
		// Makes _glpyhs.size() == 0, however it does not free internal memory.
		// So when we later call emplace_back it doesn't need to internally call new.
		for (int i = 0; i < _glyphs.size(); i++)
		{
			delete _glyphs[i];
		}
		_glyphs.clear();
	}
	void SpriteBatch::end()
	{
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
	{
		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w );
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
	}

	void SpriteBatch::renderBatch()
	{
		// Bind our VAO. This sets up the opengl state we need, including the 
		// vertex attribute pointers and it binds the VBO
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
		
	}

	void SpriteBatch::createRenderBatches()
	{
		// This will store all the vertices that we need to upload
		std::vector<Vertex> vertices;
		vertices.resize(_glyphs.size() * 6);
		// Resize the buffer to the exact size we need so we can treat
		// it like an array

		if (_glyphs.empty())
		{
			return;
		}

		int currentVertex = 0;
		int offset = 0;


		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
		vertices[currentVertex++] = _glyphs[0]->topLeft;
		vertices[currentVertex++] = _glyphs[0]->bottomLeft;
		vertices[currentVertex++] = _glyphs[0]->bottomRight;
		vertices[currentVertex++] = _glyphs[0]->bottomRight;
		vertices[currentVertex++] = _glyphs[0]->topRight;
		vertices[currentVertex++] = _glyphs[0]->topLeft;
		offset += 6;
		
		//Add all the rest of the glyphs

		for (int cg = 1; cg < _glyphs.size(); cg++)
		{
			// Check if this glyph can be part of the current batch
			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture)
			{
				// Make a new batch
				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			}
			else
			{
				// If its part of the current batch, just increase numVertices
				_renderBatches.back().numVertices += 6;
			}
			vertices[currentVertex++] = _glyphs[cg]->topLeft;
			vertices[currentVertex++] = _glyphs[cg]->bottomLeft;
			vertices[currentVertex++] = _glyphs[cg]->bottomRight;
			vertices[currentVertex++] = _glyphs[cg]->bottomRight;
			vertices[currentVertex++] = _glyphs[cg]->topRight;
			vertices[currentVertex++] = _glyphs[cg]->topLeft;
			offset += 6;
		}

		// Bind our VBO
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// Orphan the buffer (for speed)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		// Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void SpriteBatch::createVertexArray()
	{
		// Generate the VAO if it isn't already generated
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		// Bind the VAO. All subsequent opengl calls will modify it's state.
		glBindVertexArray(_vao);
		//Generate the VBO if it isn't already generated
		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//Tell opengl what attribute arrays we need
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);

	}

	

	void SpriteBatch::sortGlyphs()
	{
		switch (_sortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;
		}
		
	}

	bool  SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}
	bool  SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}
	bool  SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);
	}

}