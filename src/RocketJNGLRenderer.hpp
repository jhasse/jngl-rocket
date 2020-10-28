#pragma once

#include <RmlUi/Core/RenderInterface.h>
#include <memory>

namespace jngl {
class Sprite;
}

class RocketJNGLRenderer : public Rml::RenderInterface {
public:
	void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices,
	                    Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

	void EnableScissorRegion(bool) override {
		// TODO
	}

	void SetScissorRegion(int, int, int, int) override {
		// TODO
	}

	// Called by RmlUi when a texture is required by the library.
	bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions,
	                 const Rml::String& source) override;

	bool GenerateTexture(Rml::TextureHandle&, const Rml::byte* source,
	                     const Rml::Vector2i& sourceDimensions) override;

private:
	std::vector<std::unique_ptr<jngl::Sprite>> sprites;
};
