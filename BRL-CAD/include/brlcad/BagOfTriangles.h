/*                      B A G O F T R I A N G L E S . H
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
/** @file BagOfTriangles.h
 *
 *  BRL-CAD core C++ interface:
 *      bot (ID_BOT) database object declaration
 */

#ifndef BRLCAD_BAGOFTRIANGLES_INCLUDED
#define BRLCAD_BAGOFTRIANGLES_INCLUDED

#include <brlcad/Object.h>


struct rt_bot_internal;


namespace BRLCAD {
    class BRLCAD_COREINTERFACE_EXPORT BagOfTriangles : public Object {
    public:
        enum BotMode {
            Surface,
            Solid,
            Plate,
            EqualLineOfSightPlate // LOS is set equal to face thickness, not the thickness divided by the cosine of the obliquity angle
        };

        enum BotOrientation {
            Unoriented,
            ClockWise,
            CounterClockWise
        };

        BagOfTriangles(void) throw(bad_alloc);
        BagOfTriangles(const BagOfTriangles& original) throw(bad_alloc);
        virtual ~BagOfTriangles(void) throw();

        const BagOfTriangles& operator=(const BagOfTriangles& original) throw(bad_alloc);

        class BRLCAD_COREINTERFACE_EXPORT Face {
        public:
            Face(void) throw() : m_bot(0), m_faceIndex(0) {}
            Face(const Face& original) throw() : m_bot(original.m_bot), m_faceIndex(original.m_faceIndex) {}
            ~Face(void) throw() {}

            const Face& operator=(const Face& original) throw() {
                m_bot       = original.m_bot;
                m_faceIndex = original.m_faceIndex;

                return *this;
            }

            Vector3D    Point(size_t index) const throw();
            void        SetPoint(size_t          index,
                                 const Vector3D& point) throw(bad_alloc);
            void        SetPoints(const Vector3D& point1,
                                  const Vector3D& point2,
                                  const Vector3D& point3) throw(bad_alloc);

            double      Thickness(void) const throw();
            void        SetThickness(double value) throw();

            bool        ApendThickness(void) const throw();
            void        SetApendThickness(bool apendThickness) throw();

            Vector3D    Normal(size_t index) const throw();
            void        SetNormal(size_t          index,
                                  const Vector3D& normal) throw();
            void        SetNormals(const Vector3D& normal1,
                                   const Vector3D& normal2,
                                   const Vector3D& normal3) throw(bad_alloc);

                        operator void*(void) { ///< to test if the face is NULL
                return m_bot;
            }

        protected:
            Face(rt_bot_internal* original,
                 size_t           originalIndex) throw() : m_bot(original), m_faceIndex(originalIndex) {}

            friend BagOfTriangles;

        private:
            rt_bot_internal* m_bot;
            size_t           m_faceIndex;
        };

        BotMode               Mode(void) const throw();
        void                  SetMode(BotMode mode) throw(bad_alloc);

        BotOrientation        Orientation(void) const throw();
        void                  SetOrientation(BotOrientation orientation) throw();

        bool                  FacesHaveNormals(void) const throw();
        void                  SetFacesHaveNormals(bool facesHaveNormals) throw(bad_alloc);

        bool                  UseFaceNormals(void) const throw();
        void                  SetUseFaceNormals(bool useFaceNormals) throw();

        bool                  UseFloats(void) const throw();
        void                  SetUseFloats(bool useFloats) throw();

        size_t                NumberOfFaces(void) const throw();

        Face                  GetFace(size_t index) throw();
        Face                  AddFace(const Vector3D& point1,
                                      const Vector3D& point2,
                                      const Vector3D& point3) throw(bad_alloc);

        void                  DeleteFace(size_t index) throw(bad_alloc);

        // inherited from BRLCAD::Object
        virtual const Object& operator=(const Object& original) throw(bad_alloc);
        virtual Object*       Clone(void) const throw(bad_alloc, std::bad_alloc);
        static const char*    ClassName(void) throw();
        virtual const char*   Type(void) const throw();
        virtual bool          IsValid(void) const throw();

    protected:
        BagOfTriangles(resource*       resp,
                       directory*      pDir,
                       rt_db_internal* ip,
                       db_i*           dbip = 0) throw();

        friend class ConstDatabase;

    private:
        struct rt_bot_internal *m_internalp;
        const rt_bot_internal* Internal(void) const throw();
        rt_bot_internal*       Internal(void) throw();

        friend class Database;
    };
};


#endif // BRLCAD_BAGOFTRIANGLES_INCLUDED
