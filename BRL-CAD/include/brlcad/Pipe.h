/*                      P I P E . H
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
/** @file Pipe.h
 *
 *  BRL-CAD core C++ interface:
 *      pipe (ID_PIPE) database object declaration
 */

#ifndef BRLCAD_PIPE_INCLUDED
#define BRLCAD_PIPE_INCLUDED

#include <brlcad/Object.h>


struct rt_pipe_internal;
struct wdb_pipept;


namespace BRLCAD {
    class BRLCAD_COREINTERFACE_EXPORT Pipe : public Object {
    public:
        Pipe(void) throw(bad_alloc);
        Pipe(const Pipe& original) throw(bad_alloc);
        virtual ~Pipe(void) throw();

        const Pipe&           operator=(const Pipe& original) throw(bad_alloc);

        class BRLCAD_COREINTERFACE_EXPORT ControlPoint {
        public:
            ControlPoint(void) throw() : m_pipe(0), m_controlPoint(0) {}
            ControlPoint(const ControlPoint& original) throw() : m_pipe(original.m_pipe), m_controlPoint(original.m_controlPoint) {}
            ~ControlPoint(void) throw() {}

            const ControlPoint& operator=(const ControlPoint& original) throw() {
                m_pipe         = original.m_pipe;
                m_controlPoint = original.m_controlPoint;

                return *this;
            }

                                operator void*(void) {
                return m_pipe;
            }

            Vector3D            Point(void) const throw();
            void                SetPoint(const Vector3D& point) throw();

            double              InnerDiameter(void) const throw();
            void                SetInnerDiameter(double id) throw();

            double              OuterDiameter(void) const throw();
            void                SetOuterDiameter(double od) throw();

            double              BendRadius(void) const throw();
            void                SetBendRadius(double br) throw();

        private:
            rt_pipe_internal* m_pipe;
            wdb_pipept*       m_controlPoint;

            ControlPoint(rt_pipe_internal* pipe,
                         wdb_pipept*       controlPoint) throw() : m_pipe(pipe), m_controlPoint(controlPoint) {}

            friend class Pipe;
        };

        ControlPoint          Get(size_t index) throw();
        ControlPoint          AppendControlPoint(const Vector3D& point,
                                                 double          innerDiameter,
                                                 double          outerDiameter,
                                                 double          bendRadius) throw(bad_alloc);
        ControlPoint          InsertControlPoint(size_t          index,
                                                 const Vector3D& point,
                                                 double          innerDiameter,
                                                 double          outerDiameter,
                                                 double          bendRadius) throw(bad_alloc);
        void                  DeleteControlPoint(size_t index) throw();

        // inherited from BRLCAD::Object
        virtual const Object& operator=(const Object& original) throw(bad_alloc);
        virtual Object*       Clone(void) const throw(bad_alloc, std::bad_alloc);
        static const char*    ClassName(void) throw();
        virtual const char*   Type(void) const throw();
        virtual bool          IsValid(void) const throw();

    protected:
        Pipe(resource*       resp,
             directory*      pDir,
             rt_db_internal* ip,
             db_i*           dbip = 0) throw();

        friend class ConstDatabase;

    private:
        rt_pipe_internal* m_internalp;
        
        const rt_pipe_internal* Internal(void) const throw();
        rt_pipe_internal*       Internal(void) throw();

        friend class Database;
    };
};


#endif // BRLCAD_PIPE_INCLUDED
