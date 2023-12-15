#include <LucyGL/Status.h>
#include <LucyGL/DrawCall.h>

#include <glad/glad.h>
#include <assert.h>

enum DrawMode {
	DEFAULT = lgl::NONE,

	INDIRECT,
	INSTANCED,
	INSTANCED_BASEINSTANCE,
	
	INDEXED,
	INDEXED_INDIRECT,
	INDEXED_INSTANCED,
	INDEXED_BASEVERTEX,
	INDEXED_INSTANCED_BASEINSTANCE,
	INDEXED_INSTANCED_BASEVERTEX,
	INDEXED_INSTANCED_BASEVERTEX_BASEINSTANCE,

	MULTI,
	MULTI_INDIRECT,
	MULTI_INDIRECTCOUNT,

	INDEXED_MULTI,
	INDEXED_MULTI_BASEVERTEX,
	INDEXED_MULTI_INDIRECT,
	INDEXED_MULTI_INDIRECTCOUNT,

	RANGE_INDEXED,
	RANGE_INDEXED_BASEVERTEX,

	PIXELS,

	COUNT
};

bool DrawCmd(DrawMode drawmode, lgl::Primitive mode, lgl::Type type, lgl::Format format, int first, int start, int end, int count, int basevertex, int baseinstance, int instancecount, int width, int height, void* indices, void* indirect, void* pixels) {
	switch (drawmode) {
		case DEFAULT:
			glDrawArrays(mode, first, count);
			break;
		case INDIRECT:
			glDrawArraysIndirect(mode, indirect);
			break;
		case INSTANCED:
			glDrawArraysInstanced(mode, first, count, instancecount);
			break;
		case INSTANCED_BASEINSTANCE:
			glDrawArraysInstancedBaseInstance(mode, first, count, instancecount, baseinstance);
			break;
			
		case INDEXED:
			glDrawElements(mode, count, type, indices);
			break;
		case INDEXED_INDIRECT:
			glDrawElementsIndirect(mode, type, indirect);
			break;
		case INDEXED_INSTANCED:
			glDrawElementsInstanced(mode, count, type, indices, instancecount);
			break;
		case INDEXED_BASEVERTEX:
			glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
			break;
		case INDEXED_INSTANCED_BASEINSTANCE:
			glDrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex);
			break;
		case INDEXED_INSTANCED_BASEVERTEX:
			glDrawElementsInstancedBaseVertex(mode, count,  type, indices, instancecount, basevertex);
			break;
		case INDEXED_INSTANCED_BASEVERTEX_BASEINSTANCE:
			glDrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instancecount, basevertex, baseinstance);
			break;
		
		case MULTI:
			break;
		case MULTI_INDIRECT:
			break;
		case MULTI_INDIRECTCOUNT:
			break;

		case INDEXED_MULTI:
			break;
		case INDEXED_MULTI_BASEVERTEX:
			break;
		case INDEXED_MULTI_INDIRECT:
			break;
		case INDEXED_MULTI_INDIRECTCOUNT:
			break;
		
		case RANGE_INDEXED:
			glDrawRangeElements(mode, start, end, count, type, indices);
			break;
		case RANGE_INDEXED_BASEVERTEX:
			glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
			break;

		case PIXELS:
			glDrawPixels(width, height, format, type, pixels);
			break;

		default:
			return false;
	}

	// lgl::GetStatus().drawcount++;

	return true;
}

bool lgl::DrawIndexed(Primitive mode, int count, Type type, void* indices) {
	return DrawCmd(INDEXED, mode, type, (Format)0, 0, 0, 0, count, 0, 0, 0, 0, 0, indices, nullptr, nullptr);
}

bool lgl::MultiDrawIndexed(Primitive mode, int count, Type type, void* indices) {
	assert(false);
	return false;
}

bool lgl::Draw(Primitive mode, int first, int count) {
	return DrawCmd(DEFAULT, mode, (Type)0, (Format)0, first, 0, 0, count, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr);
}