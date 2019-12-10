#include "RocketJNGLRenderer.hpp"

#include <Rocket/Core.h>
#include <jngl.hpp>

void RocketJNGLRenderer::RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices,
                                        int* /*indices*/, int /*numIndices*/,
                                        Rocket::Core::TextureHandle texture,
                                        const Rocket::Core::Vector2f& translation) {
	assert(numVertices > 0 && numVertices % 4 == 0);
	Rocket::Core::Vertex arr[4];
	arr[0] = vertices[0];
	arr[1] = vertices[1];
	arr[2] = vertices[2];
	arr[3] = vertices[3];
	jngl::pushMatrix();
	jngl::translate(translation);
	jngl::translate(vertices[0].position);

	assert(texture != 0);
	const jngl::Sprite& sprite = *reinterpret_cast<const jngl::Sprite*>(texture);

	jngl::scale((vertices[2].position.x - vertices[0].position.x) / sprite.getWidth() /
	                (vertices[2].tex_coord.x - vertices[0].tex_coord.x),
	            (vertices[2].position.y - vertices[0].position.y) / sprite.getHeight() /
	                (vertices[2].tex_coord.y - vertices[0].tex_coord.y));

	sprite.drawClipped({ vertices[0].tex_coord.x, vertices[0].tex_coord.y },
	                   { vertices[2].tex_coord.x, vertices[2].tex_coord.y });
	jngl::popMatrix();
	if (numVertices > 4) {
		RenderGeometry(vertices + 4, numVertices - 4, nullptr, 0, texture, translation);
	}
}

bool RocketJNGLRenderer::LoadTexture(Rocket::Core::TextureHandle& texture_handle,
                                     Rocket::Core::Vector2i& texture_dimensions,
                                     const Rocket::Core::String& source) {

	if (source.Length() >= 4 and source.Substring(source.Length() - 4, 4) != ".tga") {
		sprites.emplace_back(std::make_unique<jngl::Sprite>(source.CString()));
		texture_dimensions.x = sprites.back()->getWidth();
		texture_dimensions.y = sprites.back()->getHeight();
		sprites.back()->setPos(0, 0);
		texture_handle = reinterpret_cast<uintptr_t>(sprites.back().get());
		return true;
	}

	// Code taken from libRocket's ShellRenderInterfaceOpenGL::LoadTexture

	Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
	Rocket::Core::FileHandle file_handle = file_interface->Open(source);
	if (!file_handle) {
		return false;
	}

	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly
// from file
#pragma pack(1)
	struct TGAHeader {
		char idLength;
		char colourMapType;
		char dataType;
		short int colourMapOrigin;
		short int colourMapLength;
		char colourMapDepth;
		short int xOrigin;
		short int yOrigin;
		short int width;
		short int height;
		char bitsPerPixel;
		char imageDescriptor;
	};
// Restore packing
#pragma pack()

	ROCKET_ASSERTMSG(
	    buffer_size > sizeof(TGAHeader),
	    "Texture file size is smaller than TGAHeader, file must be corrupt or otherwise invalid");
	if (buffer_size <= sizeof(TGAHeader)) {
		file_interface->Close(file_handle);
		return false;
	}

	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);

	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));

	int color_mode = header.bitsPerPixel / 8;
	int image_size = header.width * header.height * 4; // We always make 32bit textures

	if (header.dataType != 2) {
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR,
		                           "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}

	// Ensure we have at least 3 colors
	if (color_mode < 3) {
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR,
		                           "Only 24 and 32bit textures are supported");
		return false;
	}

	const char* image_src = buffer + sizeof(TGAHeader);
	unsigned char* image_dest = new unsigned char[image_size];

	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++) {
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0)
		                       ? read_index
		                       : (header.height - y - 1) * header.width * color_mode;
		for (long x = 0; x < header.width; x++) {
			image_dest[write_index] = image_src[read_index + 2];
			image_dest[write_index + 1] = image_src[read_index + 1];
			image_dest[write_index + 2] = image_src[read_index];
			if (color_mode == 4)
				image_dest[write_index + 3] = image_src[read_index + 3];
			else
				image_dest[write_index + 3] = 255;

			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;

	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);

	delete[] image_dest;
	delete[] buffer;

	return success;
}

bool RocketJNGLRenderer::GenerateTexture(Rocket::Core::TextureHandle& handle,
                                         const Rocket::Core::byte* source,
                                         const Rocket::Core::Vector2i& sourceDimensions) {
	sprites.emplace_back(
	    std::make_unique<jngl::Sprite>(source, sourceDimensions.x, sourceDimensions.y));
	sprites.back()->setPos(0, 0);
	handle = reinterpret_cast<uintptr_t>(sprites.back().get());
	return true;
}
