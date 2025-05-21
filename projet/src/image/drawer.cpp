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


#include "drawer.hpp"

#include "discrete/ivec2.hpp"
#include "discrete/line_discrete.hpp"
#include "discrete/line_interpolation_parameter.hpp"
#include "discrete/bresenham.hpp"
#include "discrete/triangle_scanline.hpp"
#include "image.hpp"
#include "image_zbuffer.hpp"
#include "color.hpp"
#include "lib/3d/vec2.hpp"
#include "lib/3d/vec3.hpp"
#include "texture.hpp"
#include <cmath>
namespace cpe
{


void draw_line(image& im,ivec2 const& p0,ivec2 const& p1,color const& c)
{
    /*************************************
    // TO DO
    /*************************************
    // Affichez une ligne sur l'image
    //
    // Completez une structure line_discrete a l'aide de
    //   l'algorithme de Bresenham entre p0 et p1.
    // Pour tous les pixel p du segment
    //   Affecter im(p) a la couleur c
    //
    */
    line_discrete line;
    bresenham(p0,p1,line);
    std::cout<<line<<std::endl;
    for(ivec2 p : line)
        im(p) = c;
}

void draw_line(image& im,ivec2 const& p0,ivec2 const& p1,color const& c0,color const& c1)
{

    /*************************************
    // TO DO
    /*************************************
    // Affichez une ligne de couleur variable entre p0 de couleur c0, et p1 de couleur c1
    //
    // Completez une structure line_discrete a l'aide de
    //   l'algorithme de Bresenham entre p0 et p1.
    // Construire une structure line_interpolation_parameter
    //   a partir de la ligne discrete
    //
    // Pour toute les pixels de la ligne a l'indice k
    //    p <- position a l'indice k
    //    alpha <- parametre d'interpolation a l'indice k
    //    Calculer couleur courante en fonction de alpha, c0, et c1
    //    Appliquer la couleur courante a im(p)
    //
    */
    line_discrete line;
    bresenham(p0,p1,line);
    line_interpolation_parameter interpolation(line);

    color alpha;
    for(int i=0;i<interpolation.size();i++)
    {
        alpha=color(interpolation[i]);
        im(line[i])=color((color(1)-alpha)*c0+alpha*c1);
    }
}



void draw_triangle(image& im,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,color const& c)
{


    /*************************************
    // TO DO
    /*************************************
    // Affichez un triangle plein de couleur uniforme
    //
    // Definir une structure scanline sur (p0,p1,p2).
    //   le parametere peut etre definit arbitrairement.
    //   (par exemple triangle_scanline_factory(p0,p1,p2 , c,c,c); )
    //
    // Pour toutes les valeurs y du scanline
    //   pa <- position a gauche
    //   pb <- position a droite
    //   Afficher segment entre pa et pb de couleur c
    //
    */

    auto scanline=triangle_scanline_factory(p0,p1,p2,c,c,c);

    auto it=scanline.begin();
    auto it_end=scanline.end();
    for(;it!=it_end;++it)
    {
        int const x=it->first;

        auto const& left=it->second.left;
        auto const& right=it->second.right;

        ivec2 const& p_left=left.coordinate;
        ivec2 const& p_right=right.coordinate;

        draw_line(im,p_left,p_right,c);
    }
}

void draw_triangle(image& im,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2)
{

    /*************************************
    // TO DO
    /*************************************
    // Affichez un triangle plein de couleur uniforme
    //
    // Definir une structure scanline sur (p0,p1,p2).
    //   de parametre (c0,c1,c2)
    //
    // Pour toutes les valeurs y du scanline
    //   pa <- position a gauche
    //   pb <- position a droite
    //
    //   ca <- couleur a gauche
    //   cb <- couleur a droite
    //
    //   Afficher segment entre pa et pb
    //        avec couleur variant entre ca et cb
    //
    */

    auto scanline=triangle_scanline_factory(p0,p1,p2,c0,c1,c2);

    auto it=scanline.begin();
    auto it_end=scanline.end();
    for(;it!=it_end;++it)
    {
        int const x=it->first;

        auto const& left=it->second.left;
        auto const& right=it->second.right;

        ivec2 const& p_left=left.coordinate;
        ivec2 const& p_right=right.coordinate;

        color const& param_left=left.parameter;
        color const& param_right=right.parameter;

        draw_line(im,p_left,p_right,param_left,param_right);
    }
}


void draw_point(image& im,image_zbuffer& zbuffer,ivec2 const& p,float const z,color const& c)
{
    if(p.x()<0 || p.x()>=im.Nx())
        return ;
    if(p.y()<0 || p.y()>=im.Ny())
        return ;

    /*************************************
    // TO DO
    /*************************************
    // Si z est compris dans [-1,1] et que z
    //   est plus faible que zbuffer(p)
    //  Alors mettre a jour la couleur et le zbuffer
    */

    if (zbuffer(p)>z && abs(z)<1)
        {
            im(p)=c;
            zbuffer(p)=z;
        }
    std::cout<<std::endl;
}





void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2)
{


    /*************************************
    // TO DO
    /*************************************
    // Construire scanline avec couleur
    // Construire scanline avec profondeur
    // Parcourir les deux scanline (utilisez les iterateurs)
    //   pa <- point gauche
    //   pb <- point droit
    //
    //   ca <- couleur a gauche
    //   cb <- couleur a droite
    //
    //   za <- profondeur a gauche
    //   zb <- profondeur a droite
    //
    //   affiche ligne entre pa et pb
    //     de couleur variant entre ca et cb
    //     de pronfondeur variant entre za et zb
    */

    auto scanline=triangle_scanline_factory(p0,p1,p2,c0,c1,c2);
    auto scanline_z = triangle_scanline_factory(p0,p1,p2,z0,z1,z2);
    auto it=scanline.begin();
    auto it_z=scanline_z.begin();
    auto it_end=scanline.end();
    for(;it!=it_end;++it,++it_z)
    {
        int const x=it->first;

        //couleur
        auto const& left=it->second.left;
        auto const& right=it->second.right;

        ivec2 const& p_left=left.coordinate;
        ivec2 const& p_right=right.coordinate;

        color const& param_left=left.parameter;
        color const& param_right=right.parameter;

        //z
        auto const& left_z=it_z->second.left;
        auto const& right_z=it_z->second.right;

        ivec2 const& p_left_z=left_z.coordinate;
        ivec2 const& p_right_z=right_z.coordinate;

        float const& param_left_z=left_z.parameter;
        float const& param_right_z=right_z.parameter;
        line_discrete line;
        bresenham(p_left,p_right,line);
        line_interpolation_parameter interpolation(line);

        color alpha;
        color c;
        float z;
        for(int i=0;i<interpolation.size();i++)
        {
            alpha=color(interpolation[i]);
            c=color((color(1)-alpha)*param_left+alpha*param_right);
            z=(1-interpolation[i])*param_left_z+interpolation[i]*param_right_z;
            draw_point(im,zbuffer,line[i],z,c);
        }
    }

}

