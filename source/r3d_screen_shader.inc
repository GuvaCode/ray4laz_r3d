// R3D Screen Shader Module.

// ========================================
// OPAQUE TYPES
// ========================================

type
  {*
   * @brief Screen shader opaque type.
   *
   * Opaque handle to a screen shader object.
   * Use the provided API functions to create, configure, and destroy shaders.
   *}
  TR3D_ScreenShader = record
    // Opaque type
  end;
  PR3D_ScreenShader = ^TR3D_ScreenShader;
  PPR3D_ScreenShader = ^PR3D_ScreenShader;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Loads a screen shader from a file.
 *
 * The shader must define a single entry point:
 * `void fragment()`. Any other entry point, such as `vertex()`,
 * or any varyings will be ignored.
 *
 * @param filePath Path to the shader source file.
 * @return Pointer to the loaded screen shader, or nil on failure.
 *}
function R3D_LoadScreenShader(const filePath: PAnsiChar): PR3D_ScreenShader; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadScreenShader';

{*
 * @brief Loads a screen shader from a source code string in memory.
 *
 * The shader must define a single entry point:
 * `void fragment()`. Any other entry point, such as `vertex()`,
 * or any varyings will be ignored.
 *
 * @param code Null-terminated shader source code.
 * @return Pointer to the loaded screen shader, or nil on failure.
 *}
function R3D_LoadScreenShaderFromMemory(const code: PAnsiChar): PR3D_ScreenShader; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadScreenShaderFromMemory';

{*
 * @brief Creates an alias of an existing screen shader.
 *
 * The alias shares the same compiled program as the original but holds its own
 * independent uniform and sampler state. Typical use cases include pre-configuring
 * aliases for distinct effects (e.g. different convolution kernels), or running
 * the same shader multiple times in a post-process chain with different parameters
 * at each pass.
 *
 * Uniform and sampler state is copied from the original at the moment this
 * function is called, not from the shader source defaults. Any values set
 * on the original after compilation but before this call will be reflected
 * in the alias; values set afterward will not.
 *
 * @note The alias does not own the program. Always unload all aliases before
 *       unloading the original, or the alias program references become dangling.
 *
 * @param shader The original screen shader to alias.
 * @return Pointer to the alias, or nil on failure.
 *}
function R3D_LoadScreenShaderAlias(shader: PR3D_ScreenShader): PR3D_ScreenShader; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadScreenShaderAlias';

{*
 * @brief Unloads and destroys a screen shader.
 *
 * If the shader owns its program shaders (i.e. it was created with
 * R3D_LoadScreenShader or R3D_LoadScreenShaderFromMemory), they are deleted.
 * Aliases created from this shader via R3D_LoadScreenShaderAlias must be
 * unloaded beforehand, as they share the same programs and will be left
 * with dangling references.
 *
 * @param shader Screen shader to unload.
 *}
procedure R3D_UnloadScreenShader(shader: PR3D_ScreenShader); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadScreenShader';

{*
 * @brief Sets a uniform value for the current frame.
 *
 * Once a uniform is set, it remains valid for the all frames.
 * If an uniform is set multiple times during the same frame,
 * the last value defined before R3D_End() is used.
 *
 * Supported types:
 * bool, int, float,
 * ivec2, ivec3, ivec4,
 * vec2, vec3, vec4,
 * mat2, mat3, mat4
 *
 * @warning Boolean values are read as 4 bytes.
 *
 * @param shader Target screen shader.
 *               May be NULL. In that case, the call is ignored
 *               and a warning is logged.
 * @param name   Name of the uniform. Must not be NULL.
 * @param value  Pointer to the uniform value. Must not be NULL.
 *}
procedure R3D_SetScreenShaderUniform(shader: PR3D_ScreenShader; const name: PAnsiChar; const value: Pointer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetScreenShaderUniform';

{*
 * @brief Sets a texture sampler for the current frame.
 *
 * Once a sampler is set, it remains valid for all frames.
 * If a sampler is set multiple times during the same frame,
 * the last value defined before R3D_End() is used.
 *
 * Supported samplers:
 * sampler1D, sampler2D, sampler3D, samplerCube
 *
 * @param shader  Target screen shader.
 *                May be NULL. In that case, the call is ignored
 *                and a warning is logged.
 * @param name    Name of the sampler uniform. Must not be NULL.
 * @param texture Texture to bind to the sampler.
 *}
procedure R3D_SetScreenShaderSampler(shader: PR3D_ScreenShader; const name: PAnsiChar; texture: TTexture); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetScreenShaderSampler';

{*
 * @brief Sets the list of screen shaders to execute at the end of the frame.
 *
 * The maximum number of shaders is defined by `R3D_MAX_SCREEN_SHADERS`.
 * If the provided count exceeds this limit, a warning is emitted and only
 * the first `R3D_MAX_SCREEN_SHADERS` shaders are used.
 *
 * Shader pointers are copied internally, so the original array can be modified or freed after the call.
 * NULL entries are allowed safely within the list.
 *
 * Calling this function resets all internal screen shaders before copying the new list.
 * To disable all screen shaders, call this function with `shaders = NULL` and/or `count = 0`.
 *
 * @param shaders Array of pointers to R3D_ScreenShader objects.
 * @param count Number of shaders in the array.
 *}
procedure R3D_SetScreenShaderChain(shaders: PPR3D_ScreenShader; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetScreenShaderChain';
