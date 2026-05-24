// R3D Camera Module.
// Complete translation of r3d_camera.h to Pascal

// ========================================
// CONSTANTS
// ========================================

//#define R3D_CAMERA_BASE                                 \
//    R3D_LITERAL(R3D_Camera) {                           \
//        .position   = {0, 0, 0},                        \
//        .rotation   = {0, 0, 0, 1},                     \
//        .fovy       = 60.0,                             \
//        .nearPlane  = 0.05,                             \
//        .farPlane   = 4000.0,                           \
//        .cullMask   = R3D_LAYER_ALL,                    \
//        .projection = R3D_PROJECTION_PERSPECTIVE,       \
//    }


{*
 * @defgroup Camera Camera
 *
 * @brief Projection mode used by an R3D camera.
 *}
type
  TR3D_Projection = (
    R3D_PROJECTION_PERSPECTIVE,   ///< Perspective projection.
    R3D_PROJECTION_ORTHOGRAPHIC   ///< Orthographic projection.
  );

{*
 * @brief Bitfield type used to specify rendering layers.
 *
 * This type is used by cameras and renderable objects to control visibility.
 * A renderable object is visible to a camera when at least one bit is shared
 * between the object's layer mask and the camera's layer mask.
 *
 * For convenience, 16 predefined layers are provided here, but the underlying
 * type supports up to 32 layers.
 *}
type
  TR3D_Layer = UInt32;

const
  R3D_LAYER_01 = (1 shl 0);
  R3D_LAYER_02 = (1 shl 1);
  R3D_LAYER_03 = (1 shl 2);
  R3D_LAYER_04 = (1 shl 3);
  R3D_LAYER_05 = (1 shl 4);
  R3D_LAYER_06 = (1 shl 5);
  R3D_LAYER_07 = (1 shl 6);
  R3D_LAYER_08 = (1 shl 7);
  R3D_LAYER_09 = (1 shl 8);
  R3D_LAYER_10 = (1 shl 9);
  R3D_LAYER_11 = (1 shl 10);
  R3D_LAYER_12 = (1 shl 11);
  R3D_LAYER_13 = (1 shl 12);
  R3D_LAYER_14 = (1 shl 13);
  R3D_LAYER_15 = (1 shl 14);
  R3D_LAYER_16 = (1 shl 15);
  R3D_LAYER_ALL = $FFFFFFFF;

{*
 * @brief Quaternion-based 3D camera used by R3D.
 *
 * Unlike raylib's Camera3D, this camera stores its orientation directly as a
 * quaternion instead of using a target/up pair. It also stores near/far clipping
 * planes and a layer mask used to filter visible renderables.
 *}
type
  PR3D_Camera = ^TR3D_Camera;
  TR3D_Camera = record
    position: TVector3;               ///< Camera world-space position.
    rotation: TQuaternion;            ///< Camera world-space orientation.
    fovy: Double;                     ///< Vertical field of view in degrees for perspective projection; vertical size for orthographic projection.
    nearPlane: Double;                ///< Distance to the near clipping plane.
    farPlane: Double;                 ///< Distance to the far clipping plane.
    cullMask: TR3D_Layer;             ///< Camera visibility layer mask.
    projection: TR3D_Projection;      ///< Camera projection mode.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Creates an R3D camera from a raylib camera.
 *
 * The camera position, orientation, field of view and projection mode are
 * derived from the given raylib camera.
 *
 * Since raylib's Camera3D does not store near/far clipping planes or layer
 * masks, the near/far planes are initialized from the current rlgl culling
 * distances, while the layer mask is set to the default R3D camera layer mask.
 *
 * @param camera Raylib camera to convert.
 *
 * @return Converted R3D camera.
 *}
function R3D_CameraFromRL(camera: TCamera3D): TR3D_Camera; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CameraFromRL';

{*
 * @brief Converts an R3D camera to a raylib camera.
 *
 * The raylib target/up vectors are derived from the camera quaternion.
 * Near/far clipping planes and layer masks are not represented by Camera3D.
 *}
function R3D_CameraToRL(camera: TR3D_Camera): TCamera3D; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CameraToRL';

{*
 * @brief Sets the camera orientation so it looks at a target point.
 *}
procedure R3D_CameraLookAt(camera: PR3D_Camera; target: TVector3; up: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CameraLookAt';

{*
 * @brief Returns the camera forward direction in world space.
 *}
function R3D_GetCameraForward(camera: TR3D_Camera): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetCameraForward';

{*
 * @brief Returns the camera right direction in world space.
 *}
function R3D_GetCameraRight(camera: TR3D_Camera): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetCameraRight';

{*
 * @brief Returns the camera up direction in world space.
 *}
function R3D_GetCameraUp(camera: TR3D_Camera): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetCameraUp';

{*
 * @brief Returns the camera view matrix.
 *}
function R3D_GetCameraView(camera: TR3D_Camera): TMatrix; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetCameraView';

{*
 * @brief Returns the camera projection matrix.
 *}
function R3D_GetCameraProj(camera: TR3D_Camera; aspect: Double): TMatrix; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetCameraProj';

{*
 * @brief Returns the combined view-projection matrix.
 *}
function R3D_GetCameraViewProj(camera: TR3D_Camera; aspect: Double): TMatrix; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetCameraViewProj';

{*
 * @brief Moves the camera in world space.
 *}
procedure R3D_MoveCamera(camera: PR3D_Camera; delta: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_MoveCamera';

{*
 * @brief Moves the camera in local space.
 *}
procedure R3D_MoveCameraLocal(camera: PR3D_Camera; delta: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_MoveCameraLocal';

{*
 * @brief Rotates the camera using a quaternion.
 *}
procedure R3D_CameraRotate(camera: PR3D_Camera; rotation: TQuaternion); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CameraRotate';

{*
 * @brief Rotates the camera around its local X axis.
 *}
procedure R3D_CameraPitch(camera: PR3D_Camera; angle: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CameraPitch';

{*
 * @brief Rotates the camera around its local Y axis.
 *}
procedure R3D_CameraYaw(camera: PR3D_Camera; angle: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CameraYaw';

{*
 * @brief Rotates the camera around its local Z axis.
 *}
procedure R3D_CameraRoll(camera: PR3D_Camera; angle: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CameraRoll';

{*
 * @brief Replaces the camera culling mask.
 *}
procedure R3D_SetCameraCullMask(camera: PR3D_Camera; cullMask: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetCameraCullMask';

{*
 * @brief Enables one or more layers in the camera culling mask.
 *}
procedure R3D_EnableCameraCullLayers(camera: PR3D_Camera; layerMask: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EnableCameraCullLayers';

{*
 * @brief Disables one or more layers from the camera culling mask.
 *}
procedure R3D_DisableCameraCullLayers(camera: PR3D_Camera; layerMask: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DisableCameraCullLayers';

{*
 * @brief Toggles one or more layers in the camera culling mask.
 *}
procedure R3D_ToggleCameraCullLayers(camera: PR3D_Camera; layerMask: TR3D_Layer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ToggleCameraCullLayers';

{*
 * @brief Checks whether at least one object layer is visible to the camera.
 *}
function R3D_IsCameraLayerVisible(camera: TR3D_Camera; layerMask: TR3D_Layer): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsCameraLayerVisible';
