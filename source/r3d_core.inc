// R3D Core Module.
// Complete translation of r3d_core.h to Pascal

// ========================================
// ENUMS TYPES
// ========================================

{*
 * @brief Configuration hints used to customize R3D before initialization.
 *
 * Hints must be set via @R3D_SetHint before calling @R3D_Init.
 * Any hint not explicitly set falls back to its default value.
 *
 * @note Some hints may be clamped internally, accordingly to hardware limits.
 *}
type
  TR3D_Hint = (
    R3D_HINT_MESH_VERTEX_BUFFER_CAPACITY,   ///< Initial vertex capacity of the global VBO. Default: 65'536
    R3D_HINT_MESH_INDEX_BUFFER_CAPACITY,    ///< Initial index capacity of the global EBO. Default: 131'072
    R3D_HINT_MESH_STREAMING_CAPACITY,       ///< Initial capacity for tracking freed mesh slots. Default: 128
    R3D_HINT_DRAW_CALL_CAPACITY,            ///< Initial capacity of the CPU-side draw call list. Default: 1024
    R3D_HINT_FORWARD_LIGHT_PER_MESH,        ///< Max lights per mesh in forward pass. Default: 16
    R3D_HINT_PROBE_MAX_ACTIVE,              ///< Max probes rendered simultaneously. Default: 8
    R3D_HINT_PROBE_CAPTURE_SIZE,            ///< Probe capture cubemap face size (px). Default: 256
    R3D_HINT_SHADOW_DIR_SIZE,               ///< Directional light shadow map size (px). Default: 4096
    R3D_HINT_SHADOW_SPOT_SIZE,              ///< Spot light shadow map size (px). Default: 2048
    R3D_HINT_SHADOW_OMNI_SIZE,              ///< Omni light shadow map size (px). Default: 2048
    R3D_HINT_IBL_IRRADIANCE_SIZE,           ///< Irradiance cubemap face size. Default: 32
    R3D_HINT_IBL_PREFILTER_SIZE,            ///< Prefiltered cubemap face size. Default: 128
    R3D_HINT_COUNT                          ///< Sentinel, not a valid hint
  );

{*
 * @brief Anti-aliasing modes used during rendering.
 *}
type
  TR3D_AntiAliasingMode = (
    R3D_ANTI_ALIASING_MODE_NONE,  ///< No anti-aliasing.
    R3D_ANTI_ALIASING_MODE_FXAA,  ///< Fast Approximate AA.
    R3D_ANTI_ALIASING_MODE_SMAA   ///< Subpixel Morphological AA.
  );

{*
 * @brief Quality presets for anti-aliasing.
 *}
type
  TR3D_AntiAliasingPreset = (
    R3D_ANTI_ALIASING_PRESET_LOW,     ///< Performance-oriented.
    R3D_ANTI_ALIASING_PRESET_MEDIUM,  ///< Balanced quality/performance.
    R3D_ANTI_ALIASING_PRESET_HIGH,    ///< High quality.
    R3D_ANTI_ALIASING_PRESET_ULTRA,   ///< Maximum quality.
    R3D_ANTI_ALIASING_PRESET_COUNT    ///< Number of presets (not valid).
  );

{*
 * @brief Aspect ratio handling modes for rendering.
 *}
type
  TR3D_AspectMode = (
    R3D_ASPECT_EXPAND,      ///< Fill target fully.
    R3D_ASPECT_KEEP         ///< Preserve aspect ratio.
  );

{*
 * @brief Upscaling/filtering methods for rendering output.
 *}
type
  TR3D_UpscaleMode = (
    R3D_UPSCALE_NEAREST,    ///< Nearest-neighbor upscaling.
    R3D_UPSCALE_LINEAR,     ///< Bilinear upscaling.
    R3D_UPSCALE_BICUBIC,    ///< Bicubic upscaling.
    R3D_UPSCALE_LANCZOS     ///< Lanczos-2 upscaling.
  );

