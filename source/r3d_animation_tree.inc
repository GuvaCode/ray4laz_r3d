// R3D Animation Tree Module.

// ========================================
// FORWARD DECLARATIONS
// ========================================

type
  PR3D_AnimationTreeNode = ^TR3D_AnimationTreeNode;
  TR3D_AnimationTreeNode = record end; // Forward declaration

  R3D_AnimationStmIndex = Integer;

// ========================================
// CALLBACKS TYPES
// ========================================

{*
 * @brief Callback type for manipulating the animation before it is used by the animation tree.
 *
 * @param animation Pointer to the processed animation.
 * @param state Current animation state.
 * @param boneIndex Index of the processed bone.
 * @param out Transformation of the processed bone.
 * @param userData Optional user-defined data passed when the callback was registered.
 *}
type
  TR3D_AnimationNodeCallback = procedure(const animation: PR3D_Animation; state: TR3D_AnimationState;
    boneIndex: Integer; outTransform: PTransform; userData: Pointer); cdecl;

{*
 * @brief Callback type for manipulating the final animation.
 *
 * @param player Pointer to the animation player used by the animation tree.
 * @param boneIndex Index of the processed bone.
 * @param out Transformation of the processed bone.
 * @param userData Optional user-defined data passed when the callback was registered.
 *}
type
  TR3D_AnimationTreeCallback = procedure(const player: PR3D_AnimationPlayer; boneIndex: Integer;
    outTransform: PTransform; userData: Pointer); cdecl;

// ========================================
// ENUM TYPES
// ========================================

{*
 * @brief Types of operation modes for state machine edge.
 *}
type
  TR3D_StmEdgeMode = (
    R3D_STM_EDGE_INSTANT = 0, ///< Switch to next state instantly, with respecting cross fade time.
    R3D_STM_EDGE_ONDONE       ///< Switch to next state when associated animation is done or looped with looper parameter set true.
  );

{*
 * @brief Types of travel status for state machine edge.
 *}
type
  TR3D_StmEdgeStatus = (
    R3D_STM_EDGE_ON = 0, ///< Edge is traversable by travel function.
    R3D_STM_EDGE_AUTO,   ///< Edge is traversable automatically and by travel function.
    R3D_STM_EDGE_ONCE,   ///< Edge is traversable automatically and by travel function, but only once; edge status changes to nextStatus when traversed.
    R3D_STM_EDGE_OFF     ///< Edge is not traversable.
  );

// ========================================
// STRUCTS TYPES
// ========================================

{*
 * @brief Bone mask for Blend2 and Add2 animation nodes.
 *
 * Bone mask structure, can by created by R3D_ComputeBoneMask.
 *}
type
  PR3D_BoneMask = ^TR3D_BoneMask;
  TR3D_BoneMask = record
    mask: array[0..7] of Int32;    ///< Bit mask buffer for maximum of 256 bones (32bits * 8).
    boneCount: Integer;             ///< Actual bones count.
  end;

{*
 * @brief Parameters for animation tree node Animation.
 *
 * Animation is a leaf node, holding the R3D_Animation structure.
 *}
type
  PR3D_AnimationNodeParams = ^TR3D_AnimationNodeParams;
  TR3D_AnimationNodeParams = record
    name: array[0..31] of AnsiChar;                 ///< Animation name (null-terminated string).
    state: TR3D_AnimationState;                      ///< Animation state.
    looper: Boolean;                                 ///< Flag to control whether the animation is considered done when looped; yes when true.

    evalCallback: TR3D_AnimationNodeCallback;        ///< Callback function to receive and modify animation transformation before been used.
    evalUserData: Pointer;                           ///< Optional user data pointer passed to the callback.
  end;

{*
 * @brief Parameters for animation tree node Blend2.
 *
 * Blend2 node blends channels of any two animation nodes together, with respecting optional bone mask.
 *}
type
  PR3D_Blend2NodeParams = ^TR3D_Blend2NodeParams;
  TR3D_Blend2NodeParams = record
    boneMask: PR3D_BoneMask;      ///< Pointer to bone mask structure, can be NULL; calculated by R3D_ComputeBoneMask().
    blend: Single;                 ///< Blend weight value, can be in interval from 0.0 to 1.0.
  end;

{*
 * @brief Parameters for animation tree node Add2.
 *
 * Add2 node adds channels of any two animation nodes together, with respecting optional bone mask.
 *}
