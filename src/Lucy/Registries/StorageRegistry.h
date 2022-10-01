#pragma once

#include "StorageRegistryTemplate.h"
#include <Lucy/Material.h>

namespace lucy {
	template <typename T>
	struct StorageRegistry: public StorageRegistryTemplate<T> {
		UTIL_UUID Insert(std::string name, const T& data, UTIL_UUID id = UTIL_GENERATE_UUID) override  {
			assert(this->data_store.find(id) == this->data_store.end());

			this->data_store[id] = { name, data };

			return id;
		}

		T& Get(UTIL_UUID id) override {
			assert(this->data_store.find(id) != this->data_store.end());

			return this->data_store[id].store;
		}

		T& Get(std::string name) override {
			return this->data_store[GetID(name)].store;
		}

		void Remove(UTIL_UUID id) override;
		void Remove(std::string name) override;

		std::string GetName(UTIL_UUID id) override;
		UTIL_UUID GetID(std::string name) override;

		bool IsIDPresent(UTIL_UUID id) override;
		bool IsNamePresent(std::string id) override;
	};
}
