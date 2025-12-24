program BloomExample;

{$mode objfpc}{$H+}

uses
  SysUtils, Math,
  raylib, raymath,
  r3d;

// Прямой порт на Паскаль

function IsKeyDownDelay(key: Integer): Boolean;
begin
  Result := IsKeyPressedRepeat(key) or IsKeyPressed(key);
end;

function GetBloomModeName: string;
const
  modes: array[0..3] of string = ('Disabled', 'Mix', 'Additive', 'Screen');
var
  env: PR3D_Environment;
  mode: Integer;
begin
  env := R3D_GetEnvironment();
  if Assigned(env) then
  begin
    mode := Integer(env^.bloom.mode);
    if (mode >= 0) and (mode <= Integer(R3D_BLOOM_SCREEN)) then
      Result := modes[mode]
    else
      Result := 'Unknown';
  end
  else
    Result := 'No Environment';
end;

procedure DrawTextRight(const text: string; y, fontSize: Integer; color: TColor);
var
  width: Integer;
begin
  width := MeasureText(PChar(text), fontSize);
  DrawText(PChar(text), GetScreenWidth() - width - 10, y, fontSize, color);
end;

procedure AdjustBloomParam(var param: Single; direction: Integer; step, minVal, maxVal: Single);
begin
  if direction <> 0 then
    param := Clamp(param + direction * step, minVal, maxVal);
end;


var
  cube: TR3D_Mesh;
  material: TR3D_Material;
  hueCube: Single;
  camera: TCamera3D;
  delta: Single;
  env: PR3D_Environment;
  intensity, radius, levels: Single;
  intensityDir, radiusDir, levelDir: Integer;

begin
  // Инициализация окна
  InitWindow(800, 450, '[r3d] - Bloom example');
  SetTargetFPS(60);

  // Инициализация R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Получаем указатель на environment
  env := R3D_GetEnvironment();
  if not Assigned(env) then
  begin
    TraceLog(LOG_ERROR, 'Failed to get R3D environment');
    CloseWindow();
    Halt(1);
  end;

  // Настройка bloom и tonemapping (как в оригинале)
  env^.tonemap.mode := R3D_TONEMAP_ACES;
  env^.bloom.mode := R3D_BLOOM_MIX;
  env^.bloom.levels := 1.0;

  // Установка фона
  env^.background.color := BLACK;

  // Создание куба и материала
  cube := R3D_GenMeshCube(1.0, 1.0, 1.0);
  material := R3D_GetDefaultMaterial();

  hueCube := 0.0;
  material.emission.color := ColorFromHSV(hueCube, 1.0, 1.0);
  material.emission.energy := 1.0;
  material.albedo.color := BLACK;

  // Настройка камеры (аналогично C-коду)
  camera.position := Vector3Create(0, 3.5, 5);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Основной цикл
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();
    UpdateCamera(@camera, CAMERA_ORBITAL);

    // Изменение цвета куба
    if IsKeyDown(KEY_C) then
    begin
      hueCube := Wrap(hueCube + 45.0 * delta, 0, 360);
      material.emission.color := ColorFromHSV(hueCube, 1.0, 1.0);
    end;

    // Регулировка параметров bloom
    intensity := env^.bloom.intensity;
    intensityDir := Integer(IsKeyDownDelay(KEY_RIGHT)) - Integer(IsKeyDownDelay(KEY_LEFT));
    AdjustBloomParam(intensity, intensityDir, 0.01, 0.0, MaxSingle); // INFINITY заменено на MaxSingle
    env^.bloom.intensity := intensity;

    radius := env^.bloom.filterRadius;
    radiusDir := Integer(IsKeyDownDelay(KEY_UP)) - Integer(IsKeyDownDelay(KEY_DOWN));
    AdjustBloomParam(radius, radiusDir, 0.1, 0.0, MaxSingle); // INFINITY заменено на MaxSingle
    env^.bloom.filterRadius := radius;

    levelDir := Integer(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) -
                Integer(IsMouseButtonDown(MOUSE_BUTTON_LEFT));
    levels := env^.bloom.levels;
    AdjustBloomParam(levels, levelDir, 0.01, 0.0, 1.0);
    env^.bloom.levels := levels;

    // Переключение режима bloom
    if IsKeyPressed(KEY_SPACE) then
    begin
      env^.bloom.mode := TR3D_Bloom((Integer(env^.bloom.mode) + 1) mod (Integer(R3D_BLOOM_SCREEN) + 1));
    end;

    // Отрисовка сцены
    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);
        R3D_DrawMesh(@cube, @material, MatrixIdentity());
      R3D_End();

      // Отображение информации о bloom
      DrawTextRight('Mode: ' + GetBloomModeName(), 10, 20, LIME);
      DrawTextRight(Format('Intensity: %.2f', [env^.bloom.intensity]), 40, 20, LIME);
      DrawTextRight(Format('Filter Radius: %.2f', [env^.bloom.filterRadius]), 70, 20, LIME);
      DrawTextRight(Format('Levels: %.2f', [env^.bloom.levels]), 100, 20, LIME);

    EndDrawing();
  end;

  // Очистка ресурсов
  R3D_UnloadMesh(@cube);
  R3D_Close();
  CloseWindow();
end.
