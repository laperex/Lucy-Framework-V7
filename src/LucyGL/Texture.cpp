#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stb_image.h>
#include <LucyGL/Texture.h>
#include <glad/glad.h>
//#include <iostream>

#define NULL_TEXTURE {\
	255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255,\
		0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,\
	255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255,\
		0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,\
	255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255,\
		0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,\
	255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255,\
		0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,\
	255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255,\
		0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,\
	255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255,\
		0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,\
	255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255,\
		0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,\
	255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255,\
		0,   0,   0, 255, 255,   0, 255, 255,   0,   0,   0, 255, 255,   0, 255, 255,\
}

lgl::Texture::Texture(TextureMode mode) {
	this->mode = mode;

	glGenTextures(1, &id);

	//std::cout << "Texture Created: " << id << '\n';
}

lgl::Texture::~Texture() {
	//std::cout << "Texture Deleted: " << id << '\n';

	glDeleteTextures(1, &id);
}

bool lgl::Texture::LoadTexture(const char* filename, bool flip) {
	assert(mode == TEXTURE_2D);

	mode = TEXTURE_2D;

	SetWrapMode(WrapMode_MIRRORED_REPEAT, WrapMode_MIRRORED_REPEAT);
	SetFilteringMode(FilterMode_NEAREST, FilterMode_NEAREST);

	unsigned char* data = nullptr;
	stbi_set_flip_vertically_on_load(flip);
	if (filename) data = stbi_load(filename, &width, &height, &channels, 0);
		
	if (!data) {
		static uint8_t default_data[] = NULL_TEXTURE;

		Load2D(0, RGBA, 4, 4, 0, RGBA, UNSIGNED_BYTE, default_data);

		return false;
	} else {
		if (channels == 4) Load2D(0, RGBA, width, height, 0, RGBA, UNSIGNED_BYTE, data);
		if (channels == 3) Load2D(0, RGBA, width, height, 0, RGB, UNSIGNED_BYTE, data);
	}

	stbi_image_free(data);

	return true;
}

bool lgl::Texture::LoadSpriteSheet(const char* filename, int x, int y, int w, int h, bool flip) {
	int width, height, channels;

	stbi_set_flip_vertically_on_load(flip);
	auto* data = stbi_load(filename, &width, &height, &channels, 0);

	mode = TEXTURE_2D_ARRAY;

	if (!data) {
		static uint8_t default_data[] = NULL_TEXTURE;

		mode = TEXTURE_2D;
		Load2D(0, RGBA, 4, 4, 0, RGBA, UNSIGNED_BYTE, default_data);

		return false;
	}

	this->width = width;
	this->height = height;
	this->channels = channels;

	this->tile_x = x;
	this->tile_y = y;
	this->tile_w = w;
	this->tile_h = h;
	
	int tilex = width/w;
	int tiley = height/h;

	this->tile_x = tilex;
	this->tile_y = tiley;

	Load3D(0, RGBA, w, h, tilex * tiley, 0, RGBA, UNSIGNED_BYTE, nullptr);

	SetWrapMode(WrapMode_REPEAT, WrapMode_REPEAT);
	SetFilteringMode(FilterMode_LINEAR, FilterMode_LINEAR);

	int offset = 0;
	unsigned char* tmp = (unsigned char*)alloca(h * w * channels);

	for (int y = 0; y != tiley; y++) {
		for (int x = 0; x != tilex; x++) {
			for (int tx = 0; tx != w; tx++) {
				for (int ty = 0; ty != h; ty++) {
					tmp[channels * h * ty + channels * tx + 0] = data[channels * width * h * y + channels * w * x + channels * width * ty + channels * tx + 0];	// r
					tmp[channels * h * ty + channels * tx + 1] = data[channels * width * h * y + channels * w * x + channels * width * ty + channels * tx + 1];	// b
					tmp[channels * h * ty + channels * tx + 2] = data[channels * width * h * y + channels * w * x + channels * width * ty + channels * tx + 2];	// g
					if (channels == 4)
					tmp[channels * h * ty + channels * tx + 3] = data[channels * width * h * y + channels * w * x + channels * width * ty + channels * tx + 3];	// a
				}
			}

			switch (channels) {
				case 4:
					LoadSub3D(0, 0, 0, offset, w, h, 1, RGBA, UNSIGNED_BYTE, tmp);
					break;
					
				case 3:
					LoadSub3D(0, 0, 0, offset, w, h, 1, RGB, UNSIGNED_BYTE, tmp);
					break;
				
				default:
					assert(false);
			}

			offset++;
		}
	}

	stbi_image_free(data);

	return true;
}

void lgl::Texture::Load1D(int level, Format internalformat, int width, int border, Format format, Type type, void* data) {
	glTexImage1D(mode, level, internalformat, width, border, format, type, data);
}

void lgl::Texture::LoadSub1D(int level, int x, int width, Format format, Type type, void* data) {
	glTexSubImage1D(mode, level, x, width, format, type, data);
}

void lgl::Texture::Load2D(int level, Format internalformat, int width, int height, int border, Format format, Type type, void* data) {
	glTexImage2D(mode, level, internalformat, width, height, border, format, type, data);
}

void lgl::Texture::LoadSub2D(int level, int x, int y, int width, int height, Format format, Type type, void* data) {
	glTexSubImage2D(mode, level, x, y, width, height, format, type, data);
}

void lgl::Texture::Load3D(int level, Format internalformat, int width, int height, int depth, int border, Format format, Type type, void* data) {
	glTexImage3D(mode, level, internalformat, width, height, depth, border, format, type, data);
}

void lgl::Texture::LoadSub3D(int level, int x, int y, int z, int width, int height, int depth, Format format, Type type, void* data) {
	glTexSubImage3D(mode, level, x, y, z, width, height, depth, format, type, data);
}

void lgl::Texture::GenerateMimmap() {
	glGenerateMipmap(id);
}

void lgl::Texture::SetWrapMode(TextureWrapMode wrap_s, TextureWrapMode wrap_t, TextureWrapMode wrap_r) {
	if (this->wrap_s != wrap_s && wrap_s != WrapMode_None) {
		this->wrap_s = wrap_s;
		glTexParameteri(mode, GL_TEXTURE_WRAP_S, wrap_s);
	}
	if (this->wrap_t != wrap_t && wrap_t != WrapMode_None) {
		this->wrap_t = wrap_t;
		glTexParameteri(mode, GL_TEXTURE_WRAP_T, wrap_t);
	}
	if (this->wrap_r != wrap_r && wrap_r != WrapMode_None) {
		this->wrap_r = wrap_r;
		glTexParameteri(mode, GL_TEXTURE_WRAP_R, wrap_r);
	}
}

void lgl::Texture::SetFilteringMode(TextureFilteringMode mag, TextureFilteringMode min) {
	if (this->mag != mag && mag != FilterMode_None) {
		this->mag = mag;
		glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, mag);
	}
	if (this->min != min && min != FilterMode_None) {
		this->min = min;
		glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, min);
	}
}

void lgl::Texture::Bind() {
	glBindTexture(mode, id);
}

void lgl::Texture::BindUnit(const unsigned int unit, const TextureId id) {
	glBindTextureUnit(unit, id);
}

void lgl::Texture::BindUnit(const unsigned int unit) {
	glBindTextureUnit(unit, id);
}

void lgl::Texture::SetMode(lgl::TextureMode mode) {
	this->mode = mode;
}

void lgl::Texture::UnBind() {
	glBindTexture(mode, 0);
}

void lgl::Texture::UnBind(const lgl::TextureMode mode) {
	glBindTexture(mode, 0);
}
