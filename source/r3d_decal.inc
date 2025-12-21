{*
 * @brief Represents a decal and its properties.
 *
 * This structure defines a decal that can be projected onto geometry that has already been rendered.
 *
 * @note Decals are drawn using deferred screen space rendering and do not interact with any
 * forward rendered or non-opaque objects.
 *}
type
  PR3D_Decal = ^TR3D_Decal;
  TR3D_Decal = record
    material: TR3D_Material;  ///< The material used for rendering the decal.
  end;
