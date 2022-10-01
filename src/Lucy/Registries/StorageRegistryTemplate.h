#pragma once

#include <LucyUtil/LucyUtil.h>
#include <unordered_map>

namespace lucy {
	template <typename T>
	struct StorageRegistryTemplate {
	public:
		struct Container {
			std::string name;
			T store_data;
		};

		std::unordered_map<UTIL_UUID, Container> storage_hashmap;

	public:
		virtual UTIL_UUID Insert(std::string name, const T& data, UTIL_UUID id = UTIL_GENERATE_UUID) = 0;

		virtual T& Get(UTIL_UUID id) = 0;
		virtual T& Get(std::string name) = 0;

		virtual void Remove(UTIL_UUID id) = 0;
		virtual void Remove(std::string name) = 0;

		virtual std::string GetName(UTIL_UUID id) = 0;
		virtual UTIL_UUID GetID(std::string name) = 0;

		virtual bool IsIDPresent(UTIL_UUID id) = 0;
		virtual bool IsNamePresent(std::string id) = 0;
	};
}
