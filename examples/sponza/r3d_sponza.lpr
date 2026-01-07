program SponzaExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  ScreenWidth, ScreenHeight: Integer;
  sponza: TR3D_Model;
  skybox: TR3D_Cubemap;
  ambient: TR3D_AmbientMap;
  lights: array[0..1] of TR3D_Light;
  camera: TCamera3D;
  skyEnabled: Boolean;
  tonemap: TR3D_Tonemap;
  tonemapText: string;
  emptyCubemap: TR3D_Cubemap;
  emptyAmbientMap: TR3D_AmbientMap;
  i: Integer;
  tonemapInt, tonemapCount: Integer;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Sponza example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Post-processing setup
  R3D_GetEnvironment^.bloom.mode := R3D_BLOOM_MIX;
  R3D_GetEnvironment^.ssao.enabled := True;

  // Background and ambient
  R3D_GetEnvironment^.background.color := SKYBLUE;
  R3D_GetEnvironment^.ambient.color := GRAY;

  // Load Sponza model
  R3D_SetTextureFilter(TEXTURE_FILTER_ANISOTROPIC_8X);
  sponza := R3D_LoadModel(PAnsiChar(RESOURCES_PATH + 'sponza.glb'));

  // Load skybox (disabled by default)
  skybox := R3D_LoadCubemap(PAnsiChar(RESOURCES_PATH + 'sky/skybox3.png'), R3D_CUBEMAP_LAYOUT_AUTO_DETECT);
  ambient := R3D_GenAmbientMap(skybox, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  skyEnabled := False;

  // Initialize empty structures
  FillChar(emptyCubemap, SizeOf(emptyCubemap), 0);
  FillChar(emptyAmbientMap, SizeOf(emptyAmbientMap), 0);

  // Setup lights
  for i := 0 to 1 do
  begin
    lights[i] := R3D_CreateLight(R3D_LIGHT_OMNI);
    if i = 0 then
      R3D_SetLightPosition(lights[i], Vector3Create(10.0, 20.0, 0.0))
    else
      R3D_SetLightPosition(lights[i], Vector3Create(-10.0, 20.0, 0.0));
    R3D_SetLightActive(lights[i], True);
    R3D_SetLightEnergy(lights[i], 4.0);
    R3D_SetShadowUpdateMode(lights[i], R3D_SHADOW_UPDATE_MANUAL);
    R3D_EnableShadow(lights[i], 4096);
  end;

  // Setup camera
  camera.position := Vector3Create(8.0, 1.0, 0.5);
  camera.target := Vector3Create(0.0, 2.0, -2.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Capture mouse
  DisableCursor();

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_FREE);

    // Toggle skybox
    if IsKeyPressed(KEY_ZERO) then
    begin
      if skyEnabled then
      begin
        R3D_GetEnvironment^.background.sky := emptyCubemap;
        R3D_GetEnvironment^.ambient.map := emptyAmbientMap;
      end
      else
      begin
        R3D_GetEnvironment^.background.sky := skybox;
        R3D_GetEnvironment^.ambient.map := ambient;
      end;
      skyEnabled := not skyEnabled;
    end;

    // Toggle SSAO
    if IsKeyPressed(KEY_ONE) then
    begin
      R3D_GetEnvironment^.ssao.enabled := not R3D_GetEnvironment^.ssao.enabled;
    end;

    // Toggle SSIL
    if IsKeyPressed(KEY_TWO) then
    begin
      R3D_GetEnvironment^.ssil.enabled := not R3D_GetEnvironment^.ssil.enabled;
    end;

    // Toggle SSR
    if IsKeyPressed(KEY_THREE) then
    begin
      R3D_GetEnvironment^.ssr.enabled := not R3D_GetEnvironment^.ssr.enabled;
    end;

    // Toggle fog
    if IsKeyPressed(KEY_FOUR) then
    begin
      if R3D_GetEnvironment^.fog.mode = R3D_FOG_DISABLED then
        R3D_GetEnvironment^.fog.mode := R3D_FOG_EXP
      else
        R3D_GetEnvironment^.fog.mode := R3D_FOG_DISABLED;
    end;

    // Toggle FXAA
    if IsKeyPressed(KEY_FIVE) then
    begin
      if R3D_HasState(R3D_FLAG_FXAA) then
        R3D_ClearState(R3D_FLAG_FXAA)
      else
        R3D_SetState(R3D_FLAG_FXAA);
    end;

    // Cycle tonemapping (left mouse button - previous)
    if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
    begin
      tonemap := R3D_GetEnvironment^.tonemap.mode;
      tonemapInt := Integer(tonemap);
      tonemapCount := Integer(R3D_TONEMAP_COUNT);
      tonemapInt := (tonemapInt + tonemapCount - 1) mod tonemapCount;
      R3D_GetEnvironment^.tonemap.mode := TR3D_Tonemap(tonemapInt);
    end;

    // Cycle tonemapping (right mouse button - next)
    if IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) then
    begin
      tonemap := R3D_GetEnvironment^.tonemap.mode;
      tonemapInt := Integer(tonemap);
      tonemapCount := Integer(R3D_TONEMAP_COUNT);
      tonemapInt := (tonemapInt + 1) mod tonemapCount;
      R3D_GetEnvironment^.tonemap.mode := TR3D_Tonemap(tonemapInt);
    end;

    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Draw Sponza model
      R3D_Begin(camera);
        R3D_DrawModel(sponza, Vector3Zero(), 1.0);
      R3D_End();

      // Draw lights
      BeginMode3D(camera);
        DrawSphere(R3D_GetLightPosition(lights[0]), 0.5, WHITE);
        DrawSphere(R3D_GetLightPosition(lights[1]), 0.5, WHITE);
      EndMode3D();

      // Display tonemapping
      tonemap := R3D_GetEnvironment^.tonemap.mode;
      case tonemap of
        R3D_TONEMAP_LINEAR:   tonemapText := '< TONEMAP LINEAR >';
        R3D_TONEMAP_REINHARD: tonemapText := '< TONEMAP REINHARD >';
        R3D_TONEMAP_FILMIC:   tonemapText := '< TONEMAP FILMIC >';
        R3D_TONEMAP_ACES:     tonemapText := '< TONEMAP ACES >';
        R3D_TONEMAP_AGX:      tonemapText := '< TONEMAP AGX >';
      else
        tonemapText := '';
      end;

      DrawText(PAnsiChar(tonemapText),
               GetScreenWidth() - MeasureText(PAnsiChar(tonemapText), 20) - 10,
               10, 20, LIME);

      DrawFPS(10, 10);
    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadModel(sponza, True);
  R3D_UnloadAmbientMap(ambient);
  R3D_UnloadCubemap(skybox);
  R3D_Close();

  CloseWindow();
end.