/** Draw a triangle in wireframe with uniform color*/
void draw_triangle_wireframe(image& im,
                             ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,color const& c)
{
    draw_line(im,p0,p1,c);
    draw_line(im,p1,p2,c);
    draw_line(im,p2,p0,c);
}

void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   vec2 const& t0, vec2 const& t1, vec2 const& t2,
                   float z0,float z1,float z2,cpe::texture t)
{

    //t.load("data/dino.ppm");


    auto scanline=triangle_scanline_factory(p0,p1,p2,c0,c1,c2);
    auto scanline_z = triangle_scanline_factory(p0,p1,p2,z0,z1,z2);
    auto scanline_t = triangle_scanline_factory(p0,p1,p2,t0,t1,t2);

    auto it=scanline.begin();
    auto it_z=scanline_z.begin();
    auto it_end=scanline.end();

    //texture
    auto it_t=scanline_t.begin();
    for(;it!=it_end;++it,++it_z,++it_t)
    {
        int const x=it->first;

        //couleur
        auto const& left=it->second.left;
        auto const& right=it->second.right;

        ivec2 const& p_left=left.coordinate;
        ivec2 const& p_right=right.coordinate;

        color const& param_left=left.parameter;
        color const& param_right=right.parameter;

        //z
        auto const& left_z=it_z->second.left;
        auto const& right_z=it_z->second.right;

        ivec2 const& p_left_z=left_z.coordinate;
        ivec2 const& p_right_z=right_z.coordinate;

        float const& param_left_z=left_z.parameter;
        float const& param_right_z=right_z.parameter;

        //texture
        auto const& left_t=it_t->second.left;
        auto const& right_t=it_t->second.right;

        ivec2 const& p_left_t=left_t.coordinate;
        ivec2 const& p_right_t=right_t.coordinate;

        vec2 const& param_left_t=left_t.parameter;
        vec2 const& param_right_t=right_t.parameter;


        line_discrete line;
        bresenham(p_left,p_right,line);
        line_interpolation_parameter interpolation(line);

        color alpha;
        color c;
        float z;
        float coeff;
        vec2 tcoord;
        for(int i=0;i<interpolation.size();i++)
        {
            alpha=color(interpolation[i]);
            c=color((color(1)-alpha)*param_left+alpha*param_right);
            z=(1-interpolation[i])*param_left_z+interpolation[i]*param_right_z;
            coeff = interpolation[i];
            tcoord=(1-coeff)*param_left_t+coeff*param_right_t;
            color text = t(tcoord.x(),tcoord.y());
            c = c*text;

            //float tcoord_x_corrected = ((1 - interpolation[i]) * param_left_t.x() + interpolation[i] * param_right_t.x())/z;
            //float tcoord_y_corrected = ((1 - interpolation[i]) * param_left_t.y() + interpolation[i] * param_right_t.y())/z;
            // Ajustez les coordonnées de texture interpolées pour être dans la plage [0, 1]


            //color text = t(tcoord_x_corrected,tcoord_y_corrected);
            //c = text;

            draw_point(im,zbuffer,line[i],z,c);
        }
    }
}



