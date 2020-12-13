#pragma once
#include "common.h"
#include <string.h>

#pragma warning(push)
#pragma warning(disable : 4582)

namespace TL {

template <class T, class Allocator>
struct ForwardListBase {
	ForwardListBase() = default;
	explicit ForwardListBase(umm length) {
		resize(length);
	}
	ForwardListBase(Span<T const> span) {
		reserve(span.size());
		_copyConstruct(span.begin(), span.end());
		_end = _begin + span.size();
	}
	ForwardListBase(T const *begin, umm length) : ForwardListBase(Span(begin, length)) {}
	ForwardListBase(ForwardListBase &&that) {
		_begin = that._begin;
		_end = that._end;
		_allocEnd = that._allocEnd;
		that._begin = 0;
		that._end = 0;
		that._allocEnd = 0;
	}
	ForwardListBase(ForwardListBase const &that) {
		_begin = ALLOCATE_T(Allocator, T, that.size(), 0);
		_allocEnd = _end = _begin + that.size();
		_copyConstruct(that._begin, that._end);
	}
	~ForwardListBase() {
		clear();
		if (_begin)
			DEALLOCATE(Allocator, _begin);
		_begin = 0;
		_end = 0;
		_allocEnd = 0;
	}
	ForwardListBase &set(Span<T const> span) {
		clear();
		if (span.size() > capacity()) {
			_reallocate(span.size());
		}
		_copyConstruct(span.begin(), span.end());
		_end = _begin + span.size();
		return *this;
	}
	ForwardListBase &operator=(ForwardListBase const &that) {
		clear();
		reserve(that.size());
		_end = _begin + that.size();
		_copyConstruct(that._begin, that._end);
		return *this;
	}
	ForwardListBase &operator=(ForwardListBase &&that) {
		clear();
		if (_begin)
			DEALLOCATE(Allocator, _begin);
		_begin = that._begin;
		_end = that._end;
		_allocEnd = that._allocEnd;
		that._begin = 0;
		that._end = 0;
		that._allocEnd = 0;
		return *this;
	}

	void _copyConstruct(T const *srcBegin, T const *srcEnd) {
		umm srcSize = (umm)(srcEnd - srcBegin);
		for (umm i = 0; i < srcSize; ++i) {
			new (_begin + i) T(srcBegin[i]);
		}
	}

	T *begin() { return _begin; }
	T *end() { return _end; }
	T const *begin() const { return _begin; }
	T const *end() const { return _end; }

	umm remainingCapacity() const { return (umm)(_allocEnd - _end); }
	umm capacity() const { return (umm)(_allocEnd - _begin); }
	
	umm size() const { return (umm)(_end - _begin); }
	bool empty() const { return size() == 0; }

	T *data() { return _begin; }
	T &front() { TL_BOUNDS_CHECK(size()); return *_begin; }
	T &back() { TL_BOUNDS_CHECK(size()); return _end[-1]; }
	T &operator[](umm i) { TL_BOUNDS_CHECK(size()); return _begin[i]; }

	T const *data() const { return _begin; }
	T const &front() const { TL_BOUNDS_CHECK(size()); return *_begin; }
	T const &back() const { TL_BOUNDS_CHECK(size()); return _end[-1]; }
	T const &operator[](umm i) const { TL_BOUNDS_CHECK(size()); return _begin[i]; }

	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(std::move(val)); }
	void pop_back() { TL_BOUNDS_CHECK(size()); (--_end)->~T(); }
	void reserve(umm count) {
		if (count > capacity())
			_reallocate(count);
	}
	void resize(umm newSize) {
		if (newSize > capacity())
			_reallocate(newSize);
		if (newSize > size()) {
			for (T *t = _end; t < _begin + newSize; ++t)
				new (t) T();
			_end = _begin + newSize;
		} else if (newSize < size()) {
			for (T *t = _begin + newSize; t < _end; ++t)
				t->~T();
			_end = _begin + newSize;
		}
	}
	void clear() {
		for (auto &val : *this) {
			val.~T();
		}
		_end = _begin;
	}
	
	operator Span<T>() { return {begin(), end()}; }
	operator Span<T const>() const { return {begin(), end()}; }

	T *_begin = 0;
	T *_end = 0;
	T *_allocEnd = 0;

