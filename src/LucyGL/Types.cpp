#include <LucyGL/Types.h>
#include <glad/glad.h>
#include <map>
#include <assert.h>

void lgl::CheckValues() {
	assert(lgl::BYTE == GL_BYTE);
	assert(lgl::UNSIGNED_BYTE == GL_UNSIGNED_BYTE);
	assert(lgl::SHORT == GL_SHORT);
	assert(lgl::UNSIGNED_SHORT == GL_UNSIGNED_SHORT);
	assert(lgl::INT == GL_INT);
	assert(lgl::UNSIGNED_INT == GL_UNSIGNED_INT);
	assert(lgl::FLOAT == GL_FLOAT);
	assert(lgl::FIXED == GL_FIXED);
	assert(lgl::UNSIGNED_INT_24_8 == GL_UNSIGNED_INT_24_8);


	assert(lgl::RED == GL_RED);
	assert(lgl::RG == GL_RG);
	assert(lgl::RGB == GL_RGB);
	assert(lgl::RGBA == GL_RGBA);
	assert(lgl::RED_INTEGER == GL_RED_INTEGER);
	assert(lgl::GREEN_INTEGER == GL_GREEN_INTEGER);
	assert(lgl::BLUE_INTEGER == GL_BLUE_INTEGER);
	assert(lgl::RG_INTEGER == GL_RG_INTEGER);
	assert(lgl::RGB_INTEGER == GL_RGB_INTEGER);
	assert(lgl::RGBA_INTEGER == GL_RGBA_INTEGER);
	assert(lgl::BGR_INTEGER == GL_BGR_INTEGER);
	assert(lgl::BGRA_INTEGER == GL_BGRA_INTEGER);
	assert(lgl::R8 == GL_R8);
	assert(lgl::R16 == GL_R16);
	assert(lgl::R16F == GL_R16F);
	assert(lgl::R32F == GL_R32F);
	assert(lgl::R8I == GL_R8I);
	assert(lgl::R8UI == GL_R8UI);
	assert(lgl::R16I == GL_R16I);
	assert(lgl::R16UI == GL_R16UI);
	assert(lgl::R32I == GL_R32I);
	assert(lgl::R32UI == GL_R32UI);
	assert(lgl::RG8 == GL_RG8);
	assert(lgl::RG16 == GL_RG16);
	assert(lgl::RG16F == GL_RG16F);
	assert(lgl::RG32F == GL_RG32F);
	assert(lgl::RG8I == GL_RG8I);
	assert(lgl::RG8UI == GL_RG8UI);
	assert(lgl::RG16I == GL_RG16I);
	assert(lgl::RG16UI == GL_RG16UI);
	assert(lgl::RG32I == GL_RG32I);
	assert(lgl::RG32UI == GL_RG32UI);
	assert(lgl::RGB4 == GL_RGB4);
	assert(lgl::RGB5 == GL_RGB5);
	assert(lgl::RGB8 == GL_RGB8);
	assert(lgl::RGB10 == GL_RGB10);
	assert(lgl::RGB12 == GL_RGB12);
	assert(lgl::RGB16 == GL_RGB16);
	assert(lgl::RGB5_A1 == GL_RGB5_A1);
	assert(lgl::RGB10_A2 == GL_RGB10_A2);
	assert(lgl::RGB32F == GL_RGB32F);
	assert(lgl::RGB16F == GL_RGB16F);
	assert(lgl::RGB32UI == GL_RGB32UI);
	assert(lgl::RGB16UI == GL_RGB16UI);
	assert(lgl::RGB8UI == GL_RGB8UI);
	assert(lgl::RGB32I == GL_RGB32I);
	assert(lgl::RGB16I == GL_RGB16I);
	assert(lgl::RGB8I == GL_RGB8I);
	assert(lgl::RGBA2 == GL_RGBA2);
	assert(lgl::RGBA4 == GL_RGBA4);
	assert(lgl::RGBA8 == GL_RGBA8);
	assert(lgl::RGBA12 == GL_RGBA12);
	assert(lgl::RGBA16 == GL_RGBA16);
	assert(lgl::RGBA32F == GL_RGBA32F);
	assert(lgl::RGBA16F == GL_RGBA16F);
	assert(lgl::RGBA32UI == GL_RGBA32UI);
	assert(lgl::RGBA16UI == GL_RGBA16UI);
	assert(lgl::RGBA8UI == GL_RGBA8UI);
	assert(lgl::RGBA32I == GL_RGBA32I);
	assert(lgl::RGBA16I == GL_RGBA16I);
	assert(lgl::RGBA8I == GL_RGBA8I);
	assert(lgl::DEPTH_STENCIL == GL_DEPTH_STENCIL);
	assert(lgl::DEPTH24_STENCIL8 == GL_DEPTH24_STENCIL8);


	assert(lgl::POINT == GL_POINTS);
	assert(lgl::LINE == GL_LINES);
	assert(lgl::LINE_STRIP == GL_LINE_STRIP);
	assert(lgl::TRIANGLE == GL_TRIANGLES);
	assert(lgl::TRIANGLE_FAN == GL_TRIANGLE_FAN);
	assert(lgl::TRIANGLE_STRIP == GL_TRIANGLE_STRIP);


	assert(lgl::FilterMode_LINEAR == GL_LINEAR);
	assert(lgl::FilterMode_NEAREST == GL_NEAREST);


	assert(lgl::WrapMode_CLAMP_TO_EDGE == GL_CLAMP_TO_EDGE);
	assert(lgl::WrapMode_CLAMP_TO_BORDER == GL_CLAMP_TO_BORDER);
	assert(lgl::WrapMode_MIRRORED_REPEAT == GL_MIRRORED_REPEAT);
	assert(lgl::WrapMode_REPEAT == GL_REPEAT);
	assert(lgl::WrapMode_MIRROR_CLAMP_TO_EDGE == GL_MIRROR_CLAMP_TO_EDGE);


	assert(lgl::TEXTURE_1D == GL_TEXTURE_1D);
	assert(lgl::TEXTURE_2D == GL_TEXTURE_2D);
	assert(lgl::TEXTURE_3D == GL_TEXTURE_3D);
	assert(lgl::TEXTURE_1D_ARRAY == GL_TEXTURE_1D_ARRAY);
	assert(lgl::TEXTURE_2D_ARRAY == GL_TEXTURE_2D_ARRAY);
	assert(lgl::TEXTURE_RECTANGLE == GL_TEXTURE_RECTANGLE);
	assert(lgl::TEXTURE_CUBE_MAP == GL_TEXTURE_CUBE_MAP);
	assert(lgl::TEXTURE_BUFFER == GL_TEXTURE_BUFFER);
	assert(lgl::TEXTURE_2D_MULTISAMPLE == GL_TEXTURE_2D_MULTISAMPLE);
	assert(lgl::TEXTURE_2D_MULTISAMPLE_ARRAY == GL_TEXTURE_2D_MULTISAMPLE_ARRAY);


	assert(lgl::COLOR_ATTACHMENT0 == GL_COLOR_ATTACHMENT0);
	assert(lgl::COLOR_ATTACHMENT1 == GL_COLOR_ATTACHMENT1);
	assert(lgl::COLOR_ATTACHMENT2 == GL_COLOR_ATTACHMENT2);
	assert(lgl::COLOR_ATTACHMENT3 == GL_COLOR_ATTACHMENT3);
	assert(lgl::COLOR_ATTACHMENT4 == GL_COLOR_ATTACHMENT4);
	assert(lgl::COLOR_ATTACHMENT5 == GL_COLOR_ATTACHMENT5);
	assert(lgl::COLOR_ATTACHMENT6 == GL_COLOR_ATTACHMENT6);
	assert(lgl::COLOR_ATTACHMENT7 == GL_COLOR_ATTACHMENT7);
	assert(lgl::COLOR_ATTACHMENT8 == GL_COLOR_ATTACHMENT8);
	assert(lgl::COLOR_ATTACHMENT9 == GL_COLOR_ATTACHMENT9);
	assert(lgl::COLOR_ATTACHMENT10 == GL_COLOR_ATTACHMENT10);
	assert(lgl::COLOR_ATTACHMENT11 == GL_COLOR_ATTACHMENT11);
	assert(lgl::COLOR_ATTACHMENT12 == GL_COLOR_ATTACHMENT12);
	assert(lgl::COLOR_ATTACHMENT13 == GL_COLOR_ATTACHMENT13);
	assert(lgl::COLOR_ATTACHMENT14 == GL_COLOR_ATTACHMENT14);
	assert(lgl::COLOR_ATTACHMENT15 == GL_COLOR_ATTACHMENT15);
	assert(lgl::COLOR_ATTACHMENT16 == GL_COLOR_ATTACHMENT16);
	assert(lgl::COLOR_ATTACHMENT17 == GL_COLOR_ATTACHMENT17);
	assert(lgl::COLOR_ATTACHMENT18 == GL_COLOR_ATTACHMENT18);
	assert(lgl::COLOR_ATTACHMENT19 == GL_COLOR_ATTACHMENT19);
	assert(lgl::COLOR_ATTACHMENT20 == GL_COLOR_ATTACHMENT20);
	assert(lgl::COLOR_ATTACHMENT21 == GL_COLOR_ATTACHMENT21);
	assert(lgl::COLOR_ATTACHMENT22 == GL_COLOR_ATTACHMENT22);
	assert(lgl::COLOR_ATTACHMENT23 == GL_COLOR_ATTACHMENT23);
	assert(lgl::COLOR_ATTACHMENT24 == GL_COLOR_ATTACHMENT24);
	assert(lgl::COLOR_ATTACHMENT25 == GL_COLOR_ATTACHMENT25);
	assert(lgl::COLOR_ATTACHMENT26 == GL_COLOR_ATTACHMENT26);
	assert(lgl::COLOR_ATTACHMENT27 == GL_COLOR_ATTACHMENT27);
	assert(lgl::COLOR_ATTACHMENT28 == GL_COLOR_ATTACHMENT28);
	assert(lgl::COLOR_ATTACHMENT29 == GL_COLOR_ATTACHMENT29);
	assert(lgl::COLOR_ATTACHMENT30 == GL_COLOR_ATTACHMENT30);
	assert(lgl::COLOR_ATTACHMENT31 == GL_COLOR_ATTACHMENT31);
	assert(lgl::DEPTH_ATTACHMENT == GL_DEPTH_ATTACHMENT);
	assert(lgl::STENCIL_ATTACHMENT == GL_STENCIL_ATTACHMENT);
	assert(lgl::DEPTH_STENCIL_ATTACHMENT == GL_DEPTH_STENCIL_ATTACHMENT);
}
