#pragma once

#include "StorageRegistryTemplate.h"
#include <Lucy/Material.h>

namespace lucy {
	template <typename T>
	struct StorageRegistry: public StorageRegistryTemplate<T> {
		LUCY_UUID Insert(std::string name, const T& data, LUCY_UUID id = LUCY_GENERATE_UUID) override  {
			assert(this->storage_hashmap.find(id) == this->storage_hashmap.end());

			this->storage_hashmap[id] = { name, data };

			return id;
		}

		template <typename ...Targs>
		std::pair<LUCY_UUID, T&> Insert(std::string name, Targs ...arg) {
			auto id = Insert(name, T{arg...});
			return { id, Get(id) };
		}

		T& Get(LUCY_UUID id) override {
			assert(this->storage_hashmap.find(id) != this->storage_hashmap.end());

			return this->storage_hashmap[id].store_data;
		}

		T& Get(std::string name) override {
			auto id = GetID(name);

			assert(id != LUCY_NULL_UUID);

			return this->storage_hashmap[id].store_data;
		}

		void Remove(LUCY_UUID id) override {

		}
		void Remove(std::string name) override {

		}

		std::string GetName(LUCY_UUID id) override {
			return "";
		}


		LUCY_UUID GetID(std::string name) override {
			for (auto& pair: this->storage_hashmap) {
				if (pair.second.name == name) {
					return pair.first;
				}
			}

			return LUCY_NULL_UUID;
		}

		bool IsIDPresent(LUCY_UUID id) override {
			return false;
		}
		bool IsNamePresent(std::string id) override {
			return false;
		}
	};
}