	void _reallocate(umm newCapacity) {
		ASSERT(capacity() < newCapacity);
		umm oldSize = size();
		T *newBegin = ALLOCATE_T(Allocator, T, newCapacity, 0);
		for (T *src = _begin, *dst = newBegin; src != _end; ++src, ++dst) {
			new (dst) T(std::move(*src));
			src->~T();
		}
		if (_begin)
			DEALLOCATE(Allocator, _begin);
		_begin = newBegin;
		_end = _begin + oldSize;
		_allocEnd = _begin + newCapacity;
	}
	void _grow(umm requiredSize) {
		umm newCapacity = capacity();
		if (newCapacity == 0)
			newCapacity = 1;
		while (newCapacity < requiredSize) {
			newCapacity *= 2;
		}
		_reallocate(newCapacity);
	}
	template <class... Args>
	void emplace_back(Args &&... args) {
		if (remainingCapacity() == 0) {
			_grow(size() + 1);
		}
		new ((void *)_end++) T(std::forward<Args>(args)...);
	}
};

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR>
struct List : ForwardListBase<T, Allocator> {
	using Base = ForwardListBase<T, Allocator>;
	List() = default;
	List(T const *first, umm count) : Base(first, count) {}
	explicit List(umm size) : Base(size) {}
	List(Span<T const> span) : Base(span) {}
	List(List const &that) = default;
	List(List &&that) = default;
	List &operator=(List const &that) = default;
	List &operator=(List &&that) = default;
	List &set(Span<char const> span) { return Base::set(span), *this; }
	template <class... Args>
	void push_front(Args &&... args) {
		if (this->remainingCapacity() == 0) {
			umm newCapacity = this->capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			this->_reallocate(newCapacity);
		}
		new (this->_end) T(std::move(this->_end[-1]));
		for (T *dest = this->_end - 1; dest > this->_begin; --dest) {
			*dest = std::move(dest[-1]);
		}
		++this->_end;
		new (this->_begin) T(std::forward<Args>(args)...);
	}
	void erase(T *val) {
		ASSERT(this->_begin <= val && val < this->_end, "value is not in container");
		val->~T();
		--this->_end;
		for (T *dest = val; dest != this->_end; ++dest) {
			*dest = std::move(dest[1]);
		}
		this->_end->~T();
	}
	void erase(T &val) { erase(std::addressof(val)); }

	List &operator+=(T const &v) { this->push_back(v); return *this; }
	List &operator+=(T &&v) { this->push_back(std::move(v)); return *this; }
};

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR>
struct UnorderedList : ForwardListBase<T, Allocator> {
	using Base = ForwardListBase<T, Allocator>;
	UnorderedList() = default;
	explicit UnorderedList(umm size) : Base(size) {}
	UnorderedList(Span<T const> span) : Base(span) {}
	UnorderedList(UnorderedList const &that) = default;
	UnorderedList(UnorderedList &&that) = default;
	UnorderedList &operator=(UnorderedList const &that) = default;
	UnorderedList &operator=(UnorderedList &&that) = default;
	template <class... Args>
	void push_front(Args &&... args) {
		if (this->remainingCapacity() == 0) {
			umm newCapacity = this->capacity() * 2;
			if (newCapacity == 0)
				newCapacity = 1;
			this->_reallocate(newCapacity);
		}
		new (this->_end) T(std::move(*this->_begin));
		++this->_end;
		new (this->_begin) T(std::forward<Args>(args)...);
	}
	void erase(T *val) {
		ASSERT(this->_begin <= val && val < this->_end, "value is not in container");
		val->~T();
		new (val) T(std::move(*(this->_end-- - 1)));
	}
	void erase(T &val) { erase(&val); }
};

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR>
struct Queue {
	Queue() = default;
	explicit Queue(umm size) {
		resize(size);
	}
	Queue(Span<T const> span) {
		_reallocate(span.size());
		for (auto &src : span) {
			push(src);
		}
	}
	Queue(Queue const &that) : Queue((Span<T const>)that) {}
	Queue(Queue &&that) {
		_reallocate(that.size());
		for (auto &src : that) {
			push(std::move(src));
		}
		that._allocBegin = 0;
		that._allocEnd = 0;
		that._begin = 0;
		that._end = 0;
	}
	Queue &operator=(Queue const &that) { clear(); return *new (this) Queue(that); }
	Queue &operator=(Queue &&that) { clear(); return *new (this) Queue(std::move(that)); }
	~Queue() {
		clear();
		if (_allocBegin)
			DEALLOCATE(Allocator, _allocBegin);
		_begin = 0;
		_end = 0;
		_allocBegin = 0;
		_allocEnd = 0;
	}

