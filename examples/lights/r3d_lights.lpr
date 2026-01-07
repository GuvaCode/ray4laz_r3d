program ManyLightsExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  GRID_SIZE = 100;
  LIGHT_GRID = 10;

var
  ScreenWidth, ScreenHeight: Integer;
  plane, sphere: TR3D_Mesh;
  material: TR3D_Material;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  lights: array[0..LIGHT_GRID * LIGHT_GRID - 1] of TR3D_Light;
  camera: TCamera3D;
  x, z, index, i: Integer;
  ambientColor: TColor;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Many lights example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Set ambient light
  ambientColor := ColorCreate(10, 10, 10, 255);
  R3D_GetEnvironment^.ambient.color := ambientColor;

  // Create plane and sphere meshes
  plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  sphere := R3D_GenMeshSphere(0.35, 16, 16);
  material := R3D_GetDefaultMaterial();

  // Allocate transforms for all spheres
  instances := R3D_LoadInstanceBuffer(GRID_SIZE * GRID_SIZE, R3D_INSTANCE_POSITION);
  positions := PVector3(R3D_MapInstances(instances, R3D_INSTANCE_POSITION));

  for x := -50 to 49 do
  begin
    for z := -50 to 49 do
    begin
      positions[(z+50)*GRID_SIZE + (x+50)] := Vector3Create(x, 0, z);
    end;
  end;

  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION);

  // Create lights
  for x := -5 to 4 do
  begin
    for z := -5 to 4 do
    begin
      index := (z+5)*LIGHT_GRID + (x+5);
      lights[index] := R3D_CreateLight(R3D_LIGHT_OMNI);
      R3D_SetLightPosition(lights[index], Vector3Create(x*10, 10, z*10));
      R3D_SetLightColor(lights[index], ColorFromHSV(index/100.0*360, 1.0, 1.0));
      R3D_SetLightRange(lights[index], 20.0);
      R3D_SetLightActive(lights[index], True);
    end;
  end;

  // Setup camera
  camera.position := Vector3Create(0, 2, 2);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_ORBITAL);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Draw scene
      R3D_Begin(camera);
        R3D_DrawMesh(plane, material, Vector3Create(0, -0.5, 0), 1.0);
        R3D_DrawMeshInstanced(sphere, material, instances, GRID_SIZE*GRID_SIZE);
      R3D_End();

      // Optionally show lights shapes
      if IsKeyDown(KEY_SPACE) then
      begin
        BeginMode3D(camera);
        for i := 0 to LIGHT_GRID*LIGHT_GRID - 1 do
          R3D_DrawLightShape(lights[i]);
        EndMode3D();
      end;

      DrawFPS(10, 10);
      DrawText('Press SPACE to show the lights', 10, GetScreenHeight()-34, 24, BLACK);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadInstanceBuffer(instances);
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.
