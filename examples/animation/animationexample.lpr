program AnimationExample;

uses
  SysUtils, Math,
  raylib, raymath, r3d;

const
  RESOURCES_PATH = 'resources/';

var
  plane: TR3D_Mesh;
  planeMat: TR3D_Material;

  dancer: TR3D_Model;
  dancerInstances: array[0..3] of TMatrix;
  dancerAnims: TR3D_AnimationLib;
  dancerPlayer: PR3D_AnimationPlayer;

  camera: TCamera3D;
  lights: array[0..1] of TR3D_Light;

  frame: Integer = 0;

function Init: PChar;
var
  checked: TImage;
  x, z: Integer;
begin
  { --- Initialize R3D with FXAA and disable frustum culling --- }

  R3D_Init(GetScreenWidth, GetScreenHeight,
    R3D_FLAG_FXAA or R3D_FLAG_NO_FRUSTUM_CULLING);

  { --- Set the application frame rate --- }

  SetTargetFPS(60);

  { --- Enable post-processing effects --- }

  R3D_GetEnvironment^.ssao.enabled := true;
  R3D_GetEnvironment^.bloom.intensity := 0.03;
  R3D_GetEnvironment^.bloom.mode := R3D_BLOOM_ADDITIVE;
  R3D_GetEnvironment^.tonemap.mode := R3D_TONEMAP_ACES;

  { --- Set background and ambient lighting colors --- }

  R3D_GetEnvironment^.background.color := BLACK;
  R3D_GetEnvironment^.ambient.color := ColorCreate(7, 7, 7, 255);

  { --- Generate a plane to serve as the ground and setup its material --- }

  plane := R3D_GenMeshPlane(32, 32, 1, 1);

  planeMat := R3D_GetDefaultMaterial();
  planeMat.orm.roughness := 0.5;
  planeMat.orm.metalness := 0.5;
  planeMat.uvScale.x := 64.0;
  planeMat.uvScale.y := 64.0;

  checked := GenImageChecked(2, 2, 1, 1,
    ColorCreate(20, 20, 20, 255), WHITE);
  planeMat.albedo.texture := LoadTextureFromImage(checked);
  UnloadImage(checked);

  SetTextureWrap(planeMat.albedo.texture, TEXTURE_WRAP_REPEAT);

  { --- Load the 3D model and its default material --- }

  dancer := R3D_LoadModel(RESOURCES_PATH + 'dancer.glb');

  { --- Create instance matrices for multiple model copies --- }

  for z := 0 to 1 do
  begin
    for x := 0 to 1 do
    begin
      dancerInstances[z * 2 + x] :=
        MatrixTranslate(x - 0.5, 0, z - 0.5);
    end;
  end;

  { --- Load model animations --- }

  dancerAnims := R3D_LoadAnimationLib(RESOURCES_PATH + 'dancer.glb');
  dancer.player := R3D_LoadAnimationPlayer(@dancer.skeleton, @dancerAnims);
  dancer.player^.states[0].weight := 1.0;
  dancer.player^.states[0].loop := true;

  { --- Setup scene lights with shadows --- }

  lights[0] := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightPosition(lights[0], Vector3Create(-10.0, 25.0, 0.0));
  R3D_EnableShadow(lights[0], 4096);
  R3D_SetLightActive(lights[0], true);

  lights[1] := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightPosition(lights[1], Vector3Create(10.0, 25.0, 0.0));
  R3D_EnableShadow(lights[1], 4096);
  R3D_SetLightActive(lights[1], true);

  { --- Setup the camera --- }

  camera := Default(TCamera3D);
  camera.position := Vector3Create(0, 2.0, 3.5);
  camera.target := Vector3Create(0, 1.0, 1.5);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  { --- Capture the mouse and let's go! --- }

  DisableCursor();

  Result := '[r3d] - Animation example';
end;

procedure Update(delta: Single);
var
  hue1, hue2: Single;
begin
  UpdateCamera(@camera, CAMERA_FREE);
  R3D_UpdateAnimationPlayer(dancer.player, delta);

  hue1 := 90.0 * GetTime() + 90.0;
  hue2 := 90.0 * GetTime() - 90.0;

  R3D_SetLightColor(lights[0], ColorFromHSV(hue1, 1.0, 1.0));
  R3D_SetLightColor(lights[1], ColorFromHSV(hue2, 1.0, 1.0));
end;

procedure Draw;
begin
  R3D_Begin(camera);
    R3D_DrawMesh(@plane, @planeMat, MatrixIdentity());
    R3D_DrawModel(@dancer, Vector3Create(0, 0, 1.5), 1.0);
    R3D_DrawModelInstanced(@dancer, @dancerInstances[0], 4);
  R3D_End();

  DrawText('Model made by zhuoyi0904', 10, GetScreenHeight - 30, 20, LIGHTGRAY);
end;

procedure Close;
begin
  R3D_UnloadMesh(@plane);
  R3D_UnloadModel(@dancer, true);
  R3D_UnloadMaterial(@planeMat);
  R3D_Close();
end;

{ Main program }
var
  screenWidth, screenHeight: Integer;
begin
  screenWidth := 800;
  screenHeight := 600;

  InitWindow(screenWidth, screenHeight, 'R3D Animation Example');

  Init();

  while not WindowShouldClose() do
  begin
    Update(GetFrameTime());

    BeginDrawing();
      ClearBackground(BLACK);
      Draw();
    EndDrawing();
  end;

  Close();

  CloseWindow();
end.
