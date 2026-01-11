program PBRExample;

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
  model: TR3D_Model;
  modelMatrix: TMatrix;
  modelScale: Single;
  camera: TCamera3D;
  pitch, yaw: Single;
  rotateMatrix: TMatrix;
  scaleMatrix: TMatrix;
  transformMatrix: TMatrix;

begin
  // Initialize window
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - PBR example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);
  R3D_SetAntiAliasing(R3D_ANTI_ALIASING_FXAA);

  // Setup environment sky
  cubemap := R3D_LoadCubemap(RESOURCES_PATH + 'panorama/indoor.hdr', R3D_CUBEMAP_LAYOUT_AUTO_DETECT);

  // Use R3D_GetEnvironment()^ instead of R3D_ENVIRONMENT_SET
  R3D_GetEnvironment()^.background.skyBlur := 0.775;
  R3D_GetEnvironment()^.background.sky := cubemap;

  // Setup environment ambient
  ambientMap := R3D_GenAmbientMap(cubemap, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  R3D_GetEnvironment()^.ambient.map := ambientMap;

  // Setup bloom
  R3D_GetEnvironment()^.bloom.mode := R3D_BLOOM_MIX;
  R3D_GetEnvironment()^.bloom.intensity := 0.02;

  // Setup tonemapping
  R3D_GetEnvironment()^.tonemap.mode := R3D_TONEMAP_FILMIC;
  R3D_GetEnvironment()^.tonemap.exposure := 0.5;
  R3D_GetEnvironment()^.tonemap.white := 4.0;

  // Load model
  R3D_SetTextureFilter(TEXTURE_FILTER_ANISOTROPIC_4X);
  model := R3D_LoadModel(RESOURCES_PATH + 'models/DamagedHelmet.glb');
  modelMatrix := MatrixIdentity();
  modelScale := 1.0;

  // Setup camera
  camera.position := Vector3Create(0.0, 0.0, 2.5);
  camera.target := Vector3Create(0.0, 0.0, 0.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    // Update model scale with mouse wheel
    modelScale := Clamp(modelScale + GetMouseWheelMove() * 0.1, 0.25, 2.5);

    // Rotate model with left mouse button
    if IsMouseButtonDown(MOUSE_BUTTON_LEFT) then
    begin
      pitch := GetMouseDelta().y * 0.005 / modelScale;
      yaw := GetMouseDelta().x * 0.005 / modelScale;
      rotateMatrix := MatrixRotateXYZ(Vector3Create(pitch, yaw, 0.0));
      modelMatrix := MatrixMultiply(modelMatrix, rotateMatrix);
    end;

    BeginDrawing();
      ClearBackground(RAYWHITE);
      R3D_Begin(camera);
        scaleMatrix := MatrixScale(modelScale, modelScale, modelScale);
        transformMatrix := MatrixMultiply(modelMatrix, scaleMatrix);
        R3D_DrawModelPro(model, transformMatrix);
      R3D_End();
    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadModel(model, True);
  R3D_UnloadAmbientMap(ambientMap);
  R3D_UnloadCubemap(cubemap);
  R3D_Close();

  CloseWindow();
end.
