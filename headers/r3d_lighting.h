/* r3d_lighting.h -- R3D Lighting Module.
 *
 * Copyright (c) 2025-2026 Le Juez Victor
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * For conditions of distribution and use, see accompanying LICENSE file.
 */

#ifndef R3D_LIGHTING_H
#define R3D_LIGHTING_H

#include "./r3d_platform.h"
#include "./r3d_camera.h"
#include <raylib.h>
#include <stdint.h>

/**
 * @defgroup Lighting
 * @{
 */

// ========================================
// ENUMS TYPES
// ========================================

/**
 * @brief Types of lights supported by the rendering engine.
 *
 * Each light type has different behaviors and use cases.
 */
typedef enum R3D_LightType {
    R3D_LIGHT_DIR,                      ///< Directional light, affects the entire scene with parallel rays.
    R3D_LIGHT_SPOT,                     ///< Spot light, emits light in a cone shape.
    R3D_LIGHT_OMNI,                     ///< Omni light, emits light in all directions from a single point.
    R3D_LIGHT_TYPE_COUNT
} R3D_LightType;

// ========================================
// STRUCTS TYPES
// ========================================

/**
 * @brief Describes a light source.
 */
typedef struct R3D_Light {
    Vector3 position;       ///< Light position (spot/omni)
    Vector3 direction;      ///< Light direction (spot/dir)
    Color color;            ///< Light color
    float energy;           ///< Light intensity/brightness multiplier
    float specular;         ///< Specular reflection intensity multiplier
    float range;            ///< Maximum distance (spot/omni)
    float falloff;          ///< Distance falloff factor (spot/omni)
    float innerCutOff;      ///< Spot light inner cutoff angle (degrees)
    float outerCutOff;      ///< Spot light outer cutoff angle (degrees)
    float fogEnergy;        ///< Volumetric fog energy multiplier
    R3D_LightType type;     ///< Light type (directional/spot/omni)
} R3D_Light;

/**
 * @brief Represents an allocated shadow map for a light.
 */
typedef struct R3D_ShadowMap {
    uint32_t handle;        ///< Internal shadow map handle (don't touch)
    float softness;         ///< Softness factor for penumbra
    float opacity;          ///< Shadow opacity factor
    float depthBias;        ///< Constant depth bias
    float slopeBias;        ///< Slope-scaled depth bias
    R3D_Layer cullMask;     ///< Layers considered when culling shadow casters for this map
    R3D_LightType type;     ///< Light type this shadow map was allocated for
} R3D_ShadowMap;

// ========================================
// PUBLIC API
// ========================================

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------
// LIGHTING: Lights Functions
// ----------------------------------------

/**
 * @brief Creates a directional light.
 */
R3DAPI R3D_Light R3D_CreateDirLight(Vector3 dir, Color color, float energy);

/**
 * @brief Creates a spot light, with default inner/outer cutoff angles of 22.5/45 degrees.
 */
R3DAPI R3D_Light R3D_CreateSpotLight(Vector3 pos, Vector3 dir, float range, Color color, float energy);

/**
 * @brief Creates an omnidirectional (point) light.
 */
R3DAPI R3D_Light R3D_CreateOmniLight(Vector3 pos, float range, Color color, float energy);

// ----------------------------------------
// LIGHTING: Shadow Functions
// ----------------------------------------

/**
 * @brief Allocates a shadow map layer for a given light type.
 *
 * The shadow map resolution is fixed per light type and configured via
 * R3D_HINT_SHADOW_DIR_SIZE, R3D_HINT_SHADOW_SPOT_SIZE and R3D_HINT_SHADOW_OMNI_SIZE
 * before R3D is initialized.
 *
 * @param type The light type this shadow map will be used with (must match
 *             the type of the light it is later passed to via R3D_PushLight()).
 */
R3DAPI R3D_ShadowMap R3D_LoadShadowMap(R3D_LightType type);

/**
 * @brief Releases a shadow map, freeing its layer for reuse.
 */
R3DAPI void R3D_UnloadShadowMap(R3D_ShadowMap shadowMap);

/**
 * @brief Returns whether the shadow map has a valid allocated layer.
 */
R3DAPI bool R3D_IsShadowMapValid(R3D_ShadowMap shadowMap);

// ----------------------------------------
// LIGHTING: Light Helper Functions
// ----------------------------------------

/**
 * @brief Draws the area of influence of the light in 3D space.
 *
 * This function visualizes the area affected by a light in 3D space.
 * It draws the light's influence, such as the cone for spotlights or the volume for omni-lights.
 * This function is only relevant for spotlights and omni-lights.
 * 
 * @note This function should be called while using the default 3D rendering mode of raylib, 
 *       not with r3d's rendering mode. It uses raylib's 3D drawing functions to render the light's shape.
 *
 * @param light The light to visualize (see R3D_Light).
 */
R3DAPI void R3D_DrawLightDebug(R3D_Light light);

#ifdef __cplusplus
} // extern "C"
#endif

/** @} */ // end of Lighting

#endif // R3D_LIGHTING_H
