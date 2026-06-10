#pragma once

#include "componentstorage.hpp"
#include "entity.hpp"

namespace clz::ecs
{
        inline std::vector<IComponentStorage*> ecs_componentStorages;
        inline uint32_t ecs_componentCounter = 0;
}
namespace clz::ecs
{
        template<typename T>
        uint32_t componentID()
        {
                static uint32_t id = ecs_componentCounter++;
                return id;
        }

        template<typename T>
        ComponentStorage<T>& getStorage()
        {
                auto id = componentID<T>();
                if (id >= ecs_componentStorages.size())
                {
                        ecs_componentStorages.resize(id + 1, nullptr);
                        ecs_componentStorages[id] = new ComponentStorage<T>;
                        return static_cast<ComponentStorage<T>&>(*ecs_componentStorages[id]);
                }
                return static_cast<ComponentStorage<T>&>(*ecs_componentStorages[id]);
        }
	template<typename T>
	std::vector<T>& getComponentArray()
        {
	        auto& componentStorage = getStorage<T>();
        	return componentStorage.storage;
        }

        inline void deleteAllComponents()
        {
                for (const auto& storage : ecs_componentStorages)
                {
                        delete storage;
                }
                ecs_componentStorages.clear();
        }

        template<typename T>
        void addComponent(const entity e, const T component)
        {
                auto& componentStorage = getStorage<T>();
                uint32_t index = componentStorage.storage.size();
                componentStorage.storage.emplace_back(component);
                componentStorage.dense.emplace_back(e);
                componentStorage.sparse[e] = index;
        }
        template<typename T>
        bool hasComponent(const entity e)
        {
                auto& componentStorage = getStorage<T>();
                return componentStorage.sparse[e] != NULL_ENTITY;
        }

        template<typename T>
        void removeComponent(const entity e)
        {
                auto& componentStorage = getStorage<T>();
                if (componentStorage.sparse[e] == NULL_ENTITY)
                        return;

                uint32_t index = componentStorage.sparse[e];
                uint32_t last = componentStorage.storage.size() - 1;

                componentStorage.storage[index] = componentStorage.storage[last];
                componentStorage.dense[index] = componentStorage.dense[last];
                componentStorage.sparse[componentStorage.dense[index]] = index;

                componentStorage.dense.pop_back();
                componentStorage.storage.pop_back();
                componentStorage.sparse[e] = NULL_ENTITY;

        }
        inline void removeAllComponentsForEntity(const entity e)
        {
                for (const auto& storage : ecs_componentStorages)
                {
                        storage->removeEntityData(e);
                }
        }

        template<typename T>
        T& getComponent(entity e)
        {
                auto& componentStorage = getStorage<T>();
                return componentStorage.storage[componentStorage.sparse[e]];
        }

        template<typename... Components>
        std::tuple<Components&...> getComponents(entity e)
        {
                return std::tie(getComponent<Components>(e)...);
        }
}