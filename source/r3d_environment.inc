// R3D Environment Module.

{*
 * @brief Bloom effect blending modes.
 *
 * Specifies different blending techniques for the bloom glow effect.
 *}
type
  TR3D_Bloom = (
    R3D_BLOOM_DISABLED,     ///< No bloom effect applied
    R3D_BLOOM_MIX,          ///< Linear interpolation blend between scene and bloom
    R3D_BLOOM_ADDITIVE,     ///< Additive blending, intensifying bright regions
    R3D_BLOOM_SCREEN        ///< Screen blending for softer highlight enhancement
  );

{*
 * @brief Fog effect density distribution modes.
 *
 * Distance-based fog density distribution methods.
 *}
type
  TR3D_Fog = (
    R3D_FOG_DISABLED,       ///< No fog effect
    R3D_FOG_LINEAR,         ///< Linear density increase between start and end distances
    R3D_FOG_EXP2,           ///< Exponential squared density (exp2), more realistic
    R3D_FOG_EXP             ///< Simple exponential density increase
  );

{*
 * @brief Depth of field effect modes.
 *}
type
  TR3D_DoF = (
    R3D_DOF_DISABLED,       ///< No depth of field effect
    R3D_DOF_ENABLED         ///< Depth of field enabled with focus point and blur
  );

{*
 * @brief Tone mapping algorithms.
 *
 * HDR to LDR color compression methods.
 *}
type
  TR3D_Tonemap = (
    R3D_TONEMAP_LINEAR,     ///< Direct linear mapping (no compression)
    R3D_TONEMAP_REINHARD,   ///< Reinhard operator, balanced HDR compression
    R3D_TONEMAP_FILMIC,     ///< Film-like response curve
    R3D_TONEMAP_ACES,       ///< Academy Color Encoding System (cinematic standard)
    R3D_TONEMAP_AGX,        ///< Modern algorithm preserving highlights and shadows
    R3D_TONEMAP_COUNT       ///< Internal: number of tonemap modes
  );

{*
 * @brief Complete environment configuration structure.
 *
 * Contains all rendering environment parameters: background, lighting, and post-processing effects.
 *}
