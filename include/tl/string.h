#pragma once
#include "common.h"
#include "list.h"
#include "optional.h"

#pragma warning(push)
#pragma warning(disable: 4820)

namespace TL {

//
// List of strings pointing into one shared buffer
//
// Note: To start using 'strings' member ensure that list is in 'absolute mode',
// so 'data' member of strings' elements represent a pointer to the beginning of a string
// stored in 'buffer'
//
// Addition is allowed only In 'relative mode', where 'data' member of string's elements holds
// an index to the 'buffer'
//
template <class Char>
struct StringList {
	List<Span<Char>> strings;
	List<Char> buffer;
#ifdef TL_DEBUG
	bool is_absolute = false;
#endif

	void add(Span<Char> string) {
#ifdef TL_DEBUG
		assert(is_absolute == false);
#endif

		Span<Char> dest;
		dest.size = string.size;
		dest.data = (Char *)buffer.size;
		strings.add(dest);

		buffer += string;
	}
	void add(Char const *string) {
		add(Span((Char *)string, length(string)));
	}
	
	void make_absolute() {
#ifdef TL_DEBUG
		is_absolute = true;
#endif
		for (auto &string : strings) {
			string.data = buffer.data + (umm)string.data;
		}
	}
	void make_relative() {
#ifdef TL_DEBUG
		is_absolute = false;
#endif
		for (auto &string : strings) {
			string.data = (Char *)(string.data - buffer.data);
		}
	}

	Span<Char> *begin() { return strings.begin(); }
	Span<Char> *end() { return strings.end(); }
};

template <class Char, class Predicate>
Span<Char> *find_if(StringList<Char> &list, Predicate &&predicate) {
	return find_if(list.strings, std::forward<Predicate>(predicate));
}

template <class Char>
List<Char> null_terminate(Span<Char> span) {
	List<Char> result;
	result.size = span.size + 1;
	result.data = ALLOCATE(Char, current_allocator, result.size);
	result.capacity = result.size;
	memcpy(result.data, span.data, span.size * sizeof(Char));
	result.data[result.size - 1] = 0;
	return result;
}

inline u32 character_to_digit(char character, u32 base) {
	u32 digit;
	if (base > 10) {
		digit = (u64)character - '0';
		if (digit > 9) digit = (u64)to_lower_ascii(character) - ('a' - 10);
	} else {
		digit = (u64)character - '0';
	}
	return digit;
}

inline Optional<u64> parse_u64(Span<char> string, u32 base) {
	u64 result = 0;
	u64 previous = 0;
	for (auto character : string) {
		u64 digit = character_to_digit(character, base);
		if (digit >= base) return {};
		previous = result;
		result = result * base + digit;
		if (result < previous) return {};
	}
	return result;
}

inline Optional<u64> parse_u64(Span<char> string) {
	if (string.size > 1) {
		if (string[1] == 'x') {
			string.data += 2;
			return parse_u64(string, 16);
		} else if (string[1] == 'b') {
			string.data += 2;
			return parse_u64(string, 2);
		}
	}
	return parse_u64(string, 10);

}
#if 0
inline ParseResult<f64> parseDecimalFloat(Span<char> s) {
	ParseResult<f64> result = {};
	bool negative = false;
	if (s.front() == '-') {
		negative = true;
		s._begin++;
	}
	bool dot = false;
	u32 placeAfterDot = 0;
	for (auto c : s) {
		if (c == '.') {
			dot = true;
			continue;
		}
		if (dot) {
			s64 digit = (s64)c - '0';
			if ((u64)digit > 9)
				return {};
			result.value += (f64)digit / pow(10, placeAfterDot++);
		} else {
			result.value *= 10;
			s64 digit = (s64)c - '0';
			if ((u64)digit > 9)
				return {};
			result.value += (f64)digit;
		}
	}
	if (negative) {
		result.value = -result.value;
	}
	result.success = true;
	return result;
}
#endif

template <umm block_size_ = 4096>
struct StringBuilder {
	static constexpr umm block_size = block_size_;
	struct Block {
		char buffer[block_size];
		umm size = 0;
		Block *next = 0;

		Block() {}
		char *end() { return buffer + size; }
		umm available_space() { return block_size - size; }
	};

