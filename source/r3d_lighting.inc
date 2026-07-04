// r3d_lighting.inc -- R3D Lighting Module.

// ========================================
// ENUM TYPES
// ========================================

{*
 * @brief Types of lights supported by the rendering engine.
 *
 * Each light type has different behaviors and use cases.
 *}
type
  TR3D_LightType = (
    R3D_LIGHT_DIR,          ///< Directional light, affects the entire scene with parallel rays.
    R3D_LIGHT_SPOT,         ///< Spot light, emits light in a cone shape.
    R3D_LIGHT_OMNI,         ///< Omni light, emits light in all directions from a single point.
    R3D_LIGHT_TYPE_COUNT
  );

{*
 * @brief Modes for updating shadow maps.
 *
 * Determines how often the shadow maps are refreshed.
 *}
type
  TR3D_ShadowUpdateMode = (
    R3D_SHADOW_UPDATE_MANUAL,     ///< Shadow maps update only when explicitly requested.
    R3D_SHADOW_UPDATE_INTERVAL,   ///< Shadow maps update at defined time intervals.
    R3D_SHADOW_UPDATE_CONTINUOUS  ///< Shadow maps update every frame for real-time accuracy.
  );

// ========================================
// HANDLE TYPE
// ========================================

{*
 * @brief Unique identifier for an R3D light.
 *
 * ID type used to reference a light.
 * A zero value indicates an invalid light.
 *}
type
  TR3D_Light = UInt32;

// ========================================
// CONSTANTS
// ========================================

const
  R3D_INVALID_LIGHT: TR3D_Light = 0;  ///< Invalid light ID constant.

// ========================================
// PUBLIC API
// ========================================

// ----------------------------------------
// LIGHTING: Lights Config Functions
// ----------------------------------------

{*
 * @brief Creates a new light of the specified type.
 *
 * This function creates a light of the given type. The light must be destroyed
 * manually when no longer needed by calling `R3D_DestroyLight`.
 *
 * @param typ The type of light to create (directional, spot or omni-directional).
 * @return The ID of the created light.
 *}
function R3D_CreateLight(typ: TR3D_LightType): TR3D_Light; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateLight';

{*
 * @brief Destroys the specified light.
 *
 * This function deallocates the resources associated with the light and makes
 * the light ID invalid. It must be called after the light is no longer needed.
 *
 * @param id The ID of the light to destroy.
 *}