type
  PR3D_EnvBackground = ^TR3D_EnvBackground;
  TR3D_EnvBackground = record
    color: TColor;          ///< Background color when skybox is disabled
    energy: Single;         ///< Energy multiplier applied to background (skybox or color)
    sky: TR3D_Skybox;       ///< Skybox asset (used if ID is non-zero)
    rotation: TQuaternion;  ///< Skybox rotation (pitch, yaw, roll as quaternion)
  end;

  PR3D_EnvAmbient = ^TR3D_EnvAmbient;
  TR3D_EnvAmbient = record
    color: TColor;          ///< Ambient light color when skybox is disabled
    energy: Single;         ///< Energy multiplier for ambient light (skybox or color)
    reflect: Single;        ///< Reflection intensity from skybox (no effect if skybox disabled)
  end;

  PR3D_EnvSSAO = ^TR3D_EnvSSAO;
  TR3D_EnvSSAO = record
    intensity: Single;      ///< Base occlusion strength multiplier (default: 1.0)
    power: Single;          ///< Exponential falloff for sharper darkening (default: 1.0)
    radius: Single;         ///< Sampling radius in world space (default: 0.5)
    bias: Single;           ///< Depth bias to prevent self-shadowing artifacts (default: 0.025)
    lightAffect: Single;    ///< How much SSAO affects direct lighting [0.0-1.0] (default: 0.0)
    enabled: Boolean;       ///< Enable/disable SSAO effect (default: false)
  end;

  PR3D_EnvSSIL = ^TR3D_EnvSSIL;
  TR3D_EnvSSIL = record
    sampleCount: Integer;   ///< Number of samples to compute indirect lighting (default: 4)
    sliceCount: Integer;    ///< Number of depth slices for accumulation (default: 4)
    sampleRadius: Single;   ///< Maximum distance to gather light from (default: 5.0)
    hitThickness: Single;   ///< Thickness threshold for occluders (default: 0.5)
    aoPower: Single;        ///< Exponential falloff for visibility factor (default: 1.0)
    energy: Single;         ///< Multiplier for indirect light intensity (default: 1.0)
    enabled: Boolean;       ///< Enable/disable SSIL effect (default: false)
  end;

  PR3D_EnvBloom = ^TR3D_EnvBloom;
  TR3D_EnvBloom = record
    mode: TR3D_Bloom;       ///< Bloom blending mode (default: R3D_BLOOM_DISABLED)
    levels: Single;         ///< Mipmap spread factor [0-1]: higher = wider glow (default: 0.5)
    intensity: Single;      ///< Bloom strength multiplier (default: 0.05)
    threshold: Single;      ///< Minimum brightness to trigger bloom (default: 0.0)
    softThreshold: Single;  ///< Softness of brightness cutoff transition (default: 0.5)
    filterRadius: Single;   ///< Blur filter radius during upscaling (default: 1.0)
  end;

  PR3D_EnvSSR = ^TR3D_EnvSSR;
  TR3D_EnvSSR = record
    maxRaySteps: Integer;       ///< Maximum ray marching iterations (default: 64)
    binarySearchSteps: Integer; ///< Refinement steps for intersection (default: 8)
    rayMarchLength: Single;     ///< Maximum ray distance in view space (default: 8.0)
    depthThickness: Single;     ///< Depth tolerance for valid hits (default: 0.2)
    depthTolerance: Single;     ///< Negative margin to prevent false negatives (default: 0.005)
    edgeFadeStart: Single;      ///< Screen edge fade start [0-1] (default: 0.7)
    edgeFadeEnd: Single;        ///< Screen edge fade end [0-1] (default: 1.0)
    enabled: Boolean;           ///< Enable/disable SSR (default: false)
  end;

  PR3D_EnvFog = ^TR3D_EnvFog;
  TR3D_EnvFog = record
    mode: TR3D_Fog;         ///< Fog distribution mode (default: R3D_FOG_DISABLED)
    color: TColor;          ///< Fog tint color (default: white)
    start: Single;          ///< Linear mode: distance where fog begins (default: 1.0)
    end_: Single;           ///< Linear mode: distance of full fog density (default: 50.0)
    density: Single;        ///< Exponential modes: fog thickness factor (default: 0.05)
    skyAffect: Single;      ///< Fog influence on skybox [0-1] (default: 0.5)
  end;

  PR3D_EnvDoF = ^TR3D_EnvDoF;
  TR3D_EnvDoF = record
    mode: TR3D_DoF;         ///< Enable/disable state (default: R3D_DOF_DISABLED)
    focusPoint: Single;     ///< Focus distance in meters from camera (default: 10.0)
    focusScale: Single;     ///< Depth of field depth: lower = shallower (default: 1.0)
    maxBlurSize: Single;    ///< Maximum blur radius, similar to aperture (default: 20.0)
    debugMode: Boolean;     ///< Color-coded visualization: green=near, blue=far (default: false)
  end;

  PR3D_EnvTonemap = ^TR3D_EnvTonemap;
  TR3D_EnvTonemap = record
    mode: TR3D_Tonemap;     ///< Tone mapping algorithm (default: R3D_TONEMAP_LINEAR)
    exposure: Single;       ///< Scene brightness multiplier (default: 1.0)
    white: Single;          ///< Reference white point (not used for AGX) (default: 1.0)
  end;

  PR3D_EnvColor = ^TR3D_EnvColor;
  TR3D_EnvColor = record
    brightness: Single;     ///< Overall brightness multiplier (default: 1.0)
    contrast: Single;       ///< Contrast between dark and bright areas (default: 1.0)
    saturation: Single;     ///< Color intensity (default: 1.0)
  end;

  TR3D_Environment = record
    background: TR3D_EnvBackground; ///< Background and skybox settings
    ambient: TR3D_EnvAmbient;       ///< Ambient lighting configuration
    ssao: TR3D_EnvSSAO;             ///< Screen space ambient occlusion
    ssil: TR3D_EnvSSIL;             ///< Screen space indirect lighting
    bloom: TR3D_EnvBloom;           ///< Bloom glow effect
    ssr: TR3D_EnvSSR;               ///< Screen space reflections
    fog: TR3D_EnvFog;               ///< Atmospheric fog
    dof: TR3D_EnvDoF;               ///< Depth of field focus effect
    tonemap: TR3D_EnvTonemap;       ///< HDR tone mapping
    color: TR3D_EnvColor;           ///< Color grading adjustments
  end;
  PR3D_Environment = ^TR3D_Environment;

