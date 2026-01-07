program EmissionExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';



var
  ScreenWidth, ScreenHeight: Integer;
  skybox: TR3D_Cubemap;
  ambient: TR3D_AmbientMap;
  model: TR3D_Model;
  plane: TR3D_Mesh;
  material: TR3D_Material;
  light: TR3D_Light;
  camera: TCamera3D;
  rotModel, delta: Single;
  mouseDelta: TVector2;
  blackColor: TColor;



  procedure ToggleLight(light: TR3D_Light; skybox: TR3D_Cubemap; ambient: TR3D_AmbientMap);
  var
    emptyCubemap: TR3D_Cubemap;
    emptyAmbientMap: TR3D_AmbientMap;
  begin
    if R3D_IsLightActive(light) then
    begin
      R3D_SetLightActive(light, False);

      // Initialize empty structs
      FillChar(emptyCubemap, SizeOf(emptyCubemap), 0);
      FillChar(emptyAmbientMap, SizeOf(emptyAmbientMap), 0);

      R3D_GetEnvironment^.background.sky := emptyCubemap;
      R3D_GetEnvironment^.ambient.map := emptyAmbientMap;

      blackColor := ColorCreate(0, 0, 0, 255);
      R3D_GetEnvironment^.background.color := blackColor;
      R3D_GetEnvironment^.ambient.color := blackColor;
    end
    else
    begin
      R3D_SetLightActive(light, True);
      R3D_GetEnvironment^.background.sky := skybox;
      R3D_GetEnvironment^.ambient.map := ambient;
    end;
  end;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Emission example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Configure post-processing (Tonemap + Bloom)
  R3D_GetEnvironment^.tonemap.mode := R3D_TONEMAP_AGX;
  R3D_GetEnvironment^.bloom.mode := R3D_BLOOM_ADDITIVE;
  R3D_GetEnvironment^.bloom.softThreshold := 0.2;
  R3D_GetEnvironment^.bloom.threshold := 0.6;
  R3D_GetEnvironment^.bloom.intensity := 0.2;
  R3D_GetEnvironment^.bloom.levels := 0.5;

  R3D_GetEnvironment^.ssil.enabled := True;
  R3D_GetEnvironment^.ssil.energy := 4.0;

  // Load skybox and ambient map
  skybox := R3D_LoadCubemap(PAnsiChar(RESOURCES_PATH + 'sky/skybox3.png'), R3D_CUBEMAP_LAYOUT_AUTO_DETECT);
  ambient := R3D_GenAmbientMap(skybox, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  R3D_GetEnvironment^.background.sky := skybox;
  R3D_GetEnvironment^.ambient.map := ambient;

  // Load model
  model := R3D_LoadModel(PAnsiChar(RESOURCES_PATH + 'emission.glb'));

  // Create ground plane
  plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  material := R3D_GetDefaultMaterial();

  // Setup spotlight
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(light, Vector3Create(0, 10, 5), Vector3Zero());
  R3D_SetLightOuterCutOff(light, 45.0);
  R3D_SetLightInnerCutOff(light, 22.5);
  R3D_EnableShadow(light, 4096);
  R3D_SetLightActive(light, True);

  // Setup camera
  camera.position := Vector3Create(-1.0, 1.75, 1.75);
  camera.target := Vector3Create(0, 0.5, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  rotModel := 0.0;

  // Main loop
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();

    // Input
    if IsKeyPressed(KEY_SPACE) then
    begin
      ToggleLight(light, skybox, ambient);
    end;

    if IsMouseButtonDown(MOUSE_BUTTON_LEFT) then
    begin
      mouseDelta := GetMouseDelta();
      camera.position.y := Clamp(camera.position.y + 0.01 * mouseDelta.y, 0.25, 2.5);
      rotModel := rotModel + 0.01 * mouseDelta.x;
    end;

    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Render scene
      R3D_Begin(camera);
        R3D_DrawMesh(plane, material, Vector3Zero(), 1.0);
        R3D_DrawModelEx(model, Vector3Zero(), QuaternionFromEuler(0.0, rotModel, 0.0), Vector3One());
      R3D_End();

      // UI
      DrawText('Press SPACE to toggle the light', 10, 10, 20, LIME);
      DrawText('Model by har15204405', 10, GetScreenHeight() - 26, 16, LIME);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadModel(model, True);
  R3D_UnloadAmbientMap(ambient);
  R3D_UnloadCubemap(skybox);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.

