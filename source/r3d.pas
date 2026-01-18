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
  SysUtils, raylib, ctypes, Variants;

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
  {$I r3d_animation_player.inc}
  {$I r3d_mesh_data.inc}
  {$I r3d_mesh.inc}
  {$I r3d_model.inc}

  {$I r3d_utils.inc}
  {$I r3d_instance.inc}
  {$I r3d_draw.inc}


  function R3D_CubemapSkyBase: TR3D_CubemapSky;
  function R3D_GetBaseMaterial: TR3D_Material;
  function R3D_GetEnvironment_base: TR3D_Environment;
  function R3D_DECAL_BASE: TR3D_Decal;

  // Перегруженные версии для разных типов
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Single); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Integer); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Boolean); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TColor); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Cubemap); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_AmbientMap); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TQuaternion); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Bloom); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Fog); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_DoF); overload;
  procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Tonemap); overload;

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

function R3D_GetEnvironment_base: TR3D_Environment;
begin
  Result := Default(TR3D_Environment);

  with Result do
  begin
    // Background
    with background do
    begin
      color := GRAY;
      energy := 1.0;
      skyBlur := 0.0;
      sky := Default(TR3D_Cubemap);
      rotation := QuaternionCreate(0.0, 0.0, 0.0, 1.0);
    end;

    // Ambient
    with ambient do
    begin
      color := BLACK;
      energy := 1.0;
      map := Default(TR3D_AmbientMap);
    end;

    // SSAO
    with ssao do
    begin
      sampleCount := 16;
      intensity := 1.0;
      power := 1.5;
      radius := 0.35;
      bias := 0.007;
      enabled := False;
    end;

    // SSIL
    with ssil do
    begin
      sampleCount := 4;
      sliceCount := 4;
      sampleRadius := 2.0;
      hitThickness := 0.5;
      aoPower := 1.0;
      energy := 1.0;
      bounce := 0.5;
      convergence := 0.5;
      enabled := False;
    end;

    // Bloom
    with bloom do
    begin
      mode := R3D_BLOOM_DISABLED;
      levels := 0.5;
      intensity := 0.05;
      threshold := 0.0;
      softThreshold := 0.5;
      filterRadius := 1.0;
    end;

    // SSR
    with ssr do
    begin
      maxRaySteps := 64;
      binarySearchSteps := 8;
      rayMarchLength := 8.0;
      depthThickness := 0.2;
      depthTolerance := 0.005;
      edgeFadeStart := 0.7;
      edgeFadeEnd := 1.0;
      enabled := False;
    end;

    // Fog
    with fog do
    begin
      mode := R3D_FOG_DISABLED;
      color := ColorCreate(255, 255, 255, 255);
      start := 1.0;
      end_ := 50.0;
      density := 0.05;
      skyAffect := 0.5;
    end;

    // DOF
    with dof do
    begin
      mode := R3D_DOF_DISABLED;
      focusPoint := 10.0;
      focusScale := 1.0;
      maxBlurSize := 20.0;
      debugMode := False;
    end;

    // Tonemap
    with tonemap do
    begin
      mode := R3D_TONEMAP_LINEAR;
      exposure := 1.0;
      white := 1.0;
    end;

    // Color
    with color do
    begin
      brightness := 1.0;
      contrast := 1.0;
      saturation := 1.0;
    end;
  end;
end;

function R3D_DECAL_BASE: TR3D_Decal;
begin
  Result.albedo.texture := Default(TTexture);
  Result.albedo.color := WHITE;

  Result.emission.texture := Default(TTexture);
  Result.emission.color := WHITE;
  Result.emission.energy := 0.0;

  Result.normal.texture := Default(TTexture);
  Result.normal.scale := 1.0;

  Result.orm.texture := Default(TTexture);
  Result.orm.occlusion := 1.0;
  Result.orm.roughness := 1.0;
  Result.orm.metalness := 0.0;

  Result.uvOffset := Vector2Create(0.0, 0.0);
  Result.uvScale := Vector2Create(1.0, 1.0);
  Result.alphaCutoff := 0.01;
  Result.normalThreshold := 0;
  Result.fadeWidth := 0;
end;

