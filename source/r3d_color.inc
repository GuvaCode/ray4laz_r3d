// R3D Color Module.


{*
 * @brief Converts an sRGB color to linear space.
 *
 * @param color The sRGB color to convert.
 * @return The converted linear color as a Vector4.
 *}
function R3D_ColorSrgbToLinear(color: TColor): TVector4; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ColorSrgbToLinear';
{*
 * @brief Converts an sRGB color to linear space.
 *
 * @param color The sRGB color to convert.
 * @return The converted linear color as a Vector3.
 *}
function R3D_ColorSrgbToLinearVector3(color: TColor): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ColorSrgbToLinearVector3';
{*
 * @brief Converts a linear color to sRGB space.
 *
 * @param color The linear color to convert.
 * @return The converted sRGB color.
 *}
function R3D_ColorLinearToSrgb(color: TVector4): TColor; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ColorLinearToSrgb';

{*
 * @brief Converts a color from the current color space to linear space.
 *
 * @param color The color to convert.
 * @return The converted linear color as a Vector4.
 *}
function R3D_ColorFromCurrentSpace(color: TColor): TVector4; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ColorFromCurrentSpace';

{*
 * @brief Converts a color from the current color space to linear space.
 *
 * @param color The color to convert.
 * @return The converted linear color as a Vector3.
 *}
function R3D_ColorFromCurrentSpaceVector3(color: TColor): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ColorFromCurrentSpaceVector3';

