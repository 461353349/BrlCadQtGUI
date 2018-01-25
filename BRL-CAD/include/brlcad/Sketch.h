/*                      S K E T C H. H
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
/** @file Sketch.h
 *
 *  BRL-CAD core C++ interface:
 *      sketch (ID_SKETCH) database object declaration
 */

#ifndef BRLCAD_SKETCH_INCLUDED
#define BRLCAD_SKETCH_INCLUDED

#include <brlcad/Object.h>


struct rt_sketch_internal;
struct line_seg;
struct carc_seg;
struct nurb_seg;
struct bezier_seg;


namespace BRLCAD {
    class BRLCAD_COREINTERFACE_EXPORT Sketch : public Object {
    public:
        Sketch(void) throw(bad_alloc);
        Sketch(const Sketch& original) throw(bad_alloc);
        virtual ~Sketch(void) throw();

        const Sketch&           operator=(const Sketch& original) throw(bad_alloc);

        class BRLCAD_COREINTERFACE_EXPORT Segment {
        public:
            enum SegmentType {
                Null,
                Line,
                CircularArc,
                Nurb,
                Bezier
            };

            virtual ~Segment(void) throw() {}

            void                Destroy(void) throw();
            virtual Segment*    Clone(void) const throw(bad_alloc, std::bad_alloc) = 0;
            virtual SegmentType Type(void) const throw()                           = 0;

            virtual Vector2D    StartPoint(void) const throw()                     = 0;
            virtual Vector2D    EndPoint(void) const throw()                       = 0;

            bool                Reverse(void) const throw();
            void                SetReverse(bool reverse) throw();

        protected:
            rt_sketch_internal* m_sketch;

            Segment(void) throw() : m_sketch(0)  {}
            Segment(const Segment& original) throw() : m_sketch(original.m_sketch) {}
            Segment(rt_sketch_internal* sketch) throw() : m_sketch(sketch) {}

            const Segment& operator=(const Segment& original) throw() {return *this;}
        };

        class BRLCAD_COREINTERFACE_EXPORT Line : public Segment {
        public:
            Line(void) throw() : Segment(), m_lineSegment(0) {}
            Line(const Line& original) throw() : Segment(original), m_lineSegment(original.m_lineSegment) {}
            virtual ~Line(void) throw() {}

            const Line&         operator=(const Line& original) throw() {
                Segment::operator=(original);
                m_lineSegment = original.m_lineSegment;
                return *this;
            }

            virtual SegmentType Type(void) const throw();
            virtual Segment*    Clone(void) const throw(bad_alloc, std::bad_alloc);

            virtual Vector2D    StartPoint(void) const throw();
            void                SetStartPoint(const Vector2D& startPoint) throw(bad_alloc);
            virtual Vector2D    EndPoint(void) const throw();
            void                SetEndPoint(const Vector2D& endPoint) throw(bad_alloc);

        private:
            line_seg* m_lineSegment;

            Line(line_seg*           lineSegment,
                 rt_sketch_internal* sketch) throw() : Segment(sketch), m_lineSegment(lineSegment) {}
            friend class Sketch;
        };

        class BRLCAD_COREINTERFACE_EXPORT CircularArc : public Segment {
        public:
            CircularArc(void) throw() : Segment(), m_circularArcSegment(0) {}
            CircularArc(const CircularArc& original) throw() : Segment(original), m_circularArcSegment(original.m_circularArcSegment) {}
            virtual ~CircularArc(void) throw() {}

            const CircularArc&  operator=(const CircularArc& original) throw() {
                Segment::operator=(original);
                m_circularArcSegment = original.m_circularArcSegment;
                return *this;
            }

            virtual SegmentType Type(void) const throw();
            virtual Segment*    Clone(void) const throw(bad_alloc, std::bad_alloc);

            virtual Vector2D    StartPoint(void) const throw();
            void                SetStartPoint(const Vector2D& startPoint) throw(bad_alloc);
            virtual Vector2D    EndPoint(void) const throw();
            void                SetEndPoint(const Vector2D& endPoint) throw(bad_alloc);

            Vector3D            Center(void) const throw();
            void                SetCenter(Vector2D c) throw();
            double              Radius(void) const throw();
            void                SetRadius(double radius) throw();
            bool                CenterIsLeft(void) const throw();
            void                SetCenterIsLeft(bool centerIsLeft) throw();
            bool                ClockwiseOriented(void) const throw();
            void                SetClockwiseOriented(bool clockwiseOriented) throw();

        private:
            carc_seg* m_circularArcSegment;

            CircularArc(carc_seg*           circularArcSegment,
                        rt_sketch_internal* sketch) throw() : Segment(sketch), m_circularArcSegment(circularArcSegment) {}
            friend class Sketch;
        };

        class BRLCAD_COREINTERFACE_EXPORT Nurb : public Segment {
        public:
            Nurb(void) throw() : Segment(), m_nurbSegment(0) {}
            Nurb(const Nurb& original) throw() : Segment(original), m_nurbSegment(original.m_nurbSegment) {}
            virtual ~Nurb(void) throw() {}

            const Nurb&         operator=(const Nurb& original) throw() {
                Segment::operator=(original);
                m_nurbSegment = original.m_nurbSegment;
                return *this;
            }

            virtual SegmentType Type(void) const throw();
            virtual Segment*    Clone(void) const throw(bad_alloc, std::bad_alloc);

            virtual Vector2D    StartPoint(void) const throw();
            void                SetStartPoint(const Vector2D& startPoint) throw(bad_alloc);
            virtual Vector2D    EndPoint(void) const throw();
            void                SetEndPoint(const Vector2D& endPoint) throw(bad_alloc);