	T *begin() { return _begin; }
	T *end() { return _end; }
	T const *begin() const { return _begin; }
	T const *end() const { return _end; }

	umm space_back() const { return (umm)(_allocEnd - _end); }

	umm size() const { return (umm)(_end - _begin); }
	bool empty() const { return size() == 0; }

	T *data() { return _begin; }
	T &front() { TL_BOUNDS_CHECK(size()); return *_begin; }
	T &back() { TL_BOUNDS_CHECK(size()); return _end[-1]; }
	T &operator[](umm i) { TL_BOUNDS_CHECK(size()); return _begin[i]; }

	T const *data() const { return _begin; }
	T const &front() const { TL_BOUNDS_CHECK(size()); return *_begin; }
	T const &back() const { TL_BOUNDS_CHECK(size()); return _end[-1]; }
	T const &operator[](umm i) const { TL_BOUNDS_CHECK(size()); return _begin[i]; }
	
	template <class... Args>
	void emplace(Args &&... args) {
		if (space_back() == 0) {
			_grow(size() + 1);
		}
		new ((void *)_end++) T(std::forward<Args>(args)...);
	}

	void push(T const &val) { emplace(val); }
	void push(T &&val) { emplace(std::move(val)); }
	void pop() { TL_BOUNDS_CHECK(size()); (_begin++)->~T(); }
	void resize(umm newSize) {
		if (newSize > this->capacity())
			_reallocate(newSize);
		if (newSize > size()) {
			for (T *t = _end; t < _begin + newSize; ++t)
				new (t) T();
			_end = _begin + newSize;
		} else if (newSize < size()) {
			for (T *t = _begin + newSize; t < _end; ++t)
				t->~T();
			_end = _begin + newSize;
		}
	}
	void clear() {
		for (auto &val : *this) {
			val.~T();
		}
		_end = _begin;
	}

	operator Span<T>() { return {begin(), end()}; }
	operator Span<T const>() const { return {begin(), end()}; }

	T *_begin = 0;
	T *_end = 0;
	T *_allocBegin = 0;
	T *_allocEnd = 0;
	void _reallocate(umm newCapacity) {
		umm oldSize = size();
		T *newBegin = ALLOCATE_T(Allocator, T, newCapacity, 0);
		for (T *src = _begin, *dst = newBegin; src != _end; ++src, ++dst) {
			new (dst) T(std::move(*src));
			src->~T();
		}
		if (_begin)
			DEALLOCATE(Allocator, _allocBegin);
		_allocBegin = newBegin;
		_begin = newBegin;
		_end = _begin + oldSize;
		_allocEnd = _begin + newCapacity;
	}
	void _grow(umm requiredSize) {
		umm newCapacity = (umm)(_allocEnd - _allocBegin);
		if (newCapacity == 0)
			newCapacity = 1;
		while (newCapacity < requiredSize) {
			newCapacity *= 2;
		}
		_reallocate(newCapacity);
	}

	void erase(T *val) {
		ASSERT(this->_begin <= val && val < this->_end, "value is not in container");
		val->~T();

		umm leftCount = val - _begin;
		umm rightCount = _end - val - 1;

		if (leftCount <= rightCount) {
			// move left part +1
			for (T *dest = val; dest != this->_begin; --dest) {
				*dest = std::move(dest[-1]);
			}
			(this->_begin++)->~T();
		} else {
			// move right part -1
			--this->_end;
			for (T *dest = val; dest != this->_end; ++dest) {
				*dest = std::move(dest[1]);
			}
			this->_end->~T();
		}
	}
	void erase(T &val) { erase(std::addressof(val)); }
};

