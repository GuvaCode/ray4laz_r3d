 unit r3d;
(*
 * r3d header for pascal 2025 Gunko Vadim @guvacode
 * this is part of ray4laz project
 * original c lang code r3d by Le Juez Victor
 * https://github.com/Bigfoot71/r3d
 *
 * This software is provided "as-is", without any express or implied warranty. In no event
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you
 *   wrote the original software. If you use this software in a product, an acknowledgment
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 *)


{$mode objfpc}{$H+}

interface

uses
  raylib, ctypes;

const
  r3dName =
  {$IFDEF WINDOWS} 'libr3d.dll'; {$IFEND}
  {$IFDEF LINUX} 'libr3d.so'; {$IFEND}

  {$I r3d_core.inc}
  {$I r3d_culling.inc}

  {$I r3d_cubemap.inc}
  {$I r3d_ambient_map.inc}

  {$I r3d_probe.inc}
  {$I r3d_environment.inc}
  {$I r3d_lighting.inc}
  {$I r3d_material.inc}
  {$I r3d_decal.inc}
  {$I r3d_skeleton.inc}
  {$I r3d_animation.inc}
  {$I r3d_mesh_data.inc}
  {$I r3d_mesh.inc}
  {$I r3d_model.inc}

  {$I r3d_utils.inc}
  {$I r3d_instance.inc}
  {$I r3d_draw.inc}


  function R3D_CubemapSkyBase: TR3D_CubemapSky;
  function R3D_GetBaseMaterial: TR3D_Material;

implementation

function R3D_CubemapSkyBase: TR3D_CubemapSky;
begin
  Result.skyTopColor := ColorCreate(98, 116, 140, 255);
  Result.skyHorizonColor := ColorCreate(165, 167, 171, 255);
  Result.skyHorizonCurve := 0.15;
  Result.skyEnergy := 1.0;

  Result.groundBottomColor := ColorCreate(51, 43, 34, 255);
  Result.groundHorizonColor := ColorCreate(165, 167, 171, 255);
  Result.groundHorizonCurve := 0.02;
  Result.groundEnergy := 1.0;

  Result.sunDirection := Vector3Create(-1.0, -1.0, -1.0);
  Result.sunColor := WHITE;
  Result.sunSize := 1.5 * DEG2RAD;
  Result.sunCurve := 0.15;
  Result.sunEnergy := 1.0;
end;

function R3D_GetBaseMaterial: TR3D_Material;

begin
//  whiteColor := ColorCreate(255, 255, 255, 255);

  Result.albedo.color := ColorCreate(255, 255, 255, 255);;
  Result.albedo.texture := R3D_GetWhiteTexture();

  Result.emission.color := ColorCreate(255, 255, 255, 255);;
  Result.emission.texture := R3D_GetWhiteTexture();
  Result.emission.energy := 0.0;

  Result.normal.texture := R3D_GetWhiteTexture();
  Result.normal.scale := 1.0;

  Result.orm.texture := R3D_GetWhiteTexture();
  Result.orm.occlusion := 1.0;
  Result.orm.roughness := 1.0;
  Result.orm.metalness := 0.0;

  Result.transparencyMode := R3D_TRANSPARENCY_DISABLED;
  Result.billboardMode := R3D_BILLBOARD_DISABLED;
  Result.blendMode := R3D_BLEND_MIX;
  Result.cullMode := R3D_CULL_BACK;

  Result.uvOffset := Vector2Create(0.0, 0.0);
  Result.uvScale := Vector2Create(1.0, 1.0);
  Result.alphaCutoff := 0.01;

end;


end.

