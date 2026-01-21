program ParticlesExample;

uses
  SysUtils, Math, raylib, r3d;

const
  MAX_PARTICLES = 4096;
  DEG2RAD = PI / 180.0;

type
  TParticle = record
    pos: TVector3;
    vel: TVector3;
    life: Single;
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
  positions: array[0..MAX_PARTICLES-1] of TVector3;
  particleCount, alive, i: Integer;
  dt, angle: Single;
  bgColor, emissionColor: TColor;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Particles example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Set environment
  bgColor := ColorCreate(4, 4, 4, 255);
  R3D_ENVIRONMENT_SET('background.color', bgColor);
  R3D_ENVIRONMENT_SET('bloom.mode', R3D_BLOOM_ADDITIVE);


  // Generate a gradient as emission texture for our particles
  image := GenImageGradientRadial(64, 64, 0.0, WHITE, BLACK);
  texture := LoadTextureFromImage(image);
  UnloadImage(image);

  // Generate a quad mesh for our particles
  mesh := R3D_GenMeshQuad(0.25, 0.25, 1, 1, Vector3Create(0, 0, 1));

  // Setup particle material
  material := R3D_GetDefaultMaterial();
  material.billboardMode := R3D_BILLBOARD_FRONT;
  material.blendMode := R3D_BLEND_ADDITIVE;
  material.albedo.texture := R3D_GetBlackTexture();

  emissionColor := ColorCreate(255, 0, 0, 255);
  material.emission.color := emissionColor;
  material.emission.texture := texture;
  material.emission.energy := 1.0;

  // Create particle instance buffer
  instances := R3D_LoadInstanceBuffer(MAX_PARTICLES, R3D_INSTANCE_POSITION);

  // Setup camera
  camera.position := Vector3Create(-7, 7, -7);
  camera.target := Vector3Create(0, 1, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Initialize particles
  particleCount := 0;
  Randomize;

  while not WindowShouldClose() do
  begin
    dt := GetFrameTime();
    UpdateCamera(@camera, CAMERA_ORBITAL);

    // Spawn particles
    for i := 0 to 9 do
    begin
      if particleCount < MAX_PARTICLES then
      begin
        angle := GetRandomValue(0, 360) * DEG2RAD;
        particles[particleCount].pos := Vector3Create(0, 0, 0);
        particles[particleCount].vel := Vector3Create(
          Cos(angle) * GetRandomValue(20, 40) / 10.0,
          GetRandomValue(60, 80) / 10.0,
          Sin(angle) * GetRandomValue(20, 40) / 10.0
        );
        particles[particleCount].life := 1.0;
        Inc(particleCount);
      end;
    end;

    // Update particles
    alive := 0;
    for i := 0 to particleCount - 1 do
    begin
      particles[i].vel.y := particles[i].vel.y - 9.81 * dt;
      particles[i].pos.x := particles[i].pos.x + particles[i].vel.x * dt;
      particles[i].pos.y := particles[i].pos.y + particles[i].vel.y * dt;
      particles[i].pos.z := particles[i].pos.z + particles[i].vel.z * dt;
      particles[i].life := particles[i].life - dt * 0.5;

      if particles[i].life > 0 then
      begin
        positions[alive] := particles[i].pos;
        particles[alive] := particles[i];
        Inc(alive);
      end;
    end;
    particleCount := alive;

    // Upload positions to GPU
    if particleCount > 0 then
    begin
      R3D_UploadInstances(instances, R3D_INSTANCE_POSITION, 0, particleCount, @positions[0]);
    end;

    BeginDrawing();
      R3D_Begin(camera);
        if particleCount > 0 then
        begin
          R3D_DrawMeshInstanced(mesh, material, instances, particleCount);
        end;
      R3D_End();
      DrawFPS(10, 10);
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
