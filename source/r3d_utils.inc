// R3D Utility Module.

{*
 * @brief Retrieves a default white texture.
 *
 * This texture is fully white (1,1,1,1), useful for default material properties.
 *
 * @return A white texture.
 *}
function R3D_GetWhiteTexture: TTexture2D; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetWhiteTexture';

{*
 * @brief Retrieves a default black texture.
 *
 * This texture is fully black (0,0,0,1), useful for masking or default values.
 *
 * @return A black texture.
 *}
function R3D_GetBlackTexture: TTexture2D; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBlackTexture';

{*
 * @brief Retrieves a default normal map texture.
 *
 * This texture represents a neutral normal map (0.5, 0.5, 1.0), which applies no normal variation.
 *
 * @return A neutral normal texture.
 *}
function R3D_GetNormalTexture: TTexture2D; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetNormalTexture';

// ----------------------------------------
// UTILS: Render Texture Retrieval Functions
// ----------------------------------------

{*
 * @brief Retrieves the final scene color buffer.
 *
 * This texture stores the final rendered scene as a 24-bit RGB buffer.
 *
 * @return The final color buffer texture.
 *}
function R3D_GetBufferColor: TTexture2D; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBufferColor';

{*
 * @brief Retrieves the buffer containing the scene's normal data.
 *
 * This texture stores octahedral-compressed normals using two 16-bit per-channel RG components.
 *
 * @note You can find the decoding functions in the embedded shaders, such as 'screen/lighting.fs.glsl'.
 *
 * @return The normal buffer texture.
 *}
function R3D_GetBufferNormal: TTexture2D; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBufferNormal';

{*
 * @brief Retrieves the final depth buffer.
 *
 * This texture contains the depth stored in 24 bits and a stencil buffer where each value is 0 or 1, indicating the presence of geometry.
 * It is useful for post-processing effects outside of R3D.
 *
 * @note If you modify the texture parameters to sample the stencil instead of the depth,
 * make sure to reset the parameters afterward.
 *
 * @return The final depth buffer texture.
 *}
function R3D_GetBufferDepth: TTexture2D; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBufferDepth';

// ----------------------------------------
// UTILS: Camera Matrices Retrieval Functions
// ----------------------------------------

{*
 * @brief Retrieves the view matrix.
 *
 * This matrix represents the camera's transformation from world space to view space.
 * It is updated at the last call to 'R3D_Begin'.
 *
 * @return The current view matrix.
 *}
function R3D_GetMatrixView: TMatrix; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetMatrixView';

{*
 * @brief Retrieves the inverse view matrix.
 *
 * This matrix transforms coordinates from view space back to world space.
 * It is updated at the last call to 'R3D_Begin'.
 *
 * @return The current inverse view matrix.
 *}
function R3D_GetMatrixInvView: TMatrix; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetMatrixInvView';

{*
 * @brief Retrieves the projection matrix.
 *
 * This matrix defines the transformation from view space to clip space.
 * It is updated at the last call to 'R3D_Begin'.
 *
 * @return The current projection matrix.
 *}
function R3D_GetMatrixProjection: TMatrix; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetMatrixProjection';

{*
 * @brief Retrieves the inverse projection matrix.
 *
 * This matrix transforms coordinates from clip space back to view space.
 * It is updated at the last call to 'R3D_Begin'.
 *
 * @return The current inverse projection matrix.
 *}
function R3D_GetMatrixInvProjection: TMatrix; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetMatrixInvProjection';

// ----------------------------------------
// UTILS: Debug Buffer Rendering Functions
// ----------------------------------------

{*
 * @brief Renders the internal albedo buffer to the screen.
 *
 * This function displays the albedo (diffuse) buffer as a 2D texture.
 * It must be called outside of `R3D_Begin` and `R3D_End`.
 *
 * @param x X position to draw the buffer.
 * @param y Y position to draw the buffer.
 * @param w Width of the drawn buffer.
 * @param h Height of the drawn buffer.
 *}
procedure R3D_DrawBufferAlbedo(x, y, w, h: Single); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawBufferAlbedo';

{*
 * @brief Renders the internal emission buffer to the screen.
 *
 * Displays the emission buffer, which contains emissive lighting data.
 * Must be called outside of `R3D_Begin` and `R3D_End`.
 *
 * @param x X position to draw the buffer.
 * @param y Y position to draw the buffer.
 * @param w Width of the drawn buffer.
 * @param h Height of the drawn buffer.
 *}
procedure R3D_DrawBufferEmission(x, y, w, h: Single); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawBufferEmission';

{*
 * @brief Renders the internal normal buffer to the screen.
 *
 * Displays the normal buffer, showing world-space normal data as colors.
 * Must be called outside of `R3D_Begin` and `R3D_End`.
 *
 * @param x X position to draw the buffer.
 * @param y Y position to draw the buffer.
 * @param w Width of the drawn buffer.
 * @param h Height of the drawn buffer.
 *}
procedure R3D_DrawBufferNormal(x, y, w, h: Single); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawBufferNormal';

{*
 * @brief Renders the ORM (Occlusion, Roughness, Metalness) buffer to the screen.
 *
 * Displays the ORM buffer, where each channel stores different material properties:
 * - Red: Ambient occlusion
 * - Green: Roughness
 * - Blue: Metalness
 *
 * Must be called outside of `R3D_Begin` and `R3D_End`.
 *
 * @param x X position to draw the buffer.
 * @param y Y position to draw the buffer.
 * @param w Width of the drawn buffer.
 * @param h Height of the drawn buffer.
 *}
procedure R3D_DrawBufferORM(x, y, w, h: Single); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawBufferORM';

{*
 * @brief Renders the SSAO (Screen Space Ambient Occlusion) buffer to the screen.
 *
 * Displays the SSAO buffer, showing ambient occlusion data in grayscale.
 * Must be called outside of `R3D_Begin` and `R3D_End`.
 *
 * @param x X position to draw the buffer.
 * @param y Y position to draw the buffer.
 * @param w Width of the drawn buffer.
 * @param h Height of the drawn buffer.
 *}
procedure R3D_DrawBufferSSAO(x, y, w, h: Single); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawBufferSSAO';

{*
 * @brief Renders the bloom buffer to the screen.
 *
 * Displays the bloom effect buffer, showing the extracted bright areas after blur processing.
 * Must be called outside of `R3D_Begin` and `R3D_End`.
 *
 * @param x X position to draw the buffer.
 * @param y Y position to draw the buffer.
 * @param w Width of the drawn buffer.
 * @param h Height of the drawn buffer.
 *}
procedure R3D_DrawBufferBloom(x, y, w, h: Single); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawBufferBloom';


