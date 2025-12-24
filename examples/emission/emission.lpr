program EmissionExample;

uses
  SysUtils, Math,
  raylib, raymath, r3d;

const
  RESOURCES_PATH = 'resources/';

var
  model: TR3D_Model;
  plane: TR3D_Mesh;
  material: TR3D_Material;
  camera: TCamera3D;
  light: TR3D_Light;
  rotModel: Single = 0.0;

procedure ToggleLight;
begin
  if R3D_IsLightActive(light) then
  begin
    R3D_SetLightActive(light, false);
    R3D_GetEnvironment^.ambient.color := BLACK;
  end
  else
  begin
    R3D_SetLightActive(light, true);
    R3D_GetEnvironment^.ambient.color := DARKGRAY;
  end;
end;

function Init: PChar;
begin
  { --- Initialize R3D with its internal resolution --- }

  R3D_Init(GetScreenWidth, GetScreenHeight, 0);
  SetTargetFPS(60);

  { --- Configure the background color and ambient lighting --- }

  R3D_GetEnvironment^.background.color := BLACK;
  R3D_GetEnvironment^.ambient.color := DARKGRAY;

  { --- Configure the post process parameters --- }

  R3D_GetEnvironment^.tonemap.mode := R3D_TONEMAP_ACES;
  R3D_GetEnvironment^.tonemap.exposure := 0.8;
  R3D_GetEnvironment^.tonemap.white := 2.5;

  R3D_GetEnvironment^.bloom.mode := R3D_BLOOM_ADDITIVE;
  R3D_GetEnvironment^.bloom.softThreshold := 0.2;
  R3D_GetEnvironment^.bloom.threshold := 0.6;
  R3D_GetEnvironment^.bloom.intensity := 0.2;

  { --- Loads the main model of the scene --- }

  model := R3D_LoadModel(PChar(RESOURCES_PATH + 'emission.glb'));

  { --- Generates a mesh for the ground and load a material for it --- }

  plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  material := R3D_GetDefaultMaterial();

  { --- Setup the scene lighting --- }

  light := R3D_CreateLight(R3D_LIGHT_SPOT);

  R3D_LightLookAt(light, Vector3Create(0, 10, 5), Vector3Create(0, 0, 0));
  R3D_SetLightOuterCutOff(light, 45.0);
  R3D_SetLightInnerCutOff(light, 22.5);
  R3D_EnableShadow(light, 4096);
  R3D_SetLightActive(light, true);

  { --- Setup the camera --- }

  camera := Default(TCamera3D);
  camera.position := Vector3Create(-1.0, 1.75, 1.75);
  camera.target := Vector3Create(0, 0.5, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  Result := '[r3d] - Emission example';
end;

procedure Update(delta: Single);
var
  mouseDelta: TVector2;
begin
  if IsKeyPressed(KEY_SPACE) then
  begin
    ToggleLight;
  end;

  if IsMouseButtonDown(MOUSE_LEFT_BUTTON) then
  begin
    mouseDelta := GetMouseDelta();
    camera.position.y := Clamp(camera.position.y + 0.01 * mouseDelta.y, 0.25, 2.5);
    rotModel := rotModel + 0.01 * mouseDelta.x;
  end;
end;

procedure Draw;
begin
  R3D_Begin(camera);
    R3D_DrawMesh(@plane, @material, MatrixIdentity());
    R3D_DrawModelEx(@model, Vector3Create(0, 0, 0),
      Vector3Create(0, 1, 0), rotModel, Vector3Create(1.0, 1.0, 1.0));
  R3D_End();

  DrawText('Press SPACE to toggle the light', 10, 10, 20, LIME);
  DrawText('Model by har15204405', 10, GetScreenHeight - 30, 20, LIGHTGRAY);
end;

procedure Close;
begin
  R3D_UnloadModel(@model, true);
  R3D_UnloadMesh(@plane);
  R3D_Close();
end;

{ Main program }
var
  screenWidth, screenHeight: Integer;
begin
  screenWidth := 800;
  screenHeight := 600;

  InitWindow(screenWidth, screenHeight, 'R3D Emission Example');

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