	Allocator allocator = {};
	Block first = {};
	Block *last = &first;
	Block *alloc_last = &first;

	StringBuilder() { allocator = current_allocator; }
	StringBuilder(StringBuilder const &that) = delete;
	StringBuilder(StringBuilder&& that) = default;
	StringBuilder &operator=(StringBuilder const &that) = delete;
	StringBuilder &operator=(StringBuilder &&that) = default;
	umm available_space() {
		umm space = 0;
		Block *block = last;
		while (block) {
			space += block->available_space();
			block = block->next;
		}
		return space;
	}
	void ensureSpace(umm amount) {
		umm space = available_space();
		while (space < amount) {
			Block *new_block = allocate_block();
			alloc_last->next = new_block;
			alloc_last = new_block;
			space += block_size;
		}
	}
	void ensure_consecutive_space(umm amount) {
		assert(amount <= block_size);
		if (last->available_space() < amount) {
			if (last->next) {
				last = last->next;
			} else {
				last->next = allocate_block();
				last = alloc_last = last->next;
			}
		}
	}
	umm append(Span<char> span) {
		umm chars_written = span.size;
		umm chars_to_write = span.size;
		while (last->available_space() < chars_to_write) {
			umm space_in_block = last->available_space();
			memcpy(last->end(), span.data, space_in_block * sizeof(char));
			chars_to_write -= space_in_block;
			last->size += space_in_block;
			span = {span.begin() + space_in_block, span.end()};
			if (!last->next) {
				last->next = alloc_last = allocate_block();
				last = last->next;
			}
		}
		memcpy(last->end(), span.data, chars_to_write * sizeof(char));
		last->size += chars_to_write;
		return chars_written;
	}
	umm append(char const *str) { return append(as_span(str)); }
	//umm append(char ch, umm count = 1) {
	//	umm chars_to_write = count;
	//	while (last->available_space() < chars_to_write) {
	//		umm space_in_block = last->available_space();
	//		populate(last->end(), ch, space_in_block);
	//		chars_to_write -= space_in_block;
	//		last->size += space_in_block;
	//		if (!last->next) {
	//			last = alloc_last = allocate_block();
	//			last = last->next;
	//		}
	//	}
	//	populate(last->end(), ch, chars_to_write);
	//	last->size += chars_to_write;
	//	return count;
	//}
	template <class T>
	umm append(T const &val) {
		return to_string<char>(val, [&](Span<char> span) {
			return append(span);
		});
	}
	umm append_bytes(void const *address, umm size) {
		append(Span((char *)address, (char *)address + size));
		return size;
	}
	template <class T>
	umm append_bytes(T const &value) {
		return append_bytes(std::addressof(value), sizeof(value));
	}
	umm append_format(char const *fmt) {
		if (!*fmt)
			return 0;
		char const *c = fmt;
		while (*c) {
			if (*c == '%') {
				++c;
				if (*c != '`') {
					invalid_code_path("bad format");
				}
				append(as_span(fmt, c));
				++c;
				fmt = c;
			} else {
				++c;
			}
		}
		return append(as_span(fmt));
	}
	template <class Arg, class ...Args>
	umm append_format(char const *fmt, Arg const &arg, Args const &...args) {
		if (!*fmt)
			return 0;
		char const *c = fmt;
		char const *fmtBegin = 0;
		char const *fmtEnd = 0;

		bool argAsFormat = false;
		
		while (*c) {
			if (*c == '%') {
				fmtBegin = c;
				++c;
				if (*c == '`') {
					append(as_span(fmt, c));
					++c;
					fmt = c;
					fmtBegin = 0;
					continue;
				}
				if (starts_with(c, "fmt%")) {
					argAsFormat = true;
					c += 4;
				}
				fmtEnd = c;
				break;
			}
			++c;
		}
		//assert(fmtBegin && fmtEnd, "invalid format");
		if (!(fmtBegin && fmtEnd)) {
			return append_format(fmt);
		}

		umm charsAppended = append(as_span(fmt, fmtBegin));

		if (argAsFormat) {
			if constexpr (isSame<RemoveReference<std::decay_t<Arg>>, char const *> || isSame<RemoveReference<std::decay_t<Arg>>, char *>)
				charsAppended += append_format(arg, args...);
			else 
				invalid_code_path("'%fmt%' arg is not a string");
		} else {
			to_string<char>(arg, [&] (Span<char> span) {
				charsAppended += span.size;
				append(span);
			});
		}

		charsAppended += append_format(fmtEnd, args...);
		return charsAppended;
	}
	umm size() {
		umm totalSize = 0;
		for (Block *block = &first; block != 0; block = block->next) {
			totalSize += block->size;
		}
		return totalSize;
	}
	Span<char> fill(Span<char> dst_string) {
		char *dst_char = dst_string.data;
		for (Block *block = &first; block != 0; block = block->next) {
			memcpy(dst_char, block->buffer, block->size * sizeof(char));
			dst_char += block->size;
		}
		return Span<char>(dst_string.begin(), dst_char);
	}
	List<char> get() {
		List<char> result;
		result.reserve(size());
		fill(result);
		result.size = result.capacity;
		return result;
	}
	List<char> getNullTerminated() {
		List<char> result;
		result.reserve(size() + 1);
		fill(result);
		result.size = result.capacity;
		result.back() = '\0';
		return result;
	}

