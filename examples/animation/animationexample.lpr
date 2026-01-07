program AnimationExample;

{$mode objfpc}{$H+}

uses
  raylib,
  r3d,
  raymath,
  math;

const
  RESOURCES_PATH = 'resources/';

var
  plane: TR3D_Mesh;
  planeMat: TR3D_Material;
  checkedImg: TImage;
  dancer: TR3D_Model;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  dancerAnims: TR3D_AnimationLib;
  dancerPlayer: TR3D_AnimationPlayer;
  lights: array[0..1] of TR3D_Light;
  camera: TCamera3D;
  delta: Single;
  z, x: Integer;
begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Animation example');
  SetTargetFPS(60);

  // Initialize R3D with FXAA and no frustum culling
  R3D_Init(GetScreenWidth(), GetScreenHeight(), R3D_FLAG_FXAA);

  // Enable post-processing effects
  R3D_GetEnvironment()^.ssao.enabled := True;
  R3D_GetEnvironment()^.bloom.intensity := 0.03;
  R3D_GetEnvironment()^.bloom.mode := R3D_BLOOM_ADDITIVE;
  R3D_GetEnvironment()^.tonemap.mode := R3D_TONEMAP_AGX;

  // Set background and ambient colors
  R3D_GetEnvironment()^.background.color := ColorCreate(12, 10, 15, 255);
  R3D_GetEnvironment()^.ambient.color := ColorCreate(12, 10, 15, 255);

  // Create ground plane
  plane := R3D_GenMeshPlane(32, 32, 1, 1);
  planeMat := R3D_GetDefaultMaterial();
  planeMat.orm.roughness := 0.5;
  planeMat.orm.metalness := 0.5;
  planeMat.uvScale := Vector2Create(8.0, 8.0);

  checkedImg := GenImageChecked(512, 512, 32, 32, ColorCreate(20, 20, 20, 255), WHITE);
  planeMat.albedo.texture := LoadTextureFromImage(checkedImg);
  UnloadImage(checkedImg);

  GenTextureMipmaps(@planeMat.albedo.texture);
  SetTextureFilter(planeMat.albedo.texture, TEXTURE_FILTER_TRILINEAR);
  SetTextureWrap(planeMat.albedo.texture, TEXTURE_WRAP_REPEAT);

  // Load animated model
  dancer := R3D_LoadModel(PChar(RESOURCES_PATH + 'dancer.glb'));

  // Create instance matrices
  instances := R3D_LoadInstanceBuffer(4, R3D_INSTANCE_POSITION);
  positions := R3D_MapInstances(instances, R3D_INSTANCE_POSITION);

  for z := 0 to 1 do
  begin
    for x := 0 to 1 do
    begin
      positions[z * 2 + x] := Vector3Create(x - 0.5, 0, z - 0.5);
    end;
  end;

  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION);

  // Load animations
  dancerAnims := R3D_LoadAnimationLib(PChar(RESOURCES_PATH + 'dancer.glb'));
  dancerPlayer := R3D_LoadAnimationPlayer(dancer.skeleton, dancerAnims);
  dancerPlayer.states[0].weight := 1.0;
  dancerPlayer.states[0].loop := True;

  // Setup lights with shadows
  lights[0] := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightPosition(lights[0], Vector3Create(-10.0, 25.0, 0.0));
  R3D_EnableShadow(lights[0], 4096);
  R3D_SetLightEnergy(lights[0], 1.25);
  R3D_SetLightActive(lights[0], True);

  lights[1] := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightPosition(lights[1], Vector3Create(10.0, 25.0, 0.0));
  R3D_EnableShadow(lights[1], 4096);
  R3D_SetLightEnergy(lights[1], 1.25);
  R3D_SetLightActive(lights[1], True);

  // Setup camera
  camera.position := Vector3Create(0, 2.0, 3.5);
  camera.target := Vector3Create(0, 1.0, 1.5);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Capture mouse
  DisableCursor();

  // Main loop
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();

    UpdateCamera(@camera, CAMERA_FREE);
    R3D_UpdateAnimationPlayer(@dancerPlayer, delta);

    // Animate lights
    R3D_SetLightColor(lights[0], ColorFromHSV(90.0 * GetTime() + 90.0, 1.0, 1.0));
    R3D_SetLightColor(lights[1], ColorFromHSV(90.0 * GetTime() - 90.0, 1.0, 1.0));

    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);
        R3D_DrawMesh(plane, planeMat, Vector3Zero(), 1.0);
        R3D_DrawAnimatedModel(dancer, dancerPlayer, Vector3Create(0, 0, 1.5), 1.0);
        R3D_DrawAnimatedModelInstanced(dancer, dancerPlayer, instances, 4);
      R3D_End();

      DrawText('Model made by zhuoyi0904', 10, GetScreenHeight() - 26, 16, LIME);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadAnimationPlayer(dancerPlayer);
  R3D_UnloadAnimationLib(dancerAnims);
  R3D_UnloadModel(dancer, True);
  R3D_UnloadMaterial(planeMat);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.
