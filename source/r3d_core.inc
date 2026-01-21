// R3D Core Module.
{*
 * @brief Bitfield type used to specify rendering layers for 3D objects.
 *
 * This type is used by `R3D_Mesh` and `R3D_Sprite` objects to indicate
 * which rendering layer(s) they belong to. Active layers are controlled
 * globally via the functions:
 *
 * - void R3D_EnableLayers(R3D_Layer bitfield);
 * - void R3D_DisableLayers(R3D_Layer bitfield);
 *
 * A mesh or sprite will be rendered if at least one of its assigned layers is active.
 *
 * For simplicity, 16 layers are defined in this header, but the maximum number
 * of layers is 32 for an uint32_t.
 *}
type
  TR3D_Layer = LongWord;

const
  R3D_LAYER_01 = (1 shl 0);
  R3D_LAYER_02 = (1 shl 1);
  R3D_LAYER_03 = (1 shl 2);
  R3D_LAYER_04 = (1 shl 3);
  R3D_LAYER_05 = (1 shl 4);
  R3D_LAYER_06 = (1 shl 5);
  R3D_LAYER_07 = (1 shl 6);
  R3D_LAYER_08 = (1 shl 7);
  R3D_LAYER_09 = (1 shl 8);
  R3D_LAYER_10 = (1 shl 9);
  R3D_LAYER_11 = (1 shl 10);
  R3D_LAYER_12 = (1 shl 11);
  R3D_LAYER_13 = (1 shl 12);
  R3D_LAYER_14 = (1 shl 13);
  R3D_LAYER_15 = (1 shl 14);
  R3D_LAYER_16 = (1 shl 15);
  R3D_LAYER_ALL = $FFFFFFFF;

{*
 * @brief Anti-aliasing modes for rendering.
 *}
 type
   TR3D_AntiAliasing = (
     R3D_ANTI_ALIASING_DISABLED, ///< Anti-aliasing is disabled. Edges may appear jagged.
     R3D_ANTI_ALIASING_FXAA     ///< FXAA is applied. Smooths edges efficiently but may appear blurry.
     );

{*
 * @brief Aspect ratio handling modes for rendering.
 *}
type
  TR3D_AspectMode = (
    R3D_ASPECT_EXPAND,      ///< Expands the rendered output to fully fill the target (render texture or window).
    R3D_ASPECT_KEEP         ///< Preserves the target's aspect ratio without distortion, adding empty gaps if necessary.
    );

{*
 * @brief Upscaling/filtering methods for rendering output.
 *
 * Upscale mode to apply when the output window is larger than the internal render resolution.
 *}
type
  TR3D_UpscaleMode = (
    R3D_UPSCALE_NEAREST,    ///< Nearest-neighbor upscaling: very fast, but produces blocky pixels.
    R3D_UPSCALE_LINEAR,     ///< Bilinear upscaling: very fast, smoother than nearest, but can appear blurry.
    R3D_UPSCALE_BICUBIC,    ///< Bicubic (Catmull-Rom) upscaling: slower, smoother, and less blurry than linear.
    R3D_UPSCALE_LANCZOS     ///< Lanczos-2 upscaling: preserves more fine details, but is the most expensive.
    );

{*
 * @brief Downscaling/filtering methods for rendering output.
 *
 * Downscale mode to apply when the output window is smaller than the internal render resolution.
 *}
type
  TR3D_DownscaleMode = (
    R3D_DOWNSCALE_NEAREST,  ///< Nearest-neighbor downscaling: very fast, but produces aliasing.
    R3D_DOWNSCALE_LINEAR,   ///< Bilinear downscaling: very fast, can serve as a basic form of anti-aliasing (SSAA).
    R3D_DOWNSCALE_BOX       ///< Box-blur downscaling: uses a simple but effective box blur, slightly more expensive than linear, smooths moiré better.
    );

{*
 * @brief Defines the buffer to output (render texture or window).
 * @note Nothing will be output if the requested target has not been created / used.
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
    R3D_OUTPUT_SSR,
    R3D_OUTPUT_BLOOM
  );

{*
 * @brief Specifies the color space for user-provided colors and color textures.
 *
 * This enum defines how colors are interpreted for material inputs:
 * - Surface colors (e.g., albedo or emission tint)
 * - Color textures (albedo, emission maps)
 *
 * Lighting values (direct or indirect light) are always linear and
 * are not affected by this setting.
 *
 * Used with `R3D_SetColorSpace()` to control whether input colors
 * should be treated as linear or sRGB.
 *}
type
  TR3D_ColorSpace = (
    R3D_COLORSPACE_LINEAR,  ///< Linear color space: values are used as-is.
    R3D_COLORSPACE_SRGB     ///< sRGB color space: values are converted to linear on load.
    );



// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Initializes the rendering engine.
 *
 * This function sets up the internal rendering system with the provided resolution.
 *
 * @param resWidth Width of the internal resolution.
 * @param resHeight Height of the internal resolution.
 *}
procedure R3D_Init(resWidth, resHeight: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Init';

{*
 * @brief Closes the rendering engine and deallocates all resources.
 *
 * This function shuts down the rendering system and frees all allocated memory,
 * including the resources associated with the created lights.
 *}
procedure R3D_Close; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Close';

{*
 * @brief Gets the current internal resolution.
 *
 * This function retrieves the current internal resolution being used by the
 * rendering engine.
 *
 * @param width Pointer to store the width of the internal resolution.
 * @param height Pointer to store the height of the internal resolution.
 *}
procedure R3D_GetResolution(width, height: PInteger); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetResolution';

{*
 * @brief Updates the internal resolution.
 *
 * This function changes the internal resolution of the rendering engine. Note that
 * this process destroys and recreates all framebuffers, which may be a slow operation.
 *
 * @param width The new width for the internal resolution.
 * @param height The new height for the internal resolution.
 *
 * @warning This function may be slow due to the destruction and recreation of framebuffers.
 *}
procedure R3D_UpdateResolution(width, height: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateResolution';

{*
 * @brief Retrieves the current anti-aliasing mode used for rendering.
 * @return The currently active R3D_AntiAliasing mode.
 *}
function R3D_GetAntiAliasing(): TR3D_AntiAliasing; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAntiAliasing';

{*
 * @brief Sets the anti-aliasing mode for rendering.
 * @param mode The desired R3D_AntiAliasing mode.
 *}
procedure R3D_SetAntiAliasing(mode: TR3D_AntiAliasing); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAntiAliasing';

{*
 * @brief Retrieves the current aspect ratio handling mode.
 * @return The currently active R3D_AspectMode.
 *}
function R3D_GetAspectMode(): TR3D_AspectMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAspectMode';

{*
 * @brief Sets the aspect ratio handling mode for rendering.
 * @param mode The desired R3D_AspectMode.
 *}
procedure R3D_SetAspectMode(mode: TR3D_AspectMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAspectMode';

{*
 * @brief Retrieves the current upscaling/filtering method.
 * @return The currently active R3D_UpscaleMode.
 *}
function R3D_GetUpscaleMode(): TR3D_UpscaleMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetUpscaleMode';

{*
 * @brief Sets the upscaling/filtering method for rendering output.
 * @param mode The desired R3D_UpscaleMode.
 *}
procedure R3D_SetUpscaleMode(mode: TR3D_UpscaleMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetUpscaleMode';

{*
 * @brief Retrieves the current downscaling mode used for rendering.
 * @return The currently active R3D_DownscaleMode.
 *}
function R3D_GetDownscaleMode(): TR3D_DownscaleMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDownscaleMode';

{*
 * @brief Sets the downscaling mode for rendering output.
 * @param mode The desired R3D_DownscaleMode.
 *}
procedure R3D_SetDownscaleMode(mode: TR3D_DownscaleMode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDownscaleMode';

{*
 * @brief Gets the current output mode.
 * @return The currently active R3D_OutputMode.
 *}
function R3D_GetOutputMode(): TR3D_OutputMode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetOutputMode';

{*
 * @brief Sets the output mode for rendering.
 * @param mode The R3D_OutputMode to use.
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

{*
 * @brief Get the currently active global rendering layers.
 *
 * Returns the bitfield representing the currently active layers in the renderer.
 * By default, the internal active layers are set to 0, which means that any
 * non-zero layer assigned to an object will NOT be rendered unless explicitly
 * activated.
 *
 * @return R3D_Layer Bitfield of active layers.
 *}
function R3D_GetActiveLayers: TR3D_Layer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetActiveLayers';

{*
 * @brief Set the active global rendering layers.
 *
 * Replaces the current set of active layers with the given bitfield.
 *
  * @param bitfield Bitfield representing the layers to activate.
 *}
procedure R3D_SetActiveLayers(bitfield: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetActiveLayers';

{*
 * @brief Enable one or more layers without affecting other active layers.
 *
 * This function sets the bits in the global active layers corresponding to
 * the bits in the provided bitfield. Layers already active remain active.
 *
 * @param bitfield Bitfield representing one or more layers to enable.
 *}
procedure R3D_EnableLayers(bitfield: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EnableLayers';

{*
 * @brief Disable one or more layers without affecting other active layers.
 *
 * This function clears the bits in the global active layers corresponding to
 * the bits in the provided bitfield. Layers not included in the bitfield
 * remain unchanged.
 *
 * @param bitfield Bitfield representing one or more layers to disable.
 *}
procedure R3D_DisableLayers(bitfield: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DisableLayers';