// ========================================
// Helper Macro Equivalents
// ========================================

{*
 * @brief Quick read access to environment members.
 *
 * @param member The environment member path (e.g., bloom.intensity)
 * @return The current value of the specified member
 *
 * Example: `intensity := R3D_GetEnvironment()^.bloom.intensity;`
 *}

{*
 * @brief Quick write access to environment members.
 *
 * @param member The environment member path (e.g., bloom.intensity)
 * @param value The value to assign
 *
 * Example: `R3D_GetEnvironment()^.bloom.intensity := 0.05;`
 *}

// ========================================
// Public API Functions
// ========================================

{*
 * @brief Retrieves a pointer to the current environment configuration.
 *
 * Provides direct read/write access to environment settings.
 * Modifications take effect immediately.
 *
 * @return Pointer to the active R3D_Environment structure
 *}
function R3D_GetEnvironment: PR3D_Environment; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetEnvironment';

{*
 * @brief Replaces the entire environment configuration.
 *
 * Copies all settings from the provided structure to the active environment.
 * Useful for switching between presets or restoring saved states.
 *
 * @param env Pointer to the R3D_Environment structure to copy from
 *}
procedure R3D_SetEnvironment(env: PR3D_Environment); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetEnvironment';

{*
 * @brief Default environment configuration.
 *
 * Initializes an R3D_Environment structure with sensible default values for all
 * rendering parameters. Use this as a starting point for custom configurations.
 *}
(*const
  R3D_ENVIRONMENT_BASE: TR3D_Environment = (
    background: (
      color: (r: 80; g: 80; b: 80; a: 255);
      energy: 1.0;
      sky: (0);
      rotation: (x: 0.0; y: 0.0; z: 0.0; w: 1.0)
    );
    ambient: (
      color: (r: 0; g: 0; b: 0; a: 255);
      energy: 1.0;
      reflect: 1.0
    );
    ssao: (
      intensity: 1.0;
      power: 1.0;
      radius: 0.5;
      bias: 0.025;
      lightAffect: 0.0;
      enabled: False
    );
    ssil: (
      sampleCount: 4;
      sliceCount: 4;
      sampleRadius: 5.0;
      hitThickness: 0.5;
      aoPower: 1.0;
      energy: 1.0;
      enabled: False
    );
    bloom: (
      mode: R3D_BLOOM_DISABLED;
      levels: 0.5;
      intensity: 0.05;
      threshold: 0.0;
      softThreshold: 0.5;
      filterRadius: 1.0
    );
    ssr: (
      maxRaySteps: 64;
      binarySearchSteps: 8;
      rayMarchLength: 8.0;
      depthThickness: 0.2;
      depthTolerance: 0.005;
      edgeFadeStart: 0.7;
      edgeFadeEnd: 1.0;
      enabled: False
    );
    fog: (
      mode: R3D_FOG_DISABLED;
      color: (r: 255; g: 255; b: 255; a: 255);
      start: 1.0;
      end_: 50.0;
      density: 0.05;
      skyAffect: 0.5
    );
    dof: (
      mode: R3D_DOF_DISABLED;
      focusPoint: 10.0;
      focusScale: 1.0;
      maxBlurSize: 20.0;
      debugMode: False
    );
    tonemap: (
      mode: R3D_TONEMAP_LINEAR;
      exposure: 1.0;
      white: 1.0
    );
    color: (
      brightness: 1.0;
      contrast: 1.0;
      saturation: 1.0
    )
  ); *)
