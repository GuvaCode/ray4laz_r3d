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

// ========================================
// STRUCT TYPES
// ========================================

{*
 * @brief Describes a light source.
 *}
type
  TR3D_Light = record
    position: TVector3;       ///< Light position (spot/omni)
    direction: TVector3;      ///< Light direction (spot/dir)
    color: TColor;            ///< Light color
    energy: Single;           ///< Light intensity/brightness multiplier
    specular: Single;         ///< Specular reflection intensity multiplier
    range: Single;            ///< Maximum distance (spot/omni)
    falloff: Single;          ///< Distance falloff factor (spot/omni)
    innerCutOff: Single;      ///< Spot light inner cutoff angle (degrees)
    outerCutOff: Single;      ///< Spot light outer cutoff angle (degrees)
    fogEnergy: Single;        ///< Volumetric fog energy multiplier
    type_: TR3D_LightType;    ///< Light type (directional/spot/omni)
  end;

{*
 * @brief Represents an allocated shadow map for a light.
 *}
type
  TR3D_ShadowMap = record
    handle: UInt32;           ///< Internal shadow map handle (don't touch)
    softness: Single;         ///< Softness factor for penumbra
    opacity: Single;          ///< Shadow opacity factor
    depthBias: Single;        ///< Constant depth bias
    slopeBias: Single;        ///< Slope-scaled depth bias
    cullMask: TR3D_Layer;     ///< Layers considered when culling shadow casters for this map
    type_: TR3D_LightType;    ///< Light type this shadow map was allocated for
  end;

// ========================================
// PUBLIC API
// ========================================

// ----------------------------------------
// LIGHTING: Lights Functions
// ----------------------------------------

{*
 * @brief Creates a directional light.
 *}
function R3D_CreateDirLight(dir: TVector3; color: TColor; energy: Single): TR3D_Light; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateDirLight';

{*
 * @brief Creates a spot light, with default inner/outer cutoff angles of 22.5/45 degrees.
 *}
function R3D_CreateSpotLight(pos: TVector3; dir: TVector3; range: Single; color: TColor; energy: Single): TR3D_Light; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateSpotLight';

{*
 * @brief Creates an omnidirectional (point) light.
 *}
function R3D_CreateOmniLight(pos: TVector3; range: Single; color: TColor; energy: Single): TR3D_Light; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateOmniLight';

// ----------------------------------------
// LIGHTING: Shadow Functions
// ----------------------------------------

{*
 * @brief Allocates a shadow map layer for a given light type.
 *
 * The shadow map resolution is fixed per light type and configured via
 * R3D_HINT_SHADOW_DIR_SIZE, R3D_HINT_SHADOW_SPOT_SIZE and R3D_HINT_SHADOW_OMNI_SIZE
 * before R3D is initialized.
 *
 * @param type_ The light type this shadow map will be used with (must match
 *              the type of the light it is later passed to via R3D_PushLight()).
 *}
function R3D_LoadShadowMap(type_: TR3D_LightType): TR3D_ShadowMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadShadowMap';

{*
 * @brief Releases a shadow map, freeing its layer for reuse.
 *}
procedure R3D_UnloadShadowMap(shadowMap: TR3D_ShadowMap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadShadowMap';

{*
 * @brief Returns whether the shadow map has a valid allocated layer.
 *}
function R3D_IsShadowMapValid(shadowMap: TR3D_ShadowMap): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsShadowMapValid';

// ----------------------------------------
// LIGHTING: Light Helper Functions
// ----------------------------------------

{*
 * @brief Draws the area of influence of the light in 3D space.
 *
 * This function visualizes the area affected by a light in 3D space.
 * It draws the light's influence, such as the cone for spotlights or the volume for omni-lights.
 * This function is only relevant for spotlights and omni-lights.
 *
 * @note This function should be called while using the default 3D rendering mode of raylib,
 *       not with r3d's rendering mode. It uses raylib's 3D drawing functions to render the light's shape.
 *
 * @param light The light to visualize (see TR3D_Light).
 *}
procedure R3D_DrawLightDebug(light: TR3D_Light); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawLightDebug';
