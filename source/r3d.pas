 unit r3d;
(*
 * r3d header for pascal 2025 Gunko Vadim @guvacode
 * this is part of ray4laz project
 * original code r3d by Le Juez Victor
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
  raylib;


const
  r3dName =
    {$IFDEF WINDOWS} 'libr3d.dll'; {$IFEND}
    {$IFDEF LINUX} 'libr3d.so'; {$IFEND}

{$I r3d_core.inc}
{$I r3d_culling.inc}
{$I r3d_curves.inc}

{$I r3d_skybox.inc}
{$I r3d_environment.inc}
{$I r3d_lighting.inc}
{$I r3d_material.inc}
{$I r3d_decal.inc}
{$I r3d_skeleton.inc}
{$I r3d_animation.inc}
{$I r3d_mesh_data.inc}
{$I r3d_mesh.inc}
{$I r3d_model.inc}
{$I r3d_particles.inc}



{$I r3d_utils.inc}
{$I r3d_draw.inc}

implementation


end.

