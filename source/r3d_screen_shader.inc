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
 * @brief Unloads and destroys a screen shader.
 *
 * @param shader Screen shader to unload.
 *}
procedure R3D_UnloadScreenShader(shader: PR3D_ScreenShader); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadScreenShader';

{*
 * @brief Sets a uniform value for the current frame.
 *
 * Once a uniform is set, it remains valid for all frames.
 * If a uniform is set multiple times during the same frame,
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
 * @param name Name of the uniform.
 * @param value Pointer to the uniform value.
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
 * @param shader Target screen shader.
 * @param name Name of the sampler uniform.
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



