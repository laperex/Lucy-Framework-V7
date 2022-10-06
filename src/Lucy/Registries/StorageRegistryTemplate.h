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

		std::unordered_map<LUCY_UUID, Container> storage_hashmap;

	public:
		virtual LUCY_UUID Insert(std::string name, const T& data, LUCY_UUID id = LUCY_GENERATE_UUID) = 0;

		virtual T& Get(LUCY_UUID id) = 0;
		virtual T& Get(std::string name) = 0;

		virtual void Remove(LUCY_UUID id) = 0;
		virtual void Remove(std::string name) = 0;

		virtual std::string GetName(LUCY_UUID id) = 0;
		virtual LUCY_UUID GetID(std::string name) = 0;

		virtual bool IsIDPresent(LUCY_UUID id) = 0;
		virtual bool IsNamePresent(std::string id) = 0;
	};
}
