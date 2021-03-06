#pragma once
#include "opengl.h"
#include "file.h"
#include "string.h"
#include "std_hash.h"
#include "buffer.h"
#include "console.h"
#include <unordered_map>

using namespace TL;
using namespace TL::OpenGL;

struct Shader {
	GLuint program;
	List<char> path;
	FileTracker tracker;
	std::unordered_map<Span<char>, GLuint> uniforms;
	bool valid;
	
	u32 cull;
	u32 src_blend, dst_blend;
	u32 depth_func;
	bool depth_write;
	bool enable_blend;
};
struct ShaderCatalog {
	Allocator allocator;
	std::unordered_map<Span<char>, Shader> shaders;
	StringList<char> shader_file_names;
	Shader *fallback_shader;
};

Shader *find(ShaderCatalog &catalog, Span<char> name) {
	auto result = catalog.shaders.find(name);
	if (result != catalog.shaders.end()) {
		if (result->second.valid) {
			return &result->second;
		} else {
			print("Shader '%' is invalid\n", name);
		}
	} else {
		print("Shader '%' not found\n", name);
	}
	return catalog.fallback_shader;
}

void parse_shader(Shader &shader, Span<char> source) {
	shader.cull = 0;
	shader.depth_write = true;
	shader.enable_blend = false;
	shader.depth_func = GL_LEQUAL;

	auto get_blend = [](Span<char> str) {
		if (str == "src_alpha"s) {
			return GL_SRC_ALPHA;
		} else if (str == "inv_src_alpha"s) {
			return GL_ONE_MINUS_SRC_ALPHA;
		}
		print("bad blend param\n");
		return GL_ONE;
	};
	auto get_cull = [](Span<char> str) {
		if (str == "off"s) {
			return 0;
		} else if (str == "back"s) {
			return GL_BACK;
		} else if (str == "front"s) {
			return GL_FRONT;
		}
		print("bad cull param\n");
		return 0;
	};
	auto get_bool = [](Span<char> str) {
		if (str == "off"s) {
			return false;
		} else if (str == "on"s) {
			return true;
		}
		print("bad bool param\n");
		return false;
	};

	auto params_start = find(source, "/*!\n"s);
	auto params_end   = find(source, "\n!*/"s);
	if ((bool)params_start != (bool)params_end) {
		print("Bad parameter scope\n");
	} else if (params_start && params_end) {
		StaticList<Span<char>, 16> tokens;
		auto start = params_start + 4;
		while (1) {
			while (is_whitespace(*start))
				++start;

			if (*start == '!')
				break;

			auto end = start;
			while (!is_whitespace(*end))
				++end;
			tokens.add({start, end});
			start = end + 1;
		}
		auto token = tokens.begin();
		while (token != tokens.end()) {
			if (*token == "blend"s) {
				shader.enable_blend = true;
				shader.src_blend = get_blend(*++token);
				shader.dst_blend = get_blend(*++token);
			} else if (*token == "cull"s) {
				shader.cull = get_cull(*++token);
			} else if (*token == "depth_write"s) {
				shader.depth_write = get_bool(*++token);
			}
			++token;
		}
	}
}

Buffer load_shader_file(Span<char> full_path) {
	auto source = read_entire_file(full_path);
	assert(source.data);

	for (auto &c : source) {
		if (c == '\r') {
			c = '\n';
		}
	}
		
	auto directory = full_path;
	while (directory.back() != '/') --directory.size;

	while (1) {
		auto directive = "//include "s;
		auto include_start = find(source, as_bytes(directive));
		if (!include_start) {
			break;
		}
		auto file_name_start = include_start + directive.size;
		auto file_name_end = file_name_start;
		while (*file_name_end != '\n') {
			++file_name_end;
		}

		Span<char> file_name = {(char *)file_name_start, (char *)file_name_end};

		auto file_path = concatenate(directory, file_name);
		auto included_file = load_shader_file(file_path);
		free(file_path);

		auto full_directive_size = directive.size + file_name.size;
		auto new_source_size = source.size - full_directive_size + included_file.size;

		auto new_source = create_buffer(new_source_size);

		auto cursor = new_source.data;
		auto append_copy = [&](u8 const *from, umm size) {
			memcpy(cursor, from, size);
			cursor += size;
			assert(cursor <= (new_source.data + new_source_size));
		};
		append_copy(source.begin(), include_start - source.begin());
		append_copy(included_file.data, included_file.size);
		append_copy(file_name_end, source.end() - file_name_end);

		free(source);
		free(included_file);

		source = new_source;
	}
	return source;
}

