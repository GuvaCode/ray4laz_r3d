program PBRCarExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  ScreenWidth, ScreenHeight: Integer;
  flags: TR3D_Flags;
  model: TR3D_Model;
  ground: TR3D_Mesh;
  groundMat: TR3D_Material;
  skybox: TR3D_Cubemap;
  ambient: TR3D_AmbientMap;
  light: TR3D_Light;
  camera: TCamera3D;
  showSkybox: Boolean;
  emptyCubemap: TR3D_Cubemap;
  emptyAmbientMap: TR3D_AmbientMap;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - PBR car example');
  SetTargetFPS(60);

  // Initialize R3D with flags
  flags := R3D_FLAG_TRANSPARENT_SORTING or R3D_FLAG_FXAA;
  R3D_Init(GetScreenWidth(), GetScreenHeight(), flags);

  // Set environment
  R3D_GetEnvironment^.background.color := DARKGRAY;
  R3D_GetEnvironment^.ambient.color := DARKGRAY;

  // Post-processing
  R3D_GetEnvironment^.ssr.enabled := True;
  R3D_GetEnvironment^.ssao.enabled := True;
  R3D_GetEnvironment^.ssao.radius := 1.0;
  R3D_GetEnvironment^.bloom.levels := 0.5;
  R3D_GetEnvironment^.bloom.intensity := 0.025;
  R3D_GetEnvironment^.bloom.mode := R3D_BLOOM_MIX;
  R3D_GetEnvironment^.tonemap.mode := R3D_TONEMAP_FILMIC;

  // Load model
  model := R3D_LoadModel(PAnsiChar(RESOURCES_PATH + 'pbr/car.glb'));

  // Ground mesh
  ground := R3D_GenMeshPlane(10.0, 10.0, 1, 1);
  groundMat := R3D_GetDefaultMaterial();
  groundMat.albedo.color := DARKGRAY;
  groundMat.orm.roughness := 0.0;
  groundMat.orm.metalness := 0.5;

  // Load skybox and ambient map (disabled by default)
  skybox := R3D_LoadCubemap(PAnsiChar(RESOURCES_PATH + 'sky/skybox3.png'), R3D_CUBEMAP_LAYOUT_AUTO_DETECT);
  ambient := R3D_GenAmbientMap(skybox, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  showSkybox := False;

  // Initialize empty structures
  FillChar(emptyCubemap, SizeOf(emptyCubemap), 0);
  FillChar(emptyAmbientMap, SizeOf(emptyAmbientMap), 0);

  // Setup directional light
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(-1, -1, -1));
  R3D_SetShadowDepthBias(light, 0.003);
  R3D_EnableShadow(light, 4096);
  R3D_SetLightActive(light, True);
  R3D_SetLightEnergy(light, 2.0);
  R3D_SetLightRange(light, 10);

  // Setup camera
  camera.position := Vector3Create(0, 0, 5);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Capture mouse
  DisableCursor();

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_FREE);

    // Toggle SSAO
    if IsKeyPressed(KEY_O) then
    begin
      R3D_GetEnvironment^.ssao.enabled := not R3D_GetEnvironment^.ssao.enabled;
    end;

    // Toggle skybox
    if IsKeyPressed(KEY_T) then
    begin
      showSkybox := not showSkybox;
      if showSkybox then
      begin
        R3D_GetEnvironment^.background.sky := skybox;
        R3D_GetEnvironment^.ambient.map := ambient;
      end
      else
      begin
        R3D_GetEnvironment^.background.sky := emptyCubemap;
        R3D_GetEnvironment^.ambient.map := emptyAmbientMap;
      end;
    end;

    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Draw scene
      R3D_Begin(camera);
        R3D_DrawMesh(ground, groundMat, Vector3Zero(), 1.0);
        R3D_DrawModel(model, Vector3Zero(), 1.0);
      R3D_End();

      DrawText('Model made by MaximePages', 10, GetScreenHeight()-26, 16, LIME);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadModel(model, True);
  R3D_UnloadAmbientMap(ambient);
  R3D_UnloadCubemap(skybox);
  R3D_Close();

  CloseWindow();
end.
