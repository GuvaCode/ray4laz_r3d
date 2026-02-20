program LaserImpactEffect;

uses
  SysUtils, Math, raylib, r3d;

const
  MAX_PARTICLES = 4096;
  DEG2RAD = PI / 180.0;
  IMPACT_DURATION = 1.5;

type
  TParticle = record
    pos: TVector3;
    vel: TVector3;
    life: Single;
    size: Single;
    rotation: Single;
    color: TColor;
  end;

  TQuaternion = record
    x, y, z, w: Single;
  end;

var
  ScreenWidth, ScreenHeight: Integer;
  image: TImage;
  texture: TTexture;
  mesh: TR3D_Mesh;
  material: TR3D_Material;
  instances: TR3D_InstanceBuffer;
  camera: TCamera3D;
  particles: array[0..MAX_PARTICLES-1] of TParticle;

  // Данные для инстансинга
  positions: array[0..MAX_PARTICLES-1] of TVector3;
  rotations: array[0..MAX_PARTICLES-1] of TQuaternion;
  scales: array[0..MAX_PARTICLES-1] of TVector3;
  colors: array[0..MAX_PARTICLES-1] of TColor;

  particleCount, alive, i: Integer;
  dt, time: Single;
  impactPos: TVector3;
  bgColor: TColorB;
  impactActive: Boolean;

// Создание кватерниона из угла поворота вокруг оси Y
function QuaternionFromAngleY(angle: Single): TQuaternion;
var
  halfAngle: Single;
begin
  halfAngle := angle * 0.5;
  Result.x := 0;
  Result.y := Sin(halfAngle);
  Result.z := 0;
  Result.w := Cos(halfAngle);
end;

// Создание эффекта попадания лазера в космосе
procedure SpawnLaserImpact(position: TVector3);
var
  i: Integer;
  angle, speed: Single;
begin
  for i := 0 to 299 do // Немного больше частиц для космоса
  begin
    if particleCount < MAX_PARTICLES then
    begin
      // Случайное направление - ПОЛНАЯ СФЕРА (0-180 градусов)
      angle := GetRandomValue(0, 360) * DEG2RAD;

      particles[particleCount].pos := position;

      // Скорость частиц
      speed := GetRandomValue(50, 200) / 10.0; // Чуть быстрее для космоса
      particles[particleCount].vel := Vector3Create(
        Cos(angle) * Sin(GetRandomValue(0, 180) * DEG2RAD) * speed, // Полная сфера
        Cos(GetRandomValue(0, 180) * DEG2RAD) * speed,
        Sin(angle) * Sin(GetRandomValue(0, 180) * DEG2RAD) * speed
      );

      // Добавляем случайность
      particles[particleCount].vel.x := particles[particleCount].vel.x + GetRandomValue(-30, 30) / 10.0;
      particles[particleCount].vel.y := particles[particleCount].vel.y + GetRandomValue(-30, 30) / 10.0;
      particles[particleCount].vel.z := particles[particleCount].vel.z + GetRandomValue(-30, 30) / 10.0;

      particles[particleCount].life := 1.0;
      particles[particleCount].size := GetRandomValue(3, 12) / 10.0; // Чуть мельче для космоса
      particles[particleCount].rotation := GetRandomValue(0, 360) * DEG2RAD;

      // Цвета для лазерного попадания в космосе (более яркие и холодные оттенки)
      case GetRandomValue(0, 5) of
        0: particles[particleCount].color := ColorCreate(255, 100, 100, 255); // Красный
        1: particles[particleCount].color := ColorCreate(255, 150, 50, 255);  // Оранжевый
        2: particles[particleCount].color := ColorCreate(255, 200, 50, 255);  // Желтый
        3: particles[particleCount].color := ColorCreate(200, 100, 255, 255); // Фиолетовый (лазерный)
        4: particles[particleCount].color := ColorCreate(100, 200, 255, 255); // Голубой (лазерный)
        5: particles[particleCount].color := ColorCreate(255, 255, 100, 255); // Желтый
      end;
      material.emission.color := particles[particleCount].color;
      Inc(particleCount);
    end;
  end;

  impactActive := true;
  time := 0;
