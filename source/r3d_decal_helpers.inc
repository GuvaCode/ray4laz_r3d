// R3D Decal Helpers Module

function R3D_DECAL_BASE: TR3D_Decal; inline;
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
  Result.applyColor:= True;
  Result.shader := nil;
end;
