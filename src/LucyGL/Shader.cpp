#include <LucyGL/Shader.h>

#include <glad/glad.h>
#include <assert.h>

#include <fstream>
#include <iostream>

static std::string read_file(const std::string &filename) {
	std::string line, text;
	std::fstream file(filename);

	while (std::getline(file, line)) text += (line + '\n');

	file.close();

	return text;
}

void Compile(unsigned int &program, const std::string &filename, unsigned int target, bool file) {
	unsigned int shader = glCreateShader(target);
	std::string text;
	if (file)
		text = read_file(filename);
	else
		text = filename;

	const char* src = text.c_str();

	assert(src != nullptr);

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	unsigned int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, (GLint*)&result);

	if (!result) {
		unsigned int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, (GLint*)&logLength);
		GLchar* message = (GLchar*)alloca(logLength*sizeof(char));
		glGetShaderInfoLog(shader, logLength, (GLsizei*)&logLength, message);
		std::cout << message << std::endl;
	}

	assert(result);

	glAttachShader(program, shader);
	glDeleteShader(shader);
};

lgl::Shader::~Shader() {
	//std::cout << "Shader Deleted: " << program << '\n';

	glDeleteProgram(program);
}

void lgl::Shader::VertexShader(const std::string& src, bool file) {
	Compile(program, src, GL_VERTEX_SHADER, file);
}

void lgl::Shader::FragmentShader(const std::string& src, bool file) {
	Compile(program, src, GL_FRAGMENT_SHADER, file);
}

void lgl::Shader::ComputeShader(const std::string& src, bool file) {
	Compile(program, src, GL_COMPUTE_SHADER, file);
}

void lgl::Shader::GeometryShader(const std::string& src, bool file) {
	Compile(program, src, GL_GEOMETRY_SHADER, file);
}

void lgl::Shader::Link() {
	glLinkProgram(program);
}

lgl::Shader::Shader() {
	program = glCreateProgram();

	//std::cout << "Shader Created: " << program << '\n';
}

lgl::Shader::Shader(const std::string &vs_filename, const std::string &fs_filename, bool file) {
	program = glCreateProgram();

	//std::cout << "Shader Created: " << program << '\n';

	VertexShader(vs_filename, file);
	FragmentShader(fs_filename, file);

	Link();
}

lgl::Shader::Shader(const std::string &vs_filename, const std::string &fs_filename) {
	program = glCreateProgram();

	//std::cout << "Shader Created: " << program << '\n';

	VertexShader(vs_filename, true);
	FragmentShader(fs_filename, true);

	Link();
}

void lgl::Shader::Bind() {
	glUseProgram(program);
}

void lgl::Shader::UnBind() {
	glUseProgram(0);
}

void lgl::Shader::BindUniformBlock(std::string name, unsigned int index) {
	glUniformBlockBinding(program, glGetUniformBlockIndex(program, name.c_str()), index);
}

unsigned int lgl::Shader::GetUniformLoc(std::string name) {
	if (uniform_location_map.find(name) == uniform_location_map.end())
		uniform_location_map[name] = glGetUniformLocation(program, name.c_str());
		
	return uniform_location_map[name];
}

bool lgl::Shader::SetUniformMat4(std::string uniform, const float* matrix) {
	auto location = GetUniformLoc(uniform);
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix);

	return (location > 0);
}

bool lgl::Shader::SetUniformi(std::string uniform, int v0) {
	auto location = GetUniformLoc(uniform);
	glUniform1i(location, v0);

	return (location > 0);
}

bool lgl::Shader::SetUniformf(std::string uniform, float v0) {
	auto location = GetUniformLoc(uniform);
	glUniform1f(location, v0);

	return (location > 0);
}

bool lgl::Shader::SetUniformVec2(std::string uniform, const float *v) {
	auto location = GetUniformLoc(uniform);
	glUniform2fv(location, 1, v);

	return (location > 0);
}

bool lgl::Shader::SetUniformVec3(std::string uniform, const float *v) {
	auto location = GetUniformLoc(uniform);
	glUniform3fv(location, 1, v);

	return (location > 0);
}

bool lgl::Shader::SetUniformVec4(std::string uniform, const float *v) {
	auto location = GetUniformLoc(uniform);
	glUniform4fv(location, 1, v);

	return (location > 0);
}

bool lgl::Shader::SetUniformArray(std::string uniform, size_t count, const float *v) {
	auto location = GetUniformLoc(uniform);
	glUniform1fv(location, count, v);

	return (location > 0);
}

bool lgl::Shader::SetUniformArray(std::string uniform, size_t count, const unsigned int *v) {
	auto location = GetUniformLoc(uniform);
	glUniform1uiv(location, count, v);

	return (location > 0);
}

bool lgl::Shader::SetUniformArray(std::string uniform, size_t count, const int *v) {
	auto location = GetUniformLoc(uniform);
	glUniform1iv(location, count, v);

	return (location > 0);
}
