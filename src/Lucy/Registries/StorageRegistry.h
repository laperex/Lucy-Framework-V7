#pragma once

#include "StorageRegistryTemplate.h"
#include <Lucy/Material.h>

namespace lucy {
	template <typename T>
	struct StorageRegistry: public StorageRegistryTemplate<T> {
		UTIL_UUID Insert(std::string name, const T& data, UTIL_UUID id = UTIL_GENERATE_UUID) override  {
			assert(this->storage_hashmap.find(id) == this->storage_hashmap.end());

			this->storage_hashmap[id] = { name, data };

			return id;
		}

		template <typename ...Targs>
		std::pair<UTIL_UUID, T&> Insert(std::string name, Targs ...arg) {
			auto id = Insert(name, T{arg...});
			return { id, Get(id) };
		}

		T& Get(UTIL_UUID id) override {
			assert(this->storage_hashmap.find(id) != this->storage_hashmap.end());

			return this->storage_hashmap[id].store_data;
		}

		T& Get(std::string name) override {
			auto id = GetID(name);

			assert(id != UTIL_NULL_UUID);

			return this->storage_hashmap[id].store_data;
		}

		void Remove(UTIL_UUID id) override {

		}
		void Remove(std::string name) override {

		}

		std::string GetName(UTIL_UUID id) override {
			return "";
		}


		UTIL_UUID GetID(std::string name) override {
			for (auto& pair: this->storage_hashmap) {
				if (pair.second.name == name) {
					return pair.first;
				}
			}

			return UTIL_NULL_UUID;
		}

		bool IsIDPresent(UTIL_UUID id) override {
			return false;
		}
		bool IsNamePresent(std::string id) override {
			return false;
		}
	};
}
