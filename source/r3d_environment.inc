// R3D Environment Module.

{*
 * @brief Bloom effect modes.
 *
 * Specifies different post-processing bloom techniques that can be applied
 * to the rendered scene. Bloom effects enhance the appearance of bright areas
 * by simulating light bleeding, contributing to a more cinematic and realistic look.
 *}
type
  TR3D_Bloom = (
    R3D_BLOOM_DISABLED,     ///< Bloom effect is disabled. The scene is rendered without any glow enhancement.
    R3D_BLOOM_MIX,          ///< Blends the bloom effect with the original scene using linear interpolation (Lerp).
    R3D_BLOOM_ADDITIVE,     ///< Adds the bloom effect additively to the scene, intensifying bright regions.
    R3D_BLOOM_SCREEN        ///< Combines the scene and bloom using screen blending, which brightens highlights
  );

{*
 * @brief Fog effect modes.
 *
 * Determines how fog is applied to the scene, affecting depth perception and atmosphere.
 *}
type
  TR3D_Fog = (
    R3D_FOG_DISABLED, ///< Fog effect is disabled.
    R3D_FOG_LINEAR,   ///< Fog density increases linearly with distance from the camera.
    R3D_FOG_EXP2,     ///< Exponential fog (exp2), where density increases exponentially with distance.
    R3D_FOG_EXP       ///< Exponential fog, similar to EXP2 but with a different rate of increase.
  );

{*
 * @brief Depth of field effect modes.
 *
 * Controls how depth of field is applied to the scene, affecting the focus and blur of objects.
 *}
type
  TR3D_Dof = (
    R3D_DOF_DISABLED, ///< Depth of field effect is disabled.
    R3D_DOF_ENABLED  ///< Depth of field effect is enabled.
  );

{*
 * @brief Tone mapping modes.
 *
 * Controls how high dynamic range (HDR) colors are mapped to low dynamic range (LDR) for display.
 *}
type
  TR3D_Tonemap = (
    R3D_TONEMAP_LINEAR,   ///< Simple linear mapping of HDR values.
    R3D_TONEMAP_REINHARD, ///< Reinhard tone mapping, a balanced method for compressing HDR values.
    R3D_TONEMAP_FILMIC,   ///< Filmic tone mapping, mimicking the response of photographic film.
    R3D_TONEMAP_ACES,     ///< ACES tone mapping, a high-quality cinematic rendering technique.
    R3D_TONEMAP_AGX,      ///< AGX tone mapping, a modern technique designed to preserve both highlight and shadow details for HDR rendering.
    R3D_TONEMAP_COUNT     ///< Number of tone mapping modes (used internally)
  );

// ========================================
// PUBLIC API
// ========================================

// ----------------------------------------
// ENVIRONMENT: Background And Ambient
// ----------------------------------------

{*
 * @brief Sets the background color when no skybox is enabled.
 *
 * This function defines the background color to be used when no skybox is active.
 * The color will be used for the clear color of the scene.
 *
 * @param color The color to set as the background color.
 *}
procedure R3D_SetBackgroundColor(color: TColor); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBackgroundColor';

{*
 * @brief Sets the ambient light color when no skybox is enabled.
 *
 * This function defines the ambient light color to be used when no skybox is active.
 * It affects the overall lighting of the scene when no skybox is present.
 *
 * @param color The color to set for ambient light.
 *}
procedure R3D_SetAmbientColor(color: TColor); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAmbientColor';

{*
 * @brief Sets the energy level for ambient light when no skybox is enabled.
 *
 * This function defines the ambient light energy to be used when no skybox is active.
 * Applied multiplicatively to the base ambient color.
 *
 * @param energy The energy to set for ambient light.
 *}
