// R3D Curves Module.

{*
 * @brief Represents a keyframe in an interpolation curve.
 *
 * A keyframe contains two values: the time at which the keyframe occurs and the value of the interpolation at that time.
 * The time is normalized between 0.0 and 1.0, where 0.0 represents the start of the curve and 1.0 represents the end.
 *}

type
  PR3D_Keyframe = ^TR3D_Keyframe;
  TR3D_Keyframe = record
    time:  Single; // Normalized time of the keyframe, ranging from 0.0 to 1.0.
    value: Single; // The value of the interpolation at this keyframe.
  end;

type (* Represents an interpolation curve composed of keyframes.                                                            *)
     (* This structure contains an array of keyframes and metadata about the array, such as the current number of keyframes *)
     (* and the allocated capacity. The keyframes define a curve that can be used for smooth interpolation between values   *)
     (* over a normalized time range (0.0 to 1.0).                                                                          *)
  PR3D_InterpolationCurve = ^TR3D_InterpolationCurve;
  TR3D_InterpolationCurve = record
    keyframes: PR3D_Keyframe; // Dynamic array of keyframes defining the interpolation curve.
    capacity:  Cardinal;      // Allocated size of the keyframes array.
    count:     Cardinal;      // Current number of keyframes in the array.
  end;

  {*
   * @brief Loads an interpolation curve with a specified initial capacity.
   *
   * This function initializes an interpolation curve with the given capacity. The capacity represents the initial size of
   * the memory allocated for the curve. You can add keyframes to the curve using `R3D_AddKeyframe`. If adding a keyframe
   * exceeds the initial capacity, the system will automatically reallocate memory and double the initial capacity.
   *
   * @param capacity The initial capacity (size) of the interpolation curve. This is the number of keyframes that can be added
   *                 before a reallocation occurs.
   * @return An initialized interpolation curve with the specified capacity.
   *}
  function R3D_LoadInterpolationCurve(capacity: Integer): TR3D_InterpolationCurve; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadInterpolationCurve';

  {*
   * @brief Unloads the interpolation curve and frees the allocated memory.
   *
   * This function deallocates the memory associated with the interpolation curve and clears any keyframes stored in it.
   * It should be called when the curve is no longer needed to avoid memory leaks.
   *
   * @param curve The interpolation curve to be unloaded.
   *}
  procedure R3D_UnloadInterpolationCurve(curve: TR3D_InterpolationCurve); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadInterpolationCurve';

  {*
   * @brief Adds a keyframe to the interpolation curve.
   *
   * This function adds a keyframe to the given interpolation curve at a specific time and value. If the addition of the
   * keyframe requires reallocating memory and the reallocation fails, the previously allocated memory and keyframes are
   * preserved, but the new keyframe is not added.
   *
   * @param curve A pointer to the interpolation curve to which the keyframe will be added.
   * @param time The time at which the keyframe will be added.
   * @param value The value associated with the keyframe.
   * @return `true` if the keyframe was successfully added, or `false` if the reallocation failed.
   *}
  function R3D_AddKeyframe(curve: PR3D_InterpolationCurve; time, value: Single): Boolean; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_AddKeyframe';

  {*
   * @brief Evaluates the interpolation curve at a specific time.
   *
   * This function evaluates the value of the interpolation curve at a given time. The curve will interpolate between
   * keyframes based on the time provided.
   *
   * @param curve The interpolation curve to be evaluated.
   * @param time The time at which to evaluate the curve.
   * @return The value of the curve at the specified time.
   *}
   function R3D_EvaluateCurve(curve: TR3D_InterpolationCurve; time: Single): Single; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EvaluateCurve';


