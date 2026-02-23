program r3d_skyboxnebula;

{$mode objfpc}{$H+}
{$WARN 5036 off : Local variable "$1" does not seem to be initialized}
{$WARN 5037 off : Variable "$1" does not seem to be initialized}
uses
  SysUtils, Math,
  raylib,
  r3d,
  raymath;

const
  RESOURCES_PATH = 'resources/';

var
  screenWidth, screenHeight: Integer;
  camera: TCamera3D;
  sphere: TR3D_Mesh;
  shader: PR3D_SkyShader;
  skyCustom: TR3D_Cubemap;
  ambientCustom: TR3D_AmbientMap;
  material: TR3D_Material;
  x, y: Integer;

  resolution: TVector2;
  showHelp: Boolean;

  // Цвета туманностей
  nebulaColor1, nebulaColor2, nebulaColor3, nebulaColor4: TVector3;
  nebulaIntensity1, nebulaIntensity2, nebulaIntensity3, nebulaIntensity4: Single;
  brightness: Single;
  seed: Single;
  noiseScale: Single;

  // Индекс текущего редактируемого цвета
  currentColorIndex: Integer;

  procedure UpdateShaderUniforms;
  begin
    if shader = nil then Exit;

    R3D_SetSkyShaderUniform(shader, 'uNebulaColor1', @nebulaColor1);
    R3D_SetSkyShaderUniform(shader, 'uNebulaColor2', @nebulaColor2);
    R3D_SetSkyShaderUniform(shader, 'uNebulaColor3', @nebulaColor3);
    R3D_SetSkyShaderUniform(shader, 'uNebulaColor4', @nebulaColor4);

    R3D_SetSkyShaderUniform(shader, 'uNebulaIntensity1', @nebulaIntensity1);
    R3D_SetSkyShaderUniform(shader, 'uNebulaIntensity2', @nebulaIntensity2);
    R3D_SetSkyShaderUniform(shader, 'uNebulaIntensity3', @nebulaIntensity3);
    R3D_SetSkyShaderUniform(shader, 'uNebulaIntensity4', @nebulaIntensity4);

    R3D_SetSkyShaderUniform(shader, 'uBrightness', @brightness);
    R3D_SetSkyShaderUniform(shader, 'u_resolution', @resolution);
    R3D_SetSkyShaderUniform(shader, 'u_seed', @seed);
    R3D_SetSkyShaderUniform(shader, 'uNoiseScale', @noiseScale);

    // Обновляем skybox с новыми параметрами
    R3D_UpdateCustomSky(@skyCustom, shader);

    // Обновляем ambient map для новых цветов
    if ambientCustom.irradiance <> 0 then
      R3D_UnloadAmbientMap(ambientCustom);

    ambientCustom := R3D_GenAmbientMap(skyCustom, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);

    // Применяем новое окружение
    R3D_ENVIRONMENT_SET('background.sky', skyCustom);
    R3D_ENVIRONMENT_SET('ambient.map', ambientCustom);
  end;

  procedure InitDefaultColors;
  begin
    // Фиолетовая туманность
    nebulaColor1 := Vector3Create(0.6, 0.3, 0.8);
    nebulaIntensity1 := 0.5;

    // Синяя туманность
    nebulaColor2 := Vector3Create(0.2, 0.4, 0.9);
    nebulaIntensity2 := 0.4;

    // Розовая туманность
    nebulaColor3 := Vector3Create(0.9, 0.4, 0.5);
    nebulaIntensity3 := 0.3;

    // Зеленоватая туманность
    nebulaColor4 := Vector3Create(0.3, 0.7, 0.5);
    nebulaIntensity4 := 0.2;

    brightness := 1.0;
    seed := 0.0;
    noiseScale := 1.0;
    currentColorIndex := 1;
    showHelp := True;
  end;

  procedure ChangeIntensity(var intensity: Single; increment: Boolean);
  var
    step: Single;
  begin
    step := 0.05;
    if increment then
      intensity := intensity + step
    else
      intensity := intensity - step;

    intensity := EnsureRange(intensity, 0.0, 2.0);
  end;

  procedure ChangeColorChannel(var color: TVector3; channel: Char; increment: Boolean);
  var
    step: Single;
  begin
    step := 0.05;

    case channel of
      'R':
        if increment then
          color.x := color.x + step
        else
          color.x := color.x - step;
      'G':
        if increment then
          color.y := color.y + step
        else
          color.y := color.y - step;
      'B':
        if increment then
          color.z := color.z + step
        else
          color.z := color.z - step;
    end;

    color.x := EnsureRange(color.x, 0.0, 1.0);
    color.y := EnsureRange(color.y, 0.0, 1.0);
    color.z := EnsureRange(color.z, 0.0, 1.0);
  end;

  procedure ChangeSeed(increment: Boolean);
  var
    step: Single;
  begin
    step := 0.01;  // Изменено для диапазона 0-0.1
    if increment then
      seed := seed + step
    else
      seed := seed - step;

    seed := EnsureRange(seed, 0.0, 0.1);  // Диапазон 0-0.1
  end;

  procedure ChangeNoiseScale(increment: Boolean);
  var
    step: Single;
  begin
    step := 0.1;
    if increment then
      noiseScale := noiseScale + step
    else
      noiseScale := noiseScale - step;

    noiseScale := EnsureRange(noiseScale, 0.1, 3.0);
  end;

  procedure DrawHelp;
  var
    yPos: Integer;
    currentColor: TVector3;
  begin
    yPos := 10;

    DrawRectangle(5, 5, 280, 300, Fade(BLACK, 0.7));
    DrawRectangleLines(5, 5, 280, 300, WHITE);

    DrawText('Space Skybox Controls:', 20, yPos, 10, WHITE); yPos += 20;
    DrawText('H - Toggle help', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('1-4 - Select nebula layer', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Alt+R / Shift+R - Change Red channel', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Alt+G / Shift+G - Change Green channel', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Alt+B / Shift+B - Change Blue channel', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Alt+I / Shift+I - Change intensity', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Alt+L / Shift+L - Change brightness', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Alt+N / Shift+N - Change noise scale', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Alt+S / Shift+S - Change seed', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Space - Random colors + seed', 20, yPos, 10, LIGHTGRAY); yPos += 15;
    DrawText('Enter - Reset to defaults', 20, yPos, 10, LIGHTGRAY); yPos += 20;

    DrawText(TextFormat('Current layer: %d', currentColorIndex), 20, yPos, 10, YELLOW); yPos += 20;

    case currentColorIndex of
      1: currentColor := nebulaColor1;
      2: currentColor := nebulaColor2;
      3: currentColor := nebulaColor3;
      4: currentColor := nebulaColor4;
    end;

    DrawText(TextFormat('Color: R=%.2f G=%.2f B=%.2f', currentColor.x, currentColor.y, currentColor.z), 20, yPos, 10, WHITE); yPos += 15;

    case currentColorIndex of
      1: DrawText(TextFormat('Intensity 1: %.2f', nebulaIntensity1), 20, yPos, 10, WHITE);
      2: DrawText(TextFormat('Intensity 2: %.2f', nebulaIntensity2), 20, yPos, 10, WHITE);
      3: DrawText(TextFormat('Intensity 3: %.2f', nebulaIntensity3), 20, yPos, 10, WHITE);
      4: DrawText(TextFormat('Intensity 4: %.2f', nebulaIntensity4), 20, yPos, 10, WHITE);
    end; yPos += 15;

    DrawText(TextFormat('Brightness: %.2f', brightness), 20, yPos, 10, WHITE); yPos += 15;
    DrawText(TextFormat('Noise Scale: %.2f', noiseScale), 20, yPos, 10, WHITE); yPos += 15;
    DrawText(TextFormat('Seed: %.4f', seed), 20, yPos, 10, WHITE);  // Изменен формат для отображения 4 знаков
  end;

  procedure RandomizeColors;
  begin
    nebulaColor1 := Vector3Create(Random, Random, Random);
    nebulaColor2 := Vector3Create(Random, Random, Random);
    nebulaColor3 := Vector3Create(Random, Random, Random);
    nebulaColor4 := Vector3Create(Random, Random, Random);

    nebulaIntensity1 := Random * 1.5 + 0.2;
    nebulaIntensity2 := Random * 1.5 + 0.2;
    nebulaIntensity3 := Random * 1.5 + 0.2;
    nebulaIntensity4 := Random * 1.5 + 0.2;

    brightness := Random * 1.5 + 0.5;
  end;

  procedure RandomizeSeed;
  begin
    seed := Random * 0.1;  // Диапазон 0-0.1
  end;

begin
  screenWidth := 1024;
  screenHeight := 768;
    // Инициализация генератора случайных чисел
  Randomize();
  InitWindow(screenWidth, screenHeight, '[r3d] - Space Skybox with Nebula Forms');
  SetTargetFPS(60);

  R3D_Init(screenWidth, screenHeight);

  sphere := R3D_GenMeshSphere(0.5, 32, 64);

  // Загружаем шейдер
  shader := R3D_LoadSkyShader(PAnsiChar(RESOURCES_PATH + 'shaders/nebula.glsl'));
  if shader = nil then
  begin
    TraceLog(LOG_ERROR, 'Failed to load sky shader');
    CloseWindow();
    Exit;
  end;


  // Устанавливаем uniform-переменные
  resolution := Vector2Create(screenWidth, screenHeight);
  R3D_SetSkyShaderUniform(shader, 'u_resolution', @resolution);

     InitDefaultColors;
  UpdateShaderUniforms;

  // Создаем skybox
  skyCustom := R3D_GenCustomSky(512, shader);

  camera.position := Vector3Create(0, 0, 10);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  DisableCursor();

  InitDefaultColors;
  UpdateShaderUniforms;



  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_FREE);

    // Обработка клавиш
    if IsKeyPressed(KEY_H) then
      showHelp := not showHelp;

    if IsKeyPressed(KEY_ONE) then currentColorIndex := 1;
    if IsKeyPressed(KEY_TWO) then currentColorIndex := 2;
    if IsKeyPressed(KEY_THREE) then currentColorIndex := 3;
    if IsKeyPressed(KEY_FOUR) then currentColorIndex := 4;

    // Alt+Key для увеличения, Shift+Key для уменьшения

    // Изменение цветовых каналов
    if IsKeyDown(KEY_LEFT_ALT) or IsKeyDown(KEY_RIGHT_ALT) then
    begin
      if IsKeyPressed(KEY_R) then
        ChangeColorChannel(nebulaColor1, 'R', True);
      if IsKeyPressed(KEY_G) then
        ChangeColorChannel(nebulaColor1, 'G', True);
      if IsKeyPressed(KEY_B) then
        ChangeColorChannel(nebulaColor1, 'B', True);
    end;

    if IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT) then
    begin
      if IsKeyPressed(KEY_R) then
        ChangeColorChannel(nebulaColor1, 'R', False);
      if IsKeyPressed(KEY_G) then
        ChangeColorChannel(nebulaColor1, 'G', False);
      if IsKeyPressed(KEY_B) then
        ChangeColorChannel(nebulaColor1, 'B', False);
    end;

    // Изменение интенсивности (I)
    if IsKeyDown(KEY_LEFT_ALT) or IsKeyDown(KEY_RIGHT_ALT) then
    begin
      if IsKeyPressed(KEY_I) then
      begin
        case currentColorIndex of
          1: ChangeIntensity(nebulaIntensity1, True);
          2: ChangeIntensity(nebulaIntensity2, True);
          3: ChangeIntensity(nebulaIntensity3, True);
          4: ChangeIntensity(nebulaIntensity4, True);
        end;
        UpdateShaderUniforms;
      end;
    end;

    if IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT) then
    begin
      if IsKeyPressed(KEY_I) then
      begin
        case currentColorIndex of
          1: ChangeIntensity(nebulaIntensity1, False);
          2: ChangeIntensity(nebulaIntensity2, False);
          3: ChangeIntensity(nebulaIntensity3, False);
          4: ChangeIntensity(nebulaIntensity4, False);
        end;
        UpdateShaderUniforms;
      end;
    end;

    // Изменение яркости (L)
    if IsKeyDown(KEY_LEFT_ALT) or IsKeyDown(KEY_RIGHT_ALT) then
    begin
      if IsKeyPressed(KEY_L) then
      begin
        brightness := brightness + 0.1;
        UpdateShaderUniforms;
      end;
    end;

    if IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT) then
    begin
      if IsKeyPressed(KEY_L) then
      begin
        brightness := brightness - 0.1;
        UpdateShaderUniforms;
      end;
    end;

    // Изменение seed (S)
    if IsKeyDown(KEY_LEFT_ALT) or IsKeyDown(KEY_RIGHT_ALT) then
    begin
      if IsKeyPressed(KEY_S) then
      begin
        ChangeSeed(True);
        UpdateShaderUniforms;
      end;
    end;

    if IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT) then
    begin
      if IsKeyPressed(KEY_S) then
      begin
        ChangeSeed(False);
        UpdateShaderUniforms;
      end;
    end;

    // Изменение noise scale (N)
    if IsKeyDown(KEY_LEFT_ALT) or IsKeyDown(KEY_RIGHT_ALT) then
    begin
      if IsKeyPressed(KEY_N) then
      begin
        ChangeNoiseScale(True);
        UpdateShaderUniforms;
      end;
    end;

    if IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT) then
    begin
      if IsKeyPressed(KEY_N) then
      begin
        ChangeNoiseScale(False);
        UpdateShaderUniforms;
      end;
    end;

    // Пробел - случайные цвета + seed
    if IsKeyPressed(KEY_SPACE) then
    begin
      RandomizeColors;
      RandomizeSeed;
      UpdateShaderUniforms;
    end;

    // Сброс к значениям по умолчанию
    if IsKeyPressed(KEY_ENTER) then
    begin
      InitDefaultColors;
      UpdateShaderUniforms;
    end;

    BeginDrawing();
      ClearBackground(DARKGRAY);

      R3D_Begin(camera);
        for x := 0 to 8 do
        begin
          for y := 0 to 8 do
          begin
            material := R3D_MATERIAL_BASE;
            material.orm.roughness := Remap(y, 0, 8, 0.0, 1.0);
            material.orm.metalness := Remap(x, 0, 8, 0.0, 1.0);

            R3D_DrawMesh(sphere, material,
              Vector3Create(
                (x - 4) * 1.25,
                (y - 4) * 1.25,
                0.0
              ),
              1.0
            );
          end;
        end;
      R3D_End();

      if showHelp then
        DrawHelp;

      DrawFPS(10, screenHeight - 30);
      DrawText(TextFormat('Seed: %.4f', seed), 10, screenHeight - 50, 20, GREENYELLOW);  // Изменен формат

    EndDrawing();
  end;

  // Cleanup
  if ambientCustom.irradiance <> 0 then
    R3D_UnloadAmbientMap(ambientCustom);

  R3D_UnloadCubemap(skyCustom);
  R3D_UnloadMesh(sphere);
  R3D_Close();
  CloseWindow();
end.
