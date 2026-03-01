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


{$IFDEF FPC}
  {$mode objfpc}{$H+}
{$ENDIF}
{.$DEFINE USED_R3D}

interface

uses
  SysUtils, raylib, {$IFDEF FPC} ctypes, {$ENDIF} Variants, math;

{$IFDEF LINUX}
  {$DEFINE RAY_STATIC}
{$IFEND}

{$IFNDEF RAY_STATIC}
  const r3dName =
  {$IFDEF MSWINDOWS} 'libr3d.dll'; {$IFEND}
   {$IFDEF LINUX} 'libr3d.so'; {$IFEND}
 {$ENDIF}


  {$I r3d_core.inc}
  {$I r3d_screen_shader.inc}

  {$I r3d_cubemap.inc}
  {$I r3d_ambient_map.inc}
  {$I r3d_importer.inc}
  {$I r3d_probe.inc}
  {$I r3d_environment.inc}
  {$I r3d_lighting.inc}
  {$I r3d_sky_shader.inc}
  {$I r3d_sky.inc}
  {$I r3d_surface_shader.inc}
  {$I r3d_material.inc}

  {$I r3d_decal.inc}
  {$I r3d_skeleton.inc}
  {$I r3d_animation.inc}
  {$I r3d_animation_player.inc}
  {$I r3d_animation_tree.inc}
  {$I r3d_mesh_data.inc}
  {$I r3d_mesh.inc}
  {$I r3d_model.inc}
  {$I r3d_kinematics.inc}
  {$I r3d_utils.inc}
  {$I r3d_instance.inc}
  {$I r3d_draw.inc}
  {$I r3d_visibility.inc}

implementation

{$IFDEF UNIX}
  {$IFDEF RAY_STATIC}
    {$linklib c}
    {$linklib m}
    {$linklib dl}
    {$linklib pthread}
    {$linklib libr3d.a}
    {$linklib libassimp}
  {$ENDIF}
{$ENDIF}

{$I r3d_material_helpers.inc}
{$I r3d_decal_helpers.inc}
{$I r3d_sky_helpers.inc}
{$IFDEF FPC}
  {$I r3d_environment_helpers.inc}
{$ELSE}
  {$I r3d_environment_helpers_delphi.inc}
{$ENDIF}

end.

