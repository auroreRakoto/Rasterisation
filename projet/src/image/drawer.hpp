/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#ifndef DRAWER_HPP
#define DRAWER_HPP

namespace cpe
{

struct image;
class ivec2;
struct color;
struct image_zbuffer;
class vec2;
struct texture;
class vec3;

/** Draw a point with a given color and depth, check if the point should be drawn depending on its depth and zbuffer value */
void draw_point(image& im,image_zbuffer& zbuffer,ivec2 const& p,float z,color const& c);


/** Draw a line of uniform color */
void draw_line(image& im,ivec2 const& p0,ivec2 const& p1,color const& c);
/** Draw a line of varying color */
void draw_line(image& im,ivec2 const& p0,ivec2 const& p1,color const& c0,color const& c1);




/** Draw a triangle in wireframe with uniform color*/
void draw_triangle_wireframe(image& im,
                             ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,color const& c);

/** Draw a triangle filled triangle with uniform color */
void draw_triangle(image& im,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,color const& c);

/** Draw a triangle filled triangle with varying color */
void draw_triangle(image& im,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2);


/** Draw a triangle filled triangle with varying color and depth */
void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2);
void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   vec2 const& t0, vec2 const& t1, vec2 const& t2,
                   float z0,float z1,float z2,cpe::texture t);

void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2,
                   vec3 const& fragPos0, vec3 const& fragPos1, vec3 const& fragPos2,
                   vec3 const& n0, vec3 const& n1, vec3 const& n2);

void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2,
                   vec3 const& fragPos0, vec3 const& fragPos1, vec3 const& fragPos2,
                   vec3 const& n0, vec3 const& n1, vec3 const& n2,
                   vec2 const& t0, vec2 const& t1, vec2 const& t2,cpe::texture t);

void fragment_shader(color &c, vec3 const& fragPos, vec3 const& n);
}


#endif
