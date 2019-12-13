#pragma once

#include <RmlUi/Core/RenderInterface.h>
#include <memory>

namespace jngl {
class Sprite;
}

class RocketJNGLRenderer : public Rml::Core::RenderInterface {
public:
	void RenderGeometry(Rml::Core::Vertex* vertices, int num_vertices, int* indices,
	                    int num_indices, Rml::Core::TextureHandle texture,
	                    const Rml::Core::Vector2f& translation) override;

	void EnableScissorRegion(bool) override {
		// TODO
	}

	void SetScissorRegion(int, int, int, int) override {
		// TODO
	}

	// Called by RmlUi when a texture is required by the library.
	bool LoadTexture(Rml::Core::TextureHandle& texture_handle,
	                 Rml::Core::Vector2i& texture_dimensions,
	                 const Rml::Core::String& source) override;

	bool GenerateTexture(Rml::Core::TextureHandle&, const Rml::Core::byte* source,
	                     const Rml::Core::Vector2i& sourceDimensions) override;

private:
	std::vector<std::unique_ptr<jngl::Sprite>> sprites;
};
