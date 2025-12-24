program PBRCarExample;

uses
  SysUtils, Math,
  raylib, raymath, r3d;

const
  RESOURCES_PATH = 'resources/';

var
  model: TR3D_Model;
  ground: TR3D_Mesh;
  groundMat: TR3D_Material;
  skybox: TR3D_Skybox;
  camera: TCamera3D;
  light: TR3D_Light;
  showSkybox: Boolean = false;

function Init: PChar;
begin
  { --- Initialize R3D with its internal resolution and flags --- }

  R3D_Init(GetScreenWidth, GetScreenHeight,
    R3D_FLAG_TRANSPARENT_SORTING or R3D_FLAG_FXAA);
  SetTargetFPS(60);

  { --- Setup background color and ambient light --- }

  R3D_GetEnvironment^.background.color := BLACK;
  R3D_GetEnvironment^.ambient.color := DARKGRAY;

  { --- Setup post processing parameters --- }

  R3D_GetEnvironment^.ssr.enabled := true;

  R3D_GetEnvironment^.ssao.enabled := true;
  R3D_GetEnvironment^.ssao.radius := 2.0;

  R3D_GetEnvironment^.bloom.intensity := 0.1;
  R3D_GetEnvironment^.bloom.mode := R3D_BLOOM_MIX;
  R3D_GetEnvironment^.tonemap.mode := R3D_TONEMAP_ACES;

  { --- Load the car model and apply scaling on import --- }

  model := R3D_LoadModel(PChar(RESOURCES_PATH + 'pbr/car.glb'));

  { --- Generate ground mesh and setup its material --- }

  ground := R3D_GenMeshPlane(10.0, 10.0, 1, 1);

  groundMat := R3D_GetDefaultMaterial();
  groundMat.albedo.color := ColorCreate(31, 31, 31, 255);
  groundMat.orm.roughness := 0.0;
  groundMat.orm.metalness := 0.5;

  { --- Load skybox (disabled by default) --- }

  skybox := R3D_LoadSkybox(
    PChar(RESOURCES_PATH + 'sky/skybox3.png'),
    CUBEMAP_LAYOUT_AUTO_DETECT
  );

  { --- Configure the scene lighting --- }

  light := R3D_CreateLight(R3D_LIGHT_DIR);

  R3D_SetLightDirection(light, Vector3Create(-1, -1, -1));
  R3D_EnableShadow(light, 4096);
  R3D_SetLightActive(light, true);
  R3D_SetLightRange(light, 10);

  { --- Setup camera --- }

  camera := Default(TCamera3D);
  camera.position := Vector3Create(0, 0, 5);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  { --- Capture the mouse and let's go! --- }

  DisableCursor();

  Result := '[r3d] - PBR car example';
end;

procedure Update(delta: Single);
begin
  UpdateCamera(@camera, CAMERA_FREE);

  if IsKeyPressed(KEY_O) then
  begin
    R3D_GetEnvironment^.ssao.enabled := not R3D_GetEnvironment^.ssao.enabled;
  end;

  if IsKeyPressed(KEY_T) then
  begin
    showSkybox := not showSkybox;
    if showSkybox then
      R3D_GetEnvironment^.background.sky := skybox
    else
      R3D_GetEnvironment^.background.sky := Default(TR3D_Skybox);
  end;
end;

procedure Draw;
begin
  R3D_Begin(camera);
    R3D_DrawMesh(@ground, @groundMat,
      MatrixTranslate(0.0, -0.4, 0.0));
    R3D_DrawModel(@model, Vector3Create(0, 0, 0), 1.0);
  R3D_End();

  DrawText('Model made by MaximePages', 10, GetScreenHeight - 30, 20, LIGHTGRAY);
end;

procedure Close;
begin
  R3D_UnloadModel(@model, true);
  R3D_UnloadSkybox(skybox);
  R3D_Close();
end;

{ Main program }
var
  screenWidth, screenHeight: Integer;
begin
  screenWidth := 800;
  screenHeight := 600;

  InitWindow(screenWidth, screenHeight, 'R3D PBR Car Example');

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
