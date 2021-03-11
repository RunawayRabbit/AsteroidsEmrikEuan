#pragma once

#include <stdint.h>
#include <string>
#include <xhash>

class Entity
{
public:
	inline bool operator==(const Entity& other) const
	{
		return id == other.id;
	}

	inline bool operator!=(const Entity& other) const
	{
		return id != other.id;
	}

	inline size_t hash() const {
		return std::hash<uint32_t>{}(id);
	}

	static const inline Entity null()
	{
		//return Entity();

		Entity retval;
		retval.id = 0;
		return retval;
	}

	const std::string ToString() const
	{
		return std::to_string(id);
	}

	bool operator<(const Entity& other) const
	{
		return (this->id < other.id);
	}

private:
	uint32_t id;
	Entity& operator++()
	{
		++id;
		return *this;
	}

	Entity operator++(int)
	{
		Entity copy = *this;
		++id;
		return copy;
	}
	friend class EntityManager;
};

namespace std {
	template<>
	struct hash<Entity> {
		inline size_t operator()(const Entity& entity) const {
			return entity.hash();
		}
	};
}