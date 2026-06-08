program to_texture;

{$mode objfpc}{$H+}

uses
  SysUtils, Math, raylib, r3d, raymath;

//const
//  RESOURCES_PATH = 'resources/';
var
  plane, sphere: TR3D_Mesh;
  material: TR3D_Material;
  light: TR3D_Light;
  target: TRenderTexture;
  r3dCamera, rlCamera: TCamera3D;
  view: TR3D_View;
begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Render to texture');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create meshes
  plane  := R3D_GenMeshPlane(1000, 1000, 1, 1);
  sphere := R3D_GenMeshSphere(0.5, 64, 64);
  material := R3D_GetDefaultMaterial();

  // Setup environment
  R3D_ENVIRONMENT_SET('ambient.color', ColorCreate(10, 10, 10, 255));

  // Create light
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(light, Vector3Create(0, 10, 5), Vector3Create(0,0,0) );
  R3D_EnableShadow(light);
  R3D_SetLightActive(light, true);

  // Render texture
  target := LoadRenderTexture(1024, 512);

  // Setup cameras
    r3dCamera.position := Vector3Create(0, 2, 2);
    r3dCamera.target := Vector3Create(0, 0, 0);
    r3dCamera.up := Vector3Create(0, 1, 0);
    r3dCamera.fovy := 60;

    rlCamera.position := Vector3Create(0, 1, 4);
    rlCamera.target := Vector3Create(0, 1, -1);
    rlCamera.up := Vector3Create(0, 1, 0);
    rlCamera.fovy := 60;


    DisableCursor();

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@r3dCamera, CAMERA_ORBITAL);
    UpdateCamera(@rlCamera, CAMERA_FREE);

    BeginDrawing();
        ClearBackground(DARKGRAY);

        view.camera := R3D_CameraFromRL(r3dCamera);
        view.target := target;


        R3D_BeginPro(view);
            R3D_DrawMesh(plane, material, Vector3Create(0, -0.5, 0), 1.0);
            R3D_DrawMesh(sphere, material, Vector3Zero(), 1.0);
        R3D_End();

        BeginMode3D(rlCamera);
            DrawBillboard(rlCamera, target.texture, Vector3Create(0, 1, 0), -2, WHITE);
            DrawGrid(10, 1);
        EndMode3D();

    EndDrawing();

  end;


  // Cleanup
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.