void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2,
                   vec3 const& fragPos0, vec3 const& fragPos1, vec3 const& fragPos2,
                   vec3 const& n0, vec3 const& n1, vec3 const& n2)
{
    auto scanline=triangle_scanline_factory(p0,p1,p2,c0,c1,c2);
    auto scanline_z = triangle_scanline_factory(p0,p1,p2,z0,z1,z2);
    auto it=scanline.begin();
    auto it_z=scanline_z.begin();
    auto it_end=scanline.end();

    //interpolation de la fragmentPos
    auto scanline_fragPos=triangle_scanline_factory(p0,p1,p2,fragPos0,fragPos1,fragPos2);
    auto it_fragPos=scanline_fragPos.begin();
    //interpolation de la normale
    auto scanline_n = triangle_scanline_factory(p0,p1,p2,n0,n1,n2);
    auto it_n=scanline_n.begin();


    for(;it!=it_end;++it,++it_n,++it_fragPos,++it_z)
    {
        int const x=it->first;

        //couleur
        auto const& left=it->second.left;
        auto const& right=it->second.right;

        ivec2 const& p_left=left.coordinate;
        ivec2 const& p_right=right.coordinate;

        color const& param_left=left.parameter;
        color const& param_right=right.parameter;

        //z
        auto const& left_z=it_z->second.left;
        auto const& right_z=it_z->second.right;

        ivec2 const& p_left_z=left_z.coordinate;
        ivec2 const& p_right_z=right_z.coordinate;

        float const& param_left_z=left_z.parameter;
        float const& param_right_z=right_z.parameter;

        //frag pos
        auto const& left_fragPos=it_fragPos->second.left;
        auto const& right_fragPos=it_fragPos->second.right;

        ivec2 const& p_left_fragPos=left_fragPos.coordinate;
        ivec2 const& p_right_fragPos=right_fragPos.coordinate;

        vec3 const& param_left_fragPos=left_fragPos.parameter;
        vec3 const& param_right_fragPos=right_fragPos.parameter;

        //normal
        auto const& left_n=it_n->second.left;
        auto const& right_n=it_n->second.right;

        ivec2 const& p_left_n=left_n.coordinate;
        ivec2 const& p_right_n=right_n.coordinate;

        vec3 const& param_left_n=left_n.parameter;
        vec3 const& param_right_n=right_n.parameter;

        //hkbj
        line_discrete line;
        bresenham(p_left,p_right,line);
        line_interpolation_parameter interpolation(line);

        color alpha;
        color c;
        float z;
        vec3 n;
        vec3 fragPos;
        float coeff;
        for(int i=0;i<interpolation.size();i++)
        {
            alpha=color(interpolation[i]);
            c=color((color(1)-alpha)*param_left+alpha*param_right);


            //fragment shader part
            coeff = interpolation[i];
            n=normalized((1-coeff)*param_left_n+coeff*param_right_n);
            fragPos = (1-coeff)*param_left_fragPos+coeff*param_right_fragPos;

            fragment_shader(c,fragPos,n);

            //
            z=(1-interpolation[i])*param_left_z+interpolation[i]*param_right_z;
            draw_point(im,zbuffer,line[i],z,c);
        }
    }
}


