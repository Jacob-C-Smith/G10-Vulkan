#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXTransform.h>
#include <G10/GXRigidbody.h>

struct GXEntity_s
{
	char *name;
	GXTransform_t* transform;
	GXRigidbody_t* rigidbody;
};

DLLEXPORT int load_entity ( GXEntity_t** entity, char* path);

DLLEXPORT int load_entity_as_json ( GXEntity_t** entity, char* token_text);

DLLEXPORT int move_entity ( GXEntity_t* entity );