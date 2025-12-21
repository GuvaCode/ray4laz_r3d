program PBRCarExample;

{$mode objfpc}{$H+}

uses
  Math, SysUtils,
  raylib, raymath, r3d; // Предполагается, что модуль R3D называется r3d

const
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 600;

var
  // === Resources ===
  Model: TR3D_Model;
  Ground: TR3D_Mesh;
  GroundMat: TR3D_Material;
  Skybox: TR3D_Skybox;
  Camera: TCamera3D;
  ShowSkybox: Boolean;

procedure InitExample;
var
  Flags: TR3D_Flags;
  Light: TR3D_Light;
begin
  // --- Initialize R3D with its internal resolution and flags ---
  Flags := R3D_FLAG_TRANSPARENT_SORTING or R3D_FLAG_FXAA;
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, Flags);
  SetTargetFPS(60);

  // --- Setup background color and ambient light ---
  R3D_SetBackgroundColor(BLACK);
  R3D_SetAmbientColor(DARKGRAY);

  // --- Setup post processing parameters ---
  R3D_SetSSR(True);

  R3D_SetSSAO(True);
  R3D_SetSSAORadius(2.0);

  R3D_SetBloomIntensity(0.1);
  R3D_SetBloomMode(R3D_BLOOM_MIX);
  R3D_SetTonemapMode(R3D_TONEMAP_ACES);

  // --- Load the car model and apply scaling on import ---
  Model := R3D_LoadModel(RESOURCES_PATH + 'pbr/car.glb');

  // --- Generate ground mesh and setup its material ---
  Ground := R3D_GenMeshPlane(10.0, 10.0, 1, 1);

  GroundMat := R3D_GetDefaultMaterial();
  GroundMat.albedo.color := ColorCreate(31, 31, 31, 255);
  GroundMat.orm.roughness := 0.0;
  GroundMat.orm.metalness := 0.5;

  // --- Load skybox (disabled by default) ---
  Skybox := R3D_LoadSkybox(RESOURCES_PATH + 'sky/skybox3.png', CUBEMAP_LAYOUT_AUTO_DETECT);
  ShowSkybox := False;

  // --- Configure the scene lighting ---
  R3D_SetSceneBounds(BoundingBoxCreate(
    Vector3Create(-10, -10, -10),
    Vector3Create(10, 10, 10)
  ));

  Light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(Light, Vector3Create(-1, -1, -1));
  R3D_EnableShadow(Light, 4096);
  R3D_SetLightActive(Light, True);

  // --- Setup camera ---
  Camera.position := Vector3Create(0, 0, 5);
  Camera.target := Vector3Create(0, 0, 0);
  Camera.up := Vector3Create(0, 1, 0);
  Camera.fovy := 60;
  Camera.projection := CAMERA_PERSPECTIVE;

  // --- Capture the mouse and let's go! ---
  DisableCursor();
end;

procedure UpdateExample(Delta: Single);
begin
  UpdateCamera(@Camera, CAMERA_FREE);

  if IsKeyPressed(KEY_O) then
  begin
    R3D_SetSSAO(not R3D_GetSSAO());
  end;

  if IsKeyPressed(KEY_T) then
  begin
    ShowSkybox := not ShowSkybox;
    if ShowSkybox then
      R3D_EnableSkybox(Skybox)
    else
      R3D_DisableSkybox();
  end;
end;

procedure DrawExample;
begin
  R3D_Begin(Camera);
    R3D_DrawMesh(@Ground, @GroundMat, MatrixTranslate(0.0, -0.4, 0.0));
      R3D_DrawModel(@Model, Vector3Create(0, 0, 0), 1.0);
  R3D_End();



  DrawText('Model made by MaximePages', 10, GetScreenHeight() - 30, 20, RAYWHITE);
end;

procedure CloseExample;
begin
  R3D_UnloadModel(@Model, True);
  R3D_UnloadSkybox(Skybox);
  R3D_UnloadMesh(@Ground);
  R3D_UnloadMaterial(@GroundMat);
  R3D_Close();
end;

begin
  // Инициализация окна
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - PBR car example');

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
