// R3D Vertex Module.
// Complete translation of r3d_vertex.h to Pascal

// ========================================
// STRUCTS TYPES
// ========================================

{*
 * @brief Compact vertex format used by R3D meshes.
 *
 * Texture coordinates are stored as float16 values.
 * Normals and tangents are stored as signed normalized 8-bit values.
 * Bone weights are stored as unsigned 8-bit values and should sum to 255.
 *}
type
  PR3D_Vertex = ^TR3D_Vertex;
  TR3D_Vertex = record
    position: TVector3;                     ///< Vertex position in object space.
    texcoord: array[0..1] of UInt16;        ///< Texture coordinates stored as float16.
    normal: array[0..3] of Int8;            ///< Normal vector stored as SNORM8. XYZ are used, W is unused.
    tangent: array[0..3] of Int8;           ///< Tangent vector stored as SNORM8. XYZ are tangent, W stores handedness.
    color: TColorB;                         ///< Vertex color in RGBA8.
    boneIndices: array[0..3] of UInt8;      ///< Indices of up to 4 bones influencing this vertex.
    boneWeights: array[0..3] of UInt8;      ///< Bone weights in UNORM8. Values should sum to 255.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Constructs a packed R3D vertex from unpacked attribute data.
 *
 * Texture coordinates are packed to float16.
 * Normals and tangents are packed to SNORM8.
 *
 * @param position Vertex position in object space.
 * @param texcoord Texture coordinates in float32. Any range is supported.
 * @param normal Normal vector. Components are clamped to the [-1, 1] range.
 * @param tangent Tangent vector. XYZ components are clamped to [-1, 1], W stores handedness.
 * @param color Vertex color in RGBA8.
 *
 * @return Packed vertex ready for GPU upload.
 *}
function R3D_MakeVertex(position: TVector3; texcoord: TVector2;
  normal: TVector3; tangent: TVector4; color: TColorB): TR3D_Vertex; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_MakeVertex';

{*
 * @brief Packs texture coordinates from float32 to float16.
 *
 * @param dst Output buffer of 2 uint16_t values. Must not be NULL.
 * @param src Texture coordinates in float32. Any range is supported.
 *}
procedure R3D_PackTexCoord(dst: PWord; src: TVector2); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PackTexCoord';

{*
 * @brief Unpacks texture coordinates from float16 to float32.
 *
 * @param src Input buffer of 2 uint16_t values. Must not be NULL.
 *
 * @return Unpacked texture coordinates in float32.
 *}
function R3D_UnpackTexCoord(src: PWord): TVector2; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnpackTexCoord';

{*
 * @brief Packs a normal vector from float32 to SNORM8.
 *
 * XYZ components are clamped to the [-1, 1] range before packing.
 * The fourth component is set to 0.
 *
 * @param dst Output buffer of 4 int8_t values. Must not be NULL.
 * @param src Normal vector to pack.
 *}
procedure R3D_PackNormal(dst: PShortInt; src: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PackNormal';

{*
 * @brief Unpacks a normal vector from SNORM8 to float32.
 *
 * Only XYZ components are read.
 *
 * @param src Input buffer of 4 int8_t values. Must not be NULL.
 *
 * @return Unpacked normal vector. Not guaranteed to be unit length.
 *}
function R3D_UnpackNormal(src: PShortInt): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnpackNormal';

{*
 * @brief Packs a tangent vector from float32 to SNORM8.
 *
 * XYZ components are clamped to the [-1, 1] range before packing.
 * W stores tangent handedness and is packed as either +1 or -1.
 *
 * @param dst Output buffer of 4 int8_t values. Must not be NULL.
 * @param src Tangent vector to pack. W is interpreted as handedness.
 *}
procedure R3D_PackTangent(dst: PShortInt; src: TVector4); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PackTangent';

{*
 * @brief Unpacks a tangent vector from SNORM8 to float32.
 *
 * XYZ components are unpacked from SNORM8.
 * W is returned as exactly +1.0f or -1.0f.
 *
 * @param src Input buffer of 4 int8_t values. Must not be NULL.
 *
 * @return Unpacked tangent vector.
 *}
function R3D_UnpackTangent(src: PShortInt): TVector4; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnpackTangent';
