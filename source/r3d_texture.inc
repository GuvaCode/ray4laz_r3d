// R3D Texture Module.

{*
 * @brief Loads a texture from the specified file path.
 *
 * If 'isColor' is true, the texture is loaded using the currently defined color space (sRGB by default).
 * The wrap and filter modes are taken from the current global default state.
 *
 * @param fileName The path to the texture file.
 * @param isColor Whether the texture should be treated as color data.
 * @return The loaded texture.
 *}
function R3D_LoadTexture(const fileName: PAnsiChar; isColor: Boolean): TTexture2D; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadTexture';

{*
 * @brief Loads a texture from the specified file path.
 *
 * If 'isColor' is true, the texture is loaded using the currently defined color space (sRGB by default).
 *
 * @param fileName The path to the texture file.
 * @param wrap The texture wrap mode.
 * @param filter The texture filter mode.
 * @param isColor Whether the texture should be treated as color data.
 * @return The loaded texture.
 *}
function R3D_LoadTextureEx(const fileName: PAnsiChar; wrap: TTextureWrap; filter: TTextureFilter; isColor: Boolean): TTexture2d; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadTextureEx';

{*
 * @brief Loads a texture from the specified image.
 *
 * If 'isColor' is true, the texture is loaded using the currently defined color space (sRGB by default).
 * The wrap and filter modes are taken from the current global default state.
 *
 * @param image The source image.
 * @param isColor Whether the texture should be treated as color data.
 * @return The loaded texture.
 *}
function R3D_LoadTextureFromImage(image: TImage; isColor: Boolean): TTexture2D; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadTextureFromImage';

{*
 * @brief Loads a texture from the specified image.
 *
 * If 'isColor' is true, the texture is loaded using the currently defined color space (sRGB by default).
 *
 * @param image The source image.
 * @param wrap The texture wrap mode.
 * @param filter The texture filter mode.
 * @param isColor Whether the texture should be treated as color data.
 * @return The loaded texture.
 *}
function R3D_LoadTextureFromImageEx(image: TImage; wrap: TTextureWrap; filter: TTextureFilter; isColor: Boolean): TTexture2D; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadTextureFromImageEx';

{*
 * @brief Loads a texture directly from memory.
 *
 * If 'isColor' is true, the texture is loaded using the currently defined color space (sRGB by default).
 * The wrap and filter modes are taken from the current global default state.
 *
 * @param fileType The file type/extension used to interpret the data.
 * @param fileData A pointer to the file data in memory.
 * @param dataSize The size of the file data in bytes.
 * @param isColor Whether the texture should be treated as color data.
 * @return The loaded texture.
 *}
function R3D_LoadTextureFromMemory(const fileType: PAnsiChar; const fileData: Pointer; dataSize: Integer; isColor: Boolean): TTexture2D; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadTextureFromMemory';

{*
 * @brief Loads a texture directly from memory.
 *
 * If 'isColor' is true, the texture is loaded using the currently defined color space (sRGB by default).
 *
 * @param fileType The file type/extension used to interpret the data.
 * @param fileData A pointer to the file data in memory.
 * @param dataSize The size of the file data in bytes.
 * @param wrap The texture wrap mode.
 * @param filter The texture filter mode.
 * @param isColor Whether the texture should be treated as color data.
 * @return The loaded texture.
 *}
function R3D_LoadTextureFromMemoryEx(const fileType: PAnsiChar; const fileData: Pointer; dataSize: Integer; wrap: TTextureWrap; filter: TTextureFilter; isColor: Boolean): TTexture2D; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadTextureFromMemoryEx';

{*
 * @brief Unloads a texture.
 *
 * This function calls raylib's `UnloadTexture` internally, while ensuring that
 * the provided texture is not an internal r3d texture.
 *
 * @param texture The texture to unload.
 *}
procedure R3D_UnloadTexture(texture: TTexture2D); cdecl;
   external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadTexture';

