program animtree_example;

{$IFDEF FPC}
  {$MODE Delphi}
{$ENDIF}

uses
  SysUtils,
  raylib,
  r3d,
  raymath;

const
  RESOURCES_PATH = 'resources/';

var
  camera: TCamera3D;
  cubemap: TR3D_Cubemap;
  ambientMap: TR3D_AmbientMap;
  plane: TR3D_Mesh;
  model: TR3D_Model;
  modelAnims: TR3D_AnimationLib;
  modelPlayer: TR3D_AnimationPlayer;
  animTree: TR3D_AnimationTree;
  light: TR3D_Light;

  // Animation nodes
  leftRightStmNode, forwBackStmNode, switchNode, idleNode: PR3D_AnimationTreeNode;
  animNode0, animNode1, animNode2, animNode3: PR3D_AnimationTreeNode;

  // Parameters
  animState: TR3D_AnimationState;
  edgeParams, fadedEdgeParams: TR3D_StmEdgeParams;
  loopingAnimParams: TR3D_AnimationNodeParams;
  switchParams: TR3D_SwitchNodeParams;

  // State indices
  stateIdx0, stateIdx1, stateIdx2, stateIdx3: R3D_AnimationStmIndex;

  delta: Single;

begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Animation tree example');
  SetTargetFPS(60);

  // Initialize R3D with FXAA
  R3D_Init(GetScreenWidth(), GetScreenHeight());
  R3D_SetAntiAliasingMode(R3D_ANTI_ALIASING_MODE_FXAA);

  // Setup environment sky
  cubemap := R3D_LoadCubemap(RESOURCES_PATH + 'panorama/indoor.hdr', R3D_CUBEMAP_LAYOUT_AUTO_DETECT);
  R3D_ENVIRONMENT_SET('background.skyBlur', 0.3);
  R3D_ENVIRONMENT_SET('background.energy', 0.6);
  R3D_ENVIRONMENT_SET('background.sky', cubemap);

  // Setup environment ambient
  ambientMap := R3D_GenAmbientMap(cubemap, R3D_AMBIENT_ILLUMINATION);
  R3D_ENVIRONMENT_SET('ambient.map', ambientMap);
  R3D_ENVIRONMENT_SET('ambient.energy', 0.25);

  // Setup tonemapping
  R3D_ENVIRONMENT_SET('tonemap.mode', R3D_TONEMAP_FILMIC);
  R3D_ENVIRONMENT_SET('tonemap.exposure', 0.75);

  // Generate a ground plane and load the animated model
  plane := R3D_GenMeshPlane(10, 10, 1, 1);
  model := R3D_LoadModel(RESOURCES_PATH + 'models/YBot.glb');

  // Load animations
  modelAnims := R3D_LoadAnimationLib(RESOURCES_PATH + 'models/YBot.glb');
  modelPlayer := R3D_LoadAnimationPlayer(model.skeleton, modelAnims);

  // Create & define animation tree structure
  animTree := R3D_LoadAnimationTreeEx(modelPlayer, 12, 0);

  // Initialize animation state
  animState.speed := 0.8;
  animState.play := True;
  animState.loop := True;

  // Initialize edge parameters
  edgeParams.mode := R3D_STM_EDGE_ONDONE;
  edgeParams.status := R3D_STM_EDGE_AUTO;
  edgeParams.nextStatus := R3D_STM_EDGE_OFF;
  edgeParams.xFadeTime := 0.0;

  fadedEdgeParams.mode := R3D_STM_EDGE_ONDONE;
  fadedEdgeParams.status := R3D_STM_EDGE_AUTO;
  fadedEdgeParams.nextStatus := R3D_STM_EDGE_OFF;
  fadedEdgeParams.xFadeTime := 0.3;

  // Initialize looping animation parameters
  loopingAnimParams.state := animState;
  loopingAnimParams.looper := True;
  loopingAnimParams.evalCallback := nil;
  loopingAnimParams.evalUserData := nil;
  FillChar(loopingAnimParams.name, SizeOf(loopingAnimParams.name), 0);

  // ===== Create left-right state machine =====
  leftRightStmNode := R3D_CreateStmNode(@animTree, 4, 4);
  if leftRightStmNode <> nil then
  begin
    // Create animation nodes
    StrPCopy(loopingAnimParams.name, 'walk left');
    animNode0 := R3D_CreateAnimationNode(@animTree, loopingAnimParams);
    animNode1 := R3D_CreateAnimationNode(@animTree, loopingAnimParams);

    StrPCopy(loopingAnimParams.name, 'walk right');
    animNode2 := R3D_CreateAnimationNode(@animTree, loopingAnimParams);
    animNode3 := R3D_CreateAnimationNode(@animTree, loopingAnimParams);

    // Create states and edges
    stateIdx0 := R3D_CreateStmNodeState(leftRightStmNode, animNode0, 1);
    stateIdx1 := R3D_CreateStmNodeState(leftRightStmNode, animNode1, 1);
    stateIdx2 := R3D_CreateStmNodeState(leftRightStmNode, animNode2, 1);
    stateIdx3 := R3D_CreateStmNodeState(leftRightStmNode, animNode3, 1);

    R3D_CreateStmNodeEdge(leftRightStmNode, stateIdx0, stateIdx1, edgeParams);
    R3D_CreateStmNodeEdge(leftRightStmNode, stateIdx1, stateIdx2, fadedEdgeParams);
    R3D_CreateStmNodeEdge(leftRightStmNode, stateIdx2, stateIdx3, edgeParams);
    R3D_CreateStmNodeEdge(leftRightStmNode, stateIdx3, stateIdx0, fadedEdgeParams);
  end;

  // ===== Create forward-backward state machine =====
  forwBackStmNode := R3D_CreateStmNode(@animTree, 4, 4);
  if forwBackStmNode <> nil then
  begin
    // Create animation nodes
    StrPCopy(loopingAnimParams.name, 'walk forward');
    animNode0 := R3D_CreateAnimationNode(@animTree, loopingAnimParams);
    animNode1 := R3D_CreateAnimationNode(@animTree, loopingAnimParams);

    StrPCopy(loopingAnimParams.name, 'walk backward');
    animNode2 := R3D_CreateAnimationNode(@animTree, loopingAnimParams);
    animNode3 := R3D_CreateAnimationNode(@animTree, loopingAnimParams);

    // Create states and edges
    stateIdx0 := R3D_CreateStmNodeState(forwBackStmNode, animNode0, 1);
    stateIdx1 := R3D_CreateStmNodeState(forwBackStmNode, animNode1, 1);
    stateIdx2 := R3D_CreateStmNodeState(forwBackStmNode, animNode2, 1);
    stateIdx3 := R3D_CreateStmNodeState(forwBackStmNode, animNode3, 1);

    R3D_CreateStmNodeEdge(forwBackStmNode, stateIdx0, stateIdx1, edgeParams);
    R3D_CreateStmNodeEdge(forwBackStmNode, stateIdx1, stateIdx2, fadedEdgeParams);
    R3D_CreateStmNodeEdge(forwBackStmNode, stateIdx2, stateIdx3, edgeParams);
    R3D_CreateStmNodeEdge(forwBackStmNode, stateIdx3, stateIdx0, fadedEdgeParams);
  end;

  // ===== Create switch node =====
  switchParams.synced := False;
  switchParams.activeInput := 0;
  switchParams.xFadeTime := 0.4;

  switchNode := R3D_CreateSwitchNode(@animTree, 3, switchParams);

  // Create idle node
  FillChar(loopingAnimParams.name, SizeOf(loopingAnimParams.name), 0);
  StrPCopy(loopingAnimParams.name, 'idle');
  idleNode := R3D_CreateAnimationNode(@animTree, loopingAnimParams);

  // Connect nodes
  if (switchNode <> nil) and (idleNode <> nil) then
  begin
    R3D_AddAnimationNode(switchNode, idleNode, 0);
    R3D_AddAnimationNode(switchNode, leftRightStmNode, 1);
    R3D_AddAnimationNode(switchNode, forwBackStmNode, 2);
    R3D_AddRootAnimationNode(@animTree, switchNode);
  end;

  // Setup lights with shadows
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(-1.0, -1.0, -1.0));
  R3D_EnableLight(light);
  R3D_SetLightRange(light, 10.0);
  R3D_EnableShadow(light);

  // Setup camera
  camera.position := Vector3Create(0, 1.5, 3.0);
  camera.target := Vector3Create(0, 0.75, 0.0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();

    // Handle input for switch node
    if IsKeyDown(KEY_ONE) then
      switchParams.activeInput := 0;
    if IsKeyDown(KEY_TWO) then
      switchParams.activeInput := 1;
    if IsKeyDown(KEY_THREE) then
      switchParams.activeInput := 2;

    if switchNode <> nil then
      R3D_SetSwitchNodeParams(switchNode, switchParams);

    // Update camera and animation
    UpdateCamera(@camera, CAMERA_ORBITAL);
    R3D_UpdateAnimationTree(@animTree, delta);

    // Draw
    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);
        R3D_DrawMesh(plane, R3D_MATERIAL_BASE, Vector3Zero(), 1.0);
        R3D_DrawAnimatedModel(model, modelPlayer, Vector3Zero(), 1.0);
      R3D_End();

      DrawText('Press ''1'' to idle', 10, GetScreenHeight() - 74, 20, BLACK);
      DrawText('Press ''2'' to walk left and right', 10, GetScreenHeight() - 54, 20, BLACK);
      DrawText('Press ''3'' to walk forward and backward', 10, GetScreenHeight() - 34, 20, BLACK);
    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadAnimationTree(animTree);
  R3D_UnloadAnimationPlayer(modelPlayer);
  R3D_UnloadAnimationLib(modelAnims);
  R3D_UnloadModel(model, True);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.
