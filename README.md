# R3D - 3D Rendering Library for raylib

<img align="left" src="https://github.com/Bigfoot71/r3d/blob/master/logo.png" width="100" hspace="20">
<br>
R3D is a modern 3D rendering library for <a href="https://www.raylib.com/">raylib</a> that provides advanced lighting, shadows, materials, and post-processing effects without the complexity of building a full engine from scratch.
<br clear="left">


## Key Features

- **Hybrid Renderer**: Automatic (or manual) deferred/forward rendering
- **Advanced Materials**: Complete PBR material system (Burley/SchlickGGX)
- **Dynamic Lighting**: Directional, spot, and omni lights with soft shadows
- **Post-Processing**: SSAO, SSR, DoF, bloom, fog, tonemapping, and more
- **Model Loading**: Assimp integration with animations and mesh generation
- **Performance**: Built-in frustum culling, instanced rendering, and more

## Installation

```
Open and complile package/ray4laz_r3d.lpk
```

**Requirements package**: ray4laz 

## Quick Start

```pascal
program R3DExample;

{$mode objfpc}{$H+}

uses
  raylib, r3d;  

const
  SCREEN_WIDTH = 800;   // Window width in pixels
  SCREEN_HEIGHT = 600;  // Window height in pixels

var
  Mesh: TR3D_Mesh;           // 3D mesh object (sphere)
  Material: TR3D_Material;   // Material properties for the mesh
  Light: TR3D_Light;         // Directional light source
  Camera: TCamera3D;         // 3D camera for viewing the scene
  ModelRotation: Single = 0.0; // Current rotation angle of the model in degrees

begin
  // Initialize Raylib window and set target FPS
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 'R3D Example');
  SetTargetFPS(60);  // Target 60 frames per second
  
  // Initialize R3D rendering engine with screen resolution and no special flags
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  
  // Configure environment settings
  R3D_SetBackgroundColor(BLACK);  // Set clear color to black
  R3D_SetAmbientColor(ColorCreate(20, 20, 20, 255));  // Set ambient light to dark gray

  try
    // === Create 3D objects ===
    
    // Generate a sphere mesh with radius 1.0, 16 rings, and 32 slices
    Mesh := R3D_GenMeshSphere(1.0, 16, 32);
    
    // Get default material and set its base color to red
    Material := R3D_GetDefaultMaterial();
    Material.albedo.color := RED;
    
    // === Setup lighting ===
    
    // Create a directional light (simulates sunlight)
    Light := R3D_CreateLight(R3D_LIGHT_DIR);
    
    // Set light direction (coming from top-left-back)
    R3D_SetLightDirection(Light, Vector3Create(-1, -1, -1));
    
    // Set light color to white
    R3D_SetLightColor(Light, WHITE);
    
    // Enable shadows with 2048x2048 resolution shadow map
    R3D_EnableShadow(Light, 2048);
    
    // Activate the light
    R3D_SetLightActive(Light, True);
    
    // === Setup camera ===
    
    // Position camera at (-3, 3, 3) looking at origin (0, 0, 0)
    Camera.position := Vector3Create(-3, 3, 3);
    Camera.target := Vector3Create(0, 0, 0);
    Camera.up := Vector3Create(0, 1, 0);  // Y-axis is up
    Camera.fovy := 60.0;  // Field of view in degrees
    Camera.projection := CAMERA_PERSPECTIVE;  // Perspective projection

    // === Main rendering loop ===
    while not WindowShouldClose() do  // Continue until window close requested
    begin
      // Update rotation angle (1 degree per frame at 60 FPS = 60 degrees per second)
      ModelRotation := ModelRotation + 1.0;
      
      // Begin drawing frame
      BeginDrawing();
        // Clear screen with black color
        ClearBackground(BLACK);
        
        // Begin R3D rendering with our camera
        R3D_Begin(Camera);
          // Draw the mesh with material, rotated around Y-axis
          R3D_DrawMesh(@Mesh, @Material, 
            MatrixRotateY(ModelRotation * DEG2RAD));  // Convert degrees to radians
        R3D_End();  // End R3D rendering (performs all lighting, shadow, post-processing passes)
        
        // Draw 2D overlay text using standard Raylib functions
        DrawText('R3D Basic Example', 10, 10, 20, LIME);
        DrawFPS(10, 40);  // Display frames per second counter
      EndDrawing();  // End drawing frame (swap buffers)
    end;

  finally
    // === Cleanup resources (always executed, even if exception occurs) ===
    
    // Unload mesh data from GPU memory
    R3D_UnloadMesh(@Mesh);
    
    // Unload material and associated textures
    R3D_UnloadMaterial(@Material);
    
    // Shutdown R3D rendering engine
    R3D_Close();
    
    // Close Raylib window
    CloseWindow();
  end;
end.
```

## License

Licensed under the **MIT License** - see [LICENSE](LICENSE) for details.

## Screenshots

![](screenshots/sponza.webp)
![](screenshots/pbr.webp)
![](screenshots/skybox.webp)
