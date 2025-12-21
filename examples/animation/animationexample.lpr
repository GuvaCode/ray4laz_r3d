program AnimationExample;

{$mode objfpc}{$H+}

uses
  Math, SysUtils,
  raylib, raymath, r3d;

const
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 600;

var
  // === Resources ===
  Plane: TR3D_Mesh;
  PlaneMat: TR3D_Material;
  Dancer: TR3D_Model;
  DancerInstances: array[0..3] of TMatrix;
  DancerAnims: TR3D_AnimationLib;
  DancerPlayer: TR3D_AnimationPlayer;
  Camera: TCamera3D;
  Lights: array[0..1] of TR3D_Light;

procedure InitExample;
var
  Checked: TImage;
  Z, X: Integer;
begin
  // --- Initialize R3D with FXAA and disable frustum culling ---
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, R3D_FLAG_FXAA or R3D_FLAG_NO_FRUSTUM_CULLING);

  // --- Set the application frame rate ---
  SetTargetFPS(60);

  // --- Enable post-processing effects ---
  R3D_SetSSAO(True);
  R3D_SetBloomIntensity(0.03);
  R3D_SetBloomMode(R3D_BLOOM_ADDITIVE);
  R3D_SetTonemapMode(R3D_TONEMAP_ACES);

  // --- Set background and ambient lighting colors ---
  R3D_SetBackgroundColor(BLACK);
  R3D_SetAmbientColor(ColorCreate(7, 7, 7, 255));

  // --- Generate a plane to serve as the ground and setup its material ---
  Plane := R3D_GenMeshPlane(32, 32, 1, 1);

  PlaneMat := R3D_GetDefaultMaterial();
  PlaneMat.orm.roughness := 0.5;
  PlaneMat.orm.metalness := 0.5;
  PlaneMat.uvScale.x := 64.0;
  PlaneMat.uvScale.y := 64.0;

  Checked := GenImageChecked(2, 2, 1, 1, ColorCreate(20, 20, 20, 255), WHITE);
  PlaneMat.albedo.texture := LoadTextureFromImage(Checked);
  UnloadImage(Checked);

  SetTextureWrap(PlaneMat.albedo.texture, TEXTURE_WRAP_REPEAT);

  // --- Load the 3D model and its default material ---
  Dancer := R3D_LoadModel(RESOURCES_PATH + 'dancer.glb');

  // --- Create instance matrices for multiple model copies ---
  for Z := 0 to 1 do
  begin
    for X := 0 to 1 do
    begin
      DancerInstances[Z * 2 + X] := MatrixTranslate(X - 0.5, 0, Z - 0.5);
    end;
  end;

  // --- Load model animations ---
  DancerAnims := R3D_LoadAnimationLib(RESOURCES_PATH + 'dancer.glb');
  Dancer.player := R3D_LoadAnimationPlayer(@Dancer.skeleton, @DancerAnims);
  Dancer.player^.states[0].weight := 1.0;
  Dancer.player^.states[0].loop := True;

  // --- Setup scene lights with shadows ---
  Lights[0] := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightPosition(Lights[0], Vector3Create(-10.0, 25.0, 0.0));
  R3D_EnableShadow(Lights[0], 4096);
  R3D_SetLightActive(Lights[0], True);

  Lights[1] := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightPosition(Lights[1], Vector3Create(+10.0, 25.0, 0.0));
  R3D_EnableShadow(Lights[1], 4096);
  R3D_SetLightActive(Lights[1], True);

  // --- Setup the camera ---
  Camera.position := Vector3Create(0, 2.0, 3.5);
  Camera.target := Vector3Create(0, 1.0, 1.5);
  Camera.up := Vector3Create(0, 1, 0);
  Camera.fovy := 60;
  Camera.projection := CAMERA_PERSPECTIVE;

  // --- Capture the mouse and let's go! ---
  DisableCursor();
end;

procedure UpdateExample(Delta: Single);
begin
  UpdateCamera(@Camera, CAMERA_FREE);
  R3D_UpdateAnimationPlayer(Dancer.player, Delta);

  R3D_SetLightColor(Lights[0], ColorFromHSV(90.0 * GetTime() + 90.0, 1.0, 1.0));
  R3D_SetLightColor(Lights[1], ColorFromHSV(90.0 * GetTime() - 90.0, 1.0, 1.0));
end;

procedure DrawExample;
begin
  R3D_Begin(Camera);
    R3D_DrawMesh(@Plane, @PlaneMat, MatrixIdentity());
     R3D_DrawModel(@Dancer, Vector3Create(0, 0, 1.5), 1.0);
    R3D_DrawModelInstanced(@Dancer, @DancerInstances[0], 2*2);
  R3D_End();

  DrawText('Model made by zhuoyi0904', 10, GetScreenHeight() - 30, 20, RAYWHITE);
end;

procedure CloseExample;
begin
  R3D_UnloadMesh(@Plane);
  R3D_UnloadModel(@Dancer, True);
  R3D_UnloadMaterial(@PlaneMat);
  R3D_UnloadAnimationLib(@DancerAnims);
  R3D_UnloadAnimationPlayer(@DancerPlayer);
  R3D_Close();
end;

begin
  // Инициализация окна
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Animation example');

  InitExample;

  while not WindowShouldClose() do
  begin
    UpdateExample(GetFrameTime());

    BeginDrawing();
      ClearBackground(BLACK);
      DrawExample;
    EndDrawing();
  end;

  CloseExample;
  CloseWindow();
end.
