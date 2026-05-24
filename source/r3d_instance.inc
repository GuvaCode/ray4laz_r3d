// R3D Instance Module.
// Complete translation of r3d_instance.h to Pascal

// ========================================
// CONSTANTS
// ========================================

const
  R3D_INSTANCE_ATTRIBUTE_COUNT = 5;

// ========================================
// ENUM / FLAGS
// ========================================

type
  {*
   * @brief Bitmask defining which instance attributes are present.
   *
   * Each bit enables one per-instance attribute stream. Attribute data is stored
   * in separate GPU buffers, one buffer per enabled attribute.
   *
   * The memory format of each enabled attribute is defined by `R3D_InstanceLayout`.
   * In shaders, these attributes are exposed as floating-point vectors, including
   * normalized integer formats such as UNORM/SNORM.
   *}
  TR3D_InstanceFlags = UInt32;

const
  R3D_INSTANCE_POSITION = (1 shl 0);  //< Position attribute: 3 components
  R3D_INSTANCE_ROTATION = (1 shl 1);  //< Rotation attribute: 4 components (quaternion)
  R3D_INSTANCE_SCALE    = (1 shl 2);  //< Scale attribute: 3 components
  R3D_INSTANCE_COLOR    = (1 shl 3);  //< Color attribute: 4 components
  R3D_INSTANCE_CUSTOM   = (1 shl 4);  //< Custom attribute: 4 components

type
  {*
   * @brief Storage format used by an instance attribute.
   *
   * The selected format controls how the attribute is stored in GPU memory.
   * All formats are read by the shader as floating-point values.
   *
   * UNORM formats are converted to the [0, 1] range.
   * SNORM formats are converted to the [-1, 1] range.
   *}
  TR3D_InstanceFormat = (
    R3D_INSTANCE_FORMAT_FLOAT32,    ///< 32-bit floating-point component.
    R3D_INSTANCE_FORMAT_FLOAT16,    ///< 16-bit floating-point component.
    R3D_INSTANCE_FORMAT_UNORM16,    ///< 16-bit unsigned normalized component.
    R3D_INSTANCE_FORMAT_SNORM16,    ///< 16-bit signed normalized component.
    R3D_INSTANCE_FORMAT_UNORM8,     ///< 8-bit unsigned normalized component.
    R3D_INSTANCE_FORMAT_SNORM8,     ///< 8-bit signed normalized component.
    R3D_INSTANCE_FORMAT_COUNT       ///< Number of available instance formats.
  );

// ========================================
// STRUCT TYPES
// ========================================

type
  {*
   * @brief Describes the layout of an instance buffer.
   *
   * `flags` defines which instance attributes are allocated.
   * `formats` defines the storage format used by each attribute.
   *
   * The `formats` array is indexed in the same order as the instance attribute
   * flags:
   *
   * - index 0: `R3D_INSTANCE_POSITION`
   * - index 1: `R3D_INSTANCE_ROTATION`
   * - index 2: `R3D_INSTANCE_SCALE`
   * - index 3: `R3D_INSTANCE_COLOR`
   * - index 4: `R3D_INSTANCE_CUSTOM`
   *
   * Data uploaded or mapped for an attribute must match the format selected for
   * that attribute.
   *}
  PR3D_InstanceLayout = ^TR3D_InstanceLayout;
  TR3D_InstanceLayout = record
    formats: array[0..R3D_INSTANCE_ATTRIBUTE_COUNT-1] of TR3D_InstanceFormat;  ///< Storage format for each instance attribute.
    flags: TR3D_InstanceFlags;                                                  ///< Enabled instance attribute mask.
  end;

  {*
   * @brief GPU buffers storing instance attribute streams.
   *
   * Each enabled attribute owns one GPU buffer. The enabled attributes and their
   * storage formats are described by `layout`.
   *}
  PR3D_InstanceBuffer = ^TR3D_InstanceBuffer;
  TR3D_InstanceBuffer = record
    buffers: array[0..R3D_INSTANCE_ATTRIBUTE_COUNT-1] of UInt32;  ///< One GPU buffer per attribute, indexed by attribute order.
    layout: TR3D_InstanceLayout;                                  ///< Instance buffer layout.
    capacity: Integer;                                            ///< Maximum number of instances.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Creates instance buffers on the GPU using the default layout.
 *
 * This is the simple entry point. It allocates one GPU buffer for each enabled
 * attribute in `flags`.
 *
 * Default formats:
 *
 * - position: FLOAT32
 * - rotation: FLOAT32
 * - scale: FLOAT32
 * - color: UNORM8
 * - custom: FLOAT32
 *
 * @param capacity Maximum number of instances.
 * @param flags Attribute mask to allocate.
 *
 * @return Initialized instance buffer, or an empty buffer on failure.
 *}
