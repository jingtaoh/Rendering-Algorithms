/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2021 by Wojciech Jarosz
*/
#pragma once

#include <darts/sampling.h>
#include <darts/transform.h>
#include <darts/vec.h>
#include <darts/json.h>

/*!
    A virtual (pinhole) camera.

    The camera is responsible for generating primary rays. It is positioned
    using a Transform and points along the -z axis of the local coordinate
    system. It has an image plane positioned a z = -dist with size
    (width, height).

    We currently only support pinhole perspective cameras. This class could
    be made into a virtual base class to support other types of cameras
    (e.g. an orthographic camera, or omni-directional camera).

    The camera setup looks something like this, where the
    up vector points out of the screen:

    \verbatim
            top view                         side view
               ^                    up
               |                     ^
               |                     |             _,-'
             width                   |         _,-'   |
          +----|----+     +          |     _,-'       | h
           \   |   /    d |        e | _,-'           | e
            \  |  /     i |        y +'---------------+-i----->
             \ | /      s |        e  '-,_   dist     | g
              \|/       t |               '-,_        | h
               +          +                   '-,_    | t
              eye                                 '-,_|
    \endverbatim

 */
class Camera
{
public:
    /// Construct a camera from json parameters.
    Camera(const json &j = json())
    {
        m_xform           = j.value("transform", m_xform);
        m_resolution      = j.value("resolution", m_resolution);
        m_focal_distance  = j.value("fdist", m_focal_distance);
        m_aperture_radius = j.value("aperture", m_aperture_radius);

        float vfov = j.value("vfov", 90.f);; // Default vfov value. Override this with the value from json
        // TODO: Assignment 1: read the vertical field-of-view from j ("vfov"),
        // and compute the width and height of the image plane. Remember that
        // the "vfov" parameter is specified in degrees, but C++ math functions
        // expect it in radians. You can use deg2rad() from common.h to convert
        // from one to the other
        //put_your_code_here("Assignment 1: Compute the image plane size.");
        float aspect_ratio = ((float) m_resolution.x) / m_resolution.y;
        auto theta = deg2rad(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = viewport_height * aspect_ratio;
        m_size = Vec2f(viewport_width, viewport_height);
    }

    /// Return the camera's image resolution
    Vec2i resolution() const
    {
        return m_resolution;
    }

    /*!
        Generate a ray going through image-plane location \p pixel.

        \p pixel ranges from (0,0) to (m_resolution.x, m_resolution.y) along the x- and y-axis of the rendered image,
        respectively.

        \param pixel 	        The pixel position within the image.
        \return 	            The #Ray data structure filled with the appropriate position and direction.
     */
    Ray3f generate_ray(const Vec2f &pixel) const
    {
        // TODO: Assignment 1: Implement camera ray generation
        //put_your_code_here("Assignment 1: Insert your camera ray generation code here");
        Vec2f uv(pixel.x / (m_resolution.x - 1.f), pixel.y / (m_resolution.y - 1.f));

        Vec3f origin(0, 0, 0);
        auto dir = Vec3f(1.f);
        dir.x = -m_size.x / 2.f + uv.x * m_size.x;
        dir.y =  m_size.y / 2.f - uv.y * m_size.y;
        dir.z = -m_focal_distance;
        return Ray3f(origin, dir);
    }


private:
    Transform m_xform           = Transform();     ///< Local coordinate system
    Vec2f     m_size            = Vec2f(1, 1);     ///< Physical size of the image plane
    float     m_focal_distance  = 1.f;             ///< Distance to image plane along local z axis
    Vec2i     m_resolution      = Vec2i(512, 512); ///< Image resolution
    float     m_aperture_radius = 0.f;             ///< The size of the aperture for depth of field
};

/*!
    \file
    \brief Class #Camera
*/
