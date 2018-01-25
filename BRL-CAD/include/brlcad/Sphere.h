/*                      S P H E R E . H
 * BRL-CAD
 *
 * Copyright (c) 2014 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @file Sphere.h
 *
 *  BRL-CAD core C++ interface:
 *      sphere (ID_SPH) database object declaration
 */

#ifndef BRLCAD_SPHERE_INCLUDED
#define BRLCAD_SPHERE_INCLUDED

#include <brlcad/Object.h>


struct rt_ell_internal;


namespace BRLCAD {
    class BRLCAD_COREINTERFACE_EXPORT Sphere : public Object {
    public:
        Sphere(void) throw(bad_alloc);
        Sphere(const  Vector3D& center,
               double radius)throw(bad_alloc);
        Sphere(const Sphere& original) throw(bad_alloc);
        virtual ~Sphere(void) throw();

        const Sphere&         operator=(const Sphere& original) throw(bad_alloc);

        Vector3D              Center(void) const throw();
        void                  SetCenter(const Vector3D& normal) throw();
        double                Radius(void) const throw();
        void                  SetRadius(double distance) throw();
        void                  Set(const Vector3D& center,
                                  double          radius) throw();

        // inherited from BRLCAD::Object
        virtual const Object& operator=(const Object& original) throw(bad_alloc);
        virtual Object*       Clone(void) const throw(bad_alloc, std::bad_alloc);
        static const char*    ClassName(void) throw();
        virtual const char*   Type(void) const throw();
        virtual bool          IsValid(void) const throw();

    protected:
        Sphere(resource*       resp,
               directory*      pDir,
               rt_db_internal* ip,
               db_i*           dbip = 0) throw();

        friend class ConstDatabase;

    private:
        // holds Objects's content if not connected to a database
        rt_ell_internal* m_internalp;

        const rt_ell_internal* Internal(void) const throw();
        rt_ell_internal*       Internal(void) throw();

        friend class Database;
    };
}


#endif // BRLCAD_SPHERE_INCLUDED
