// R3D Skeleton Module.

{*
 * @brief Stores bone information for skeletal animation.
 *
 * Contains the bone name and the index of its parent bone.
 *}
type
  PR3D_BoneInfo = ^TR3D_BoneInfo;
  TR3D_BoneInfo = record
    name: array[0..31] of AnsiChar;  ///< Bone name (max 31 characters + null terminator).
    parent: Integer;                  ///< Index of the parent bone (-1 if root).
  end;

{*
 * @brief Represents a skeletal hierarchy used for skinning.
 *
 * Defines the bone structure, reference poses, and inverse bind matrices
 * required for skeletal animation. The skeleton provides both local and
 * global bind poses used during skinning and animation playback.
 *}
type
  PR3D_Skeleton = ^TR3D_Skeleton;
  TR3D_Skeleton = record
    bones: PR3D_BoneInfo;  ///< Array of bone descriptors defining the hierarchy and names.
    boneCount: Integer;    ///< Total number of bones in the skeleton.

    boneOffsets: PMatrix;  ///< Inverse bind matrices, one per bone. Transform vertices from mesh space to bone space (used in skinning).
    bindLocal: PMatrix;    ///< Bind pose transforms in local bone space (relative to parent).
    bindPose: PMatrix;     ///< Bind pose transforms in model space (global). Used as the default pose when not animated.
    texBindPose: Uint32;   ///< Texture ID that contains the bind pose for GPU skinning. This is a 1D Texture RGBA32F 4*boneCount.

  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Loads a skeleton hierarchy from a 3D model file.
 *
 * Skeletons are automatically loaded when importing a model,
 * but can be loaded manually for advanced use cases.
 *
 * @param filePath Path to the model file containing the skeleton data.
 * @return Return the loaded R3D_Skeleton.
 *}
function R3D_LoadSkeleton(const filePath: PAnsiChar): TR3D_Skeleton; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSkeleton';

{*
 * @brief Loads a skeleton hierarchy from memory data.
 *
 * Allows manual loading of skeletons directly from a memory buffer.
 * Typically used for advanced or custom asset loading workflows.
 *
 * @param data Pointer to the memory buffer containing skeleton data.
 * @param size Size of the memory buffer in bytes.
 * @param hint Optional format hint (can be NULL).
 * @return Return the loaded R3D_Skeleton.
 *}
function R3D_LoadSkeletonFromData(const data: Pointer; size: LongWord;
  const hint: PAnsiChar): TR3D_Skeleton; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSkeletonFromData';

{*
 * @brief Frees the memory allocated for a skeleton.
 *
 * @param skeleton R3D_Skeleton to destroy.
 *}
procedure R3D_UnloadSkeleton(skeleton: TR3D_Skeleton); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadSkeleton';

{*
* @brief Check if a skeleton is valid.
*
* Returns true if atleast the texBindPose is greater than zero.
*
* @param skeleton The skeleton to check.
* @return true if valid, false otherwise.
*}
function R3D_IsSkeletonValid(const skeleton: TR3D_Skeleton): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsSkeletonValid';
