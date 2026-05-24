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
  aaMode: TR3D_AntiAliasingMode;
  aaModeText: PChar;
  aaPreset: TR3D_AntiAliasingPreset;
  aaPresetText: PChar;

begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Sponza example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Post-processing setup
  R3D_ENVIRONMENT_SET('bloom.mode', R3D_BLOOM_MIX);
  R3D_ENVIRONMENT_SET('ssgi.intensity', 8.0);
  R3D_ENVIRONMENT_SET('ssao.enabled', True);

  // Background and ambient
  R3D_ENVIRONMENT_SET('background.color', SKYBLUE);
  R3D_ENVIRONMENT_SET('ambient.color', DARKGRAY);

  // Load Sponza model
  R3D_SetTextureFilter(TEXTURE_FILTER_ANISOTROPIC_8X);
  sponza := R3D_LoadModel(RESOURCES_PATH + 'models/hermit.glb');

  // Setup lights
  for i := 0 to 1 do
  begin
    lights[i] := R3D_CreateLight(R3D_LIGHT_OMNI);
    if i = 0 then
      R3D_SetLightPosition(lights[i], Vector3Create(10.0, 20.0, 0.0))
    else
      R3D_SetLightPosition(lights[i], Vector3Create(-10.0, 20.0, 0.0));

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
      //R3D_ENVIRONMENT_SET('ssao.enabled') := not R3D_ENVIRONMENT_GET('ssao.enabled'));
    end;

    // Toggle SSIL
    if IsKeyPressed(KEY_TWO) then
    begin
      //R3D_ENVIRONMENT_SET('ssil.enabled', not R3D_ENVIRONMENT_GET('ssil.enabled'));
    end;

    // Toggle SSGI
    if IsKeyPressed(KEY_THREE) then
    begin
      //R3D_ENVIRONMENT_SET('ssgi.enabled', not R3D_ENVIRONMENT_GET('ssgi.enabled'));
    end;

    // Toggle SSR
    if IsKeyPressed(KEY_FOUR) then
    begin
     // R3D_ENVIRONMENT_SET('ssr.enabled', not R3D_ENVIRONMENT_GET('ssr.enabled'));
    end;

    // Toggle fog
    if IsKeyPressed(KEY_FIVE) then
    begin
     // if R3D_ENVIRONMENT_GET('fog.mode', R3D_FOG_DISABLED) then
     //   R3D_ENVIRONMENT_SET('fog.mode', R3D_FOG_EXP)
     // else
     //   R3D_ENVIRONMENT_SET('fog.mode', R3D_FOG_DISABLED);
    end;

    // Switch anti aliasing mode
    if IsKeyPressed(KEY_F) then
    begin
     // aaMode := R3D_GetAntiAliasingMode;
     // R3D_SetAntiAliasingMode((aaMode + 1) mod 3);
    end;

    // Switch anti aliasing preset
    if IsKeyPressed(KEY_G) then
    begin
     // aaPreset := R3D_GetAntiAliasingPreset;
     // R3D_SetAntiAliasingPreset((aaPreset + 1) mod R3D_ANTI_ALIASING_PRESET_COUNT);
    end;

    // Cycle tonemapping (left mouse - previous, right mouse - next)
    if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
    begin
     // tonemap := R3D_ENVIRONMENT_GET('tonemap.mode');
      if tonemap = R3D_TONEMAP_LINEAR then
        R3D_ENVIRONMENT_SET('tonemap.mode', R3D_TONEMAP_AGX)
      else
        R3D_ENVIRONMENT_SET('tonemap.mode', Pred(tonemap));
    end;

    if IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) then
    begin
     // tonemap := R3D_ENVIRONMENT_GET('tonemap.mode');
     // if tonemap = R3D_TONEMAP_AGX then
     //   R3D_ENVIRONMENT_SET('tonemap.mode', R3D_TONEMAP_LINEAR)
     // else
     //   R3D_ENVIRONMENT_SET('tonemap.mode', Succ(tonemap));
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
      tonemap := R3D_GetEnvironment^.tonemap.mode;
      tonemapText := '';
      case tonemap of
        R3D_TONEMAP_LINEAR:    tonemapText := '< TONEMAP LINEAR >';
        R3D_TONEMAP_REINHARD:  tonemapText := '< TONEMAP REINHARD >';
        R3D_TONEMAP_FILMIC:    tonemapText := '< TONEMAP FILMIC >';
        R3D_TONEMAP_ACES:      tonemapText := '< TONEMAP ACES >';
        R3D_TONEMAP_AGX:       tonemapText := '< TONEMAP AGX >';
      end;
      DrawText(tonemapText, GetScreenWidth - MeasureText(tonemapText, 20) - 10, 10, 20, LIME);

      // Display anti aliasing mode
      aaMode := R3D_GetAntiAliasingMode;
      aaModeText := '';
      case aaMode of
        R3D_ANTI_ALIASING_MODE_NONE:  aaModeText := 'AA: NONE';
        R3D_ANTI_ALIASING_MODE_FXAA:  aaModeText := 'AA: FXAA';
        R3D_ANTI_ALIASING_MODE_SMAA:  aaModeText := 'AA: SMAA';
      end;
      DrawText(aaModeText, 10, GetScreenHeight - 30, 20, LIME);

      // Display anti aliasing preset
      aaPreset := R3D_GetAntiAliasingPreset;
      aaPresetText := '';
      case aaPreset of
        R3D_ANTI_ALIASING_PRESET_LOW:     aaPresetText := '- Low';
        R3D_ANTI_ALIASING_PRESET_MEDIUM:  aaPresetText := '- Medium';
        R3D_ANTI_ALIASING_PRESET_HIGH:    aaPresetText := '- High';
        R3D_ANTI_ALIASING_PRESET_ULTRA:   aaPresetText := '- Ultra';
      end;
      DrawText(aaPresetText, MeasureText(aaModeText, 20) + 20, GetScreenHeight - 30, 20, LIME);

      DrawFPS(10, 10);
    EndDrawing;
  end;

  // Cleanup
  R3D_UnloadModel(sponza, True);
  R3D_Close;

  CloseWindow;
end.
