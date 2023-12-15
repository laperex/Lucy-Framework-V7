#include <LucyGL/Status.h>
#include <glad/glad.h>
#include <LucyGL/Types.h>

lgl::Status& lgl::GetStatus() {
	static lgl::Status status;
	return status;
}
