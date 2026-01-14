// R3D Animation Module.

{*
 * @brief Animation track storing keyframe times and values.
 *
 * Represents a single animated property (translation, rotation or scale).
 * Keys are sampled by time and interpolated at runtime.
 *}
type
  PR3D_AnimationTrack = ^TR3D_AnimationTrack;
  TR3D_AnimationTrack = record
    times: PSingle;     ///< Keyframe times (sorted, in animation ticks).
    values: Pointer;    ///< Keyframe values (Vector3 or Quaternion).
    count: Integer;     ///< Number of keyframes.
  end;

{*
 * @brief Animation channel controlling a single bone.
 *
 * Contains animation tracks for translation, rotation and scale.
 * The sampled tracks are combined to produce the bone local transform.
 *}
type
  PR3D_AnimationChannel = ^TR3D_AnimationChannel;
  TR3D_AnimationChannel = record
    translation: TR3D_AnimationTrack;   ///< Translation track (Vector3).
    rotation: TR3D_AnimationTrack;      ///< Rotation track (Quaternion).
    scale: TR3D_AnimationTrack;         ///< Scale track (Vector3).
    boneIndex: Integer;                 ///< Index of the affected bone.
  end;

{*
 * @brief Represents a skeletal animation for a model.
 *
 * Contains all animation channels required to animate a skeleton.
 * Each channel corresponds to one bone and defines its transformation
 * (translation, rotation, scale) over time.
 *}
type
  PR3D_Animation = ^TR3D_Animation;
  TR3D_Animation = record
    channels: PR3D_AnimationChannel;    ///< Array of animation channels, one per animated bone.
    channelCount: Integer;              ///< Total number of channels in this animation.
    ticksPerSecond: Single;             ///< Playback rate; number of animation ticks per second.
    duration: Single;                   ///< Total length of the animation, in ticks.
    boneCount: Integer;                 ///< Number of bones in the target skeleton.
    name: array[0..31] of AnsiChar;     ///< Animation name (null-terminated string).
  end;

{*
 * @brief Represents a collection of skeletal animations sharing the same skeleton.
 *
 * Holds multiple animations that can be applied to compatible models or skeletons.
 * Typically loaded together from a single 3D model file (e.g., GLTF, FBX) containing several animation clips.
 *}
type
  PR3D_AnimationLib = ^TR3D_AnimationLib;
  TR3D_AnimationLib = record
    animations: PR3D_Animation;         ///< Array of animations included in this library.
    count: Integer;                     ///< Number of animations contained in the library.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Loads animations from a model file.
 * @param filePath Path to the model file containing animations.
 * @return Animation library structure containing loaded animations.
 * @note Free the returned array using R3D_UnloadAnimationLib().
 *}
function R3D_LoadAnimationLib(const filePath: PAnsiChar): TR3D_AnimationLib; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAnimationLib';

{*
 * @brief Loads animations from memory data.
 * @param data Pointer to memory buffer containing model animation data.
 * @param size Size of the buffer in bytes.
 * @param hint Hint on the model format (can be NULL).
 * @return Animation library structure containing loaded animations.
 * @note Free the returned array using R3D_UnloadAnimationLib().
 *}
function R3D_LoadAnimationLibFromMemory(const data: Pointer; size: LongWord;
  const hint: PAnsiChar): TR3D_AnimationLib; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAnimationLibFromMemory';

{*
* @brief Releases all resources associated with an animation library.
* @param animLib Animation library to unload.
 *}
procedure R3D_UnloadAnimationLib(animLib: TR3D_AnimationLib); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadAnimationLib';

{*
 * @brief Returns the index of an animation by name.
 * @param animLib Animation library to search.
 * @param name Name of the animation (case-sensitive).
 * @return Zero-based index if found, or -1 if not found.
 *}
function R3D_GetAnimationIndex(animLib: TR3D_AnimationLib;
  const name: PAnsiChar): Integer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimationIndex';

{*
 * @brief Retrieves an animation by name.
 * @param animLib Animation library to search.
 * @param name Name of the animation (case-sensitive).
 * @return Pointer to the animation, or NULL if not found.
 *}
function R3D_GetAnimation(animLib: TR3D_AnimationLib;
  const name: PAnsiChar): PR3D_Animation; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimation';


