// R3D Core Module.
// Complete translation of r3d_core.h to Pascal

// ========================================
// ENUMS TYPES
// ========================================

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

function R3D_Init(resWidth, resHeight: Integer): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Init';

procedure R3D_Close; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Close';

procedure R3D_GetResolution(width, height: PInteger); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetResolution';

procedure R3D_SetResolution(width, height: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetResolution';

function R3D_GetAntiAliasingMode(): TR3D_AntiAliasingMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAntiAliasingMode';

procedure R3D_SetAntiAliasingMode(mode: TR3D_AntiAliasingMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAntiAliasingMode';

function R3D_GetAntiAliasingPreset(): TR3D_AntiAliasingPreset; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAntiAliasingPreset';

procedure R3D_SetAntiAliasingPreset(preset: TR3D_AntiAliasingPreset); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAntiAliasingPreset';

function R3D_GetAspectMode(): TR3D_AspectMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAspectMode';

procedure R3D_SetAspectMode(mode: TR3D_AspectMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAspectMode';

function R3D_GetUpscaleMode(): TR3D_UpscaleMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetUpscaleMode';

procedure R3D_SetUpscaleMode(mode: TR3D_UpscaleMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetUpscaleMode';

function R3D_GetDownscaleMode(): TR3D_DownscaleMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDownscaleMode';

procedure R3D_SetDownscaleMode(mode: TR3D_DownscaleMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDownscaleMode';

function R3D_GetOutputMode(): TR3D_OutputMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetOutputMode';

procedure R3D_SetOutputMode(mode: TR3D_OutputMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetOutputMode';

procedure R3D_SetTextureFilter(filter: TTextureFilter); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetTextureFilter';

procedure R3D_SetTextureWrap(wrap: TTextureWrap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetTextureWrap';

procedure R3D_SetColorSpace(space: TR3D_ColorSpace); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetColorSpace';