            size_t              Order(void) const throw();
            bool                IsRational(void) const throw();
            size_t              NumberOfKnots(void) const throw();
            double              Knot(size_t index) const throw();
            size_t              NumberOfControlPoints(void) const throw();
            Vector2D            ControlPoint(size_t index) const throw();
            double              ControlPointWeight(size_t index) const throw();

            void                SetOrder(size_t order) throw();
            void                AddKnot(double knot) throw();
            void                AddControlPoint(const Vector2D& Point) throw();
            void                AddControlPointWeight(const Vector2D& Point, double weight) throw();

        private:
            nurb_seg* m_nurbSegment;

            Nurb(nurb_seg*           nurbSegment,
                 rt_sketch_internal* sketch) throw() : Segment(sketch), m_nurbSegment(nurbSegment) {}

            friend class Sketch;
        };

        class BRLCAD_COREINTERFACE_EXPORT Bezier : public Segment {
        public:
            Bezier(void) throw() : Segment(), m_bezierSegment(0) {}
            Bezier(const Bezier& original) throw() : Segment(original), m_bezierSegment(original.m_bezierSegment) {}
            virtual ~Bezier(void) throw() {}

            const Bezier&       operator=(const Bezier& original) throw() {
                Segment::operator=(original);
                m_bezierSegment = original.m_bezierSegment;
                return *this;
            }

            virtual SegmentType Type(void) const throw();
            virtual Segment*    Clone(void) const throw(bad_alloc, std::bad_alloc);

            virtual Vector2D    StartPoint(void) const throw();
            void                SetStartPoint(const Vector2D& startPoint) throw(bad_alloc);
            virtual Vector2D    EndPoint(void) const throw();
            void                SetEndPoint(const Vector2D& endPoint) throw(bad_alloc);

            size_t              Degree(void) const throw();
            Vector2D            ControlPoint(size_t index) const throw();
            void                AddControlPoint(const Vector2D& Point) throw();

        private:
            bezier_seg* m_bezierSegment;

            Bezier(bezier_seg*         bezierSegment,
                   rt_sketch_internal* sketch) throw() : Segment(sketch), m_bezierSegment(bezierSegment) {}

            friend class Sketch;
        };

        class ConstSegmentCallback {
        public:
            virtual ~ConstSegmentCallback(void) throw() {}

            /// the user has to implement this method to evaluate the object
            virtual void operator()(const Segment& segment) = 0;

        protected:
            ConstSegmentCallback(void) throw() {}
            ConstSegmentCallback(const ConstSegmentCallback&) throw() {}
            const ConstSegmentCallback& operator=(const ConstSegmentCallback&) throw() {return *this;}
        };

        class SegmentCallback {
        public:
            virtual ~SegmentCallback(void) throw() {}

            /// the user has to implement this method to evaluate the object
            virtual void operator()(Segment& segment) = 0;

        private:
            SegmentCallback(void) throw() {}
            SegmentCallback(const SegmentCallback&) throw() {}
            const SegmentCallback& operator=(const SegmentCallback&) throw() {return *this;}
        };

        int                   NumberOfSegments(void) const throw();

        /// selects a single object and hand it over to an SegmentCallback
        void                  Get(size_t                index,
                                  ConstSegmentCallback& callback) const throw();
        void                  Get(size_t           index,
                                  SegmentCallback& callback) throw();

        /// overloaded member function, provided for convenience: selects a single segment and and returns it
        /** Do not forget to BRLCAD::Sketch::Segment::Destroy() the copy when you are finished with it! */
        Segment*              Get(size_t index) const throw(bad_alloc, std::bad_alloc);

        Line*                 AppendLine(void) throw(bad_alloc, std::bad_alloc);
        Line*                 InsertLine(size_t index) throw(bad_alloc, std::bad_alloc);
        CircularArc*          AppendArc(void) throw(bad_alloc, std::bad_alloc);
        CircularArc*          InsertArc(size_t index) throw(bad_alloc, std::bad_alloc);
        Nurb*                 AppendNurb(void) throw(bad_alloc, std::bad_alloc);
        Nurb*                 InsertNurb(size_t index) throw(bad_alloc, std::bad_alloc);
        Bezier*               AppendBezier(void) throw(bad_alloc, std::bad_alloc);
        Bezier*               InsertBezier(size_t index) throw(bad_alloc, std::bad_alloc);

        void                  DeleteSegment(size_t index) throw(bad_alloc);

        Vector3D              EmbeddingPlaneX(void) const;
        Vector3D              EmbeddingPlaneY(void) const;
        void                  SetEmbeddingPlaneX(Vector3D& u);
        void                  SetEmbeddingPlaneY(Vector3D& v);
        Vector3D              EmbeddingPlaneOrigin(void) const;
        void                  SetEmbeddingPlaneOrigin(Vector3D& point);

        // inherited from BRLCAD::Object
        virtual const Object& operator=(const Object& original) throw(bad_alloc);
        virtual Object*       Clone(void) const throw(bad_alloc, std::bad_alloc);
        static const char*    ClassName(void) throw();
        virtual const char*   Type(void) const throw();
        virtual bool          IsValid(void) const throw();

    protected:
        Sketch(resource*       resp,
               directory*      pDir,
               rt_db_internal* ip,
               db_i*           dbip = 0) throw();

        friend class ConstDatabase;

    private:
        // holds Objects's content if not connected to a database
        rt_sketch_internal* m_internalp;

        const rt_sketch_internal* Internal(void) const throw();
        rt_sketch_internal*       Internal(void) throw();

        friend class Database;
    };
};


#endif // BRLCAD_SKETCH_INCLUDED
