program KinematicsExample;

uses
  Math, SysUtils,
  raylib,
  r3d,
  raymath;

const
  RESOURCES_PATH = 'resources/';

function GetCapsuleCenter(capsule: TR3D_Capsule): TVector3;
begin
  Result := Vector3Scale(Vector3Add(capsule.start, capsule.end_), 0.5);
end;

function GetBoxCenter(box: TBoundingBox): TVector3;
begin
  Result := Vector3Scale(Vector3Add(box.min, box.max), 0.5);
end;

var
  sky: TR3D_Cubemap;
  ambient: TR3D_AmbientMap;
  light: TR3D_Light;

  groundMesh, slopeMesh, capsMesh: TR3D_Mesh;
  slopeMeshData: TR3D_MeshData;
  groundMat, slopeMat: TR3D_Material;
  baseAlbedo: TR3D_AlbedoMap;
  groundBox: TBoundingBox;

  capsule: TR3D_Capsule;
  slopeTransform: TMatrix;

  velocity: TVector3;
  moveSpeed, gravity, jumpForce: Single;

  cameraAngle, cameraPitch: Single;
  camera: TCamera3D;

  dt: Single;
  mouseDelta: TVector2;
  dx, dz: Integer;
  moveInput: TVector3;
  angleRad, pitchRad: Single;
  right, forwardVec: TVector3;
  isGrounded: Boolean;
  movement: TVector3;
  target: TVector3;
  correction: Single;

begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Kinematics Example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());
  R3D_SetTextureFilter(TEXTURE_FILTER_ANISOTROPIC_8X);

  // Setup sky and ambient
  sky := R3D_GenCubemapSky(4096, R3D_CUBEMAP_SKY_BASE);
  ambient := R3D_GenAmbientMap(sky, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  R3D_ENVIRONMENT_SET('background.sky', sky);
  R3D_ENVIRONMENT_SET('ambient.map', ambient);

  // Setup directional light
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(-1.0, -1.0, -1.0));
  R3D_SetLightActive(light, True);
  R3D_SetLightRange(light, 16.0);
  R3D_EnableShadow(light);
  R3D_SetShadowDepthBias(light, 0.005);

  // Load base albedo texture
  baseAlbedo := R3D_LoadAlbedoMap(PAnsiChar(RESOURCES_PATH + 'images/placeholder.png'), WHITE);

  // Ground material
  groundMat := R3D_GetDefaultMaterial();
  groundMat.uvScale := Vector2Create(250.0, 250.0);
  groundMat.albedo := baseAlbedo;

  // Slope material
  slopeMat := R3D_GetDefaultMaterial();
  slopeMat.albedo.color := ColorCreate(255, 255, 0, 255);
  slopeMat.albedo.texture := baseAlbedo.texture;

  // Ground
  groundMesh := R3D_GenMeshPlane(1000.0, 1000.0, 1, 1);
  groundBox.min := Vector3Create(-500.0, -1.0, -500.0);
  groundBox.max := Vector3Create(500.0, 0.0, 500.0);

  // Slope obstacle
  slopeMeshData := R3D_GenMeshDataSlope(2.0, 2.0, 2.0, Vector3Create(0.0, 1.0, -1.0));
  slopeMesh := R3D_LoadMesh(R3D_PRIMITIVE_TRIANGLES, slopeMeshData, nil, R3D_STATIC_MESH);
  slopeTransform := MatrixTranslate(0.0, 1.0, 5.0);

  // Player capsule
  capsule.start := Vector3Create(0.0, 0.5, 0.0);
  capsule.end_ := Vector3Create(0.0, 1.5, 0.0);
  capsule.radius := 0.5;
  capsMesh := R3D_GenMeshCapsule(0.5, 1.0, 64, 32);

  // Player state
  velocity := Vector3Zero();
  moveSpeed := 5.0;
  gravity := -15.0;
  jumpForce := 8.0;

  // Camera
  cameraAngle := 0.0;
  cameraPitch := 30.0;
  camera.position := Vector3Create(0.0, 5.0, 5.0);
  camera.target := GetCapsuleCenter(capsule);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  DisableCursor();

  // Main loop
  while not WindowShouldClose() do
  begin
    dt := GetFrameTime();

    // Camera rotation
    mouseDelta := GetMouseDelta();
    cameraAngle := cameraAngle - mouseDelta.x * 0.15;
    cameraPitch := Clamp(cameraPitch + mouseDelta.y * 0.15, -7.5, 80.0);

    // Movement input relative to camera
    dx := 0;
    if IsKeyDown(KEY_A) then dx := dx + 1;
    if IsKeyDown(KEY_D) then dx := dx - 1;

    dz := 0;
    if IsKeyDown(KEY_W) then dz := dz + 1;
    if IsKeyDown(KEY_S) then dz := dz - 1;

    moveInput := Vector3Zero();
    if (dx <> 0) or (dz <> 0) then
    begin
      angleRad := cameraAngle * DEG2RAD;
      right := Vector3Create(Cos(angleRad), 0.0, -Sin(angleRad));
      forwardVec := Vector3Create(Sin(angleRad), 0.0, Cos(angleRad));
      moveInput := Vector3Normalize(Vector3Add(
        Vector3Scale(right, dx),
        Vector3Scale(forwardVec, dz)
      ));
    end;

    // Check grounded
    isGrounded := R3D_IsCapsuleGroundedBox(capsule, 0.01, groundBox, nil) or
                  R3D_IsCapsuleGroundedMesh(capsule, 0.3, slopeMeshData, slopeTransform, nil);

    // Jump and apply gravity
    if isGrounded and IsKeyPressed(KEY_SPACE) then
      velocity.y := jumpForce;

    if not isGrounded then
      velocity.y := velocity.y + gravity * dt
    else if velocity.y < 0 then
      velocity.y := 0;

    // Calculate total movement
    movement := Vector3Scale(moveInput, moveSpeed * dt);
    movement.y := velocity.y * dt;

    // Apply movement with collision
    movement := R3D_SlideCapsuleMesh(capsule, movement, slopeMeshData, slopeTransform, nil);
    capsule.start := Vector3Add(capsule.start, movement);
    capsule.end_ := Vector3Add(capsule.end_, movement);

    // Ground clamp
    if capsule.start.y < 0.5 then
    begin
      correction := 0.5 - capsule.start.y;
      capsule.start.y := capsule.start.y + correction;
      capsule.end_.y := capsule.end_.y + correction;
      velocity.y := 0;
    end;

    // Update camera position
    target := GetCapsuleCenter(capsule);
    pitchRad := cameraPitch * DEG2RAD;
    angleRad := cameraAngle * DEG2RAD;

    camera.position.x := target.x - Sin(angleRad) * Cos(pitchRad) * 5.0;
    camera.position.y := target.y + Sin(pitchRad) * 5.0;
    camera.position.z := target.z - Cos(angleRad) * Cos(pitchRad) * 5.0;
    camera.target := target;

    // Rendering
    BeginDrawing();
      ClearBackground(BLACK);

      R3D_Begin(camera);
        R3D_DrawMeshPro(slopeMesh, slopeMat, slopeTransform);
        R3D_DrawMesh(groundMesh, groundMat, Vector3Zero(), 1.0);
        R3D_DrawMesh(capsMesh, R3D_MATERIAL_BASE, GetCapsuleCenter(capsule), 1.0);
      R3D_End();

      DrawFPS(10, 10);

      if isGrounded then
        DrawText('GROUNDED', 10, GetScreenHeight() - 30, 20, LIME)
      else
        DrawText('AIRBORNE', 10, GetScreenHeight() - 30, 20, YELLOW);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMeshData(slopeMeshData);
  R3D_UnloadMesh(groundMesh);
  R3D_UnloadMesh(slopeMesh);
  R3D_UnloadMesh(capsMesh);
  R3D_Close();

  CloseWindow();
end.