template <class T, umm _capacity>
struct CircularBuffer {
	union Storage {
		T value;
		Storage() {}
		~Storage() {}
	};
	struct Iterator {
		using value_type = T;
		CircularBuffer *buffer;
		umm index;
		Iterator(CircularBuffer *buffer, umm index) : buffer(buffer), index(index) {}
		T &operator*() { return buffer->_get(index); }
		T &operator*() const { return buffer->_get(index); }
		Iterator operator+(smm v) const { return Iterator(buffer, index + v); }
		Iterator operator-(smm v) const { return Iterator(buffer, index - v); }
		smm operator-(Iterator v) const { return index - v.index; }
		Iterator &operator++() { ++index; return *this; }
		Iterator &operator--() { --index; return *this; }
		Iterator operator++(int) { Iterator result = *this; ++*this; return result; }
		Iterator operator--(int) { Iterator result = *this; --*this; return result; }
		bool operator>(Iterator const &that) const { return index > that.index; }
		bool operator<(Iterator const &that) const { return index < that.index; }
		bool operator>=(Iterator const &that) const { return index >= that.index; }
		bool operator<=(Iterator const &that) const { return index <= that.index; }
		bool operator==(Iterator const &that) const { return index == that.index; }
		bool operator!=(Iterator const &that) const { return index != that.index; }
	};
	
	CircularBuffer() = default;
	CircularBuffer(CircularBuffer const &that) {
		_size = that._size;
		umm dstIndex = that._begin;
		for (umm srcIndex = 0; srcIndex < _size; ++srcIndex) {
			new (&_storage[srcIndex].value) T(that._storage[dstIndex].value);
			if (++dstIndex == _capacity)
				dstIndex = 0;
		}
	}
	CircularBuffer(CircularBuffer &&that) {
		_size = that._size;
		umm dstIndex = that._begin;
		for (umm srcIndex = 0; srcIndex < _size; ++srcIndex) {
			new (&_storage[srcIndex].value) T(std::move(that._storage[dstIndex].value));
			if (++dstIndex == _capacity)
				dstIndex = 0;
		}
		that._begin = 0;
		that._size = 0;
	}
	CircularBuffer &operator=(CircularBuffer const &that) { clear(); return *new (this) CircularBuffer(that); }
	CircularBuffer &operator=(CircularBuffer &&that) { clear(); return *new (this) CircularBuffer(std::move(that)); }
	~CircularBuffer() {
		for (umm i = 0; i < _size; ++i) {
			_get(i).~T();
		}
		_begin = 0;
		_size = 0;
	}
	template <class ...Args>
	void emplace_back(Args &&...args) {
		TL_BOUNDS_CHECK(_size != _capacity);
		new (&_storage[(_begin + _size++) % _capacity].value) T(std::forward<Args>(args)...);
	}
	template <class ...Args>
	void emplace_front(Args &&...args) {
		TL_BOUNDS_CHECK(_size != _capacity);
		++_size;
		if (_begin) --_begin; else _begin = _capacity - 1;
		new (&_storage[_begin].value) T(std::forward<Args>(args)...);
	}
	void push_back(T const &v) { emplace_back(v); }
	void push_back(T &&v) { emplace_back(std::move(v)); }
	void push_front(T const &v) { emplace_front(v); }
	void push_front(T &&v) { emplace_front(std::move(v)); }

	void pop_back() { _get(--_size).~T(); }
	void pop_front() { _storage[_begin].value.~T(); _incBegin(); --_size; }
	
	Iterator begin() { return Iterator(this, 0); }
	Iterator end() { return Iterator(this, _size); }

	T &back() { TL_BOUNDS_CHECK(_size); return _get(_size - 1); }
	T &front() { TL_BOUNDS_CHECK(_size); return _get(0); }

	umm size() const { return _size; }
	bool empty() const { return _size == 0; }

	T &operator[](umm i) { TL_BOUNDS_CHECK(i < _size); return _get(i); }
	T const &operator[](umm i) const { TL_BOUNDS_CHECK(i < _size); return _get(i); }

	void erase(Iterator it) {
		umm index = it.index;
		TL_BOUNDS_CHECK(index < _size);
		--_size;
		if (index > _size / 2) {
			for (;index < _size; ++index) {
				_get(index) = std::move(_get(index + 1));
			}
			_get(_size).~T();
		} else {
			for (;index; --index) {
				_get(index) = std::move(_get(index - 1));
			}
			_get(0).~T();
			_incBegin();
		}
	}

	void clear() {
		for (umm i = 0; i < size(); ++i) {
			_get(i).~T();
		}
		_begin = 0;
		_size = 0;
	}

	T &_get(umm i) {
		return _storage[(_begin + i) % _capacity].value;
	}
	void _incBegin() {
		++_begin;
		if (_begin == _capacity) 
			_begin = 0;
	}

