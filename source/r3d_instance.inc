// r3d_instance -- R3D Instance Module.

// ========================================
// CONSTANTS
// ========================================

const
  R3D_INSTANCE_ATTRIBUTE_COUNT = 4;

// ========================================
// ENUM / FLAGS
// ========================================

type
  {*
   * @brief Bitmask defining which instance attributes are present.
   *}
TR3D_InstanceFlags = cint;
const
  R3D_INSTANCE_POSITION = (1 shl 0);    //< Vector3
  R3D_INSTANCE_ROTATION = (1 shl 1);    //< Quaternion
  R3D_INSTANCE_SCALE    = (1 shl 2);    //< Vector3
  R3D_INSTANCE_COLOR    = (1 shl 3);    //< Color

// ========================================
// STRUCT TYPES
// ========================================

type
  {*
   * @brief GPU buffers storing instance attribute streams.
   *
   * buffers: One VBO per attribute (indexed by flag order).
   * capacity: Maximum number of instances.
   * flags: Enabled attribute mask.
   *}
  PR3D_InstanceBuffer = ^TR3D_InstanceBuffer;
  TR3D_InstanceBuffer = record
    buffers: array[0..R3D_INSTANCE_ATTRIBUTE_COUNT-1] of cuint32;
    capacity: cint;
    flags: cint;
  end;


// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Create instance buffers on the GPU.
 * @param capacity Max instances.
 * @param flags Attribute mask to allocate.
 * @return Initialized instance buffer.
 *}
function R3D_LoadInstanceBuffer(capacity: cint; flags: TR3D_InstanceFlags): TR3D_InstanceBuffer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadInstanceBuffer';

{*
 * @brief Destroy all GPU buffers owned by this instance buffer.
 *}
procedure R3D_UnloadInstanceBuffer(buffer: TR3D_InstanceBuffer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadInstanceBuffer';

{*
 * @brief Upload a contiguous range of instance data.
 * @param flag Attribute being updated (single bit).
 * @param offset First instance index.
 * @param count Number of instances.
 * @param data Source pointer.
 *}
procedure R3D_UploadInstances(buffer: TR3D_InstanceBuffer; flag: TR3D_InstanceFlags;
  offset: cint; count: cint; data: Pointer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UploadInstances';

{*
 * @brief Map an attribute buffer for CPU write access.
 * @param flag Attribute to map (single bit).
 * @return Writable pointer, or NULL on error.
 *}
function R3D_MapInstances(buffer: TR3D_InstanceBuffer; flag: TR3D_InstanceFlags): Pointer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_MapInstances';

{*
 * @brief Unmap one or more previously mapped attribute buffers.
 * @param flags Bitmask of attributes to unmap.
 *}
procedure R3D_UnmapInstances(buffer: TR3D_InstanceBuffer; flags: TR3D_InstanceFlags); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnmapInstances';

