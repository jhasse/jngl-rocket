#pragma once

#include <Rocket/Core/RenderInterface.h>

class RocketJNGLRenderer : public Rocket::Core::RenderInterface {
public:
	void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices,
	                    int num_indices, Rocket::Core::TextureHandle texture,
	                    const Rocket::Core::Vector2f& translation) override;

	void EnableScissorRegion(bool) override {
		// TODO
	}

	void SetScissorRegion(int, int, int, int) override {
		// TODO
	}

	// Called by Rocket when a texture is required by the library.
	bool LoadTexture(Rocket::Core::TextureHandle& texture_handle,
	                 Rocket::Core::Vector2i& texture_dimensions,
	                 const Rocket::Core::String& source) override;

	bool GenerateTexture(Rocket::Core::TextureHandle&, const Rocket::Core::byte* source,
	                     const Rocket::Core::Vector2i& sourceDimensions) override;
};