	Storage _storage[_capacity];
	umm _begin = 0;
	umm _size = 0;
};

template <class T, umm _capacity>
struct CircularQueue : private CircularBuffer<T, _capacity> {
	using Base = CircularBuffer<T, _capacity>;
	using Base::begin;
	using Base::end;
	using Base::size;
	using Base::empty;
	using Base::erase;
	using Base::front;
	using Base::clear;
	using Base::operator[];
	void push(T const &v) { this->push_back(v); }
	void push(T &&v) { this->push_back(std::move(v)); }
	void pop() { this->pop_front(); }
};

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR, umm blockSize_ = 4096 / sizeof(T)>
struct BlockList {
	static constexpr umm blockSize = blockSize_;
	union Storage {
		T value;
	};
	struct Block {
		Storage buffer[blockSize];
		Storage *end = buffer;
		Block *next = 0;
		Block *previous = 0;

		Block() {}
		umm size() const { return (umm)(end - buffer); }
		umm availableSpace() const { return blockSize - size(); }
	};
	Block first;
	Block *last = &first;
	Block *allocLast = &first;

	BlockList() = default;
	BlockList(BlockList const &that) {
		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(that.first.buffer[i].value);
		}
		
		auto previous = &first;
		for (auto srcBlock = that.first.next; srcBlock != 0;) {
			if (srcBlock->size()) {
				last = allocLast = allocateBlock(previous);
				for (u32 i = 0; i < srcBlock->size(); ++i) {
					new (&last->buffer[i].value) T(srcBlock->buffer[i].value);
				}
				previous = last;
			}
		}
	}
	BlockList(BlockList &&that) {
		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(std::move(that.first.buffer[i].value));
		}

		first.next = that.first.next;
		last = that.last;
		allocLast = that.allocLast;

		that.first.end = that.first.buffer;
		that.last = that.allocLast = &that.first;
	}
	BlockList &operator=(BlockList const &that) {
		clear();
		
		first.end = first.buffer + that.first.size();
		for (u32 i = 0; i < that.first.size(); ++i) {
			new (&first.buffer[i].value) T(that.first.buffer[i].value);
		}
		
		last = &first;
		for (auto srcBlock = that.first.next; srcBlock != 0;) {
			if (srcBlock->size()) {
				if (!last->next)
					last->next = allocLast = allocateBlock(last);
				last = last->next;
				for (u32 i = 0; i < srcBlock->size(); ++i) {
					new (&last->buffer[i].value) T(srcBlock->buffer[i].value);
				}
			}
		}

		return *this;
	}
	BlockList &operator=(BlockList &&that) {
		clear();
		return *new (this) BlockList(std::move(that));
	}
	~BlockList() {
		for (u32 i = 0; i < first.size(); ++i) {
			first.buffer[i].value.~T();
		}
		first.end = first.buffer;

		for (auto block = first.next; block != 0;) {
			for (u32 i = 0; i < block->size(); ++i) {
				block->buffer[i].value.~T();
			}
			auto next = block->next;
			DEALLOCATE(Allocator, block);
			block = next;
		}

		last = &first;
		allocLast = &first;
	}

	void clear() {
		for (u32 i = 0; i < first.size(); ++i) {
			first.buffer[i].value.~T();
		}
		first.end = first.buffer;

		for (auto block = first.next; block != 0; block = block->next) {
			for (u32 i = 0; i < block->size(); ++i) {
				block->buffer[i].value.~T();
			}
			block->end = block->buffer;
		}
		last = &first;
	}
	umm availableSpace() {
		umm space = 0;
		auto block = last;
		while (block) {
			space += block->availableSpace();
			block = block->next;
		}
		return space;
	}
	void ensureSpace(umm amount) {
		umm space = availableSpace();
		while (space < amount) {
			allocLast = allocLast->next = allocateBlock(allocLast);
			space += blockSize;
		}
	}
	void ensureConsecutiveSpace(umm amount) {
		ASSERT(amount <= blockSize, "reserving this amount of consecutive space is impossible");
		if (last->availableSpace() < amount) {
			if (last->next) {
				last = last->next;
			} else {
				last->next = allocateBlock(last);
				last = allocLast = last->next;
			}
		}
	}
	void push_back(T const &value) {
		auto dstBlock = last;
		while (dstBlock && (dstBlock->availableSpace() == 0)) {
			dstBlock = dstBlock->next;
		}

		if (!dstBlock) {
			dstBlock = last = allocLast = allocateBlock(allocLast);
		}
		new (dstBlock->end++) T(value);
	}
	umm size() const {
		umm totalSize = 0;
		for (auto block = &first; block != 0; block = block->next) {
			totalSize += block->size();
		}
		return totalSize;
	}
	bool empty() const { return (last == &first) && (first.size() == 0); }

	T &back() { return last->end[-1].value; }
	T const &back() const { return last->end[-1].value; }

	void pop_back() {
		if (last == &first)
			TL_BOUNDS_CHECK(first.size());

		(last->end--)[-1].value.~T();
		if (last->end == last->buffer && last != &first) {
			last = last->previous;
		}
	}

	template <class Fn>
	void stream(Fn &&fn) const {
		auto block = &first;
		do {
			auto blockSize = block->size();
			if (blockSize) {
				fn(block->buffer, blockSize);
			}
			block = block->next;
		} while (block);
	}

