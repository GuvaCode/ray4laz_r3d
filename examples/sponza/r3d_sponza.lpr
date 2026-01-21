program SponzaExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  ScreenWidth, ScreenHeight: Integer;
  sponza: TR3D_Model;
  lights: array[0..1] of TR3D_Light;
  camera: TCamera3D;
  i: Integer;
  tonemap: TR3D_Tonemap;
  tonemapText: string;
  currentAA: TR3D_AntiAliasing;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Sponza example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Post-processing setup
  R3D_GetEnvironment()^.bloom.mode := R3D_BLOOM_MIX;
  R3D_GetEnvironment()^.ssao.enabled := True;

  // Background and ambient
  R3D_GetEnvironment()^.background.color := SKYBLUE;
  R3D_GetEnvironment()^.ambient.color := GRAY;

  // Load Sponza model
  R3D_SetTextureFilter(TEXTURE_FILTER_ANISOTROPIC_8X);
  sponza := R3D_LoadModel(PAnsiChar(RESOURCES_PATH + 'models/Sponza.glb'));

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
    R3D_EnableShadow(lights[i]);
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

    // Toggle SSAO
    if IsKeyPressed(KEY_ONE) then
    begin
      R3D_GetEnvironment()^.ssao.enabled := not R3D_GetEnvironment()^.ssao.enabled;
    end;

    // Toggle SSIL
    if IsKeyPressed(KEY_TWO) then
    begin
      R3D_GetEnvironment()^.ssil.enabled := not R3D_GetEnvironment()^.ssil.enabled;
    end;

    // Toggle SSR
    if IsKeyPressed(KEY_THREE) then
    begin
      R3D_GetEnvironment()^.ssr.enabled := not R3D_GetEnvironment()^.ssr.enabled;
    end;

    // Toggle fog
    if IsKeyPressed(KEY_FOUR) then
    begin
      if R3D_GetEnvironment()^.fog.mode = R3D_FOG_DISABLED then
        R3D_GetEnvironment()^.fog.mode := R3D_FOG_EXP
      else
        R3D_GetEnvironment()^.fog.mode := R3D_FOG_DISABLED;
    end;

    // Toggle FXAA
    if IsKeyPressed(KEY_FIVE) then
    begin
      currentAA := R3D_GetAntiAliasing();
      if currentAA = R3D_ANTI_ALIASING_DISABLED then
        R3D_SetAntiAliasing(R3D_ANTI_ALIASING_FXAA)
      else
        R3D_SetAntiAliasing(R3D_ANTI_ALIASING_DISABLED);
    end;

    // Cycle tonemapping (left mouse button - previous)
    if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
    begin
      tonemap := R3D_GetEnvironment()^.tonemap.mode;
      R3D_GetEnvironment()^.tonemap.mode :=
        TR3D_Tonemap((Integer(tonemap) + Integer(R3D_TONEMAP_COUNT) - 1) mod Integer(R3D_TONEMAP_COUNT));
    end;

    // Cycle tonemapping (right mouse button - next)
    if IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) then
    begin
      tonemap := R3D_GetEnvironment()^.tonemap.mode;
      R3D_GetEnvironment()^.tonemap.mode :=
        TR3D_Tonemap((Integer(tonemap) + 1) mod Integer(R3D_TONEMAP_COUNT));
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
      tonemap := R3D_GetEnvironment()^.tonemap.mode;
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
  R3D_Close();

  CloseWindow();
end.
