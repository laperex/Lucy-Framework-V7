#pragma once

#include <stdint.h>
#ifndef UUID_SYSTEM_GENERATOR
	#define UUID_SYSTEM_GENERATOR
#endif
#include <stduuid/uuid.h>

#define LUCY_UUID uint64_t
#define LUCY_NULL_UUID ((LUCY_UUID)0)
#define LUCY_GENERATE_UUID (*(LUCY_UUID*)uuids::uuid_system_generator{}().as_bytes().data())