function R3D_LoadInstanceBuffer(capacity: Integer; flags: TR3D_InstanceFlags): TR3D_InstanceBuffer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadInstanceBuffer';

{*
 * @brief Creates instance buffers on the GPU using a custom layout.
 *
 * This is the advanced entry point. It allocates one GPU buffer for each
 * enabled attribute in `layout.flags`, using the corresponding storage format
 * from `layout.formats`.
 *
 * Data uploaded or mapped for each attribute must match the format selected in
 * the layout.
 *
 * @param capacity Maximum number of instances.
 * @param layout Instance layout describing enabled attributes and formats.
 *
 * @return Initialized instance buffer, or an empty buffer on failure.
 *}
function R3D_LoadInstanceBufferEx(capacity: Integer; layout: TR3D_InstanceLayout): TR3D_InstanceBuffer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadInstanceBufferEx';

{*
 * @brief Destroy all GPU buffers owned by this instance buffer.
 *}
procedure R3D_UnloadInstanceBuffer(buffer: TR3D_InstanceBuffer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadInstanceBuffer';

{*
 * @brief Grow the GPU buffers of an instance buffer to a new capacity.
 *
 * Only expands; if newCapacity <= buffer->capacity the call is a no-op.
 * All attribute buffers present in buffer->flags are reallocated and
 * if keepData is true, their existing content is copied to the new
 * buffers before the old ones are deleted.
 *
 * @param buffer Instance buffer to resize (updated in place).
 * @param newCapacity Desired minimum capacity in number of instances.
 * @param keepData If true, preserves existing instance data.
 *}
procedure R3D_ResizeInstanceBuffer(buffer: PR3D_InstanceBuffer; newCapacity: Integer; keepData: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ResizeInstanceBuffer';

{*
 * @brief Upload a contiguous range of instance data.
 * @param flag Attribute being updated (single bit).
 * @param offset First instance index.
 * @param count Number of instances.
 * @param data Source pointer.
 *}
procedure R3D_UploadInstances(buffer: TR3D_InstanceBuffer; flag: TR3D_InstanceFlags;
  offset: Integer; count: Integer; data: Pointer); cdecl;
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

{*
 * @brief Sets the storage format of an instance attribute in a layout.
 *
 * `attribute` must be a single instance attribute flag, such as
 * `R3D_INSTANCE_POSITION`, not a combination of multiple flags.
 *
 * This function only changes the format stored in the layout. It does not
 * enable the attribute in `layout.flags`.
 *
 * @param layout Layout to modify.
 * @param attribute Single attribute flag to modify.
 * @param format New storage format.
 *}
procedure R3D_SetInstanceFormat(layout: PR3D_InstanceLayout; attribute: TR3D_InstanceFlags; format: TR3D_InstanceFormat); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetInstanceFormat';

{*
 * @brief Gets the storage format of an instance attribute from a layout.
 *
 * `attribute` must be a single instance attribute flag, such as
 * `R3D_INSTANCE_POSITION`, not a combination of multiple flags.
 *
 * @param layout Layout to read from.
 * @param attribute Single attribute flag to query.
 *
 * @return Storage format of the requested attribute, or FLOAT32 if the
 * attribute flag is invalid.
 *}
function R3D_GetInstanceFormat(layout: TR3D_InstanceLayout; attribute: TR3D_InstanceFlags): TR3D_InstanceFormat; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetInstanceFormat';