{*
 * @brief Downscaling/filtering methods for rendering output.
 *}
type
  TR3D_DownscaleMode = (
    R3D_DOWNSCALE_NEAREST,  ///< Nearest-neighbor downscaling.
    R3D_DOWNSCALE_LINEAR,   ///< Bilinear filtering.
    R3D_DOWNSCALE_RGSS,     ///< 4-sample supersampling.
    R3D_DOWNSCALE_PDSS      ///< 16-sample supersampling.
  );

{*
 * @brief Defines the buffer to output (render texture or window).
 *}
type
  TR3D_OutputMode = (
    R3D_OUTPUT_SCENE,
    R3D_OUTPUT_ALBEDO,
    R3D_OUTPUT_NORMAL,
    R3D_OUTPUT_ORM,
    R3D_OUTPUT_DIFFUSE,
    R3D_OUTPUT_SPECULAR,
    R3D_OUTPUT_SSAO,
    R3D_OUTPUT_SSIL,
    R3D_OUTPUT_SSGI,
    R3D_OUTPUT_SSR,
    R3D_OUTPUT_BLOOM,
    R3D_OUTPUT_DOF
  );

{*
 * @brief Specifies the color space for user-provided colors and color textures.
 *}
type
  TR3D_ColorSpace = (
    R3D_COLORSPACE_LINEAR,  ///< Linear color space.
    R3D_COLORSPACE_SRGB     ///< sRGB color space.
  );

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Sets a configuration hint.
 *
 * Must be called before @R3D_Init.
 *}