void draw_triangle(image& im,image_zbuffer& zbuffer,
                   ivec2 const& p0,ivec2 const& p1,ivec2 const& p2,
                   color const& c0,color const& c1,color const& c2,
                   float z0,float z1,float z2,
                   vec3 const& fragPos0, vec3 const& fragPos1, vec3 const& fragPos2,
                   vec3 const& n0, vec3 const& n1, vec3 const& n2,
                   vec2 const& t0, vec2 const& t1, vec2 const& t2, cpe::texture t)
{
    auto scanline=triangle_scanline_factory(p0,p1,p2,c0,c1,c2);
    auto scanline_z = triangle_scanline_factory(p0,p1,p2,z0,z1,z2);
    auto it=scanline.begin();
    auto it_z=scanline_z.begin();
    auto it_end=scanline.end();

    //interpolation de la fragmentPos
    auto scanline_fragPos=triangle_scanline_factory(p0,p1,p2,fragPos0,fragPos1,fragPos2);
    auto it_fragPos=scanline_fragPos.begin();
    //interpolation de la normale
    auto scanline_n = triangle_scanline_factory(p0,p1,p2,n0,n1,n2);
    auto it_n=scanline_n.begin();

    //texture
    auto scanline_t = triangle_scanline_factory(p0,p1,p2,t0,t1,t2);
    auto it_t=scanline_t.begin();

    for(;it!=it_end;++it,++it_n,++it_fragPos,++it_z,++it_t)
    {
        int const x=it->first;

        //couleur
        auto const& left=it->second.left;
        auto const& right=it->second.right;

        ivec2 const& p_left=left.coordinate;
        ivec2 const& p_right=right.coordinate;

        color const& param_left=left.parameter;
        color const& param_right=right.parameter;

        //z
        auto const& left_z=it_z->second.left;
        auto const& right_z=it_z->second.right;

        ivec2 const& p_left_z=left_z.coordinate;
        ivec2 const& p_right_z=right_z.coordinate;

        float const& param_left_z=left_z.parameter;
        float const& param_right_z=right_z.parameter;

        //frag pos
        auto const& left_fragPos=it_fragPos->second.left;
        auto const& right_fragPos=it_fragPos->second.right;

        ivec2 const& p_left_fragPos=left_fragPos.coordinate;
        ivec2 const& p_right_fragPos=right_fragPos.coordinate;

        vec3 const& param_left_fragPos=left_fragPos.parameter;
        vec3 const& param_right_fragPos=right_fragPos.parameter;

        //normal
        auto const& left_n=it_n->second.left;
        auto const& right_n=it_n->second.right;

        ivec2 const& p_left_n=left_n.coordinate;
        ivec2 const& p_right_n=right_n.coordinate;

        vec3 const& param_left_n=left_n.parameter;
        vec3 const& param_right_n=right_n.parameter;

        //texture
        auto const& left_t=it_t->second.left;
        auto const& right_t=it_t->second.right;

        ivec2 const& p_left_t=left_t.coordinate;
        ivec2 const& p_right_t=right_t.coordinate;

        vec2 const& param_left_t=left_t.parameter;
        vec2 const& param_right_t=right_t.parameter;

        //hkbj
        line_discrete line;
        bresenham(p_left,p_right,line);
        line_interpolation_parameter interpolation(line);

        color alpha;
        color c;
        float z;
        vec3 n;
        vec3 fragPos;
        vec2 tcoord;
        float coeff;
        for(int i=0;i<interpolation.size();i++)
        {
            alpha=color(interpolation[i]);
            c=color((color(1)-alpha)*param_left+alpha*param_right);


            //fragment shader part
            coeff = interpolation[i];
            n=normalized((1-coeff)*param_left_n+coeff*param_right_n);
            fragPos = (1-coeff)*param_left_fragPos+coeff*param_right_fragPos;

            tcoord=(1-coeff)*param_left_t+coeff*param_right_t;
            color text = t(tcoord.x(),tcoord.y());
            c = text;

            fragment_shader(c,fragPos,n);

            //
            z=(1-interpolation[i])*param_left_z+interpolation[i]*param_right_z;

//            float tcoord_x_corrected = ((1 - interpolation[i]) * param_left_t.x() / z + interpolation[i] * param_right_t.x() / z);
//            float tcoord_y_corrected = ((1 - interpolation[i]) * param_left_t.y() / z + interpolation[i] * param_right_t.y() / z);
//            color text = t(tcoord_x_corrected,tcoord_y_corrected);
//            c = text;

            fragment_shader(c,fragPos,n);

            draw_point(im,zbuffer,line[i],z,c);
        }
    }
}

void fragment_shader(color &c, vec3 const& fragPos, vec3 const& n)
{
    float ka = 0.2;
    float kd = 0.8;
    float ks = 0.6;
    int es = 128;
    //ambiant
    color ambiant = color(ka);

    //diffuse
    //vec3 light_direction = normalized(p_proj - vec3(0.2, 0.3, -1.0));
    //vec3 light_direction = normalized(p_proj - modelview * view * projection*vec3(0, 0, -2.0));
    vec3 light_direction = normalized(fragPos - vec3(0, 0, -2.0));
    //vec3 light_direction = normalized(p_proj - vec3(-1.5,0.3,1.0));
    float diffuse = kd*((dot(n, light_direction) > 0) ? dot(n, light_direction) : 0.0f);

    //specular
    vec3 reflection_direction = reflected(light_direction, n); // Direction du reflet
    float specular = ks * pow((dot(reflection_direction, n) > 0) ? dot(reflection_direction, n) : 0, es);
     // Couleur de la lumière spéculaire

    color tot = ambiant + diffuse;
    c=tot * c + specular*color(1); //a modifier
}
}
