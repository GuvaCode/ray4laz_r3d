// r3d_animation_player.inc -- R3D Animation Player Module.

// ========================================
// TYPES
// ========================================

{*
 * @brief Types of events that an animation player can emit.
 *}
type
  TR3D_AnimationEvent = (
    R3D_ANIM_EVENT_FINISHED, ///< Animation has finished playing (non-looping).
    R3D_ANIM_EVENT_LOOPED    ///< Animation has completed a loop.
  );

{*
 * @brief Describes the playback state of a single animation within a player.
 *
 * Tracks the current time, speed, play/pause state, and looping behavior.
 *}
type
  PR3D_AnimationState = ^TR3D_AnimationState;
  TR3D_AnimationState = record
    currentTime: Single;  ///< Current playback time in animation ticks.
    speed: Single;        ///< Playback speed; can be negative for reverse playback.
    play: Boolean;        ///< Whether the animation is currently playing.
    loop: Boolean;        ///< True to enable looping playback.
  end;

{*
 * @brief Callback type for receiving animation events.
 *
 * @param player Pointer to the animation player emitting the event.
 * @param eventType Type of the event (finished, looped).
 * @param animIndex Index of the animation triggering the event.
 * @param userData Optional user-defined data passed when the callback was registered.
 *}
type
  PR3D_AnimationPlayer = ^TR3D_AnimationPlayer;
  R3D_AnimationEventCallback = procedure(
    player: PR3D_AnimationPlayer;
    eventType: TR3D_AnimationEvent;
    animIndex: Integer;
    userData: Pointer
  ); cdecl;

{*
 * @brief Manages playback of multiple animations for a skeleton.
 *
 * The animation player updates animation states, interpolates keyframes,
 * blends animations according to their weights, and stores the resulting
 * local and global bone transforms. It also supports GPU skinning by uploading
 * the global pose into a texture.
 *}
  TR3D_AnimationPlayer = record
    animLib: TR3D_AnimationLib;       ///< Animation library providing the available animations.
    skeleton: TR3D_Skeleton;          ///< Skeleton to animate.

    states: PR3D_AnimationState;      ///< Array of animation states, one per animation.
    activeAnimIndex: Integer;         ///< Index of the current animation.

    localPose: PMatrix;          ///< Array of bone transforms representing the blended local pose.
    modelPose: PMatrix;          ///< Array of bone transforms in model space, obtained by hierarchical accumulation.
    skinBuffer: PMatrix;         ///< Array of final skinning matrices (invBind * modelPose), sent to the GPU.
    skinTexture: Uint32;         ///< GPU texture ID storing the skinning matrices as a 1D RGBA16F texture.

    eventCallback: R3D_AnimationEventCallback; ///< Callback function to receive animation events.
    eventUserData: Pointer;                     ///< Optional user data pointer passed to the callback.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Creates an animation player for a skeleton and animation library.
 *
 * Allocates memory for animation states and pose buffers.
 *
 * @param skeleton Skeleton to animate.
 * @param animLib Animation library providing animations.
 * @return Newly created animation player, or a zeroed struct on failure.
 *}
function R3D_LoadAnimationPlayer(skeleton: TR3D_Skeleton; animLib: TR3D_AnimationLib): TR3D_AnimationPlayer; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAnimationPlayer';

{*
 * @brief Releases all resources used by an animation player.
 *
 * @param player Animation player to unload.
 *}