type
  PR3D_Add2NodeParams = ^TR3D_Add2NodeParams;
  TR3D_Add2NodeParams = record
    boneMask: PR3D_BoneMask;      ///< Pointer to bone mask structure, can be NULL; calculated by R3D_ComputeBoneMask().
    weight: Single;                ///< Add weight value, can be in interval from 0.0 to 1.0.
  end;

{*
 * @brief Parameters for animation tree node Switch.
 *
 * Switch node allows instant or blended/faded transition between any animation nodes connected to inputs.
 *}
type
  PR3D_SwitchNodeParams = ^TR3D_SwitchNodeParams;
  TR3D_SwitchNodeParams = record
    synced: Boolean;                ///< Flag to control input animation nodes synchronization; activated input is reset when false.
    activeInput: Cardinal;          ///< Active input zero-based index.
    xFadeTime: Single;              ///< Animation nodes cross fade blending time, in seconds.
  end;

{*
 * @brief Parameters for animation state machine edge.
 *}
type
  PR3D_StmEdgeParams = ^TR3D_StmEdgeParams;
  TR3D_StmEdgeParams = record
    mode: TR3D_StmEdgeMode;             ///< Operation mode.
    status: TR3D_StmEdgeStatus;         ///< Current travel status.
    nextStatus: TR3D_StmEdgeStatus;     ///< Travel status used after machine traversed through this edge with status set to R3D_STM_EDGE_ONCE.
    xFadeTime: Single;                   ///< Cross fade blending time between connected animation nodes, in seconds.
  end;

{*
 * @brief Manages a tree structure of animation nodes.
 *
 * Animation tree allows to define complex logic for switching and blending animations of
 * associated animation player. It supports 5 different animation node types: Animation, Blend2, Add2,
 * Switch and State Machine. It also fully supports root motion and bone masking in Blend2/Add2.
 *}
type
  PR3D_AnimationTree = ^TR3D_AnimationTree;
  TR3D_AnimationTree = record
    player: TR3D_AnimationPlayer;                   ///< Animation player and skeleton used by all animation nodes.
    rootNode: PR3D_AnimationTreeNode;                ///< Pointer to root animation node of the tree.
    nodePool: PR3D_AnimationTreeNode;                ///< Animation node pool of size nodePoolMaxSize.
    nodePoolSize: Cardinal;                          ///< Current animation node pool size.
    nodePoolMaxSize: Cardinal;                       ///< Maximum number of animation nodes, defined during load.
    rootBone: Integer;                               ///< Optional root bone index, -1 if not defined.

    updateCallback: TR3D_AnimationTreeCallback;      ///< Callback function to receive and modify final animation transformation.
    updateUserData: Pointer;                         ///< Optional user data pointer passed to the callback.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Creates an animation tree using given animation player.
 *
 * Allocates memory for animation nodes pool.
 *
 * @param player Animation player used by the animation tree.
 * @param maxSize Size of the animation nodes pool; maximum number of animation nodes in the tree.
 * @return Newly created animation tree, or a zeroed struct on failure.
 *}
function R3D_LoadAnimationTree(player: TR3D_AnimationPlayer; maxSize: Integer): TR3D_AnimationTree; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAnimationTree';

{*
 * @brief Creates an animation tree using given animation player, with optional root motion support.
 *
 * Allocates memory for animation nodes pool and sets root bone index for root motion.
 *
 * @param player Animation player used by the animation tree.
 * @param maxSize Size of the animation nodes pool; maximum number of animation nodes in the tree.
 * @param rootBone Root bone index; set -1 to disable root motion.
 * @return Newly created animation tree, or a zeroed struct on failure.
 *}
function R3D_LoadAnimationTreeEx(player: TR3D_AnimationPlayer; maxSize: Integer; rootBone: Integer): TR3D_AnimationTree; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAnimationTreeEx';

{*
 * @brief Creates an animation tree using given animation player, with optional root motion support and callback.
 *
 * Allocates memory for animation nodes pool, sets root bone index and update callback.
 *
 * @param player Animation player used by the animation tree.
 * @param maxSize Size of the animation nodes pool; maximum number of animation nodes in the tree.
 * @param rootBone Root bone index; set -1 to disable root motion.
 * @param updateCallback Callback function pointer to receive and modify final animation transformation, can be NULL.
 * @param updateUserData User data pointer passed to the callback, can be NULL.
 * @return Newly created animation tree, or a zeroed struct on failure.
 *}
