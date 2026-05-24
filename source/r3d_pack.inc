// R3D Pack Module.
// Complete translation of r3d_pack.h to Pascal

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Packs a 32-bit float into a 16-bit floating-point value.
 *
 * @param x Value to pack.
 *
 * @return IEEE 754 half-precision representation.
 *}
function R3D_PackFloat16(x: Single): UInt16; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PackFloat16';

{*
 * @brief Packs a float into a 16-bit unsigned normalized value.
 *
 * The input value is clamped to the [0, 1] range before packing.
 *
 * @param x Value to pack.
 *
 * @return 16-bit UNORM representation.
 *}
function R3D_PackUnorm16(x: Single): UInt16; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PackUnorm16';

{*
 * @brief Packs a float into a 16-bit signed normalized value.
 *
 * The input value is clamped to the [-1, 1] range before packing.
 *
 * @param x Value to pack.
 *
 * @return 16-bit SNORM representation.
 *}
function R3D_PackSnorm16(x: Single): Int16; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PackSnorm16';

{*
 * @brief Packs a float into an 8-bit unsigned normalized value.
 *
 * The input value is clamped to the [0, 1] range before packing.
 *
 * @param x Value to pack.
 *
 * @return 8-bit UNORM representation.
 *}
function R3D_PackUnorm8(x: Single): UInt8; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PackUnorm8';

{*
 * @brief Packs a float into an 8-bit signed normalized value.
 *
 * The input value is clamped to the [-1, 1] range before packing.
 *
 * @param x Value to pack.
 *
 * @return 8-bit SNORM representation.
 *}
function R3D_PackSnorm8(x: Single): Int8; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PackSnorm8';

{*
 * @brief Unpacks a 16-bit floating-point value into a 32-bit float.
 *
 * @param x IEEE 754 half-precision representation.
 *
 * @return Unpacked float value.
 *}
function R3D_UnpackFloat16(x: UInt16): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnpackFloat16';

{*
 * @brief Unpacks a 16-bit unsigned normalized value.
 *
 * The returned value is in the [0, 1] range.
 *
 * @param x 16-bit UNORM representation.
 *
 * @return Unpacked float value.
 *}
function R3D_UnpackUnorm16(x: UInt16): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnpackUnorm16';

{*
 * @brief Unpacks a 16-bit signed normalized value.
 *
 * The returned value is clamped to the [-1, 1] range.
 *
 * @param x 16-bit SNORM representation.
 *
 * @return Unpacked float value.
 *}
function R3D_UnpackSnorm16(x: Int16): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnpackSnorm16';

{*
 * @brief Unpacks an 8-bit unsigned normalized value.
 *
 * The returned value is in the [0, 1] range.
 *
 * @param x 8-bit UNORM representation.
 *
 * @return Unpacked float value.
 *}
function R3D_UnpackUnorm8(x: UInt8): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnpackUnorm8';

{*
 * @brief Unpacks an 8-bit signed normalized value.
 *
 * The returned value is clamped to the [-1, 1] range.
 *
 * @param x 8-bit SNORM representation.
 *
 * @return Unpacked float value.
 *}
function R3D_UnpackSnorm8(x: Int8): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnpackSnorm8';