procedure R3D_SetAmbientEnergy(energy: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAmbientEnergy';

{*
 * @brief Enables a skybox for the scene.
 *
 * This function enables a skybox in the scene, replacing the default background with
 * a 3D environment. The skybox is defined by the specified skybox asset.
 *
 * @param skybox The skybox to enable.
 *}
procedure R3D_EnableSkybox(skybox: TR3D_Skybox); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EnableSkybox';

{*
 * @brief Disables the skybox in the scene.
 *
 * This function disables the skybox, reverting back to the default background
 * color (or no background if none is set). It should be called to remove the skybox
 * from the scene.
 *}
procedure R3D_DisableSkybox; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DisableSkybox';

{*
 * @brief Sets the rotation of the skybox.
 *
 * This function allows you to specify the rotation of the skybox along the
 * pitch, yaw, and roll axes, which allows the skybox to be rotated in the scene.
 *
 * @param pitch The rotation angle around the X-axis (in degrees).
 * @param yaw The rotation angle around the Y-axis (in degrees).
 * @param roll The rotation angle around the Z-axis (in degrees).
 *}
procedure R3D_SetSkyboxRotation(pitch, yaw, roll: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSkyboxRotation';

{*
 * @brief Gets the current rotation of the skybox.
 *
 * This function returns the current rotation of the skybox as a vector containing
 * the pitch, yaw, and roll values in degrees.
 *
 * @return A vector containing the current pitch, yaw, and roll of the skybox.
 *}
function R3D_GetSkyboxRotation: TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSkyboxRotation';

{*
 * @brief Sets the intensity scaling values used for the environment's skybox.
 *
 * This function controls the intensity of both the rendered skybox as well as
 * the light that is generated from the skybox.
 *
 * @param background The intensity of the skybox rendered as the background.
 *                   A value of 0.0 will disable rendering the skybox but
 *                   allow any generated lighting to still be applied.
 * @param ambient The intensity of ambient light produced by the skybox.
 * @param reflection The intensity of reflections of the skybox in reflective materials.
 *}
procedure R3D_SetSkyboxIntensity(background, ambient, reflection: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSkyboxIntensity';

{*
 * @brief Gets the intensity scaling values used for the environment's skybox.
 *
 * This function returns the intensity values for the rendered skybox as well
 * the light that is generated from the skybox.
 *
 * @param background Pointer to store the intensity value for the rendered skybox.
 * @param ambient Pointer to store the intensity value for ambient light produced by the skybox.
 * @param reflection Pointer to store the intensity value for reflections from the skybox.
 *}
procedure R3D_GetSkyboxIntensity(background, ambient, reflection: PSingle); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSkyboxIntensity';

// ----------------------------------------
// ENVIRONMENT: SSAO Config Functions
// ----------------------------------------

{*
 * @brief Enables or disables Screen Space Ambient Occlusion (SSAO).
 *
 * This function toggles the SSAO effect. When enabled, SSAO enhances the realism
 * of the scene by simulating ambient occlusion, darkening areas where objects
 * are close together or in corners.
 *
 * @param enabled Whether to enable or disable SSAO.
 *
 * Default: false
 *}
procedure R3D_SetSSAO(enabled: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSAO';

{*
 * @brief Gets the current state of SSAO.
 *
 * This function checks if SSAO is currently enabled or disabled.
 *
 * @return True if SSAO is enabled, false otherwise.
 *}
function R3D_GetSSAO: Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSAO';

{*
 * @brief Sets the radius for SSAO effect.
 *
 * This function sets the radius used by the SSAO effect to calculate occlusion.
 * A higher value will affect a larger area around each pixel, while a smaller value
 * will create sharper and more localized occlusion.
 *
 * @param value The radius value to set for SSAO.
 *
 * Default: 0.5
 *}
procedure R3D_SetSSAORadius(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSAORadius';

{*
 * @brief Gets the current SSAO radius.
 *
 * This function retrieves the current radius value used by the SSAO effect.
 *
 * @return The radius value for SSAO.
 *}
function R3D_GetSSAORadius: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSAORadius';

{*
 * @brief Sets the bias for SSAO effect.
 *
 * This function sets the bias used by the SSAO effect to adjust how much occlusion
 * is applied to the scene. A higher value can reduce artifacts, but may also
 * result in less pronounced ambient occlusion.
 *
 * @param value The bias value for SSAO.
 *
 * Default: 0.025
 *}
procedure R3D_SetSSAOBias(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSAOBias';

{*
 * @brief Gets the current SSAO bias.
 *
 * This function retrieves the current bias value used by the SSAO effect.
 *
 * @return The SSAO bias value.
 *}
function R3D_GetSSAOBias: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSAOBias';

{*
 * @brief Sets the number of blur iterations for the SSAO effect.
 *
 * This function sets the number of blur iterations applied to the SSAO effect.
 * By default, one iteration is performed, using a total of 12 samples for the
 * Gaussian blur. Increasing the number of iterations results in a smoother
 * ambient occlusion but may impact performance.
 *
 * @param value The number of blur iterations for SSAO.
 *
 * Default: 1
 *}
procedure R3D_SetSSAOIterations(value: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSAOIterations';

{*
 * @brief Gets the current number of blur iterations for the SSAO effect.
 *
 * This function retrieves the current number of blur iterations applied to the SSAO effect.
 *
 * @return The number of blur iterations for SSAO.
 *}
function R3D_GetSSAOIterations: Integer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSAOIterations';

{*
 * @brief Sets the intensity multiplier for the SSAO effect.
 *
 * This function sets the the base multiplier used by the SSAO effect.
 * Higher values will result in darker occlusion.
 *
 * @param value The intensity multiplier for SSAO.
 *
 * Default: 1.0
 *}
procedure R3D_SetSSAOIntensity(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSAOIntensity';

{*
 * @brief Gets the intensity multiplier for the SSAO effect.
 *
 * This function retrieves the intensity multiplier applied to the SSAO effect.
 *
 * @return The intensity multiplier for SSAO.
 *}
function R3D_GetSSAOIntensity: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSAOIntensity';

{*
 * @brief Sets the power factor for the SSAO effect.
 *
 * This function sets the exponential distributon applied to the SSAO effect.
 * Higher values will result in darker occlusion with an increasingly sharper
 * falloff compared to the SSAO intensity value.
 *
 * @param value The power factor for SSAO.
 *
 * Default: 1.0
 *}
procedure R3D_SetSSAOPower(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSAOPower';

{*
 * @brief Gets the power factor used for the SSAO effect.
 *
 * This function retrieves the exponential distributon value applied to the SSAO effect.
 *
 * @return The power factor for SSAO.
 *}
function R3D_GetSSAOPower: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSAOPower';

{*
 * @brief Controls the influence of SSAO on direct lighting.
 *
 * This function sets the amount of direct light attenuation from the SSAO effect.
 * Values greater than 0.0 will apply the SSAO effect to direct lighting with
 * increasing intensity at higher values. This is in addition to the typical
 * application to ambient light only.
 *
 * @param value SSAO effect intensity on direct light, in the range [0.0f, 1.0f].
 *
 * Default: 0.0
 *}
procedure R3D_SetSSAOLightAffect(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSAOLightAffect';

{*
 * @brief Gets the current direct lighting effect of SSAO.
 *
 * This function retrieves the value used for direct light attenuation from the SSAO effect.
 *
 * @return SSAO effect intensity on direct light, in the range [0.0f, 1.0f].
 *}
function R3D_GetSSAOLightAffect: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSAOLightAffect';

// ----------------------------------------
// ENVIRONMENT: Bloom Config Functions
// ----------------------------------------

{*
 * @brief Sets the bloom mode.
 *
 * This function configures the bloom effect mode, which determines how the bloom
 * effect is applied to the rendered scene.
 *
 * @param mode The bloom mode to set.
 *
 * Default: R3D_BLOOM_DISABLED
 *}
procedure R3D_SetBloomMode(mode: TR3D_Bloom); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBloomMode';

{*
 * @brief Gets the current bloom mode.
 *
 * This function retrieves the bloom mode currently applied to the scene.
 *
 * @return The current bloom mode.
 *}
function R3D_GetBloomMode: TR3D_Bloom; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBloomMode';

{*
 * @brief Sets the number of mipmap levels used for the bloom effect.
 *
 * This function controls how many mipmap level are generated for use in the bloom effect.
 * More levels will give a smoother and more widely dispersed effect, while less mipmaps
 * can provide a tighter effect. Setting this value to 0 will result in the maximum
 * possible amount of levels to be used. Use of this function will rebuild the
 * mipmaps and may give a one time performance hit.
 *
 * @param value The number of mipmap level to be used for the bloom effect.
 *
 * Default: 7
 *}
procedure R3D_SetBloomLevels(value: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBloomLevels';

{*
 * @brief Gets the current amount of mipmap levels used for the bloom effect.
 *
 * This function retrieves the current amount of mipmap levels in use by the bloom effect.
 *
 * @return The number of mipmap level currently used for the bloom effect.
 *}
function R3D_GetBloomLevels: Integer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBloomLevels';

{*
 * @brief Sets the bloom intensity.
 *
 * This function controls the strength of the bloom effect. Higher values result
 * in a more intense glow effect on bright areas of the scene.
 *
 * @param value The intensity value for bloom.
 *
 * Default: 0.05
 *}
procedure R3D_SetBloomIntensity(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBloomIntensity';

{*
 * @brief Gets the current bloom intensity.
 *
 * This function retrieves the intensity value of the bloom effect.
 *
 * @return The current bloom intensity.
 *}
function R3D_GetBloomIntensity: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBloomIntensity';

{*
 * @brief Sets the bloom filter radius.
 *
 * Controls the radius of the blur filter applied during the upscaling stage
 * of the bloom effect. A larger radius results in a wider glow around bright
 * objects, creating a softer and more diffuse bloom. A value of 0 disables
 * the filtering effect, preserving sharp bloom highlights.
 *
 * @param value The radius of the bloom filter (in pixels).
 *
 * Default: 0
 *}
procedure R3D_SetBloomFilterRadius(value: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBloomFilterRadius';

{*
 * @brief Gets the current bloom filter radius.
 *
 * Retrieves the current radius used for the bloom filter. This value determines
 * how far the glow effect extends around bright areas in the scene.
 *
 * @return The current bloom filter radius.
 *}
function R3D_GetBloomFilterRadius: Integer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBloomFilterRadius';

{*
 * @brief Sets the bloom brightness threshold.
 *
 * Controls the brightness cutoff used during the downsampling stage of the
 * bloom effect. If the color channel with the brightest value is below the
 * set threshold the pixel will not be included in the bloom effect.
 *
 * @param value The lowest value to be included the bloom effect (in color value depending on implementation).
 *
 * Default: 0.0
 *}
procedure R3D_SetBloomThreshold(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBloomThreshold';

{*
 * @brief Gets the bloom brightness threshold.
 *
 * Retrieves the current brightness cutoff used for the bloom effect. This value
 * determines if a pixel will be included in the bloom effect based on it's brightness.
 *
 * @return The current bloom brightness cutoff threshold.
 *}
function R3D_GetBloomThreshold: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBloomThreshold';

{*
 * @brief Sets the bloom brightness threshold's softness.
 *
 * Controls the softness of the cutoff between being include or excluded in the
 * bloom effect. A value of 0 will result in a hard transition between being
 * included or excluded, while larger values will give an increasingly
 * softer transition.
 *
 * @param value The value of of the bloom brightness threshold's softness.
 *
 * Default: 0.5
 *}
procedure R3D_SetBloomSoftThreshold(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBloomSoftThreshold';

{*
 * @brief Gets the current bloom brightness threshold's softness.
 *
 * Retrieves the softness of the brightness cutoff for the bloom effect.
 * This value determines the softness of the transition between being
 * included or excluded in the bloom effect
 *
 * @return The current bloom brightness threshold's softness.
 *}
function R3D_GetBloomSoftThreshold: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBloomSoftThreshold';

// ----------------------------------------
// ENVIRONMENT: SSR Config Functions
// ----------------------------------------

{*
 * @brief Enable or disable Screen Space Reflections (SSR).
 *
 * @param enabled Set to true to enable SSR, false to disable it.
 *
 * Default: false
 *}
procedure R3D_SetSSR(enabled: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSR';

{*
 * @brief Check whether Screen Space Reflections (SSR) are enabled.
 *
 * @return true if SSR is enabled, false otherwise.
 *}
function R3D_GetSSR: Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSR';

{*
 * @brief Set the maximum number of ray-marching steps for SSR.
 *
 * @param maxRaySteps The maximum number of steps taken while marching
 *        along the reflection ray. Higher values improve accuracy but
 *        increase GPU cost.
 *
 * Default: 64
 *}
procedure R3D_SetSSRMaxRaySteps(maxRaySteps: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSRMaxRaySteps';

{*
 * @brief Get the maximum number of ray-marching steps for SSR.
 *
 * @return The maximum ray-marching steps.
 *}
function R3D_GetSSRMaxRaySteps: Integer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSRMaxRaySteps';

{*
 * @brief Set the number of refinement steps for the binary search phase.
 *
 * @param binarySearchSteps The number of iterations used to refine
 *        the ray-surface intersection point after a hit is detected.
 *        More steps yield a more precise intersection.
 *
 * Default: 8
 *}
procedure R3D_SetSSRBinarySearchSteps(binarySearchSteps: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSRBinarySearchSteps';

{*
 * @brief Get the number of refinement steps for the binary search phase.
 *
 * @return The number of binary search steps.
 *}
function R3D_GetSSRBinarySearchSteps: Integer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSRBinarySearchSteps';

{*
 * @brief Set the maximum ray marching distance in view space units.
 *
 * @param rayMarchLength The maximum distance a reflection ray can travel.
 *        Larger values allow longer reflections but may cause artifacts.
 *
 * Default: 8.0
 *}
procedure R3D_SetSSRRayMarchLength(rayMarchLength: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSRRayMarchLength';

{*
 * @brief Get the maximum ray marching distance.
 *
 * @return The maximum ray marching distance.
 *}
function R3D_GetSSRRayMarchLength: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSRRayMarchLength';

{*
 * @brief Set the SSR depth thickness tolerance.
 *
 * @param depthThickness The maximum depth difference allowed between
 *        the ray position and the scene depth to consider a valid hit.
 *        Larger values increase tolerance but can cause ghosting.
 *
 * Default: 0.2
 *}
procedure R3D_SetSSRDepthThickness(depthThickness: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSRDepthThickness';

{*
 * @brief Get the SSR depth thickness tolerance.
 *
 * @return The depth thickness value.
 *}
function R3D_GetSSRDepthThickness: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSRDepthThickness';

{*
 * @brief Set the SSR depth tolerance.
 *
 * @param depthTolerance The negative margin allowed when comparing the
 *        ray position against the scene depth. This prevents false negatives
 *        due to floating-point errors or slight inconsistencies in depth
 *        reconstruction.
 *
 * In practice, a hit is accepted if:
 *    -depthTolerance <= depthDiff < depthThickness
 *
 * Smaller values increase strictness but may cause missed intersections,
 * while larger values reduce artifacts but can introduce ghosting.
 *
 * Default: 0.005
 *}
procedure R3D_SetSSRDepthTolerance(depthTolerance: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSRDepthTolerance';

{*
 * @brief Get the SSR depth tolerance.
 *
 * @return The depth tolerance value.
 *}
function R3D_GetSSRDepthTolerance: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSRDepthTolerance';

{*
 * @brief Set the fade range near the screen edges to reduce artifacts.
 *
 * @param start Normalized distance from the screen center where edge fading begins (0.0–1.0).
 * @param end   Normalized distance where fading is complete (0.0–1.0).
 *
 * Pixels outside this range will have their reflections gradually
 * faded out to avoid hard cutoffs near the borders.
 *
 * Default: start = 0.7, end = 1.0
 *}
procedure R3D_SetSSRScreenEdgeFade(start, &end: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSSRScreenEdgeFade';

{*
 * @brief Get the screen edge fade range.
 *
 * @param start Pointer to receive the fade start value.
 * @param end   Pointer to receive the fade end value.
 *}
procedure R3D_GetSSRScreenEdgeFade(start, &end: PSingle); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSSRScreenEdgeFade';

// ----------------------------------------
// ENVIRONMENT: Fog Config Functions
// ----------------------------------------

{*
 * @brief Sets the fog mode.
 *
 * This function defines the type of fog effect applied to the scene.
 * Different modes may provide linear, exponential, or volumetric fog effects.
 *
 * @param mode The fog mode to set.
 *
 * Default: R3D_FOG_DISABLED
 *}
procedure R3D_SetFogMode(mode: TR3D_Fog); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetFogMode';

{*
 * @brief Gets the current fog mode.
 *
 * This function retrieves the fog mode currently applied to the scene.
 *
 * @return The current fog mode.
 *}
function R3D_GetFogMode: TR3D_Fog; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetFogMode';

{*
 * @brief Sets the color of the fog.
 *
 * This function defines the color of the fog effect applied to the scene.
 * The fog color blends with objects as they are affected by fog.
 *
 * @param color The color to set for the fog.
 *
 * Default: WHITE
 *}
procedure R3D_SetFogColor(color: TColor); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetFogColor';

{*
 * @brief Gets the current fog color.
 *
 * This function retrieves the color currently used for the fog effect.
 *
 * @return The current fog color.
 *}
function R3D_GetFogColor: TColor; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetFogColor';

{*
 * @brief Sets the start distance of the fog.
 *
 * This function defines the distance from the camera at which fog begins to appear.
 * Objects closer than this distance will not be affected by fog.
 *
 * @param value The start distance for the fog effect.
 *
 * Default: 1.0
 *}
procedure R3D_SetFogStart(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetFogStart';

{*
 * @brief Gets the current fog start distance.
 *
 * This function retrieves the distance at which the fog begins to be applied.
 *
 * @return The current fog start distance.
 *}
function R3D_GetFogStart: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetFogStart';

{*
 * @brief Sets the end distance of the fog.
 *
 * This function defines the distance from the camera at which fog reaches full intensity.
 * Objects beyond this distance will be completely covered by fog.
 *
 * @param value The end distance for the fog effect.
 *
 * Default: 50.0
 *}
procedure R3D_SetFogEnd(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetFogEnd';

{*
 * @brief Gets the current fog end distance.
 *
 * This function retrieves the distance at which the fog is fully applied.
 *
 * @return The current fog end distance.
 *}
function R3D_GetFogEnd: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetFogEnd';

{*
 * @brief Sets the density of the fog.
 *
 * This function controls how thick the fog appears. Higher values result in
 * denser fog, making objects fade out more quickly.
 *
 * @param value The density of the fog (higher values increase fog thickness).
 *
 * Default: 0.05
 *}
procedure R3D_SetFogDensity(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetFogDensity';

{*
 * @brief Gets the current fog density.
 *
 * This function retrieves the current density of the fog.
 *
 * @return The current fog density.
 *}
function R3D_GetFogDensity: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetFogDensity';

{*
 * @brief Sets how much the fog affects the sky.
 *
 * This function controls the influence of fog on the sky color and visibility.
 * A higher value makes the fog blend more strongly with the sky, reducing its clarity.
 *
 * @param value The fog effect on the sky, in the range [0.0f, 1.0f]
 *              (0 = no effect, 1 = maximum blending).
 *
 * Default: 0.5
 *}
procedure R3D_SetFogSkyAffect(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetFogSkyAffect';

{*
 * @brief Gets the current fog effect on the sky.
 *
 * This function retrieves the current influence of fog on the sky.
 *
 * @return The current fog-sky affect value, in the range [0.0f, 1.0f].
 *}
function R3D_GetFogSkyAffect: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetFogSkyAffect';

// ----------------------------------------
// ENVIRONMENT: Depth of Field (DoF) Functions
// ----------------------------------------

{*
 * @brief Enables or disables the depth of field post-process.
 *
 * @param mode The depth of field mode to set.
 *
 * Default: R3D_DOF_DISABLED
 *}
procedure R3D_SetDofMode(mode: TR3D_Dof); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDofMode';

{*
 * @brief Gets the current depth of field mode.
 *
 * @return The current depth of field mode.
 *}
function R3D_GetDofMode: TR3D_Dof; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDofMode';

{*
 * @brief Sets the focus point in world space.
 *
 * This function defines the distance (in meters) from the camera where
 * objects will be in perfect focus. Objects closer or farther will be blurred.
 *
 * @param value The focus point distance in meters.
 *
 * Default: 10.0
 *}
procedure R3D_SetDofFocusPoint(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDofFocusPoint';

{*
 * @brief Gets the current focus point.
 *
 * @return The focus point distance in meters.
 *}
function R3D_GetDofFocusPoint: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDofFocusPoint';

{*
 * @brief Sets the focus scale.
 *
 * This function controls how shallow the depth of field effect is.
 * Lower values create a shallower depth of field with more blur,
 * while higher values create a deeper depth of field with less blur.
 *
 * @param value The focus scale value.
 *
 * Default: 1.0
 *}
procedure R3D_SetDofFocusScale(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDofFocusScale';

{*
 * @brief Gets the current focus scale.
 *
 * @return The current focus scale value.
 *}
function R3D_GetDofFocusScale: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDofFocusScale';

{*
 * @brief Sets the maximum blur size.
 *
 * This function controls the maximum amount of blur applied to out-of-focus
 * areas. This value is similar to the lens aperture size, larger values
 * create more pronounced blur effects.
 *
 * @param value The maximum blur size value.
 *
 * Default: 20.0
 *}
procedure R3D_SetDofMaxBlurSize(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDofMaxBlurSize';

{*
 * @brief Gets the current maximum blur size.
 *
 * @return The current maximum blur size value.
 *}
function R3D_GetDofMaxBlurSize: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDofMaxBlurSize';

{*
 * @brief Enables or disables depth-of-field debug mode.
 *
 * In debug mode, the scene uses color coding:
 * - Green: near blur
 * - Black: sharp areas
 * - Blue: far blur
 *
 * @param enabled true to enable, false to disable.
 *
 * Default: false
 *}
procedure R3D_SetDofDebugMode(enabled: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDofDebugMode';

{*
 * @brief Gets the current debug mode state.
 *
 * @return True if debug mode is enabled, false otherwise.
 *}
function R3D_GetDofDebugMode: Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDofDebugMode';

// ----------------------------------------
// ENVIRONMENT: Tonemap Config Functions
// ----------------------------------------

{*
 * @brief Sets the tonemapping mode.
 *
 * This function defines the tonemapping algorithm applied to the final rendered image.
 * Different tonemap modes affect color balance, brightness compression, and overall
 * scene appearance.
 *
 * @param mode The tonemap mode to set.
 *
 * Default: R3D_TONEMAP_LINEAR
 *}
procedure R3D_SetTonemapMode(mode: TR3D_Tonemap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetTonemapMode';

{*
 * @brief Gets the current tonemapping mode.
 *
 * This function retrieves the tonemap mode currently applied to the scene.
 *
 * @return The current tonemap mode.
 *}
function R3D_GetTonemapMode: TR3D_Tonemap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetTonemapMode';

{*
 * @brief Sets the exposure level for tonemapping.
 *
 * This function adjusts the exposure level used in tonemapping, affecting
 * the overall brightness of the rendered scene.
 *
 * @param value The exposure value (higher values make the scene brighter).
 *
 * Default: 1.0
 *}
procedure R3D_SetTonemapExposure(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetTonemapExposure';

{*
 * @brief Gets the current tonemap exposure level.
 *
 * This function retrieves the current exposure setting used in tonemapping.
 *
 * @return The current tonemap exposure value.
 *}
function R3D_GetTonemapExposure: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetTonemapExposure';

{*
 * @brief Sets the white point for tonemapping.
 *
 * This function defines the reference white level, which determines how bright
 * areas of the scene are mapped to the final output.
 *
 * @param value The white point value.
 *
 * Default: 1.0
 *}
procedure R3D_SetTonemapWhite(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetTonemapWhite';

{*
 * @brief Gets the current tonemap white point.
 *
 * This function retrieves the white point setting used in tonemapping.
 *
 * @return The current tonemap white value.
 *}
function R3D_GetTonemapWhite: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetTonemapWhite';

// ----------------------------------------
// ENVIRONMENT: Color Adjustment Functions
// ----------------------------------------

{*
 * @brief Sets the global brightness adjustment.
 *
 * This function controls the brightness of the final rendered image.
 * Higher values make the image brighter, while lower values darken it.
 *
 * @param value The brightness adjustment value.
 *
 * Default: 1.0
 *}
procedure R3D_SetBrightness(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBrightness';

{*
 * @brief Gets the current brightness level.
 *
 * This function retrieves the brightness setting applied to the scene.
 *
 * @return The current brightness value.
 *}
function R3D_GetBrightness: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBrightness';

{*
 * @brief Sets the global contrast adjustment.
 *
 * This function controls the contrast of the final rendered image.
 * Higher values increase the difference between dark and bright areas.
 *
 * @param value The contrast adjustment value.
 *
 * Default: 1.0
 *}
procedure R3D_SetContrast(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetContrast';

{*
 * @brief Gets the current contrast level.
 *
 * This function retrieves the contrast setting applied to the scene.
 *
 * @return The current contrast value.
 *}
function R3D_GetContrast: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetContrast';

{*
 * @brief Sets the global saturation adjustment.
 *
 * This function controls the color intensity of the final rendered image.
 * Higher values make colors more vibrant, while lower values desaturate them.
 *
 * @param value The saturation adjustment value.
 *
 * Default: 1.0
 *}
procedure R3D_SetSaturation(value: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSaturation';

{*
 * @brief Gets the current saturation level.
 *
 * This function retrieves the saturation setting applied to the scene.
 *
 * @return The current saturation value.
 *}
function R3D_GetSaturation: Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSaturation';
