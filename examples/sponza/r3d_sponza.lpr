program SponzaExample;

{$mode objfpc}{$H+}

uses
  Math, SysUtils,
  raylib, r3d; // Предполагается, что модуль R3D доступен

const
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 600;

var
  // === Resources ===
  Sponza: TR3D_Model;
  Skybox: TR3D_Skybox;
  Camera: TCamera3D;
  Lights: array[0..1] of TR3D_Light;
  SkyEnabled: Boolean = False;

// === Examples ===

procedure InitExample;
var
  I: Integer;
begin
  // --- Initialize R3D with its internal resolution ---
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SetTargetFPS(60);

  // --- Configure default post process settings ---
  R3D_SetSSAO(True);
  R3D_SetSSAORadius(4.0);
  R3D_SetSSAOIntensity(1.25);
  R3D_SetSSAOPower(1.5);

  R3D_SetBloomMode(R3D_BLOOM_MIX);

  // --- Set default background and ambient color (when no skybox is activated) ---
  R3D_SetBackgroundColor(SKYBLUE);
  R3D_SetAmbientColor(DARKGRAY);

  // --- Load Sponza scene ---
  Sponza := R3D_LoadModel(RESOURCES_PATH + 'sponza.glb');

  // --- Load skybox (disabled by default) ---
  Skybox := R3D_LoadSkybox(RESOURCES_PATH + 'sky/skybox3.png', CUBEMAP_LAYOUT_AUTO_DETECT);

  // --- Set scene bounds, useful if you use directional lights ---
  R3D_SetSceneBounds(Sponza.aabb);

  // --- Configure lights ---
  for I := 0 to 1 do
  begin
    Lights[I] := R3D_CreateLight(R3D_LIGHT_OMNI);

    if I = 0 then
      R3D_SetLightPosition(Lights[I], Vector3Create(-10.0, 20.0, 0.0))
    else
      R3D_SetLightPosition(Lights[I], Vector3Create(10.0, 20.0, 0.0));

    R3D_SetLightActive(Lights[I], True);
    R3D_SetLightEnergy(Lights[I], 4.0);

    R3D_SetShadowUpdateMode(Lights[I], R3D_SHADOW_UPDATE_MANUAL);
    R3D_EnableShadow(Lights[I], 4096);
  end;

  // --- Configure camera ---
  Camera.position := Vector3Create(8.0, 1.0, 0.5);
  Camera.target := Vector3Create(0.0, 2.0, -2.0);
  Camera.up := Vector3Create(0.0, 1.0, 0.0);
  Camera.fovy := 60.0;
  Camera.projection := CAMERA_PERSPECTIVE;

  // --- Ready to go! ---
  DisableCursor();
end;

procedure UpdateExample(Delta: Single);
var
  CurrentTonemap: TR3D_Tonemap;
begin
  // --- Update the camera via raylib's functions ---
  UpdateCamera(@Camera, CAMERA_FREE);

  // --- Skybox toggling ---
  if IsKeyPressed(KEY_ZERO) then
  begin
    if SkyEnabled then
      R3D_DisableSkybox()
    else
      R3D_EnableSkybox(Skybox);

    SkyEnabled := not SkyEnabled;
  end;

  // --- SSAO toggling ---
  if IsKeyPressed(KEY_ONE) then
  begin
    R3D_SetSSAO(not R3D_GetSSAO());
  end;

  // --- Fog toggling ---
  if IsKeyPressed(KEY_TWO) then
  begin
    if R3D_GetFogMode() = R3D_FOG_DISABLED then
      R3D_SetFogMode(R3D_FOG_EXP)
    else
      R3D_SetFogMode(R3D_FOG_DISABLED);
  end;

  // --- FXAA toggling ---
  if IsKeyPressed(KEY_THREE) then
  begin
    if R3D_HasState(R3D_FLAG_FXAA) then
      R3D_ClearState(R3D_FLAG_FXAA)
    else
      R3D_SetState(R3D_FLAG_FXAA);
  end;

  // --- Tonemapping setter ---
  if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
  begin
    CurrentTonemap := R3D_GetTonemapMode();
    R3D_SetTonemapMode(TR3D_Tonemap((Ord(CurrentTonemap) + Ord(R3D_TONEMAP_COUNT) - 1) mod Ord(R3D_TONEMAP_COUNT)));
  end;

  if IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) then
  begin
    CurrentTonemap := R3D_GetTonemapMode();
    R3D_SetTonemapMode(TR3D_Tonemap((Ord(CurrentTonemap) + 1) mod Ord(R3D_TONEMAP_COUNT)));
  end;
end;

procedure DrawExample;
var
  Tonemap: TR3D_Tonemap;
  TextStr: string;
  TextWidth: Integer;
begin
  // --- Render R3D scene ---
  R3D_Begin(Camera);
    R3D_DrawModel(@Sponza, Vector3Create(0, 0, 0), 1.0);
  R3D_End();

  // --- 'Standard' raylib rendering to show where are the lights ---
  BeginMode3D(Camera);
    DrawSphere(R3D_GetLightPosition(Lights[0]), 0.5, WHITE);
    DrawSphere(R3D_GetLightPosition(Lights[1]), 0.5, WHITE);
  EndMode3D();

  // --- Indicates which tonemapping is used ---
  Tonemap := R3D_GetTonemapMode();

  case Tonemap of
    R3D_TONEMAP_LINEAR:
      TextStr := '< TONEMAP LINEAR >';
    R3D_TONEMAP_REINHARD:
      TextStr := '< TONEMAP REINHARD >';
    R3D_TONEMAP_FILMIC:
      TextStr := '< TONEMAP FILMIC >';
    R3D_TONEMAP_ACES:
      TextStr := '< TONEMAP ACES >';
    R3D_TONEMAP_AGX:
      TextStr := '< TONEMAP AGX >';
  else
    TextStr := '';
  end;

  if TextStr <> '' then
  begin
    TextWidth := MeasureText(PAnsiChar(TextStr), 20);
    DrawText(PAnsiChar(TextStr), GetScreenWidth() - TextWidth - 10, 10, 20, LIME);
  end;

  // --- I think we understand what's going on here ---
  DrawFPS(10, 10);
end;

procedure CloseExample;
begin
  R3D_UnloadModel(@Sponza, True);
  R3D_UnloadSkybox(Skybox);
  R3D_Close();
end;

begin
  // Инициализация окна
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Sponza example');

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
