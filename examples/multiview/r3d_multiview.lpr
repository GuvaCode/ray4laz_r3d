program Multiview;

uses
  SysUtils, Math, raylib, r3d, raymath;

var
  plane, sphere: TR3D_Mesh;
  material: TR3D_Material;
  light: TR3D_Light;
  cam0, cam1: TR3D_Camera;
  view0, view1: TR3D_View;
  time: Single;
  hw, h: Single;

begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Multiview');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create meshes
  plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  sphere := R3D_GenMeshSphere(0.5, 64, 64);
  material := R3D_GetDefaultMaterial();

  // Setup environment
  R3D_GetEnvironment^.ambient.color := ColorCreate(10, 10, 10, 255);

  // Create light
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_SetLightTarget(light, Vector3Create(0, 10, 5), Vector3Create(0, 0, 0));
  R3D_EnableShadow(light);
  R3D_EnableLight(light);

  // Setup cameras (initialize with default values)
  cam0.position := Vector3Create(0, 0, 0);
  cam0.rotation := QuaternionCreate(0, 0, 0, 1);
  cam0.fovy := 60.0;
  cam0.nearPlane := 0.05;
  cam0.farPlane := 4000.0;
  cam0.cullMask := R3D_LAYER_ALL;
  cam0.projection := R3D_PROJECTION_PERSPECTIVE;

  cam1 := cam0; // Same default values

  // Main loop
  while not WindowShouldClose() do
  begin
    time := GetTime();

    cam0.position.x := 4.0 * Cos(time);
    cam0.position.y := 4.0;
    cam0.position.z := 4.0 * Sin(time);

    cam1.position.x := 4.0 * Cos(-time);
    cam1.position.y := 4.0;
    cam1.position.z := 4.0 * Sin(-time);

    R3D_CameraLookAt(@cam0, Vector3Create(0, 0, 0), Vector3Create(0, 1, 0));
    R3D_CameraLookAt(@cam1, Vector3Create(0, 0, 0), Vector3Create(0, 1, 0));

    hw := GetScreenWidth() / 2;
    h := GetScreenHeight();

    view0.camera := cam0;
    view0.target := Default(TRenderTexture);
    view0.viewport := RectangleCreate(0, 0, hw, h);

    view1.camera := cam1;
    view1.target := Default(TRenderTexture);
    view1.viewport := RectangleCreate(hw, 0, hw, h);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_BeginPro(view0);
        R3D_DrawMesh(plane, material, Vector3Create(0, -0.5, 0), 1.0);
        R3D_DrawMesh(sphere, material, Vector3Create(0, 0, 0), 1.0);
      R3D_End();

      R3D_BeginPro(view1);
        R3D_DrawMesh(plane, material, Vector3Create(0, -0.5, 0), 1.0);
        R3D_DrawMesh(sphere, material, Vector3Create(0, 0, 0), 1.0);
      R3D_End();

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.
