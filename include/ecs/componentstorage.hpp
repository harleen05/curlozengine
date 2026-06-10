#pragma once

#include "entity.hpp"
#include <vector>

namespace clz::ecs
{
	struct IComponentStorage
	{
		virtual ~IComponentStorage() = default;
		virtual void removeEntityData(entity e) = 0;
	};

	template<typename T>
	struct ComponentStorage : IComponentStorage
	{
		std::vector<T> storage;
		std::vector<entity> dense;
		std::vector<uint32_t> sparse;

		ComponentStorage()
		{
			sparse.resize(MAX_ENTITIES, NULL_ENTITY);
		}

		void removeEntityData(const entity e) override
		{
			if (sparse[e] == NULL_ENTITY)
				return;

			uint32_t index = sparse[e];
			uint32_t last =  storage.size() - 1;

			storage[index] = storage[last];
			dense[index] = dense[last];
			sparse[dense[index]] = index;

			dense.pop_back();
			storage.pop_back();
			sparse[e] = NULL_ENTITY;

		}
	};
}