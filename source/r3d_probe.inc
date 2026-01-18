// r3d_probe.inc -- R3D Probe Module.


// ========================================
// ENUM TYPES
// ========================================


{*
 * @brief Bit-flags controlling what components are generated.
 *
 * - R3D_PROBE_ILLUMINATION -> generate diffuse irradiance
 * - R3D_PROBE_REFLECTION   -> generate specular prefiltered map
 *}
 type //uint32_t
   PR3D_ProbeFlags = ^TR3D_ProbeFlags;
   TR3D_ProbeFlags = uint32;
   const
     R3D_PROBE_ILLUMINATION = (1 shl 0);
     R3D_PROBE_REFLECTION = (1 shl 1);

  {*
   * @brief Modes for updating probes.
   *
   * Controls how often probe captures are refreshed.
   *}
  type
  TR3D_ProbeUpdateMode = (
    R3D_PROBE_UPDATE_ONCE,      ///< Updated only when its state or content changes
    R3D_PROBE_UPDATE_ALWAYS     ///< Updated during every frames
  );

// ========================================
// HANDLE TYPE
// ========================================

type
  {*
   * @brief Unique identifier for an R3D probe.
   *
   * Negative values indicate an invalid probe.
   *}
// cint32                 = UnixType.cint32;      pcint32                = UnixType.pcint32;
  TR3D_Probe = cint32;
//  PR3D_Probe = ^TR3D_Probe;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Creates a new probe of the specified type.
 *
 * The returned probe must be destroyed using ::R3D_DestroyProbe
 * when it is no longer needed.
 *
 * @param flags IBL components that the probe must support.
 * @return A valid probe ID, or a negative value on failure.
 *}
function R3D_CreateProbe(flags: TR3D_ProbeFlags): TR3D_Probe; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateProbe';

{*
 * @brief Destroys a probe and frees its resources.
 *
 * @param id Probe ID to destroy.
 *}
procedure R3D_DestroyProbe(id: TR3D_Probe); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DestroyProbe';

{*
 * @brief Returns whether a probe exists.
 *
 * @param id Probe ID.
 * @return true if the probe is valid and allocated, otherwise false.
 *}
function R3D_IsProbeExist(id: TR3D_Probe): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsProbeExist';

{*
* @brief Returns the probe flags.
*
* @param id Probe ID.
* @return The flags assigned to the probe.
 *}
function R3D_GetProbeFlags(id: TR3D_Probe): TR3D_ProbeFlags; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetProbeType';

{*
 * @brief Returns whether a probe is currently active.
 *
 * Inactive probes do not contribute to lighting.
 *
 * @param id Probe ID.
 * @return true if active, otherwise false.
 *}
function R3D_IsProbeActive(id: TR3D_Probe): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsProbeActive';

{*
 * @brief Enables or disables a probe.
 *
 * @param id Probe ID.
 * @param active Set to true to enable the probe.
 *}
procedure R3D_SetProbeActive(id: TR3D_Probe; active: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetProbeActive';

{*
 * @brief Gets the probe update mode.
 *
 * - R3D_PROBE_UPDATE_ONCE:
 *     Captured once, then reused unless its state changes.
 *
 * - R3D_PROBE_UPDATE_ALWAYS:
 *     Recaptured every frame.
 *
 * Use "ONCE" for static scenes, "ALWAYS" for highly dynamic scenes.
 *}
function R3D_GetProbeUpdateMode(id: TR3D_Probe): TR3D_ProbeUpdateMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetProbeUpdateMode';

{*
 * @brief Sets the probe update mode.
 *
 * Controls when the probe capture is refreshed.
 *
 * @param id Probe ID.
 * @param mode Update mode to apply.
 *}
procedure R3D_SetProbeUpdateMode(id: TR3D_Probe; mode: TR3D_ProbeUpdateMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetProbeUpdateMode';

{*
 * @brief Returns whether the probe is considered indoors.
 *
 * Indoor probes do not sample skybox or environment maps.
 * Instead they rely only on ambient and background colors.
 *
 * Use this for rooms, caves, tunnels, etc...
 * where outside lighting should not bleed inside.
 *}
function R3D_GetProbeInterior(id: TR3D_Probe): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetProbeInterior';

{*
 * @brief Enables or disables indoor mode for the probe.
 *}
procedure R3D_SetProbeInterior(id: TR3D_Probe; active: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetProbeInterior';

{*
 * @brief Returns whether shadows are captured by this probe.
 *
 * When enabled, shadowing is baked into the captured lighting.
 * This improves realism, but increases capture cost.
 *}
function R3D_GetProbeShadows(id: TR3D_Probe): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetProbeShadows';

{*
 * @brief Enables or disables shadow rendering during probe capture.
 *}
procedure R3D_SetProbeShadows(id: TR3D_Probe; active: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetProbeShadows';

{*
 * @brief Gets the world position of the probe.
 *}
function R3D_GetProbePosition(id: TR3D_Probe): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetProbePosition';

{*
 * @brief Sets the world position of the probe.
 *}
procedure R3D_SetProbePosition(id: TR3D_Probe; position: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetProbePosition';

{*
 * @brief Gets the effective range of the probe.
 *
 * The range defines the radius (in world units) within which this probe
 * contributes to lighting. Objects outside this sphere receive no influence.
 *}
function R3D_GetProbeRange(id: TR3D_Probe): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetProbeRange';

{*
 * @brief Sets the effective range of the probe.
 *
 * @param range Radius in world units. Must be > 0.
 *}
procedure R3D_SetProbeRange(id: TR3D_Probe; range: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetProbeRange';

{*
 * @brief Gets the falloff factor applied to probe contributions.
 *
 * Falloff controls how lighting fades as distance increases.
 *
 * Internally this uses a power curve:
 *     attenuation = 1.0 - pow(dist / probe.range, probe.falloff)
 *
 * Effects:
 *   - falloff = 1 -> linear fade
 *   - falloff > 1 -> light stays strong near the probe, drops faster at the edge
 *   - falloff < 1 -> softer fade across the whole range
 *}
function R3D_GetProbeFalloff(id: TR3D_Probe): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetProbeFalloff';

{*
 * @brief Sets the falloff factor used for distance attenuation.
 *
 * Larger values make the probe feel more localized.
 *}
procedure R3D_SetProbeFalloff(id: TR3D_Probe; falloff: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetProbeFalloff';