private:
	Block *allocateBlock(Block *previous) {
		Block *result = construct(ALLOCATE_T(Allocator, Block, 1, 0));
		result->previous = previous;
		return result;
	}
};

template <class T, class Allocator = TL_DEFAULT_ALLOCATOR>
struct LinkedList {
	using ValueType = T;

	struct Node {
		T value;
		Node *next;
	};
	struct Iterator {
		Node *node;

		Iterator() : node() {}
		Iterator(Node *node) : node(node) {}
		T &operator*() { return node->value; }
		Iterator &operator++() {
			node = node->next;
			return *this;
		}
		Iterator operator++(int) {
			Node *prev = node;
			node = node->next;
			return prev;
		}
		bool operator==(Iterator const &that) const { return node == that.node; }
		bool operator!=(Iterator const &that) const { return node != that.node; }
		T *operator->() { return std::addressof(node->value); }
	};

	LinkedList() = default;
	~LinkedList() { clear(); }

	void clear() {
		Node *node = head;
		for (;;) {
			if (node == 0)
				break;
			DEFER { DEALLOCATE(Allocator, node); };
			node = node->next;
		}
		head = last = 0;
	}

	template <class ...Args>
	void emplace_back(Args &&...args) {
		if (head == 0) {
			head = last = ALLOCATE_T(Allocator, Node, 1, 0);
		} else {
			last->next = ALLOCATE_T(Allocator, Node, 1, 0);
			last = last->next;
		}
		last->next = 0;

		new (&last->value) T(std::forward<Args>(args)...);
	}
	void push_back(T const &val) { emplace_back(val); }
	void push_back(T &&val) { emplace_back(std::move(val)); }

	template <class ...Args>
	void emplace_front(Args &&...args) {
		if (head == 0) {
			head = last = ALLOCATE_T(Allocator, Node, 1, 0);
			head->next = 0;
		} else {
			auto prevHead = head;
			head = ALLOCATE_T(Allocator, Node, 1, 0);
			head->next = prevHead;
		}
		new (&head->value) T(std::forward<Args>(args)...);
	}
	void push_front(T const &val) { emplace_front(val); }
	void push_front(T &&val) { emplace_front(std::move(val)); }

	T &back() { return last->value; }
	T const &back() const { return last->value; }

	Iterator begin() { return head; }
	Iterator end() { return {}; }

private:
	Node *head = 0;
	Node *last = 0;
};

template <class T>
void erase(UnorderedList<T> &list, T &val) {
	list.erase(val);
}
template <class T>
void erase(UnorderedList<T> &list, T *val) {
	list.erase(val);
}

template <class T>
umm indexof(List<T> const &list, T const *value) {
	return value - list._begin;
}

template <class T>
umm indexof(UnorderedList<T> const &list, T const *value) {
	return value - list._begin;
}

template <class T, umm capacity>
umm indexof(StaticList<T, capacity> const &list, T const *value) {
	return value - list._begin;
}

template <class T> umm countof(List<T> const &list) { return list.size(); }
template <class T> umm countof(UnorderedList<T> const &list) { return list.size(); }

template <class T> T const &front(List<T> const &list) { return list.front(); }
template <class T> T &front(List<T> &list) { return list.front(); }

template <class T> T const &back(List<T> const &list) { return list.back(); }
template <class T> T &back(List<T> &list) { return list.back(); }

#pragma warning(pop)

}