procedure R3D_UnloadAnimationPlayer(player: TR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadAnimationPlayer';

{*
 * @brief Checks whether an animation player is valid.
 *
 * @param player Animation player to check.
 * @return true if valid, false otherwise.
 *}
function R3D_IsAnimationPlayerValid(player: TR3D_AnimationPlayer): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsAnimationPlayerValid';

{*
 * @brief Returns whether an animation is currently playing.
 *
 * @param player Animation player.
 * @return true if playing, false otherwise.
 *}
function R3D_IsAnimationPlaying(player: TR3D_AnimationPlayer): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsAnimationPlaying';

{*
 * @brief Starts playback of the specified animation.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation to play.
 *}
procedure R3D_PlayAnimation(player: PR3D_AnimationPlayer; animIndex: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PlayAnimation';

{*
 * @brief Pauses the current animation.
 *
 * @param player Animation player.
 *}
procedure R3D_PauseAnimation(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PauseAnimation';

{*
 * @brief Stops the current animation and clamps its time.
 *
 * @param player Animation player.
 *}
procedure R3D_StopAnimation(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_StopAnimation';

{*
 * @brief Rewinds the animation to the start or end depending on playback direction.
 *
 * @param player Animation player.
 *}
procedure R3D_RewindAnimation(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RewindAnimation';

{*
 * @brief Gets the current playback time of an animation.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @return Current time in animation ticks.
 *}
function R3D_GetAnimationTime(player: TR3D_AnimationPlayer; animIndex: Integer): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimationTime';

{*
 * @brief Sets the current playback time of an animation.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @param time Time in animation ticks.
 *}
procedure R3D_SetAnimationTime(player: PR3D_AnimationPlayer; animIndex: Integer; time: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAnimationTime';

{*
 * @brief Gets the playback speed of an animation.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @return Current speed (may be negative for reverse playback).
 *}
function R3D_GetAnimationSpeed(player: TR3D_AnimationPlayer; animIndex: Integer): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimationSpeed';

{*
 * @brief Sets the playback speed of an animation.
 *
 * Negative values play the animation backwards. If setting a negative speed
 * on a stopped animation, consider calling RewindAnimation() to start at the end.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @param speed Playback speed.
 *}
procedure R3D_SetAnimationSpeed(player: PR3D_AnimationPlayer; animIndex: Integer; speed: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAnimationSpeed';

{*
 * @brief Gets whether the animation is set to loop.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @return True if looping is enabled.
 *}
function R3D_GetAnimationLoop(player: TR3D_AnimationPlayer; animIndex: Integer): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimationLoop';

{*
 * @brief Enables or disables looping for the animation.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @param loop True to enable looping.
 *}
procedure R3D_SetAnimationLoop(player: PR3D_AnimationPlayer; animIndex: Integer; loop: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAnimationLoop';

{*
 * @brief Advances the time of the current animation.
 *
 * Updates animation timer based on speed and delta time.
 * Does NOT recalculate the skeleton pose.
 *
 * @param player Animation player.
 * @param dt Delta time in seconds.
 *}
procedure R3D_AdvanceAnimationTime(player: PR3D_AnimationPlayer; dt: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_AdvanceAnimationTime';

{*
 * @brief Computes the local-space transform of each bone for the current animation.
 *
 * Samples and interpolates the current animation keyframes at the current playback time,
 * and stores the resulting bone transforms in local space into @p player.localPose.
 * Does NOT advance animation time, and does NOT compute model-space transforms.
 *
 * @param player Animation player whose local pose will be updated.
 *}
procedure R3D_ComputeAnimationLocalPose(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ComputeAnimationLocalPose';

{*
 * @brief Computes the model-space transform of each bone from the current local pose.
 *
 * Traverses the bone hierarchy and accumulates local transforms into model-space matrices,
 * stored into @p player.modelPose. This assumes @p player.localPose is already up-to-date.
 * Does NOT sample animation keyframes, and does NOT advance animation time.
 *
 * @param player Animation player whose model pose will be updated.
 *}
procedure R3D_ComputeAnimationModelPose(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ComputeAnimationModelPose';

{*
 * @brief Computes both the local and model-space transforms for the current animation.
 *
 * Equivalent to calling R3D_ComputeAnimationLocalPose() followed by R3D_ComputeAnimationModelPose().
 * Does NOT advance animation time.
 *
 * @param player Animation player whose local and model poses will be updated.
 *}
procedure R3D_ComputeAnimationPose(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ComputeAnimationPose';

{*
 * @brief Computes the final skinning matrices and uploads them to the GPU.
 *
 * Multiplies each bone's model-space transform by its inverse bind matrix to produce
 * the skinning matrices, then uploads them to the GPU skin texture.
 * This assumes @p player.modelPose is already up-to-date.
 *
 * @param player Animation player whose skinning matrices will be uploaded.
 *}
procedure R3D_UploadAnimationPose(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UploadAnimationPose';

{*
 * @brief Updates the animation player: calculates and upload the current pose, then advances time.
 *
 * Equivalent to calling R3D_ComputeAnimationLocalPose() followed by
 * R3D_ComputeAnimationModelPose() and R3D_AdvanceAnimationTime().
 *
 * @param player Animation player.
 * @param dt Delta time in seconds.
 *}
procedure R3D_UpdateAnimationPlayer(player: PR3D_AnimationPlayer; dt: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateAnimationPlayer';
