program SponzaExample;

uses
  SysUtils, Math,
  raylib, r3d;

const
  RESOURCES_PATH = 'resources/';

var
  sponza: TR3D_Model;
  skybox: TR3D_Skybox;
  camera: TCamera3D;
  lights: array[0..1] of TR3D_Light;
  sky: Boolean = false;

function Init: PChar;
var
  i: Integer;
begin
  { --- Initialize R3D with its internal resolution --- }

  R3D_Init(GetScreenWidth, GetScreenHeight, 0);
  SetTargetFPS(60);

  { --- Configure default post process settings --- }

  R3D_GetEnvironment^.ssao.enabled := true;
  R3D_GetEnvironment^.ssao.radius := 4.0;
  R3D_GetEnvironment^.ssao.intensity := 1.25;
  R3D_GetEnvironment^.ssao.power := 1.5;

  R3D_GetEnvironment^.bloom.mode := R3D_BLOOM_MIX;

  { --- Set default background and ambient color (when no skybox is activated) --- }

  R3D_GetEnvironment^.background.color := SKYBLUE;
  R3D_GetEnvironment^.ambient.color := DARKGRAY;

  { --- Load Sponza scene --- }

  sponza := R3D_LoadModel(PChar(RESOURCES_PATH + 'sponza.glb'));

  { --- Load skybox (disabled by default) --- }

  skybox := R3D_LoadSkybox(
    PChar(RESOURCES_PATH + 'sky/skybox3.png'),
    CUBEMAP_LAYOUT_AUTO_DETECT
  );

  { --- Configure lights --- }

  for i := 0 to 1 do
  begin
    lights[i] := R3D_CreateLight(R3D_LIGHT_OMNI);

    if i = 1 then
      R3D_SetLightPosition(lights[i], Vector3Create(-10.0, 20.0, 0.0))
    else
      R3D_SetLightPosition(lights[i], Vector3Create(10.0, 20.0, 0.0));

    R3D_SetLightActive(lights[i], true);
    R3D_SetLightEnergy(lights[i], 4.0);

    R3D_SetShadowUpdateMode(lights[i], R3D_SHADOW_UPDATE_MANUAL);
    R3D_EnableShadow(lights[i], 4096);
  end;

  { --- Configure camera --- }

  camera := Default(TCamera3D);
  camera.position := Vector3Create(8.0, 1.0, 0.5);
  camera.target := Vector3Create(0.0, 2.0, -2.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  { --- Ready to go! --- }

  DisableCursor();

  Result := '[r3d] - Sponza example';
end;

procedure Update(delta: Single);
var
  env: PR3D_Environment;
  tonemap: TR3D_Tonemap;
  fxaa: Boolean;
begin
  env := R3D_GetEnvironment;

  { --- Update the camera via raylib's functions --- }

  UpdateCamera(@camera, CAMERA_FREE);

  { --- Skybox toggling --- }

  if IsKeyPressed(KEY_ZERO) then
  begin
    if sky then
      env^.background.sky := Default(TR3D_Skybox)
    else
      env^.background.sky := skybox;
    sky := not sky;
  end;

  { --- SSAO toggling --- }

  if IsKeyPressed(KEY_ONE) then
  begin
    env^.ssao.enabled := not env^.ssao.enabled;
  end;

  { --- Fog toggling --- }

  if IsKeyPressed(KEY_TWO) then
  begin
    if env^.fog.mode = R3D_FOG_DISABLED then
      env^.fog.mode := R3D_FOG_EXP
    else
      env^.fog.mode := R3D_FOG_DISABLED;
  end;

  { --- FXAA toggling --- }

  if IsKeyPressed(KEY_THREE) then
  begin
    fxaa := R3D_HasState(R3D_FLAG_FXAA);
    if fxaa then
      R3D_ClearState(R3D_FLAG_FXAA)
    else
      R3D_SetState(R3D_FLAG_FXAA);
  end;

  { --- Tonemapping setter --- }

  if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
  begin
    tonemap := env^.tonemap.mode;
    env^.tonemap.mode := TR3D_Tonemap((Integer(tonemap) + Integer(R3D_TONEMAP_COUNT) - 1) mod Integer(R3D_TONEMAP_COUNT));
  end;

  if IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) then
  begin
    tonemap := env^.tonemap.mode;
    env^.tonemap.mode := TR3D_Tonemap((Integer(tonemap) + 1) mod Integer(R3D_TONEMAP_COUNT));
  end;
end;

procedure Draw;
var
  tonemap: TR3D_Tonemap;
  txt: string;
  txtWidth: Integer;
begin
  { --- Render R3D scene --- }

  R3D_Begin(camera);
    R3D_DrawModel(@sponza, Vector3Create(0, 0, 0), 1.0);
  R3D_End();

  { --- 'Standard' raylib rendering to show where are the lights --- }

  BeginMode3D(camera);
    DrawSphere(R3D_GetLightPosition(lights[0]), 0.5, WHITE);
    DrawSphere(R3D_GetLightPosition(lights[1]), 0.5, WHITE);
  EndMode3D();

  { --- Indicates which tonemapping is used --- }

  tonemap := R3D_GetEnvironment^.tonemap.mode;

  case tonemap of
    R3D_TONEMAP_LINEAR:
      txt := '< TONEMAP LINEAR >';
    R3D_TONEMAP_REINHARD:
      txt := '< TONEMAP REINHARD >';
    R3D_TONEMAP_FILMIC:
      txt := '< TONEMAP FILMIC >';
    R3D_TONEMAP_ACES:
      txt := '< TONEMAP ACES >';
    R3D_TONEMAP_AGX:
      txt := '< TONEMAP AGX >';
  else
    txt := '< UNKNOWN >';
  end;

  if tonemap <= R3D_TONEMAP_AGX then
  begin
    txtWidth := MeasureText(PChar(txt), 20);
    DrawText(PChar(txt), GetScreenWidth - txtWidth - 10, 10, 20, LIME);
  end;

  { --- I think we understand what's going on here --- }

  DrawFPS(10, 10);
end;

procedure Close;
begin
  R3D_UnloadModel(@sponza, true);
  R3D_UnloadSkybox(skybox);
  R3D_Close();
end;

{ Main program }
var
  screenWidth, screenHeight: Integer;
begin
  screenWidth := 800;
  screenHeight := 600;

  InitWindow(screenWidth, screenHeight, 'R3D Sponza Example');

  Init();

  while not WindowShouldClose() do
  begin
    Update(GetFrameTime());

    BeginDrawing();
      ClearBackground(BLACK);
      Draw();
    EndDrawing();
  end;

  Close();

  CloseWindow();
end.
