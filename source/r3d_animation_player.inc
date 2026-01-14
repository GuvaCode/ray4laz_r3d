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
 *}
type
  PR3D_AnimationState = ^TR3D_AnimationState;
  TR3D_AnimationState = record
    currentTime: Single;  ///< Current playback time in animation ticks.
    weight: Single;       ///< Blending weight; any positive value is valid.
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
 * @brief Manages playback and blending of multiple animations for a skeleton.
 *}

  TR3D_AnimationPlayer = record
    states: PR3D_AnimationState;      ///< Array of active animation states, one per animation.
    animLib: TR3D_AnimationLib;        ///< Animation library providing the available animations.
    skeleton: TR3D_Skeleton;           ///< Skeleton to animate.

    localPose: PMatrix;               ///< Array of bone transforms representing the blended local pose.
    globalPose: PMatrix;              ///< Array of bone transforms multiplied by bone offsets for GPU skinning.
    texGlobalPose: Cardinal;          ///< GPU texture ID storing the global pose as a 1D RGBA32F texture.

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
 * @brief Returns whether a given animation is currently playing.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @return true if playing, false otherwise.
 *}
function R3D_IsAnimationPlaying(player: TR3D_AnimationPlayer; animIndex: Integer): Boolean; cdecl;
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
 * @brief Pauses the specified animation.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation to pause.
 *}
procedure R3D_PauseAnimation(player: PR3D_AnimationPlayer; animIndex: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_PauseAnimation';

{*
 * @brief Stops the specified animation and clamps its time.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation to stop.
 *}
procedure R3D_StopAnimation(player: PR3D_AnimationPlayer; animIndex: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_StopAnimation';

{*
 * @brief Rewinds the animation to the start or end depending on playback direction.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation to rewind.
 *}
procedure R3D_RewindAnimation(player: PR3D_AnimationPlayer; animIndex: Integer); cdecl;
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
 * @brief Gets the blending weight of an animation.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @return Current weight.
 *}
function R3D_GetAnimationWeight(player: TR3D_AnimationPlayer; animIndex: Integer): Single; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimationWeight';

{*
 * @brief Sets the blending weight of an animation.
 *
 * @param player Animation player.
 * @param animIndex Index of the animation.
 * @param weight Blending weight to apply.
 *}
procedure R3D_SetAnimationWeight(player: PR3D_AnimationPlayer; animIndex: Integer; weight: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAnimationWeight';

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
 * @brief Advances the time of all active animations.
 *
 * Updates all internal animation timers based on speed and delta time.
 * Does NOT recalculate the skeleton pose.
 *
 * @param player Animation player.
 * @param dt Delta time in seconds.
 *}
procedure R3D_AdvanceAnimationPlayerTime(player: PR3D_AnimationPlayer; dt: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_AdvanceAnimationPlayerTime';

{*
 * @brief Calculates the current blended skeleton pose.
 *
 * Interpolates keyframes and blends all active animations according to their weights.
 * Does NOT advance animation time.
 *
 * @param player Animation player.
 *}
procedure R3D_CalculateAnimationPlayerPose(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CalculateAnimationPlayerPose';

{*
 * @brief Uploads the current global pose array to the internal GPU texture.
 *
 * @param player Animation player.
 *}
procedure R3D_UploadAnimationPlayerPose(player: PR3D_AnimationPlayer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UploadAnimationPlayerPose';

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
