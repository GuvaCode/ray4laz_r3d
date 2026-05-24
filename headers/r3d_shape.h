/* r3d_shape.h -- R3D Shape Module.
 *
 * Copyright (c) 2025-2026 Le Juez Victor
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * For conditions of distribution and use, see accompanying LICENSE file.
 */

#ifndef R3D_SHAPE_H
#define R3D_SHAPE_H

#include "./r3d_mesh_data.h"
#include "./r3d_model.h"

/**
 * @defgroup Shape
 * @brief Collision shape types and read-only queries (construction, intersection, penetration, closest points).
 *
 * Functions that modify a shape's state belong to the Kinematics module.
 * @{
 */

// ========================================
// STRUCTS TYPES
// ========================================

/**
 * @brief Alias for raylib's BoundingBox (min/max corner points).
 */
typedef BoundingBox R3D_BoundingBox;

/**
 * @brief Oriented bounding box (OBB).
 *
 * Defined by a center point, three orthogonal axes, and half-extents along each axis.
 */
typedef struct {
    Vector3 center;
    Vector3 axisX;
    Vector3 axisY;
    Vector3 axisZ;
    Vector3 halfExtents;
} R3D_OrientedBox;

/**
 * @brief Capsule shape defined by two endpoints and radius
 */
typedef struct R3D_Capsule {
    Vector3 start;      ///< Start point of capsule axis
    Vector3 end;        ///< End point of capsule axis
    float radius;       ///< Capsule radius
} R3D_Capsule;

/**
 * @brief Penetration information from an overlap test
 */
typedef struct R3D_Penetration {
    bool collides;      ///< Whether shapes are overlapping
    float depth;        ///< Penetration depth
    Vector3 normal;     ///< Collision normal (direction to resolve penetration)
    Vector3 mtv;        ///< Minimum Translation Vector (normal * depth)
} R3D_Penetration;

// ========================================
// PUBLIC API
// ========================================

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Compute an axis-aligned bounding box from a center and half-extents.
 */
R3DAPI R3D_BoundingBox R3D_GetBoundingBox(Vector3 center, Vector3 halfExtents);

/**
 * @brief Compute an oriented bounding box from an AABB and transform.
 */
R3DAPI R3D_OrientedBox R3D_GetOrientedBox(R3D_BoundingBox aabb, Matrix transform);

/**
 * @brief Check if two axis-aligned bounding boxes intersect
 * @param box1 First bounding box
 * @param box2 Second bounding box
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionBoundingBoxes(R3D_BoundingBox box1, R3D_BoundingBox box2);

/**
 * @brief Check if an axis-aligned bounding box intersects a sphere
 * @param box Bounding box
 * @param center Sphere center
 * @param radius Sphere radius
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionBoundingBoxSphere(R3D_BoundingBox box, Vector3 center, float radius);

/**
 * @brief Check if two oriented bounding boxes intersect
 * @param box1 First oriented box
 * @param box2 Second oriented box
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionOrientedBoxes(R3D_OrientedBox box1, R3D_OrientedBox box2);

/**
 * @brief Check if an oriented bounding box intersects a sphere
 * @param box Oriented bounding box
 * @param center Sphere center
 * @param radius Sphere radius
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionOrientedBoxSphere(R3D_OrientedBox box, Vector3 center, float radius);

/**
 * @brief Check if two spheres intersect
 * @param center1 First sphere center
 * @param radius1 First sphere radius
 * @param center2 Second sphere center
 * @param radius2 Second sphere radius
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionSpheres(Vector3 center1, float radius1, Vector3 center2, float radius2);

/**
 * @brief Check if capsule intersects with bounding box
 * @param capsule Capsule shape
 * @param box Bounding box
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionCapsuleBoundingBox(R3D_Capsule capsule, R3D_BoundingBox box);

/**
 * @brief Check if capsule intersects with oriented box
 * @param capsule Capsule shape
 * @param box Oriented box
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionCapsuleOrientedBox(R3D_Capsule capsule, R3D_OrientedBox box);

/**
 * @brief Check if capsule intersects with sphere
 * @param capsule Capsule shape
 * @param center Sphere center
 * @param radius Sphere radius
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionCapsuleSphere(R3D_Capsule capsule, Vector3 center, float radius);

/**
 * @brief Check if two capsules intersect
 * @param a First capsule
 * @param b Second capsule
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionCapsules(R3D_Capsule a, R3D_Capsule b);

/**
 * @brief Check if capsule intersects with mesh
 * @param capsule Capsule shape
 * @param mesh Mesh data
 * @param transform Mesh transform
 * @return true if collision detected
 */
R3DAPI bool R3D_CheckCollisionCapsuleMesh(R3D_Capsule capsule, R3D_MeshData mesh, Matrix transform);

