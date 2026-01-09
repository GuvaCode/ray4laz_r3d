program PBRMusketExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  ScreenWidth, ScreenHeight: Integer;
  model: TR3D_Model;
  modelMatrix, scaleMatrix, transformMatrix, rotateMatrix: TMatrix;
  modelScale, pitch, yaw: Single;
  skybox: TR3D_Cubemap;
  ambient: TR3D_AmbientMap;
  light: TR3D_Light;
  camera: TCamera3D;
  mouseDelta: TVector2;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - PBR musket example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);
  R3D_SetAntiAliasing(R3D_ANTI_ALIASING_FXAA);



  // Tonemapping
  R3D_GetEnvironment^.tonemap.mode := R3D_TONEMAP_FILMIC;
  R3D_GetEnvironment^.tonemap.exposure := 0.75;

  // Set texture filter for mipmaps
  R3D_SetTextureFilter(TEXTURE_FILTER_ANISOTROPIC_4X);

  // Load model
  model := R3D_LoadModel(PAnsiChar(RESOURCES_PATH + 'pbr/musket.glb'));
  modelMatrix := MatrixIdentity();
  modelScale := 1.0;

  // Load skybox and ambient map
  skybox := R3D_LoadCubemap(PAnsiChar(RESOURCES_PATH + 'sky/skybox2.png'), R3D_CUBEMAP_LAYOUT_AUTO_DETECT);
  ambient := R3D_GenAmbientMap(skybox, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  R3D_GetEnvironment^.background.sky := skybox;
  R3D_GetEnvironment^.ambient.map := ambient;

  // Setup directional light
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(0, -1, -1));
  R3D_SetLightActive(light, True);

  // Setup camera
  camera.position := Vector3Create(0, 0, 0.5);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    // Update model scale with mouse wheel
    modelScale := Clamp(modelScale + GetMouseWheelMove() * 0.1, 0.25, 2.5);

    // Rotate model with left mouse button
    if IsMouseButtonDown(MOUSE_BUTTON_LEFT) then
    begin
      mouseDelta := GetMouseDelta();
      pitch := (mouseDelta.y * 0.005) / modelScale;
      yaw := (mouseDelta.x * 0.005) / modelScale;
      rotateMatrix := MatrixRotateXYZ(Vector3Create(pitch, yaw, 0.0));
      modelMatrix := MatrixMultiply(modelMatrix, rotateMatrix);
    end;

    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Draw model
      R3D_Begin(camera);
        scaleMatrix := MatrixScale(modelScale, modelScale, modelScale);
        transformMatrix := MatrixMultiply(modelMatrix, scaleMatrix);
        R3D_DrawModelPro(model, transformMatrix);
      R3D_End();

      DrawText('Model made by TommyLingL', 10, GetScreenHeight()-26, 16, LIME);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadModel(model, True);
  R3D_UnloadAmbientMap(ambient);
  R3D_UnloadCubemap(skybox);
  R3D_Close();

  CloseWindow();
end.
