program sponza_example;

{$mode objfpc}{$H+}

uses
  raylib, raymath, r3d, math;

const
  RESOURCES_PATH = 'resources/';

var
  camera: TCamera3D;
  sponza: TR3D_Model;
  lights: array[0..1] of TR3D_Light;
  i: Integer;
  tonemap: TR3D_Tonemap;
  tonemapText: PChar;
  env: PR3D_Environment;

begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Sponza example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Get environment pointer for direct access
  env := R3D_GetEnvironment();

  // Post-processing setup
  R3D_ENVIRONMENT_SET('bloom.mode', R3D_BLOOM_MIX);
  R3D_ENVIRONMENT_SET('ssao.enabled', True);

  // Background and ambient
  R3D_ENVIRONMENT_SET('background.color', SKYBLUE);
  R3D_ENVIRONMENT_SET('ambient.color', DARKGRAY);

  // Load Sponza model
  R3D_SetTextureFilter(TEXTURE_FILTER_ANISOTROPIC_8X);
  sponza := R3D_LoadModel(RESOURCES_PATH + 'models/Sponza.glb');

  // Setup lights
  for i := 0 to 1 do
  begin
    lights[i] := R3D_CreateLight(R3D_LIGHT_OMNI);
    if i = 0 then
      R3D_SetLightPosition(lights[i], Vector3Create(-10.0, 20.0, 0.0))
    else
      R3D_SetLightPosition(lights[i], Vector3Create(10.0, 20.0, 0.0));

    R3D_SetLightActive(lights[i], True);
    R3D_SetLightEnergy(lights[i], 8.0);
    R3D_SetShadowUpdateMode(lights[i], R3D_SHADOW_UPDATE_MANUAL);
    R3D_EnableShadow(lights[i]);
  end;

  // Setup camera
  camera := Default(TCamera3D);
  camera.position := Vector3Create(8.0, 1.0, 0.5);
  camera.target := Vector3Create(0.0, 2.0, -2.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Capture mouse
  DisableCursor;

  // Main loop
  while not WindowShouldClose do
  begin
    UpdateCamera(@camera, CAMERA_FREE);

    // Toggle SSAO
    if IsKeyPressed(KEY_ONE) then
    begin
      env^.ssao.enabled := not env^.ssao.enabled;
    end;

    // Toggle SSIL
    if IsKeyPressed(KEY_TWO) then
    begin
      env^.ssil.enabled := not env^.ssil.enabled;
    end;

    // Toggle SSGI
    if IsKeyPressed(KEY_THREE) then
    begin
      env^.ssgi.enabled := not env^.ssgi.enabled;
    end;

    // Toggle SSR
    if IsKeyPressed(KEY_FOUR) then
    begin
      env^.ssr.enabled := not env^.ssr.enabled;
    end;

    // Toggle fog
    if IsKeyPressed(KEY_FIVE) then
    begin
      if env^.fog.mode = R3D_FOG_DISABLED then
        env^.fog.mode := R3D_FOG_EXP
      else
        env^.fog.mode := R3D_FOG_DISABLED;
    end;

    // Toggle FXAA
    if IsKeyPressed(KEY_SIX) then
    begin
      if R3D_GetAntiAliasing = R3D_ANTI_ALIASING_DISABLED then
        R3D_SetAntiAliasing(R3D_ANTI_ALIASING_FXAA)
      else
        R3D_SetAntiAliasing(R3D_ANTI_ALIASING_DISABLED);
    end;

    // Cycle tonemapping
    if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
    begin
      tonemap := env^.tonemap.mode;
      if tonemap = R3D_TONEMAP_LINEAR then
        env^.tonemap.mode := R3D_TONEMAP_AGX
      else
        env^.tonemap.mode := Pred(tonemap);
    end;

    if IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) then
    begin
      tonemap := env^.tonemap.mode;
      if tonemap = R3D_TONEMAP_AGX then
        env^.tonemap.mode := R3D_TONEMAP_LINEAR
      else
        env^.tonemap.mode := Succ(tonemap);
    end;

    BeginDrawing;
      ClearBackground(RAYWHITE);

      // Draw Sponza model
      R3D_Begin(camera);
        R3D_DrawModel(sponza, Vector3Zero, 1.0);
      R3D_End;

      // Draw lights
      BeginMode3D(camera);
        DrawSphere(R3D_GetLightPosition(lights[0]), 0.5, WHITE);
        DrawSphere(R3D_GetLightPosition(lights[1]), 0.5, WHITE);
      EndMode3D;

      // Display tonemapping
      tonemap := env^.tonemap.mode;
      tonemapText := '';
      case tonemap of
        R3D_TONEMAP_LINEAR:    tonemapText := '< TONEMAP LINEAR >';
        R3D_TONEMAP_REINHARD:  tonemapText := '< TONEMAP REINHARD >';
        R3D_TONEMAP_FILMIC:    tonemapText := '< TONEMAP FILMIC >';
        R3D_TONEMAP_ACES:      tonemapText := '< TONEMAP ACES >';
        R3D_TONEMAP_AGX:       tonemapText := '< TONEMAP AGX >';
      end;

      DrawText(tonemapText, GetScreenWidth - MeasureText(tonemapText, 20) - 10, 10, 20, LIME);
      DrawFPS(10, 10);
    EndDrawing;
  end;

  // Cleanup
  R3D_UnloadModel(sponza, True);
  R3D_Close;

  CloseWindow;
end.
