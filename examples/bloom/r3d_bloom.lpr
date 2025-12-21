program BloomExample;

{$mode objfpc}{$H+}

uses
  Math, SysUtils,
  raylib, raymath, r3d;

const
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 600;

var
  // === Resources ===
  Cube: TR3D_Mesh;
  Material: TR3D_Material;
  Camera: TCamera3D;
  HueCube: Single;

// === Local Functions ===

function GetBloomModeName(mode: TR3D_Bloom): PAnsiChar;
begin
  case R3D_GetBloomMode() of
    R3D_BLOOM_DISABLED:
      Result := 'Disabled';
    R3D_BLOOM_MIX:
      Result := 'Mix';
    R3D_BLOOM_ADDITIVE:
      Result := 'Additive';
    R3D_BLOOM_SCREEN:
      Result := 'Screen';
  else
    Result := 'Unknown';
  end;
end;

// === Example ===

procedure InitExample;
begin
  // --- Initialize R3D with its internal resolution ---
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SetTargetFPS(60);

  // --- Setup the default bloom parameters ---
  R3D_SetTonemapMode(R3D_TONEMAP_ACES);
  R3D_SetBloomMode(R3D_BLOOM_MIX);
  R3D_SetBackgroundColor(BLACK);

  // --- Load a cube mesh and setup its material ---
  Cube := R3D_GenMeshCube(1.0, 1.0, 1.0);
  Material := R3D_GetDefaultMaterial();

  HueCube := 0.0;
  Material.emission.color := ColorFromHSV(HueCube, 1.0, 1.0);
  Material.emission.energy := 1.0;
  Material.albedo.color := BLACK;

  // --- Setup the camera ---
  Camera.position := Vector3Create(0, 3.5, 5);
  Camera.target := Vector3Create(0, 0, 0);
  Camera.up := Vector3Create(0, 1, 0);
  Camera.fovy := 60;
  Camera.projection := CAMERA_PERSPECTIVE;
end;

procedure UpdateExample(Delta: Single);
var
  HueDir, IntensityDir, RadiusDir: Integer;
  CurrentBloomMode: TR3D_Bloom;
begin
  UpdateCamera(@Camera, CAMERA_ORBITAL);

  // Изменение цвета куба с помощью мыши
  HueDir := Ord(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) -
            Ord(IsMouseButtonDown(MOUSE_BUTTON_LEFT));

  if HueDir <> 0 then
  begin
    HueCube := Wrap(HueCube + HueDir * 90.0 * Delta, 0, 360);
    Material.emission.color := ColorFromHSV(HueCube, 1.0, 1.0);
  end;

  // Изменение интенсивности блюма с помощью клавиш влево/вправо
  IntensityDir := Ord(IsKeyPressedRepeat(KEY_RIGHT) or IsKeyPressed(KEY_RIGHT)) -
                  Ord(IsKeyPressedRepeat(KEY_LEFT) or IsKeyPressed(KEY_LEFT));

  if IntensityDir <> 0 then
  begin
    R3D_SetBloomIntensity(R3D_GetBloomIntensity() + IntensityDir * 0.01);
  end;

  // Изменение радиуса фильтра блюма с помощью клавиш вверх/вниз
  RadiusDir := Ord(IsKeyPressedRepeat(KEY_UP) or IsKeyPressed(KEY_UP)) -
               Ord(IsKeyPressedRepeat(KEY_DOWN) or IsKeyPressed(KEY_DOWN));

  if RadiusDir <> 0 then
  begin
    R3D_SetBloomFilterRadius(R3D_GetBloomFilterRadius() + RadiusDir);
  end;

  // Переключение режимов блюма с помощью пробела
  if IsKeyPressed(KEY_SPACE) then
  begin
    CurrentBloomMode := R3D_GetBloomMode();
    CurrentBloomMode := TR3D_Bloom((Ord(CurrentBloomMode) + 1) mod (Ord(R3D_BLOOM_SCREEN) + 1));
    R3D_SetBloomMode(CurrentBloomMode);
  end;
end;

procedure DrawExample;
var
  InfoStr: string;
  InfoLen: Integer;
begin
  R3D_Begin(Camera);
    R3D_DrawMesh(@Cube, @Material, MatrixIdentity());
  R3D_End();

  // Отрисовка буферов эмиссии и блюма для отладки
  // Примечание: Эти функции могут быть недоступны в Pascal версии R3D
  // R3D_DrawBufferEmission(10, 10, 100, 100);
  // R3D_DrawBufferBloom(120, 10, 100, 100);

  // Отображение информации о настройках блюма
  InfoStr := Format('Mode: %s', [GetBloomModeName(R3D_GetBloomMode())]);
  InfoLen := MeasureText(PAnsiChar(InfoStr), 20);
  DrawText(PAnsiChar(InfoStr), GetScreenWidth() - InfoLen - 10, 10, 20, LIME);

  InfoStr := Format('Intensity: %.2f', [R3D_GetBloomIntensity()]);
  InfoLen := MeasureText(PAnsiChar(InfoStr), 20);
  DrawText(PAnsiChar(InfoStr), GetScreenWidth() - InfoLen - 10, 40, 20, LIME);

  InfoStr := Format('Filter Radius: %d', [R3D_GetBloomFilterRadius()]);
  InfoLen := MeasureText(PAnsiChar(InfoStr), 20);
  DrawText(PAnsiChar(InfoStr), GetScreenWidth() - InfoLen - 10, 70, 20, LIME);
end;

procedure CloseExample;
begin
  R3D_UnloadMesh(@Cube);
  R3D_UnloadMaterial(@Material);
  R3D_Close();
end;

begin
  // Инициализация окна
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Bloom example');

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
