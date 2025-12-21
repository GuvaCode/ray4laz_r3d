program EmissionExample;

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
  Model: TR3D_Model;
  Plane: TR3D_Mesh;
  Material: TR3D_Material;
  Camera: TCamera3D;
  Light: TR3D_Light;
  RotModel: Single = 0.0;

// === Toggle Light ===

procedure ToggleLight;
begin
  if R3D_IsLightActive(Light) then
  begin
    R3D_SetLightActive(Light, False);
    R3D_SetAmbientColor(BLACK);
  end
  else
  begin
    R3D_SetLightActive(Light, True);
    R3D_SetAmbientColor(DARKGRAY);
  end;
end;

// === Example ===

procedure InitExample;
begin
  // --- Initialize R3D with its internal resolution ---
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SetTargetFPS(60);

  // --- Configure the background color and ambient lighting ---
  R3D_SetBackgroundColor(BLACK);
  R3D_SetAmbientColor(DARKGRAY);

  // --- Configure the post process parameters ---
  R3D_SetTonemapMode(R3D_TONEMAP_ACES);
  R3D_SetTonemapExposure(0.8);
  R3D_SetTonemapWhite(2.5);

  R3D_SetBloomMode(R3D_BLOOM_ADDITIVE);
  R3D_SetBloomSoftThreshold(0.2);
  R3D_SetBloomIntensity(0.2);
  R3D_SetBloomThreshold(0.6);

  // --- Loads the main model of the scene ---
  Model := R3D_LoadModel(RESOURCES_PATH + 'emission.glb');

  // --- Generates a mesh for the ground and load a material for it ---
  Plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  Material := R3D_GetDefaultMaterial();

  // --- Setup the scene lighting ---
  Light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(Light, Vector3Create(0, 10, 5), Vector3Create(0, 0, 0));
  R3D_SetLightOuterCutOff(Light, 45.0);
  R3D_SetLightInnerCutOff(Light, 22.5);
  R3D_EnableShadow(Light, 4096);
  R3D_SetLightActive(Light, True);

  // --- Setup the camera ---
  Camera.position := Vector3Create(-1.0, 1.75, 1.75);
  Camera.target := Vector3Create(0, 0.5, 0);
  Camera.up := Vector3Create(0, 1, 0);
  Camera.fovy := 60;
  Camera.projection := CAMERA_PERSPECTIVE;
end;

procedure UpdateExample(Delta: Single);
var
  MouseDelta: TVector2;
begin
  if IsKeyPressed(KEY_SPACE) then
  begin
    ToggleLight();
  end;

  if IsMouseButtonDown(MOUSE_BUTTON_LEFT) then
  begin
    MouseDelta := GetMouseDelta();
    Camera.position.y := Clamp(Camera.position.y + 0.01 * MouseDelta.y, 0.25, 2.5);
    RotModel := RotModel + 0.01 * MouseDelta.x;
  end;
end;

procedure DrawExample;
begin
  R3D_Begin(Camera);
    R3D_DrawMesh(@Plane, @Material, MatrixIdentity());
    R3D_DrawModelEx(@Model, Vector3Create(0, 0, 0),
      Vector3Create(0, 1, 0), RotModel, Vector3Create(1.0, 1.0, 1.0));
  R3D_End();

  DrawText('Press SPACE to toggle the light', 10, 10, 20, LIME);
  DrawText('Model by har15204405', 10, GetScreenHeight() - 30, 20, RAYWHITE);
end;

procedure CloseExample;
begin
  R3D_UnloadModel(@Model, True);
  R3D_UnloadMesh(@Plane);
  R3D_UnloadMaterial(@Material);
  R3D_Close();
end;

begin
  // Инициализация окна
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Emission example');

  // Инициализация примера
  InitExample;

  // Главный цикл
  while not WindowShouldClose() do
  begin
    UpdateExample(GetFrameTime());

    BeginDrawing();
      ClearBackground(BLACK);
      DrawExample;
    EndDrawing();
  end;

  // Очистка ресурсов
  CloseExample;
  CloseWindow();
end.
