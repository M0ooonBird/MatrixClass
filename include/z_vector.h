#pragma once
#include "ZMathHeader.h"
#include <iostream>

namespace ZMath 
{
	
template<typename T>
class ZMATH_API vector
{
public:
	// 类型别名
	using value_type = T;
	using iterator = T*;
	using const_iterator = const T*;
	using size_type = size_t;

	// 构造函数
	vector() : _data(nullptr), _size(0), _capacity(0) {}

	~vector() {
		clear();
		::operator delete[](_data);//释放内存，需手动析构
	}

	explicit vector(size_type num, const T& value = T())
		: _size(num), _capacity(num) 
	{
		//分配内存，需手动调用T的构造函数，适用于更复杂的类型T
		//_data = new T[num]; 若用这个只会调用T类型的默认构造函数，适合简单类型
		_data = static_cast<T*>(::operator new[](num * sizeof(T)));
		
		for (size_type i = 0; i < num; ++i) {
			//_data[i] = value;
			//  ​​placement new​​ 语法，用于在指定内存地址构造对象。
			// 使用 T(value)构造函数
			new (&_data[i]) T(value);  
		}
	}

	vector(const vector& other)
		: _data(static_cast<T*>(::operator new[](other._capacity * sizeof(T)))),
		_size(other._size),
		_capacity(other._capacity) {
		for (size_type i = 0; i < _size; ++i) {
			new (&_data[i]) T(other._data[i]);  // 深拷贝
		}
	}

	vector(vector&& other) noexcept
		: _data(other._data),
		_size(other._size),
		_capacity(other._capacity) {
		other._data = nullptr;
		other._size = 0;
		other._capacity = 0;
	}

	// 赋值函数
	vector& operator=(const vector& other) {
		if (this != &other) {
			vector temp(other);  // 拷贝构造临时对象
			this->swap(temp);
		}
		return *this;
	}

	vector& operator=(vector&& other) noexcept {
		if (this != &other) {
			clear();
			::operator delete[](_data);
			_data = other._data;
			_size = other._size;
			_capacity = other._capacity;
			other._data = nullptr;
			other._size = 0;
			other._capacity = 0;
		}
		return *this;
	}

	// 元素访问
	T& operator[](size_type pos) {
		return _data[pos];
	}
	const T& operator[](size_type pos) const {
		return _data[pos];
	}

	// 容量相关
	bool empty() const noexcept {
		return _size == 0;
	}
	size_type size() const noexcept {
		return _size;
	}
	size_type capacity() const noexcept {
		return _capacity;
	}

	void reserve(size_type new_cap) {
		if (new_cap > _capacity) {
			reallocate(new_cap);
		}
	}
	void shrink_to_fit() {
		if (_capacity > _size) {
			reallocate(_size);
		}
	}

	// 修改器
	void clear() noexcept {
		for (size_type i = 0; i < _size; ++i) {
			_data[i].~T();  // 显式调用析构
		}
		_size = 0;
	}

	void push_back(const T& value) {
		if (_size >= _capacity) {
			reallocate(_capacity * 2 + 1);
		}
		new (&_data[_size++]) T(value);  // Placement new
	}

	void push_back(T&& value) {
		if (_size >= _capacity) {
			reallocate(_capacity * 2 + 1);
		}
		new (&_data[_size++]) T(std::move(value));  // 移动语义
	}

	void pop_back() {
		if (_size > 0) {
			_data[--_size].~T();
		}
	}

	// 迭代器
	iterator begin() noexcept {
		return _data;
	}
	const_iterator begin() const noexcept {
		return _data;
	}
	iterator end() noexcept {
		return _data + _size;
	}
	const_iterator end() const noexcept {
		return _data + _size;
	}

	T* data() noexcept { 
		return _data; 
	}
	const T* data() const noexcept {
		return _data; 
	}
private:

	T* _data;
	size_type _size;
	size_type _capacity;

	void reallocate(size_type new_cap) {
		// 分配新内存
		T* new_data = static_cast<T*>(::operator new[](new_cap * sizeof(T)));

		// 移动或拷贝元素
		size_type new_size = (_size < new_cap) ? _size : new_cap;

		for (size_type i = 0; i < new_size; ++i) {
			new (&new_data[i]) T(std::move(_data[i]));  // 移动构造
			_data[i].~T();  // 原对象析构
		}

		// 释放旧内存
		::operator delete[](_data);

		// 更新指针和容量
		_data = new_data;
		_capacity = new_cap;
		_size = new_size;
	}

	void swap(vector& other) noexcept {
		std::swap(_data, other._data);
		std::swap(_size, other._size);
		std::swap(_capacity, other._capacity);
	}

};// class vector
	
}// namespace ZMath

