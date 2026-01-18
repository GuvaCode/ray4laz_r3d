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

    localBind: PMatrix;      ///< Bind pose matrices relative to parent
    modelBind: PMatrix;      ///< Bind pose matrices in model/global space
    invBind: PMatrix;        ///< Inverse bind matrices (model space) for skinning
    rootBind: TMatrix;        ///< Root correction if local bind is not identity

    skinTexture: uint32;   ///< Texture ID that contains the bind pose for GPU skinning. This is a 1D Texture RGBA16F 4*boneCount.
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

{*
 * @brief Returns the index of the bone with the given name.
 *
 * @param skeleton Skeleton to search in.
 * @param boneName Name of the bone to find.
 * @return Index of the bone, or a negative value if not found.
 *}
 function R3D_GetSkeletonBoneIndex(skeleton: TR3D_Skeleton; const boneName: PChar): Integer; cdecl;
   external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSkeletonBoneIndex';

{*
 * @brief Returns a pointer to the bone with the given name.
 *
 * @param skeleton Skeleton to search in.
 * @param boneName Name of the bone to find.
 * @return Pointer to the bone, or NULL if not found.
 *}
 function R3D_GetSkeletonBone(Skeleton: TR3D_Skeleton; const boneName: PChar): PR3D_BoneInfo; cdecl;
   external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSkeletonBoneIndex';