procedure R3D_SetHint(hint: TR3D_Hint; value: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetHint';

{*
 * @brief Returns the effective value of a hint.
 *}
function R3D_GetHint(hint: TR3D_Hint): Integer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetHint';

{*
 * @brief Initializes the rendering engine.
 *
 * This function sets up the internal rendering system with the provided resolution.
 *
 * @param resWidth Width of the internal resolution.
 * @param resHeight Height of the internal resolution.
 *
 * @return True if the initialization is successful.
 *}
function R3D_Init(resWidth, resHeight: Integer): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Init';

{*
 * @brief Closes the rendering engine and deallocates all resources.
 *}
procedure R3D_Close; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Close';

{*
 * @brief Gets the current internal resolution.
 *
 * @param width Pointer to store the width of the internal resolution.
 * @param height Pointer to store the height of the internal resolution.
 *}
procedure R3D_GetResolution(width, height: PInteger); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetResolution';

{*
 * @brief Sets the internal rendering resolution.
 *
 * Reallocates all internal render targets to the new resolution.
 * This operation may cause a stall, this is acceptable when called
 * infrequently (like window resize events), but should never be called per-frame.
 *
 * @param width New internal width in pixels.
 * @param height New internal height in pixels.
 *}
procedure R3D_SetResolution(width, height: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetResolution';

{*
 * @brief Retrieves the current anti-aliasing mode used for rendering.
 *
 * @return The currently active TR3D_AntiAliasingMode.
 *}
function R3D_GetAntiAliasingMode(): TR3D_AntiAliasingMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAntiAliasingMode';

{*
 * @brief Sets the anti-aliasing mode for rendering.
 *
 * The new mode takes effect on subsequent frames.
 *
 * @param mode The desired TR3D_AntiAliasingMode.
 * @note If the mode is invalid, no AA will be applied.
 *}
procedure R3D_SetAntiAliasingMode(mode: TR3D_AntiAliasingMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAntiAliasingMode';

{*
 * @brief Retrieves the current anti-aliasing quality preset.
 *
 * @return The currently active TR3D_AntiAliasingPreset.
 *}
function R3D_GetAntiAliasingPreset(): TR3D_AntiAliasingPreset; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAntiAliasingPreset';

{*
 * @brief Sets the anti-aliasing quality preset.
 *
 * Changing the preset triggers an internal shader recompilation.
 * Compiled variants are cached and reused if the preset is set again.
 *
 * @param preset The desired TR3D_AntiAliasingPreset.
 * @note The preset will be a clamp between low and ultra.
 *}
procedure R3D_SetAntiAliasingPreset(preset: TR3D_AntiAliasingPreset); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAntiAliasingPreset';

{*
 * @brief Retrieves the current aspect ratio handling mode.
 * @return The currently active TR3D_AspectMode.
 *}
function R3D_GetAspectMode(): TR3D_AspectMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAspectMode';

{*
 * @brief Sets the aspect ratio handling mode for rendering.
 * @param mode The desired TR3D_AspectMode.
 *}
procedure R3D_SetAspectMode(mode: TR3D_AspectMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAspectMode';

{*
 * @brief Retrieves the current upscaling/filtering method.
 * @return The currently active TR3D_UpscaleMode.
 *}
function R3D_GetUpscaleMode(): TR3D_UpscaleMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetUpscaleMode';

{*
 * @brief Sets the upscaling/filtering method for rendering output.
 * @param mode The desired TR3D_UpscaleMode.
 *}
procedure R3D_SetUpscaleMode(mode: TR3D_UpscaleMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetUpscaleMode';

{*
 * @brief Retrieves the current downscaling mode used for rendering.
 * @return The currently active TR3D_DownscaleMode.
 *}
function R3D_GetDownscaleMode(): TR3D_DownscaleMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDownscaleMode';

{*
 * @brief Sets the downscaling mode for rendering output.
 * @param mode The desired TR3D_DownscaleMode.
 *}
procedure R3D_SetDownscaleMode(mode: TR3D_DownscaleMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDownscaleMode';

{*
 * @brief Gets the current output mode.
 * @return The currently active TR3D_OutputMode.
 *}
function R3D_GetOutputMode(): TR3D_OutputMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetOutputMode';

{*
 * @brief Sets the output mode for rendering.
 * @param mode The TR3D_OutputMode to use.
 * @note Nothing will be output if the requested target has not been created / used.
 *}
procedure R3D_SetOutputMode(mode: TR3D_OutputMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetOutputMode';

{*
 * @brief Sets the default texture filtering mode.
 *
 * This function defines the default texture filter that will be applied to all subsequently
 * loaded textures, including those used in materials, sprites, and other resources.
 *
 * If a trilinear or anisotropic filter is selected, mipmaps will be automatically generated
 * for the textures, but they will not be generated when using nearest or bilinear filtering.
 *
 * The default texture filter mode is `TEXTURE_FILTER_TRILINEAR`.
 *
 * @param filter The texture filtering mode to be applied by default.
 *}
procedure R3D_SetTextureFilter(filter: TTextureFilter); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetTextureFilter';

{*
 * @brief Sets the default texture wrap mode.
 *
 * This function only affects textures that are loaded manually for material maps.
 * Textures loaded automatically during model import will use the wrap mode
 * defined in the model file itself.
 *
 * The default texture wrap mode is `TEXTURE_WRAP_CLAMP`.
 *
 * @param wrap The texture wrap mode to apply by default.
 *}
procedure R3D_SetTextureWrap(wrap: TTextureWrap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetTextureWrap';

{*
 * @brief Set the working color space for user-provided surface colors and color textures.
 *
 * Defines how all *color inputs* should be interpreted:
 * - surface colors provided in materials (e.g. albedo/emission tints)
 * - color textures such as albedo and emission maps
 *
 * When set to sRGB, these values are converted to linear before shading.
 * When set to linear, values are used as-is.
 *
 * This does NOT affect lighting inputs (direct or indirect light),
 * which are always expected to be provided in linear space.
 *
 * The default color space is `R3D_COLORSPACE_SRGB`.
 *
 * @param space Color space to use for color inputs (linear or sRGB).
 *}
procedure R3D_SetColorSpace(space: TR3D_ColorSpace); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetColorSpace';
