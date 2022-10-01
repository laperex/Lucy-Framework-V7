#pragma once

#include "StorageRegistry.h"
#include <Lucy/Material.h>
#include <Lucy/Light.h>

namespace lucy {
	typedef StorageRegistry<Material> MaterialRegistry;
	typedef StorageRegistry<Light> LightRegistry;
}