Shader &add_file(ShaderCatalog &catalog, char const *directory, Span<char> full_name) {
	auto file_name = full_name;
	file_name.size = find(file_name, '.') - file_name.data;

	auto &shader = catalog.shaders[file_name];

	shader.path = concatenate(directory, '/', full_name, '\0');
	shader.tracker = create_file_tracker(shader.path.data, [&](FileTracker &tracker) {
		print("Compiling %\n", tracker.path);

		auto source = load_shader_file(as_span(tracker.path));
		defer { FREE(default_allocator, source.data); };

		auto vertex_shader = create_shader(GL_VERTEX_SHADER, 330, true, as_chars(source)).id;
		if (vertex_shader) {
			auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, 330, true, as_chars(source)).id;
			if (fragment_shader) {
				if (shader.program) {
					glDeleteProgram(shader.program);
				}
				shader.program = create_program(vertex_shader, fragment_shader).id;
				glDeleteShader(vertex_shader);
				glDeleteShader(fragment_shader);
				if (shader.program) {
					shader.uniforms.clear();

					GLint count;
					glGetProgramiv(shader.program, GL_ACTIVE_UNIFORMS, &count);
					
					const GLsizei bufSize = 64; // maximum name length
					GLchar name[bufSize]; // variable name in GLSL
					GLsizei length; // name length
					GLint size; // size of the variable
					GLenum type; // type of the variable (float, vec3 or mat4, etc)
					for (u32 i = 0; i < (u32)count; i++) {
						glGetActiveUniform(shader.program, i, bufSize, &length, &size, &type, name);
						Span<char> stored_name = {ALLOCATE(char, catalog.allocator, length), (umm)length};
						memcpy(stored_name.data, name, length);
						shader.uniforms[stored_name] = glGetUniformLocation(shader.program, name);
					}
					shader.valid = true;
					
					parse_shader(shader, as_chars(source));
					return;
				}
			}
		}
		shader.valid = false;
	});
	return shader;
}

void init(ShaderCatalog &catalog, char const *directory, Span<char> fallback_shader_name) {
	catalog.allocator = current_allocator;
	catalog.shader_file_names = get_files_in_directory(directory);
	auto fallback_shader_path = concatenate(fallback_shader_name, ".glsl");
	defer { free(fallback_shader_path); };
	catalog.fallback_shader = &add_file(catalog, directory, fallback_shader_path);
	assert(catalog.fallback_shader->program);
	for (auto &full_name : catalog.shader_file_names) {
		if (full_name == as_span(fallback_shader_path))
			continue;
		add_file(catalog, directory, full_name);
	}
}

void update(ShaderCatalog &catalog) {
	for (auto &[name, shader] : catalog.shaders) {
		update_file_tracker(shader.tracker);
	}
}

GLuint get_uniform_location(Shader &shader, Span<char> name) {
	auto it = shader.uniforms.find(name);
	if (it == shader.uniforms.end()) {
		return -1;
	}
	return it->second;
}

void set_uniform(Shader &shader, Span<char> name, f32 value) { glUniform1f(get_uniform_location(shader, name), value); }
void set_uniform(Shader &shader, Span<char> name, v2f value) { glUniform2fv(get_uniform_location(shader, name), 1, value.s); }
void set_uniform(Shader &shader, Span<char> name, v3f value) { glUniform3fv(get_uniform_location(shader, name), 1, value.s); }
void set_uniform(Shader &shader, Span<char> name, v4f value) { glUniform4fv(get_uniform_location(shader, name), 1, value.s); }
void set_uniform(Shader &shader, Span<char> name, m4  value) { glUniformMatrix4fv(get_uniform_location(shader, name), 1, false, value.s); }

void use_shader(Shader &shader) {
	glUseProgram(shader.program);

	if (shader.cull) {
		glEnable(GL_CULL_FACE);
		glCullFace(shader.cull);
	} else {
		glDisable(GL_CULL_FACE);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(shader.depth_func);

	glDepthMask(shader.depth_write);

	if (shader.enable_blend) {
		glEnable(GL_BLEND);
		glBlendFunc(shader.src_blend, shader.dst_blend);
	} else {
		glDisable(GL_BLEND);
	}
}
