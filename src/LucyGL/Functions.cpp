#include <LucyGL/Functions.h>
#include <LucyGL/Types.h>
#include <glad/glad.h>
#include <stddef.h>
#include <assert.h>

static bool init = false;

void lgl::Initialize(void* loadprocaddress) {
	assert(!init);

	gladLoadGLLoader((GLADloadproc)loadprocaddress);

	CheckValues();

	init = true;
}

bool lgl::IsInitialized() {
	return init;
}

void lgl::Viewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

void lgl::ReadPixels(int x, int y, int width, int height, lgl::Format format, lgl::Type type, float* pixels) {
	glReadPixels(x, y, width, height, format, type, pixels);
}

void lgl::SetReadBuffer(lgl::Attachment attachment) {
	glReadBuffer(attachment);
}

void lgl::ResetReadBuffer() {
	glReadBuffer(NONE);
}

void lgl::Clear(float r, float g, float b, float a, const unsigned int flags) {
	glClearColor(r, g, b, a);
	glClear(flags);
}

void lgl::Clear(const unsigned int flags) {
	glClear(flags);
}

void lgl::ClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}