procedure R3D_DestroyLight(id: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DestroyLight';

{*
 * @brief Checks if a light exists.
 *
 * This function checks if the specified light ID is valid and if the light exists.
 *
 * @param id The ID of the light to check.
 * @return True if the light exists, false otherwise.
 *}
function R3D_IsLightValid(id: TR3D_Light): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsLightValid';

{*
 * @brief Gets the type of a light.
 *
 * This function returns the type of the specified light (directional, spot or omni-directional).
 *
 * @param id The ID of the light.
 * @return The type of the light.
 *}
function R3D_GetLightType(id: TR3D_Light): TR3D_LightType; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightType';

{*
 * @brief Returns whether a light is currently enabled.
 *
 * @param id The ID of the light to query.
 * @return True if the light is enabled, false otherwise.
 *}
function R3D_IsLightEnabled(id: TR3D_Light): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsLightEnabled';

{*
 * @brief Toggles a light between enabled and disabled states.
 *
 * @param id The ID of the light to toggle.
 *}
procedure R3D_ToggleLight(id: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ToggleLight';

{*
 * @brief Enables a light.
 *
 * Has no effect if the light is already enabled.
 *
 * @param id The ID of the light to enable.
 *}
procedure R3D_EnableLight(id: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EnableLight';

{*
 * @brief Disables a light.
 *
 * Has no effect if the light is already disabled.
 *
 * @param id The ID of the light to disable.
 *}
procedure R3D_DisableLight(id: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DisableLight';

// ----------------------------------------
// LIGHTING: Light Color Functions
// ----------------------------------------

{*
 * @brief Gets the color of a light in sRGB space.
 *
 * The internal linear color is converted to sRGB before being returned.
 *
 * @param id The ID of the light.
 * @return The color of the light as an sRGB `TColor`.
 *}
function R3D_GetLightColor(id: TR3D_Light): TColor; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightColor';

{*
 * @brief Sets the color of a light from an sRGB color.
 *
 * The provided sRGB color is converted to linear space before being stored internally.
 *
 * Default: (TColor) {255, 255, 255, 255}
 *
 * @param id The ID of the light.
 * @param color The new color to set for the light, in sRGB space.
 *}
procedure R3D_SetLightColor(id: TR3D_Light; color: TColor); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightColor';

{*
 * @brief Gets the color of a light in linear space.
 *
 * Returns the raw internal color without any color space conversion.
 *
 * @param id The ID of the light.
 * @return The color of the light as a linear RGB `TVector3`.
 *}
function R3D_GetLightColorLinear(id: TR3D_Light): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightColorLinear';

{*
 * @brief Sets the color of a light from a linear RGB color.
 *
 * The provided value is stored as-is, without any color space conversion.
 *
 * Default: (TVector3) {1.0f, 1.0f, 1.0f}
 *
 * @param id The ID of the light.
 * @param color The new color to set for the light, in linear space.
 *}
procedure R3D_SetLightColorLinear(id: TR3D_Light; color: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightColorLinear';

{*
 * @brief Sets the color of a light from a color temperature.
 *
 * Converts the given temperature to an sRGB color using the Tanner Helland approximation,
 * then applies the appropriate sRGB-to-linear conversion before storing it internally.
 *
 * @param id The ID of the light.
 * @param kelvin The color temperature in Kelvin. Valid range is 1000K to 40000K.
 *}
procedure R3D_SetLightTemperature(id: TR3D_Light; kelvin: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightTemperature';

// ----------------------------------------
// LIGHTING: Light Position & Direction
// ----------------------------------------

{*
 * @brief Gets the position of a light.
 *
 * This function retrieves the position of the specified light.
 * Only applicable to spot lights or omni-lights.
 *
 * @param id The ID of the light.
 * @return The position of the light as a `TVector3`.
 *}
function R3D_GetLightPosition(id: TR3D_Light): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightPosition';

{*
 * @brief Sets the position of a light.
 *
 * This function sets the position of the specified light.
 * Only applicable to spot lights or omni-lights.
 *
 * Default: (TVector3) {0.0f, 0.0f, 0.0f}
 *
 * @note For directional lights, the position is stored internally but has no effect
 *       on lighting calculations. It can still be retrieved via `R3D_GetLightPosition`
 *       and may be used for debug visualization with `R3D_DrawLightDebug`.
 *
 * @param id The ID of the light.
 * @param position The new position to set for the light.
 *}
procedure R3D_SetLightPosition(id: TR3D_Light; position: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightPosition';

{*
 * @brief Gets the direction of a light.
 *
 * This function retrieves the direction of the specified light.
 * Only applicable to directional lights or spot lights.
 *
 * @param id The ID of the light.
 * @return The direction of the light as a `TVector3`.
 *}
function R3D_GetLightDirection(id: TR3D_Light): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightDirection';

{*
 * @brief Sets the direction of a light.
 *
 * This function sets the direction of the specified light.
 * Only applicable to directional lights or spot lights.
 *
 * Default: (TVector3) {0.0f, 0.0f, -1.0f}
 *
 * @note Has no effect for omni-directional lights.
 *       If called on an omni-directional light,
 *       a warning will be logged.
 *
 * @param id The ID of the light.
 * @param direction The new direction to set for the light.
 *                  The vector is automatically normalized.
 *}
procedure R3D_SetLightDirection(id: TR3D_Light; direction: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightDirection';

{*
 * @brief Sets the position and direction of a light to look at a target point.
 *
 * This function sets both the position and the direction of the specified light,
 * causing it to "look at" a given target point.
 *
 * @note - For directional lights, only the direction is updated. The position is stored
 *         internally but has no effect on lighting calculations; it may still be used
 *         for debug visualization with `R3D_DrawLightDebug`.
 *       - For omni-directional lights, only the position is updated (direction is not calculated).
 *       - For spot lights, both position and direction are set accordingly.
 *
 * @param id The ID of the light.
 * @param position The position to set for the light.
 * @param target The point the light should look at.
 *}
procedure R3D_SetLightTarget(id: TR3D_Light; position: TVector3; target: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightTarget';

// ----------------------------------------
// LIGHTING: Light Energy & Specular
// ----------------------------------------

{*
 * @brief Gets the energy level of a light.
 *
 * @param id The ID of the light.
 * @return The energy level of the light.
 *}
function R3D_GetLightEnergy(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightEnergy';

{*
 * @brief Sets the energy level of a light.
 *
 * This function sets the energy (intensity) of the specified light.
 * A higher energy value will result in a brighter light.
 *
 * Default: 1.0f
 *
 * @param id The ID of the light.
 * @param energy The new energy value to set for the light.
 *}
procedure R3D_SetLightEnergy(id: TR3D_Light; energy: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightEnergy';

{*
 * @brief Gets the energy level of a light as a luminous flux.
 *
 * Converts the light's current energy factor to lumens using a reference distance of 1 unit,
 * assuming 1 unit = 1 meter. For a custom reference distance, use `R3D_EnergyToLumens`
 * with `R3D_GetLightEnergy` directly.
 *
 * @param id The ID of the light.
 * @return The luminous flux in lumens.
 *}
function R3D_GetLightLumen(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightLumen';

{*
 * @brief Sets the energy of a light from a luminous flux value.
 *
 * Converts the given flux to an energy factor using a reference distance of 1 unit,
 * assuming 1 unit = 1 meter. For a custom reference distance, use `R3D_LumensToEnergy`
 * and pass the result to `R3D_SetLightEnergy` directly.
 *
 * @param id The ID of the light.
 * @param lumens The luminous flux in lumens.
 *}
procedure R3D_SetLightLumen(id: TR3D_Light; lumens: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightLumen';

{*
 * @brief Gets the specular intensity of a light.
 *
 * This function retrieves the current specular intensity of the specified light.
 * Specular intensity affects how shiny surfaces appear when reflecting the light.
 *
 * @param id The ID of the light.
 * @return The current specular intensity of the light.
 *}
function R3D_GetLightSpecular(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightSpecular';

{*
 * @brief Sets the specular intensity of a light.
 *
 * This function sets the specular intensity of the specified light.
 * Higher specular values result in stronger and sharper highlights on reflective surfaces.
 *
 * Default: 0.5f
 *
 * @param id The ID of the light.
 * @param specular The new specular intensity value to set for the light.
 *}
procedure R3D_SetLightSpecular(id: TR3D_Light; specular: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightSpecular';

// ----------------------------------------
// LIGHTING: Light Range & Falloff
// ----------------------------------------

{*
 * @brief Gets the range of a light.
 *
 * This function retrieves the range of the specified light, which determines how far the light can affect.
 * Only applicable to spot lights or omni-lights.
 *
 * @param id The ID of the light.
 * @return The range of the light.
 *}
function R3D_GetLightRange(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightRange';

{*
 * @brief Sets the range parameter of a light.
 *
 * For spot and omni lights, this defines the maximum illumination distance.
 * For directional lights, this defines the shadow rendering radius around the camera.
 *
 * Default: 50.0f
 *
 * @param id The ID of the light.
 * @param range The range value to apply.
 *}
procedure R3D_SetLightRange(id: TR3D_Light; range: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightRange';

{*
 * @brief Gets the falloff exponent of a light.
 *
 * Controls the shape of the attenuation curve over the light's range.
 * A value of 1.0 produces a linear falloff, 2.0 a quadratic (more physically
 * plausible) falloff, and higher values concentrate the light closer to the source.
 * Only applicable to spot lights and omni-lights.
 *
 * @param id The ID of the light.
 * @return The falloff exponent of the light.
 *}
function R3D_GetLightFalloff(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightFalloff';

{*
 * @brief Sets the falloff exponent of a light.
 *
 * Controls the shape of the attenuation curve over the light's range.
 * A value of 1.0 produces a linear falloff, 2.0 a quadratic (more physically
 * plausible) falloff, and higher values concentrate the light closer to the source.
 * Values of 0.0 or below are clamped to 1.0.
 * Only applicable to spot lights and omni-lights.
 *
 * Default: 1.0f
 *
 * @param id The ID of the light.
 * @param falloff The falloff exponent to set. Typical range is [0.5, 4.0].
 *}
procedure R3D_SetLightFalloff(id: TR3D_Light; falloff: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightFalloff';

// ----------------------------------------
// LIGHTING: Light Cone Angles
// ----------------------------------------

{*
 * @brief Gets the inner and outer cone angles of a spot light.
 *
 * The inner angle defines the region of full intensity, and the outer angle
 * defines where the light fully fades out. The transition between the two
 * produces a soft edge. Both angles are in degrees.
 * Only applicable to spot lights.
 *
 * @param id The ID of the light.
 * @param inner Pointer to receive the inner cone angle, in degrees. May be NULL.
 * @param outer Pointer to receive the outer cone angle, in degrees. May be NULL.
 *}
procedure R3D_GetLightAngle(id: TR3D_Light; inner: PSingle; outer: PSingle); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightAngle';

{*
 * @brief Sets the inner and outer cone angles of a spot light.
 *
 * The inner angle defines the region of full intensity, and the outer angle
 * defines where the light fully fades out. The transition between the two
 * produces a soft edge. Both angles are in degrees. If inner exceeds outer,
 * the two values are swapped automatically.
 * Only applicable to spot lights.
 *
 * Default: 22.5f, 45.0f
 *
 * @param id The ID of the light.
 * @param inner The inner cone half-angle, in degrees.
 * @param outer The outer cone half-angle, in degrees.
 *}
procedure R3D_SetLightAngle(id: TR3D_Light; inner: Single; outer: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightAngle';

// ----------------------------------------
// LIGHTING: Volumetric Fog
// ----------------------------------------

{*
 * @brief Gets the volumetric fog energy multiplier of a light.
 *
 * @param id The ID of the light.
 * @return The volumetric fog energy multiplier of the light.
 *}
function R3D_GetLightFogEnergy(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightFogEnergy';

{*
 * @brief Sets the volumetric fog energy multiplier of a light.
 *
 * The final fog contribution is the light's energy multiplied by this factor,
 * allowing fine-grained control over its volumetric fog intensity independently
 * of its regular lighting contribution.
 *
 * Default: 1.0f
 *
 * @param id The ID of the light.
 * @param energy The new volumetric fog energy multiplier to set for the light.
 *}
procedure R3D_SetLightFogEnergy(id: TR3D_Light; energy: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetLightFogEnergy';

// ----------------------------------------
// LIGHTING: Shadow Config Functions
// ----------------------------------------

{*
 * @brief Enables shadow rendering for a light.
 *
 * Turns on shadow rendering for the light. The engine will allocate a shadow
 * map if needed, or reuse one previously allocated for another light.
 *
 * Shadow map resolutions are by default:
 *   - 2048x2048 for spot and point lights
 *   - 4096x4096 for directional lights
 *
 * @param id The ID of the light.
 *
 * @note Creating too many shadow-casting lights can exhaust GPU memory and
 * potentially crash the graphics driver. Disabling shadows on one light and
 * enabling them on another is free, since existing shadow maps are reused.
 *}
procedure R3D_EnableShadow(id: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EnableShadow';

{*
 * @brief Disables shadow rendering for a light.
 *
 * Turns off shadow rendering for the light. The associated shadow map is
 * kept in memory and may later be reused by another light.
 *
 * @param id The ID of the light.
 *}
procedure R3D_DisableShadow(id: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DisableShadow';

{*
 * @brief Checks if shadow casting is enabled for a light.
 *
 * This function checks if shadow casting is currently enabled for the specified light.
 *
 * @param id The ID of the light.
 * @return True if shadow casting is enabled, false otherwise.
 *}
function R3D_IsShadowEnabled(id: TR3D_Light): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsShadowEnabled';

{*
 * @brief Gets the shadow map update mode of a light.
 *
 * This function retrieves the current mode for updating the shadow map of a light. The mode can be:
 * - Interval: Updates the shadow map at a fixed interval.
 * - Continuous: Updates the shadow map continuously.
 * - Manual: Updates the shadow map manually (via explicit function calls).
 *
 * @param id The ID of the light.
 * @return The shadow map update mode.
 *}
function R3D_GetShadowUpdateMode(id: TR3D_Light): TR3D_ShadowUpdateMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetShadowUpdateMode';

{*
 * @brief Sets the shadow map update mode of a light.
 *
 * This function sets the mode for updating the shadow map of the specified light.
 * The update mode controls when and how often the shadow map is refreshed.
 *
 * Default: R3D_SHADOW_UPDATE_INTERVAL
 *
 * @param id The ID of the light.
 * @param mode The update mode to set for the shadow map (Interval, Continuous, or Manual).
 *}
procedure R3D_SetShadowUpdateMode(id: TR3D_Light; mode: TR3D_ShadowUpdateMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetShadowUpdateMode';

{*
 * @brief Gets the interval between shadow map updates in interval update mode.
 *
 * Only relevant when the shadow update mode is set to @ref R3D_SHADOW_UPDATE_INTERVAL.
 *
 * @param id The ID of the light.
 * @return The interval in seconds between shadow map updates.
 *}
function R3D_GetShadowUpdateInterval(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetShadowUpdateInterval';

{*
 * @brief Sets the interval between shadow map updates in interval update mode.
 *
 * Only relevant when the shadow update mode is set to @ref R3D_SHADOW_UPDATE_INTERVAL.
 *
 * Default: 0.016f
 *
 * @param id The ID of the light.
 * @param seconds The interval in seconds between shadow map updates.
 *}
procedure R3D_SetShadowUpdateInterval(id: TR3D_Light; seconds: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetShadowUpdateInterval';

{*
 * @brief Forces an immediate update of the shadow map during the next rendering pass.
 *
 * This function forces the shadow map of the specified light to be updated during the next call to `R3D_End`.
 * This is primarily used for the manual update mode, but may also work for the interval mode.
 *
 * @param id The ID of the light.
 *}
procedure R3D_UpdateShadowMap(id: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateShadowMap';

{*
 * @brief Retrieves the softness radius used to simulate penumbra in shadows.
 *
 * The softness is expressed as a sampling radius in texels within the shadow map.
 *
 * @param id The ID of the light.
 * @return The softness radius in texels currently set for the shadow.
 *}
function R3D_GetShadowSoftness(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetShadowSoftness';

{*
 * @brief Sets the softness radius used to simulate penumbra in shadows.
 *
 * This function adjusts the softness of the shadow edges for the specified light.
 * The softness value corresponds to a number of texels in the shadow map, independent
 * of its resolution. Larger values increase the blur radius, resulting in softer,
 * more diffuse shadows, while smaller values yield sharper shadows.
 *
 * Default: 4.0f
 *
 * @param id The ID of the light.
 * @param softness The softness radius in texels to apply (must be >= 0).
 *
 * @note The softness must be set only after shadows have been enabled for the light,
 *       since the shadow map resolution must be known before the softness can be applied.
 *}
procedure R3D_SetShadowSoftness(id: TR3D_Light; softness: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetShadowSoftness';

{*
 * @brief Retrieves the shadow opacity for a light.
 *
 * @param id The ID of the light.
 * @return The current shadow opacity.
 *}
function R3D_GetShadowOpacity(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetShadowOpacity';

{*
 * @brief Sets the shadow opacity for a light.
 *
 * The opacity controls the visual strength of shadows. A value of 0 makes shadows
 * fully transparent, while 1 applies full opacity. Values are not clamped, but the
 * usual range is 0 to 1.
 *
 * When the opacity is exactly 0, the light still owns its shadow map, but shadow
 * map rendering and shadow application are entirely skipped.
 *
 * Default: 1.0f
 *
 * @param id The ID of the light.
 * @param opacity The shadow opacity to apply.
 *}
procedure R3D_SetShadowOpacity(id: TR3D_Light; opacity: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetShadowOpacity';

{*
 * @brief Gets the shadow depth bias value.
 *
 * @param id The ID of the light.
 * @return The current shadow depth bias value.
 *}
function R3D_GetShadowDepthBias(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetShadowDepthBias';

{*
 * @brief Sets the shadow depth bias value.
 *
 * A higher bias helps reduce "shadow acne" artifacts
 * (shadows flickering or appearing misaligned on surfaces).
 * Be careful: too large values may cause shadows to look detached
 * or floating away from objects.
 *
 * Default per light type:
 *   - Directional: 0.001
 *   - Spot:        0.0001
 *   - Omni:        0.025
 *
 * @param id The ID of the light.
 * @param value The shadow depth bias value to set.
 *}
procedure R3D_SetShadowDepthBias(id: TR3D_Light; value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetShadowDepthBias';

{*
 * @brief Gets the shadow slope bias value.
 *
 * @param id The ID of the light.
 * @return The current shadow slope bias value.
 *}
function R3D_GetShadowSlopeBias(id: TR3D_Light): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetShadowSlopeBias';

{*
 * @brief Sets the shadow slope bias value.
 *
 * This bias mainly compensates artifacts on surfaces angled
 * relative to the light. It helps prevent shadows from
 * incorrectly appearing or disappearing along object edges.
 *
 * Default per light type:
 *   - Directional: 0.0015
 *   - Spot:        0.0005
 *   - Omni:        0.1
 *
 * @param id The ID of the light.
 * @param value The shadow slope bias value to set.
 *}
procedure R3D_SetShadowSlopeBias(id: TR3D_Light; value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetShadowSlopeBias';

{*
 * @brief Gets the shadow caster mask.
 *
 * @param id The ID of the light.
 * @return The current shadow caster mask.
 *}
function R3D_GetShadowCasterMask(id: TR3D_Light): TR3D_Layer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetShadowCasterMask';

{*
 * @brief Replaces the shadow caster mask.
 *
 * @param id The ID of the light.
 * @param cullMask The new shadow caster mask to set.
 *}
procedure R3D_SetShadowCasterMask(id: TR3D_Light; cullMask: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetShadowCasterMask';

{*
 * @brief Enables one or more layers in the shadow caster mask.
 *
 * @param id The ID of the light.
 * @param layerMask The layers to enable in the shadow caster mask.
 *}
procedure R3D_EnableShadowCasterLayers(id: TR3D_Light; layerMask: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EnableShadowCasterLayers';

{*
 * @brief Disables one or more layers from the shadow caster mask.
 *
 * @param id The ID of the light.
 * @param layerMask The layers to disable from the shadow caster mask.
 *}
procedure R3D_DisableShadowCasterLayers(id: TR3D_Light; layerMask: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DisableShadowCasterLayers';

{*
 * @brief Toggles one or more layers in the shadow caster mask.
 *
 * @param id The ID of the light.
 * @param layerMask The layers to toggle in the shadow caster mask.
 *}
procedure R3D_ToggleShadowCasterLayers(id: TR3D_Light; layerMask: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ToggleShadowCasterLayers';

{*
 * @brief Checks whether at least one object layer is visible to the shadow caster.
 *
 * @param id The ID of the light.
 * @param layerMask The layer mask to check against.
 * @return True if at least one layer is visible, false otherwise.
 *}
function R3D_IsShadowCasterLayerVisible(id: TR3D_Light; layerMask: TR3D_Layer): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsShadowCasterLayerVisible';

// ----------------------------------------
// LIGHTING: Light Helper Functions
// ----------------------------------------

{*
 * @brief Returns the bounding box encompassing the light's area of influence.
 *
 * This function computes the axis-aligned bounding box (AABB) that encloses the
 * volume affected by the specified light, based on its type:
 *
 * - For spotlights, the bounding box encloses the light cone.
 * - For omni-directional lights, it encloses a sphere representing the light's range.
 * - For directional lights, it returns an infinite bounding box to represent global influence.
 *
 * This bounding box is primarily useful for spatial partitioning, culling, or visual debugging.
 *
 * @param light The light for which to compute the bounding box.
 *
 * @return A `TBoundingBox` struct that encloses the light's influence volume.
 *}
function R3D_GetLightBoundingBox(light: TR3D_Light): TBoundingBox; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetLightBoundingBox';

{*
 * @brief Draws the area of influence of the light in 3D space.
 *
 * This function visualizes the area affected by a light in 3D space.
 * It draws the light's influence, such as the cone for spotlights or the volume for omni-lights.
 * This function is only relevant for spotlights and omni-lights.
 *
 * @note This function should be called while using the default 3D rendering mode of raylib,
 *       not with R3D's rendering mode. It uses raylib's 3D drawing functions to render the light's shape.
 *
 * @param id The ID of the light.
 *}
procedure R3D_DrawLightDebug(id: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawLightDebug';

// ----------------------------------------
// LIGHTING: Math Helper Functions
// ----------------------------------------

{*
 * @brief Converts a luminous flux to an energy factor.
 *
 * Computes the illuminance in lux at the given reference distance from an isotropic
 * point source: `energy = lumens / (4 * pi * distance * distance)`
 *
 * @param lumens The luminous flux in lumens.
 * @param referenceDistance The reference distance in scene units (1 unit = 1 meter).
 * @return The corresponding energy factor.
 *}
function R3D_LumensToEnergy(lumens: Single; referenceDistance: Single): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LumensToEnergy';

{*
 * @brief Converts an energy factor back to a luminous flux.
 *
 * Inverse of `R3D_LumensToEnergy`: `lumens = energy * 4 * pi * distance * distance`
 *
 * @param energy The energy factor.
 * @param referenceDistance The reference distance in scene units (1 unit = 1 meter).
 * @return The corresponding luminous flux in lumens.
 *}
function R3D_EnergyToLumens(energy: Single; referenceDistance: Single): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EnergyToLumens';
