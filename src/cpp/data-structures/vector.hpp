#ifndef CS_DATA_STRUCTURES_VECTOR_HPP_
#define CS_DATA_STRUCTURES_VECTOR_HPP_
#include <exception>
#include <limits>
#include <utility>


template<class T>
class Vector
{
public:
	Vector() : 
		m_data(nullptr),
		m_size(0),
		m_cap(0)
	{
	
	}


	Vector(Vector&& rhs)
		: m_data(rhs.m_data),
		m_size(rhs.m_size),
		m_cap(rhs.m_cap)
	{
		rhs.m_data = nullptr;
		rhs.m_size = 0;
		rhs.m_cap = 0;
	}


	~Vector()
	{
		delete[] m_data;
	}


	Vector& operator=(Vector&& rhs)
	{
		if (&rhs != this) {
			std::swap(m_data, rhs.m_data);
			std::swap(m_size, rhs.m_size);
			std::swap(m_cap, rhs.m_cap);
		}

		return *this;
	}


	const T& operator[](const int idx) const
	{
		return m_data[idx];
	}


	T& operator[](const int idx)
	{
		return const_cast<T&>(static_cast<const Vector<T>&>(*this)[idx]);
	}

	
	int size() const
	{
		return m_size;
	}


	int capacity() const
	{
		return m_cap;
	}

	const T* cbegin() const
	{
		return m_data;
	}

	const T* cend() const
	{
		return m_data + size();
	}

	const T* begin() const
	{
		return cbegin();
	}

	const T* end() const
	{
		return cend();
	}


	T* begin()
	{
		return const_cast<T*>(static_cast<const Vector<T>&>(*this).begin());
	}


	T* end()
	{
		return const_cast<T*>(static_cast<const Vector<T>&>(*this).end());
	}


	void resize(const unsigned long long need)
	{
		if (need >= std::numeric_limits<int>::max())
			throw std::runtime_error("Can't resize vector");

		T* newbuf = new T[need];

		try {
			const int limit = static_cast<int>(need) > size()
			                  ? size()
			                  : static_cast<int>(need);

			for (int i = 0; i < limit; ++i)
				newbuf[i] = m_data[i];

		} catch (...) {
			delete[] newbuf;
			throw;
		}

		delete[] m_data;
		m_data = newbuf;
		m_cap = static_cast<int>(need);

		if (m_cap < m_size)
			m_size = m_cap;
	}

	
	void reserve(const unsigned long long need)
	{
		if (need > static_cast<unsigned long long>(capacity()))
			resize(need + (size() / 2));
	}


	void push_back(T elem)
	{
		ensure_capacity(size() + 1);
		m_data[m_size++] = std::move(elem);
	}


	T pop_back()
	{
		return std::move(m_data[--m_size]);
	}


	T& back()
	{
		return m_data[size() - 1];
	}


private:
	void ensure_capacity(const unsigned long long need)
	{
		if (need > static_cast<unsigned long long>(capacity()))
			reserve(need + (need / 2));
	}


private:
	T* m_data;
	int m_size;
	int m_cap;
};

#endif

