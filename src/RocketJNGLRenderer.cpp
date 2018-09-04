#include "RocketJNGLRenderer.hpp"

#include <Rocket/Core.h>
#include <iostream>

void RocketJNGLRenderer::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices,
                                        int* indices, int num_indices,
                                        Rocket::Core::TextureHandle texture,
                                        const Rocket::Core::Vector2f& translation) {
	std::cout << "num_vertices: " << num_vertices << std::endl;
}

bool RocketJNGLRenderer::LoadTexture(Rocket::Core::TextureHandle& texture_handle,
                                     Rocket::Core::Vector2i& texture_dimensions,
                                     const Rocket::Core::String& source) {
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

bool RocketJNGLRenderer::GenerateTexture(Rocket::Core::TextureHandle&,
                                         const Rocket::Core::byte* source,
                                         const Rocket::Core::Vector2i& sourceDimensions) {
	return false; // TODO
}
