program AnimationExample;

{$mode objfpc}{$H+}

uses
  Classes, SysUtils, Math,
  raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 450;

var
  cubemap: TR3D_Cubemap;
  ambientMap: TR3D_AmbientMap;
  plane: TR3D_Mesh;
  model: TR3D_Model;
  modelAnims: TR3D_AnimationLib;
  modelPlayer: TR3D_AnimationPlayer;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  light: TR3D_Light;
  camera: TCamera3D;
  delta: Single;
  x, z: Integer;

begin
  // Initialize window
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Animation example');
  SetTargetFPS(60);

  // Initialize R3D with FXAA
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);
  R3D_SetAntiAliasing(R3D_ANTI_ALIASING_FXAA);

  // Setup environment sky
  cubemap := R3D_LoadCubemap(RESOURCES_PATH + 'panorama/indoor.hdr', R3D_CUBEMAP_LAYOUT_AUTO_DETECT);

  // Use R3D_GetEnvironment()^ instead of R3D_ENVIRONMENT_SET
  R3D_GetEnvironment()^.background.skyBlur := 0.3;
  R3D_GetEnvironment()^.background.energy := 0.6;
  R3D_GetEnvironment()^.background.sky := cubemap;

  // Setup environment ambient
  ambientMap := R3D_GenAmbientMap(cubemap, R3D_AMBIENT_ILLUMINATION);
  R3D_GetEnvironment()^.ambient.map := ambientMap;
  R3D_GetEnvironment()^.ambient.energy := 0.25;

  // Setup tonemapping
  R3D_GetEnvironment()^.tonemap.mode := R3D_TONEMAP_FILMIC;
  R3D_GetEnvironment()^.tonemap.exposure := 0.75;

  // Generate a ground plane and load the animated model
  plane := R3D_GenMeshPlane(10, 10, 1, 1);
  model := R3D_LoadModel(RESOURCES_PATH + 'models/CesiumMan.glb');

  // Load animations
  modelAnims := R3D_LoadAnimationLib(RESOURCES_PATH + 'models/CesiumMan.glb');
  modelPlayer := R3D_LoadAnimationPlayer(model.skeleton, modelAnims);
  modelPlayer.states[0].weight := 1.0;
  modelPlayer.states[0].loop := True;

  // Create model instances
  instances := R3D_LoadInstanceBuffer(4, R3D_INSTANCE_POSITION);
  positions := R3D_MapInstances(instances, R3D_INSTANCE_POSITION);

  z := 0;
  while z < 2 do
  begin
    x := 0;
    while x < 2 do
    begin
      positions[z * 2 + x].x := x - 0.5;
      positions[z * 2 + x].y := 0.0;
      positions[z * 2 + x].z := z - 0.5;
      Inc(x);
    end;
    Inc(z);
  end;

  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION);

  // Setup lights with shadows
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(-1.0, -1.0, -1.0));
  R3D_SetLightActive(light, True);
  R3D_SetLightRange(light, 10.0);
  R3D_EnableShadow(light);

  // Setup camera
  camera.position := Vector3Create(0.0, 1.5, 3.0);
  camera.target := Vector3Create(0.0, 0.75, 0.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();

    UpdateCamera(@camera, CAMERA_ORBITAL);
    R3D_UpdateAnimationPlayer(@modelPlayer, delta);

    BeginDrawing();
      ClearBackground(RAYWHITE);
      R3D_Begin(camera);
        R3D_DrawMesh(plane, R3D_GetBaseMaterial, Vector3Zero(), 1.0);
        R3D_DrawAnimatedModel(model, modelPlayer, Vector3Zero(), 1.25);
        R3D_DrawAnimatedModelInstanced(model, modelPlayer, instances, 4);
      R3D_End();
    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadAnimationPlayer(modelPlayer);
  R3D_UnloadAnimationLib(modelAnims);
  R3D_UnloadModel(model, True);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.