function R3D_LoadAnimationTreePro(player: TR3D_AnimationPlayer; maxSize: Integer; rootBone: Integer;
  updateCallback: TR3D_AnimationTreeCallback; updateUserData: Pointer): TR3D_AnimationTree; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAnimationTreePro';

{*
 * @brief Releases all resources used by an animation tree, including all animation nodes.
 *
 * @param tree Animation tree to unload.
 *}
procedure R3D_UnloadAnimationTree(tree: TR3D_AnimationTree); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadAnimationTree';

{*
 * @brief Updates the animation tree: calculates blended pose, sets and uploads the pose through associated animation player.
 *
 * @param tree Animation tree.
 * @param dt Delta time in seconds.
 *}
procedure R3D_UpdateAnimationTree(tree: PR3D_AnimationTree; dt: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateAnimationTree';

{*
 * @brief Updates the animation tree: calculates blended pose, sets and uploads the pose through associated animation player.
 *
 * Set the rootMotion and/or rootDistance pointers to get root motion information.
 * Divide rootMotion translation vector by dt to get root bone speed.
 *
 * @param tree Animation tree.
 * @param dt Delta time in seconds.
 * @param rootMotion Pointer to root bone motion transformation.
 * @param rootDistance Pointer to transformation containing root bone distance from rest pose.
 *}
procedure R3D_UpdateAnimationTreeEx(tree: PR3D_AnimationTree; dt: Single;
  rootMotion: PTransform; rootDistance: PTransform); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateAnimationTreeEx';

{*
 * @brief Sets root animation node of the animation tree.
 *
 * @param tree Animation tree.
 * @param node Root animation node.
 *}
procedure R3D_AddRootAnimationNode(tree: PR3D_AnimationTree; node: PR3D_AnimationTreeNode); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_AddRootAnimationNode';

{*
 * @brief Connects two animation nodes of animation tree hierarchy.
 *
 * @param parent Parent animation node.
 * @param node Child animation node.
 * @param inputIndex Index of the parent node input used for connection.
 * @return True on success; false if parent node or inputIndex is invalid.
 *}
function R3D_AddAnimationNode(parent: PR3D_AnimationTreeNode; node: PR3D_AnimationTreeNode;
  inputIndex: Cardinal): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_AddAnimationNode';

{*
 * @brief Creates animation node of type Animation.
 *
 * Animation is a leaf node, holding the R3D_Animation structure.
 *
 * @param tree Animation tree.
 * @param params Animation node initial parameters.
 * @return Pointer to created animation node; NULL if maximum number of nodes was exceeded or animation name is not found.
 *}
function R3D_CreateAnimationNode(tree: PR3D_AnimationTree;
  params: TR3D_AnimationNodeParams): PR3D_AnimationTreeNode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateAnimationNode';

{*
 * @brief Creates animation node of type Animation, optionally sets animation currentTime.
 *
 * Animation is a leaf node, holding the R3D_Animation structure.
 *
 * @param tree Animation tree.
 * @param params Animation node initial parameters.
 * @param setTime Flag to set currentTime value based on animation speed; useful for animations with negative speed in State Machine.
 * @return Pointer to created animation node; NULL if maximum number of nodes was exceeded or animation name is not found.
 *}
function R3D_CreateAnimationNodeEx(tree: PR3D_AnimationTree;
  params: TR3D_AnimationNodeParams; setTime: Boolean): PR3D_AnimationTreeNode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateAnimationNodeEx';

{*
 * @brief Creates animation node of type Blend2.
 *
 * Blend2 node blends channels of any two animation nodes together, with respecting optional bone mask.
 *
 * @param tree Animation tree.
 * @param params Blend2 node initial parameters.
 * @return Pointer to created animation node; NULL if maximum number of nodes was exceeded.
 *}
function R3D_CreateBlend2Node(tree: PR3D_AnimationTree;
  params: TR3D_Blend2NodeParams): PR3D_AnimationTreeNode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateBlend2Node';

{*
 * @brief Creates animation node of type Add2.
 *
 * Add2 node adds channels of any two animation nodes together, with respecting optional bone mask.
 *
 * @param tree Animation tree.
 * @param params Add2 node initial parameters.
 * @return Pointer to created animation node; NULL if maximum number of nodes was exceeded.
 *}
function R3D_CreateAdd2Node(tree: PR3D_AnimationTree;
  params: TR3D_Add2NodeParams): PR3D_AnimationTreeNode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateAdd2Node';

{*
 * @brief Creates animation node of type Switch.
 *
 * Switch node allows instant or blended/faded transition between any animation nodes connected to inputs.
 *
 * @param tree Animation tree.
 * @param inputCount Number of available inputs.
 * @param params Switch node initial parameters.
 * @return Pointer to created animation node; NULL if maximum number of nodes was exceeded.
 *}
function R3D_CreateSwitchNode(tree: PR3D_AnimationTree; inputCount: Cardinal;
  params: TR3D_SwitchNodeParams): PR3D_AnimationTreeNode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateSwitchNode';

{*
 * @brief Creates animation node of type State Machine (Stm).
 *
 * Stm node allows to create a state machine graph of any animation nodes.
 *
 * @param tree Animation tree.
 * @param statesCount Maximum number of states in the state machine.
 * @param edgesCount Maximum number of edges in the state machine.
 * @return Pointer to created animation node; NULL if maximum number of nodes was exceeded.
 *}
function R3D_CreateStmNode(tree: PR3D_AnimationTree; statesCount, edgesCount: Cardinal): PR3D_AnimationTreeNode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateStmNode';

{*
 * @brief Creates animation node of type State Machine (Stm), with option to disable travel feature (enabled by default).
 *
 * @param tree Animation tree.
 * @param statesCount Maximum number of states in the state machine.
 * @param edgesCount Maximum number of edges in the state machine.
 * @param enableTravel Flag to enable or disable travel feature; enabled when set true.
 * @return Pointer to created animation node; NULL if maximum number of nodes was exceeded.
 *}
function R3D_CreateStmNodeEx(tree: PR3D_AnimationTree; statesCount, edgesCount: Cardinal;
  enableTravel: Boolean): PR3D_AnimationTreeNode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateStmNodeEx';

{*
 * @brief Creates animation node of type State Machine Stop/Done (StmX).
 *
 * StmX is a helper animation node, that allows stopping state machine evaluation in the current update.
 * Sets animation node done status of the state machine to true.
 *
 * @param tree Animation tree.
 * @param nestedNode Actual animation node with animation channels.
 * @return Pointer to created animation node; NULL if maximum number of nodes was exceeded.
 *}
function R3D_CreateStmXNode(tree: PR3D_AnimationTree;
  nestedNode: PR3D_AnimationTreeNode): PR3D_AnimationTreeNode; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateStmXNode';

{*
 * @brief Creates state in a State Machine animation node.
 *
 * @param stmNode Animation node of type State Machine.
 * @param stateNode Associated animation node of the state.
 * @param outEdgesCount Number of output edges of the state.
 * @return Index of created state; -1 if maximum number of states was exceeded.
 *}
function R3D_CreateStmNodeState(stmNode: PR3D_AnimationTreeNode; stateNode: PR3D_AnimationTreeNode;
  outEdgesCount: Cardinal): R3D_AnimationStmIndex; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateStmNodeState';

{*
 * @brief Creates edge in a State Machine animation node.
 *
 * @param stmNode Animation node of type State Machine.
 * @param beginStateIndex Index of state connected to the edge beginning.
 * @param endStateIndex Index of state connected to the edge end.
 * @param params Edge initial parameters.
 * @return Index of created edge; -1 if maximum number of edges was exceeded.
 *}
function R3D_CreateStmNodeEdge(stmNode: PR3D_AnimationTreeNode;
  beginStateIndex, endStateIndex: R3D_AnimationStmIndex;
  params: TR3D_StmEdgeParams): R3D_AnimationStmIndex; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateStmNodeEdge';

{*
 * @brief Sets parameters of animation node Animation.
 *
 * @param node Animation node of type Animation.
 * @param params New parameters.
 *}
procedure R3D_SetAnimationNodeParams(node: PR3D_AnimationTreeNode;
  params: TR3D_AnimationNodeParams); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAnimationNodeParams';

{*
 * @brief Gets parameters of animation node Animation.
 *
 * @param node Animation node of type Animation.
 * @return Current parameters.
 *}
function R3D_GetAnimationNodeParams(node: PR3D_AnimationTreeNode): TR3D_AnimationNodeParams; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAnimationNodeParams';

{*
 * @brief Sets parameters of animation node Blend2.
 *
 * @param node Animation node of type Blend2.
 * @param params New parameters.
 *}
procedure R3D_SetBlend2NodeParams(node: PR3D_AnimationTreeNode;
  params: TR3D_Blend2NodeParams); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetBlend2NodeParams';

{*
 * @brief Gets parameters of animation node Blend2.
 *
 * @param node Animation node of type Blend2.
 * @return Current parameters.
 *}
function R3D_GetBlend2NodeParams(node: PR3D_AnimationTreeNode): TR3D_Blend2NodeParams; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBlend2NodeParams';

{*
 * @brief Sets parameters of animation node Add2.
 *
 * @param node Animation node of type Add2.
 * @param params New parameters.
 *}
procedure R3D_SetAdd2NodeParams(node: PR3D_AnimationTreeNode;
  params: TR3D_Add2NodeParams); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetAdd2NodeParams';

{*
 * @brief Gets parameters of animation node Add2.
 *
 * @param node Animation node of type Add2.
 * @return Current parameters.
 *}
function R3D_GetAdd2NodeParams(node: PR3D_AnimationTreeNode): TR3D_Add2NodeParams; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetAdd2NodeParams';

{*
 * @brief Sets parameters of animation node Switch.
 *
 * @param node Animation node of type Switch.
 * @param params New parameters.
 *}
procedure R3D_SetSwitchNodeParams(node: PR3D_AnimationTreeNode;
  params: TR3D_SwitchNodeParams); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSwitchNodeParams';

{*
 * @brief Gets parameters of animation node Switch.
 *
 * @param node Animation node of type Switch.
 * @return Current parameters.
 *}
function R3D_GetSwitchNodeParams(node: PR3D_AnimationTreeNode): TR3D_SwitchNodeParams; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetSwitchNodeParams';

{*
 * @brief Sets parameters of State Machine edge.
 *
 * @param node Animation node of type State Machine (Stm).
 * @param edgeIndex Index of the State Machine edge.
 * @param params New parameters of the edge.
 *}
procedure R3D_SetStmNodeEdgeParams(node: PR3D_AnimationTreeNode;
  edgeIndex: R3D_AnimationStmIndex; params: TR3D_StmEdgeParams); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetStmNodeEdgeParams';

{*
 * @brief Gets parameters of State Machine edge.
 *
 * @param node Animation node of type State Machine.
 * @param edgeIndex Index of the State Machine edge.
 * @return Current parameters of the edge.
 *}
function R3D_GetStmNodeEdgeParams(node: PR3D_AnimationTreeNode;
  edgeIndex: R3D_AnimationStmIndex): TR3D_StmEdgeParams; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetStmNodeEdgeParams';

{*
 * @brief Gets active state index of State Machine.
 *
 * @param node Animation node of type State Machine.
 * @return Index of current active state.
 *}
function R3D_GetStmStateActiveIndex(node: PR3D_AnimationTreeNode): R3D_AnimationStmIndex; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetStmStateActiveIndex';

{*
 * @brief Sets travel path inside State Machine, from current state to target.
 *
 * If travel path is not found, target is set as current state instantly (teleport).
 *
 * @param node Animation node of type State Machine.
 * @param targetStateIndex Index of the targeted state.
 *}
procedure R3D_TravelToStmState(node: PR3D_AnimationTreeNode;
  targetStateIndex: R3D_AnimationStmIndex); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_TravelToStmState';

{*
 * @brief Computes bone mask from list of bone names.
 *
 * Only listed bones will be included in evaluation of animation node with this bone mask.
 * Can be used in Blend2 and Add2 animation nodes.
 *
 * @param skeleton Skeleton of associated animation player.
 * @param boneNames Array of pointers to null-terminated strings with bone names.
 * @param boneNameCount Count of strings in boneNames array.
 * @return Calculated bone mask, or zeroed structure on failure.
 *}
function R3D_ComputeBoneMask(const skeleton: PR3D_Skeleton;
  boneNames: PPAnsiChar; boneNameCount: Cardinal): TR3D_BoneMask; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ComputeBoneMask';

