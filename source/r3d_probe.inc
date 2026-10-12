// r3d_probe.inc -- R3D Probe Module.

// ========================================
// ENUM TYPES
// ========================================

{*
 * @brief Type of data captured by a probe.
 *
 * - R3D_PROBE_ILLUMINATION -> captures indirect diffuse lighting.
 * - R3D_PROBE_REFLECTION   -> captures environment reflections.
 *}
type
  TR3D_ProbeType = (
    R3D_PROBE_ILLUMINATION,     ///< Captures indirect diffuse lighting.
    R3D_PROBE_REFLECTION        ///< Captures environment reflections.
  );

// ========================================
// STRUCT TYPES
// ========================================

{*
 * @brief Describes a probe used for indirect lighting or reflections.
 *}
type
  TR3D_Probe = record
    type_: TR3D_ProbeType;    ///< Type of data captured by the probe
    handle: UInt32;           ///< Internal probe handle (don't touch)
    position: TVector3;       ///< World-space probe position
    falloff: Single;          ///< Distance falloff factor
    range: Single;            ///< Maximum influence distance
    interior: Boolean;        ///< Whether the probe is captured as an interior
    shadows: Boolean;         ///< Whether shadows are included in the capture
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Allocates a probe of the given type.
 *
 * @param type Whether this probe contributes indirect lighting or reflections.
 * @param interior Whether the skybox is taken into account when capturing this probe.
 * @param shadow Whether shadow casters are taken into account when capturing this probe.
 * @return A valid probe, or zeroed probe on failure.
 *}
function R3D_LoadProbe(type_: TR3D_ProbeType; interior: Boolean; shadow: Boolean): TR3D_Probe; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadProbe';

{*
 * @brief Releases a probe, freeing its layer for reuse.
 *
 * @param probe Probe to unload.
 *}
procedure R3D_UnloadProbe(probe: TR3D_Probe); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadProbe';

{*
 * @brief Returns whether the probe has a valid allocated layer.
 *
 * @param probe Probe to check.
 * @return true if the probe is valid, otherwise false.
 *}
function R3D_IsProbeValid(probe: TR3D_Probe): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsProbeValid';