end;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Laser Impact Effect IN SPACE');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Set environment - еще темнее для космоса
  bgColor := ColorCreate(0, 0, 0, 255);
  R3D_ENVIRONMENT_SET('background.color', bgColor);
  R3D_ENVIRONMENT_SET('bloom.mode', R3D_BLOOM_ADDITIVE);

  // Создаем текстуру с мягким свечением
  image := GenImageGradientRadial(32, 32, 0.0, WHITE, BLACK);
  texture := LoadTextureFromImage(image);
  UnloadImage(image);

  // Создаем маленький квадратный меш для частиц
  mesh := R3D_GenMeshQuad(0.2, 0.2, 1, 1, Vector3Create(0, 0, 1));

  // Настройка материала
  material := R3D_GetDefaultMaterial();
  material.billboardMode := R3D_BILLBOARD_FRONT;
  material.blendMode := R3D_BLEND_ADDITIVE;
  material.albedo.texture := R3D_GetBlackTexture();
  material.emission.texture := texture;
  material.emission.energy := 3.0; // Ярче для космоса

  // Создаем буфер инстансов
  instances := R3D_LoadInstanceBuffer(MAX_PARTICLES,
    R3D_INSTANCE_POSITION or R3D_INSTANCE_ROTATION or R3D_INSTANCE_SCALE or R3D_INSTANCE_COLOR);

  // Setup camera
  camera.position := Vector3Create(-5, 3, -5);
  camera.target := Vector3Create(0, 1, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Initialize particles
  particleCount := 0;
  impactActive := false;
  impactPos := Vector3Create(0, 1, 0);
  time := 0;

  Randomize;

  while not WindowShouldClose() do
  begin
    dt := GetFrameTime();
    UpdateCamera(@camera, CAMERA_ORBITAL);

    // Нажимаем пробел для создания эффекта
    if IsKeyPressed(KEY_SPACE) then
    begin
      impactPos := Vector3Create(
        GetRandomValue(-30, 30) / 10.0,
        GetRandomValue(-20, 20) / 10.0,  // Теперь может быть и ниже нуля (космос)
        GetRandomValue(-30, 30) / 10.0
      );
      SpawnLaserImpact(impactPos);
    end;

    // Обновление таймера
    if impactActive then
    begin
      time := time + dt;
      if time >= IMPACT_DURATION then
        impactActive := false;
    end;

    // Update particles - В КОСМОСЕ БЕЗ ГРАВИТАЦИИ
    alive := 0;
    for i := 0 to particleCount - 1 do
    begin
      // В космосе НЕТ ГРАВИТАЦИИ - удаляем particles[i].vel.y := particles[i].vel.y - 9.81 * dt;

      // Обновление позиции
      particles[i].pos.x := particles[i].pos.x + particles[i].vel.x * dt;
      particles[i].pos.y := particles[i].pos.y + particles[i].vel.y * dt;
      particles[i].pos.z := particles[i].pos.z + particles[i].vel.z * dt;

      // В космосе почти нет трения, оставляем минимальное для красоты
      particles[i].vel.x := particles[i].vel.x * 0.995;
      particles[i].vel.y := particles[i].vel.y * 0.995;
      particles[i].vel.z := particles[i].vel.z * 0.995;

      // Время жизни
      particles[i].life := particles[i].life - dt * 1.2; // Чуть медленнее затухание

      // Вращение
      particles[i].rotation := particles[i].rotation + dt * 3;

      if particles[i].life > 0 then
      begin
        positions[alive] := particles[i].pos;
        rotations[alive] := QuaternionFromAngleY(particles[i].rotation);

        // Масштаб с учетом жизни
        scales[alive] := Vector3Create(
          particles[i].size * particles[i].life,
          particles[i].size * particles[i].life,
          particles[i].size * particles[i].life
        );

        // Цвет с затуханием
        colors[alive] := ColorCreate(
          Trunc(particles[i].color.r * particles[i].life),
          Trunc(particles[i].color.g * particles[i].life),
          Trunc(particles[i].color.b * particles[i].life),
          255
        );

        particles[alive] := particles[i];
        Inc(alive);
      end;
    end;
    particleCount := alive;

    // Upload instances to GPU
    if particleCount > 0 then
    begin
      R3D_UploadInstances(instances, R3D_INSTANCE_POSITION, 0, particleCount, @positions[0]);
      R3D_UploadInstances(instances, R3D_INSTANCE_ROTATION, 0, particleCount, @rotations[0]);
      R3D_UploadInstances(instances, R3D_INSTANCE_SCALE, 0, particleCount, @scales[0]);
      R3D_UploadInstances(instances, R3D_INSTANCE_COLOR, 0, particleCount, @colors[0]);
    end;

    BeginDrawing();
      ClearBackground(BLACK);

      R3D_Begin(camera);

        // В космосе пол не рисуем или рисуем прозрачную сетку
        // DrawGrid(10, 1.0); // Закомментировал для космоса

        // Рисуем частицы
        if particleCount > 0 then
        begin
          R3D_DrawMeshInstanced(mesh, material, instances, particleCount);
        end;

      R3D_End();

      // UI
      DrawFPS(10, 10);
      DrawText('Press SPACE for laser impact IN SPACE', 10, 30, 20, WHITE);
      DrawText('No gravity - particles fly forever!', 10, 60, 20, LIGHTGRAY);

    EndDrawing();
  end;

  // Cleanup
  UnloadTexture(texture);
  R3D_UnloadInstanceBuffer(instances);
  R3D_UnloadMaterial(material);
  R3D_UnloadMesh(mesh);
  R3D_Close();

  CloseWindow();
end.
