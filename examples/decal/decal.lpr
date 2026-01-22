program DecalExample;

uses
  SysUtils, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  plane, sphere, cylinder: TR3D_Mesh;
  material: TR3D_Material;
  decal: TR3D_Decal;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  light: TR3D_Light;
  camera: TCamera3D;
  mousePos: TVector2;
  ray: TRay;
  hitPoint: TVector3;

begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Decal example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create meshes
  plane := R3D_GenMeshPlane(5.0, 5.0, 1, 1);
  sphere := R3D_GenMeshSphere(0.5, 64, 64);
  cylinder := R3D_GenMeshCylinder(0.5, 0.5, 1, 64);
  material := R3D_GetDefaultMaterial();
  material.albedo.color := GRAY;

  // Create decal
  decal := R3D_DECAL_BASE;
  R3D_SetTextureFilter(TEXTURE_FILTER_BILINEAR);
  decal.albedo := R3D_LoadAlbedoMap(PAnsiChar(RESOURCES_PATH + 'images/decal.png'), WHITE);
  decal.normal := R3D_LoadNormalMap(PAnsiChar(RESOURCES_PATH + 'images/decal_normal.png'), 1.0);
  decal.normalThreshold := 45.0;
  decal.fadeWidth := 20.0;

  // Create data for instanced drawing
  instances := R3D_LoadInstanceBuffer(3, R3D_INSTANCE_POSITION);
  positions := R3D_MapInstances(instances, R3D_INSTANCE_POSITION);
  positions[0] := Vector3Create(-1.25, 0, 1);
  positions[1] := Vector3Create(0, 0, 1);
  positions[2] := Vector3Create(1.25, 0, 1);
  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION);

  // Setup environment
  R3D_ENVIRONMENT_SET('ambient.color', ColorCreate(10, 10, 10, 255));

  // Create light
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(0.5, -1, -0.5));
  R3D_SetShadowDepthBias(light, 0.005);
  R3D_EnableShadow(light);
  R3D_SetLightActive(light, True);

  // Setup camera
  camera.position := Vector3Create(0, 3, 3);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Capture mouse
  DisableCursor();

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_FREE);

    // Get mouse position and ray for interaction
    mousePos := GetMousePosition();
    ray := GetMouseRay(mousePos, camera);

    // Simple ray-plane intersection for demo purposes
    if ray.direction.y < 0 then
    begin
      hitPoint.y := 0;
      hitPoint.x := ray.position.x - (ray.position.y / ray.direction.y) * ray.direction.x;
      hitPoint.z := ray.position.z - (ray.position.y / ray.direction.y) * ray.direction.z;
    end
    else
    begin
      hitPoint := Vector3Create(0, 0, 0);
    end;

    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);
        R3D_DrawMesh(plane, material, Vector3Create(0, 0, 0), 1.0);
        R3D_DrawMesh(sphere, material, Vector3Create(-1, 0.5, -1), 1.0);
        R3D_DrawMeshEx(cylinder, material, Vector3Create(1, 0.5, -1),
          QuaternionFromEuler(0, 0, PI/2), Vector3Create(1, 1, 1));

        R3D_DrawDecal(decal, Vector3Create(-1, 1, -1), 1.0);
        R3D_DrawDecalEx(decal, Vector3Create(1, 0.5, -0.5),
          QuaternionFromEuler(PI/2, 0, 0), Vector3Create(1.25, 1.25, 1.25));
        R3D_DrawDecalInstanced(decal, instances, 3);

        // Draw decal at mouse hit point
        R3D_DrawDecal(decal, hitPoint, 0.5);
      R3D_End();

      DrawText('MOVE: WASD/Arrows, LOOK: Mouse, DECAL: Follows mouse', 10, 10, 20, LIME);
      DrawText(TextFormat('Hit Point: %.2f, %.2f, %.2f', hitPoint.x, hitPoint.y, hitPoint.z),
        10, 40, 20, LIME);
      DrawFPS(10, 70);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMesh(plane);
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(cylinder);
  R3D_UnloadMaterial(material);
  R3D_UnloadDecalMaps(decal);
  R3D_Close();

  CloseWindow();
end.