// Реализация для Single (числа с плавающей точкой)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Single);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Background
    'background.energy': Env^.background.energy := Value;
    'background.skyblur': Env^.background.skyBlur := Value;

    // Ambient
    'ambient.energy': Env^.ambient.energy := Value;

    // SSAO
    'ssao.intensity': Env^.ssao.intensity := Value;
    'ssao.power': Env^.ssao.power := Value;
    'ssao.radius': Env^.ssao.radius := Value;
    'ssao.bias': Env^.ssao.bias := Value;

    // SSIL
    'ssil.sampleradius': Env^.ssil.sampleRadius := Value;
    'ssil.hitthickness': Env^.ssil.hitThickness := Value;
    'ssil.aopower': Env^.ssil.aoPower := Value;
    'ssil.energy': Env^.ssil.energy := Value;
    'ssil.convergence': Env^.ssil.convergence := Value;
    'ssil.bounce': Env^.ssil.bounce := Value;

    // Bloom
    'bloom.levels': Env^.bloom.levels := Value;
    'bloom.intensity': Env^.bloom.intensity := Value;
    'bloom.threshold': Env^.bloom.threshold := Value;
    'bloom.softthreshold': Env^.bloom.softThreshold := Value;
    'bloom.filterradius': Env^.bloom.filterRadius := Value;

    // SSR
    'ssr.raymarchlength': Env^.ssr.rayMarchLength := Value;
    'ssr.depththickness': Env^.ssr.depthThickness := Value;
    'ssr.depthtolerance': Env^.ssr.depthTolerance := Value;
    'ssr.edgefadestart': Env^.ssr.edgeFadeStart := Value;
    'ssr.edgefadeend': Env^.ssr.edgeFadeEnd := Value;

    // Fog
    'fog.start': Env^.fog.start := Value;
    'fog.end': Env^.fog.end_ := Value;
    'fog.density': Env^.fog.density := Value;
    'fog.skyaffect': Env^.fog.skyAffect := Value;

    // Depth of Field
    'dof.focuspoint': Env^.dof.focusPoint := Value;
    'dof.focusscale': Env^.dof.focusScale := Value;
    'dof.maxblursize': Env^.dof.maxBlurSize := Value;

    // Tonemap
    'tonemap.exposure': Env^.tonemap.exposure := Value;
    'tonemap.white': Env^.tonemap.white := Value;

    // Color Grading
    'color.brightness': Env^.color.brightness := Value;
    'color.contrast': Env^.color.contrast := Value;
    'color.saturation': Env^.color.saturation := Value;

  else
    raise Exception.CreateFmt('Unknown Single field or wrong type: %s', [Path]);
  end;
end;

// Реализация для Integer
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Integer);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // SSAO
    'ssao.samplecount': Env^.ssao.sampleCount := Value;

    // SSIL
    'ssil.samplecount': Env^.ssil.sampleCount := Value;
    'ssil.slicecount': Env^.ssil.sliceCount := Value;

    // SSR
    'ssr.maxraysteps': Env^.ssr.maxRaySteps := Value;
    'ssr.binarysearchsteps': Env^.ssr.binarySearchSteps := Value;

  else
    raise Exception.CreateFmt('Unknown Integer field or wrong type: %s', [Path]);
  end;
end;

// Реализация для Boolean
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Boolean);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // SSAO
    'ssao.enabled': Env^.ssao.enabled := Value;

    // SSIL
    'ssil.enabled': Env^.ssil.enabled := Value;

    // SSR
    'ssr.enabled': Env^.ssr.enabled := Value;

    // Depth of Field
    'dof.debugmode': Env^.dof.debugMode := Value;

  else
    raise Exception.CreateFmt('Unknown Boolean field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TColor
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TColor);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Background
    'background.color': Env^.background.color := Value;

    // Ambient
    'ambient.color': Env^.ambient.color := Value;

    // Fog
    'fog.color': Env^.fog.color := Value;

  else
    raise Exception.CreateFmt('Unknown TColor field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_Cubemap
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Cubemap);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Background
    'background.sky': Env^.background.sky := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Cubemap field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_AmbientMap
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_AmbientMap);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Ambient
    'ambient.map': Env^.ambient.map := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_AmbientMap field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TQuaternion
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TQuaternion);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Background
    'background.rotation': Env^.background.rotation := Value;

  else
    raise Exception.CreateFmt('Unknown TQuaternion field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_Bloom (перечисление)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Bloom);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Bloom
    'bloom.mode': Env^.bloom.mode := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Bloom field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_Fog (перечисление)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Fog);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Fog
    'fog.mode': Env^.fog.mode := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Fog field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_DoF (перечисление)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_DoF);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Depth of Field
    'dof.mode': Env^.dof.mode := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_DoF field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_Tonemap (перечисление)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Tonemap);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Tonemap
    'tonemap.mode': Env^.tonemap.mode := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Tonemap field or wrong type: %s', [Path]);
  end;
end;



end.

