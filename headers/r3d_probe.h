/* r3d_probe.h -- R3D Probe Module.
 *
 * Copyright (c) 2025-2026 Le Juez Victor
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * For conditions of distribution and use, see accompanying LICENSE file.
 */

#ifndef R3D_PROBE_H
#define R3D_PROBE_H

#include "./r3d_platform.h"
#include <raylib.h>
#include <stdint.h>

/**
 * @defgroup Probe
 * @{
 */

// ========================================
// ENUM TYPES
// ========================================

/**
 * @brief Type of data captured by a probe.
 */
typedef enum R3D_ProbeType {
    R3D_PROBE_ILLUMINATION,     ///< Captures indirect diffuse lighting.
    R3D_PROBE_REFLECTION,       ///< Captures environment reflections.
} R3D_ProbeType;

// ========================================
// STRUCT TYPES
// ========================================

/**
 * @brief Describes a probe used for indirect lighting or reflections.
 */
typedef struct R3D_Probe {
    R3D_ProbeType type;     ///< Type of data captured by the probe
    uint32_t handle;        ///< Internal probe handle (don't touch)
    Vector3 position;       ///< World-space probe position
    float falloff;          ///< Distance falloff factor
    float range;            ///< Maximum influence distance
    bool interior;          ///< Whether the probe is captured as an interior
    bool shadows;           ///< Whether shadows are included in the capture
} R3D_Probe;

// ========================================
// PUBLIC API
// ========================================

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocates a probe of the given type.
 *
 * @param type Whether this probe contributes indirect lighting or reflections.
 * @param interior Whether the skybox is taken into account when capturing this probe.
 * @param shadow Whether shadow casters are taken into account when capturing this probe.
 */
R3DAPI R3D_Probe R3D_LoadProbe(R3D_ProbeType type, bool interior, bool shadow);

/**
 * @brief Releases a probe, freeing its layer for reuse.
 */
R3DAPI void R3D_UnloadProbe(R3D_Probe probe);

/**
 * @brief Returns whether the probe has a valid allocated layer.
 */
R3DAPI bool R3D_IsProbeValid(R3D_Probe probe);

#ifdef __cplusplus
} // extern "C"
#endif

/** @} */ // end of Probe

#endif // R3D_PROBE_H