	template <class Fn>
	void stream(Fn &&fn) {
		Block *block = &first;
		do {
			auto block_size = block->size;
			if (block_size) {
				fn(block->buffer, block_size);
			}
			block = block->next;
		} while (block);
	}
	void clear() {
		Block *block = &first;
		do {
			block->size = 0;
			block = block->next;
		} while (block);
		last = &first;
	}

private:
	Block *allocate_block() {
		return new (ALLOCATE(Block, allocator)) Block;
	}
};

template <umm block_size>
void free(StringBuilder<block_size> &builder) {
	for (auto block = builder.first.next; block != 0;) {
		auto next = block->next;
		FREE(builder.allocator, block);
		block = next;
	}
	builder.allocator = {};
}

template <class ...Args>
constexpr List<char> concatenate(Args &&...args) {
	StringBuilder builder;
	defer { free(builder); };
	int ___[] = { ((builder.append(args), ...), 0) };
	return builder.get();
}


#if 0
template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char, class ...Args>
String<Char, Allocator> format(Char const *fmt, Args const &...args) {
	StringBuilder<Char, Allocator> builder;
	builder.append_format(fmt, args...);
	return builder.get();
}

template <class Allocator = TL_DEFAULT_ALLOCATOR, class Char, class ...Args>
String<Char, Allocator> formatAndTerminate(Char const *fmt, Args const &...args) {
	StringBuilder<Char, Allocator> builder;
	builder.append_format(fmt, args...);
	builder.append((Char)0);
	return builder.get();
}
#endif
template <class Char, umm capacity, class T>
void append_string(StaticList<Char, capacity> &list, T const &value) {
	to_string<Char>(value, [&](Span<Char> span) { list += span; });
}

inline u8 const *advance_utf8(u8 const *string) {
	if (*string < 0x80) {
		++string;
	} else {
		u32 bytes = count_leading_ones(*string);
		if (bytes > 4) return 0;
		string += bytes;
	}
	return string;
}
template <class Ptr>
inline Ptr advance_utf8(Ptr string) { return (Ptr)advance_utf8((u8 const *&)string); }

inline u32 get_char_utf8(void const *_ptr) {
	auto ptr = (u8 const *)_ptr;
	if (*ptr < 0x80) {
		return *ptr;
	}

	switch (count_leading_ones(*ptr)) {
		case 2: return ((ptr[0] & 0x1Fu) <<  6u) | ((ptr[1] & 0x3Fu));
		case 3: return ((ptr[0] & 0x0Fu) << 12u) | ((ptr[1] & 0x3Fu) <<  6u) | ((ptr[2] & 0x3Fu));
		case 4: return ((ptr[0] & 0x07u) << 18u) | ((ptr[1] & 0x3Fu) << 12u) | ((ptr[2] & 0x3Fu) << 6u) | ((ptr[3] & 0x3Fu));
		default: return ~0u;
	}
}

}

#pragma warning(pop)