/**
 * @brief Check penetration between two axis-aligned bounding boxes
 * @param box1 First bounding box
 * @param box2 Second bounding box
 * @return Penetration information
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationBoundingBoxes(R3D_BoundingBox box1, R3D_BoundingBox box2);

/**
 * @brief Check penetration between an axis-aligned bounding box and a sphere
 * @param box Bounding box
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Penetration information
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationBoundingBoxSphere(R3D_BoundingBox box, Vector3 center, float radius);

/**
 * @brief Check penetration between two oriented bounding boxes
 * @param box1 First oriented box
 * @param box2 Second oriented box
 * @return Penetration information
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationOrientedBoxes(R3D_OrientedBox box1, R3D_OrientedBox box2);

/**
 * @brief Check penetration between an oriented bounding box and a sphere
 * @param box Oriented bounding box
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Penetration information
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationOrientedBoxSphere(R3D_OrientedBox box, Vector3 center, float radius);

/**
 * @brief Check penetration between two spheres
 * @param center1 First sphere center
 * @param radius1 First sphere radius
 * @param center2 Second sphere center
 * @param radius2 Second sphere radius
 * @return Penetration information
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationSpheres(Vector3 center1, float radius1, Vector3 center2, float radius2);

/**
 * @brief Check penetration between a capsule and a bounding box
 * @param capsule Capsule shape
 * @param box Bounding box
 * @return Penetration information
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationCapsuleBoundingBox(R3D_Capsule capsule, R3D_BoundingBox box);

/**
 * @brief Check penetration between a capsule and an oriented bounding box
 * @param capsule Capsule shape
 * @param box Oriented bounding box
 * @return Penetration information
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationCapsuleOrientedBox(R3D_Capsule capsule, R3D_OrientedBox box);

/**
 * @brief Check penetration between capsule and sphere
 * @param capsule Capsule shape
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Penetration information.
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationCapsuleSphere(R3D_Capsule capsule, Vector3 center, float radius);

/**
 * @brief Check penetration between two capsules
 * @param a First capsule
 * @param b Second capsule
 * @return Penetration information.
 */
R3DAPI R3D_Penetration R3D_CheckPenetrationCapsules(R3D_Capsule a, R3D_Capsule b);

/**
 * @brief Cast a ray against a triangle
 * @param ray Ray to cast
 * @param p1 First triangle vertex
 * @param p2 Second triangle vertex
 * @param p3 Third triangle vertex
 * @return Ray collision info (hit, distance, point, normal)
 */
R3DAPI RayCollision R3D_RaycastTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3);

/**
 * @brief Cast a ray against a quad
 * @param ray Ray to cast
 * @param p1 First quad vertex
 * @param p2 Second quad vertex
 * @param p3 Third quad vertex
 * @param p4 Fourth quad vertex
 * @note The quad must be strictly planar and non-self-intersecting
 * @return Ray collision info (hit, distance, point, normal)
 */
R3DAPI RayCollision R3D_RaycastQuad(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4);

/**
 * @brief Cast a ray against an axis-aligned bounding box
 * @param ray Ray to cast
 * @param box Bounding box to test against
 * @return Ray collision info (hit, distance, point, normal)
 */
R3DAPI RayCollision R3D_RaycastBoundingBox(Ray ray, R3D_BoundingBox box);

/**
 * @brief Cast a ray against an oriented bounding box
 * @param ray Ray to cast
 * @param box Oriented bounding box to test against
 * @return Ray collision info (hit, distance, point, normal)
 */
R3DAPI RayCollision R3D_RaycastOrientedBox(Ray ray, R3D_OrientedBox box);

/**
 * @brief Cast a ray against a sphere
 * @param ray Ray to cast
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Ray collision info (hit, distance, point, normal)
 */
R3DAPI RayCollision R3D_RaycastSphere(Ray ray, Vector3 center, float radius);

/**
 * @brief Cast a ray against a capsule
 * @param ray Ray to cast
 * @param capsule Capsule shape to test against
 * @return Ray collision info (hit, distance, point, normal)
 */
R3DAPI RayCollision R3D_RaycastCapsule(Ray ray, R3D_Capsule capsule);

/**
 * @brief Cast a ray against mesh geometry
 * @param ray Ray to cast
 * @param mesh Mesh data to test against
 * @param transform Mesh world transform
 * @return Ray collision info (hit, distance, point, normal)
 */
R3DAPI RayCollision R3D_RaycastMesh(Ray ray, R3D_MeshData mesh, Matrix transform);

/**
 * @brief Cast a ray against a model (tests all meshes)
 * @param ray Ray to cast
 * @param model Model to test against (must have valid meshData)
 * @param transform Model world transform
 * @return Ray collision info for closest hit (hit=false if no meshData)
 */
R3DAPI RayCollision R3D_RaycastModel(Ray ray, R3D_Model model, Matrix transform);

/**
 * @brief Find closest point on line segment to given point
 * @param point Query point
 * @param start Segment start
 * @param end Segment end
 * @return Closest point on segment [start, end]
 */
R3DAPI Vector3 R3D_ClosestPointOnSegment(Vector3 point, Vector3 start, Vector3 end);

/**
 * @brief Find closest point on triangle to given point
 * @param p Query point
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @return Closest point on triangle surface
 */
R3DAPI Vector3 R3D_ClosestPointOnTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c);

/**
 * @brief Find closest point on box surface to given point
 * @param point Query point
 * @param box Bounding box
 * @return Closest point on/in box (clamped to box bounds)
 */
R3DAPI Vector3 R3D_ClosestPointOnBox(Vector3 point, R3D_BoundingBox box);

#ifdef __cplusplus
} // extern "C"
#endif

/** @} */ // end of Shape

#endif // R3D_SHAPE_H
