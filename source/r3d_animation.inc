
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

{*
 * @brief Describes the playback state of a single animation.
 *
 * Each state tracks the current playback time, blending weight,
 * and looping behavior for one animation within a player.
 *}
type
  PR3D_AnimationState = ^TR3D_AnimationState;
  TR3D_AnimationState = record
    currentTime: Single;    ///< Current playback time in animation ticks.
    weight: Single;         ///< Blending weight of this animation (0.0-1.0).
    loop: Boolean;          ///< True to enable looping playback.
  end;

{*
 * @brief Controls playback and blending of animations for a skeleton.
 *
 * The animation player manages multiple animation states from a given
 * animation library and computes the blended pose for the associated skeleton.
 * On each update, it advances internal timers, interpolates keyframes,
 * blends active animations according to their weights, and updates the
 * current skeleton pose.
 *}
type
  PR3D_AnimationPlayer = ^TR3D_AnimationPlayer;
  TR3D_AnimationPlayer = record
    states: PR3D_AnimationState;        ///< Array of active animation states (for each animation).
    animLib: TR3D_AnimationLib;         ///< Animation library providing available animations.
    skeleton: TR3D_Skeleton;            ///< Target skeleton to animate.
    currentPose: PMatrix;               ///< Array of bone transforms representing the blended pose.
  end;

// ========================================
// PUBLIC API
// ========================================

// ----------------------------------------
// ANIMATION: Animation Library Functions
// ----------------------------------------

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
 * @brief Frees memory allocated for model animations.
 * @param animLib Pointer to the animation library to free.
 *}
procedure R3D_UnloadAnimationLib(animLib: PR3D_AnimationLib); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadAnimationLib';

{*
 * @brief Retrieves the index of a named animation within an animation library.
 * @param animLib Pointer to the animation library.
 * @param name Name of the animation to look for (case-sensitive).
 * @return Zero-based index of the matching animation, or -1 if not found.
 *}
function R3D_GetAnimationIndex(const animLib: PR3D_AnimationLib;
  const name: PAnsiChar): Integer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimationIndex';

{*
 * @brief Finds a named animation in an array of animations.
 * @param animLib Pointer to the animation library.
 * @param name Name of the animation to find (case-sensitive).
 * @return Pointer to the matching animation, or NULL if not found.
 *}
function R3D_GetAnimation(const animLib: PR3D_AnimationLib;
  const name: PAnsiChar): PR3D_Animation; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimation';

// ----------------------------------------
// ANIMATION: Animation Player Functions
// ----------------------------------------

{*
 * @brief Creates a new animation player for a skeleton and animation library.
 *
 * Allocates internal structures for managing animation states and poses.
 *
 * @param skeleton Pointer to the target skeleton.
 * @param animLib Pointer to the animation library containing available animations.
 * @return Pointer to a newly created animation player, or NULL on failure.
 *}
function R3D_LoadAnimationPlayer(const skeleton: PR3D_Skeleton;
  const animLib: PR3D_AnimationLib): PR3D_AnimationPlayer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAnimationPlayer';

{*
 * @brief Destroys an animation player and frees its allocated resources.
 *
 * @param player Pointer to the animation player to destroy.
 *}
procedure R3D_UnloadAnimationPlayer(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadAnimationPlayer';

{*
 * @brief Advances the animation player's time for all active animation states.
 *
 * This function updates the `currentTicks` of all internal animation states
 * in a synchronized manner, using the provided delta time and any playback speed modifiers.
 * It does not recalculate the skeleton pose. For finer control, individual animation
 * states can be updated manually.
 *
 * @param player Pointer to the animation player.
 * @param dt Delta time to advance, in seconds.
 *}
procedure R3D_AdvanceAnimationPlayerTime(player: PR3D_AnimationPlayer; dt: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_AdvanceAnimationPlayerTime';

{*
 * @brief Calculates the current skeleton pose from active animations.
 *
 * This function interpolates keyframes and blends all active animation states
 * to produce the resulting skeleton pose. It does not advance animation time.
 *
 * @note If the sum of animation state weights is less than or equal to 0.0,
 *       the bind pose will be used as the current pose.
 * @note The total sum of animation state weights is the responsibility of the user.
 *
 * @param player Pointer to the animation player.
 *}
procedure R3D_CalculateAnimationPlayerPose(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CalculateAnimationPlayerPose';

{*
 * @brief Calculates the current skeleton pose, then advances the animation player's time.
 *
 * This function first calculates the pose by blending active animation states,
 * then advances the `currentTicks` of all internal animation states by the given delta time.
 * It is equivalent to calling `R3D_CalculateAnimationPlayerPose` followed by
 * `R3D_AdvanceAnimationPlayerTime`.
 *
 * @note If the sum of animation state weights is less than or equal to 0.0,
 *       the bind pose will be used as the current pose.
 * @note The total sum of animation state weights is the responsibility of the user.
 *
 * @param player Pointer to the animation player.
 * @param dt Delta time to advance, in seconds.
 *}
procedure R3D_UpdateAnimationPlayer(player: PR3D_AnimationPlayer; dt: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateAnimationPlayer';
