/*            N O N M A N I F O L D G E O M E T R Y . H
 * BRL-CAD
 *
 * Copyright (c) 2011 United States Government as represented by
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
/** @file NonManifoldGeometry.h
 *
 *  BRL-CAD core C++ interface:
 *      non-manifold geometry (ID_NMG) database object declaration
 *
 *  Origin -
 *      IABG mbH (Germany)
 */

#ifndef BRLCAD_NONMANIFOLDGEOMETRY_INCLUDED
#define BRLCAD_NONMANIFOLDGEOMETRY_INCLUDED

#include <brlcad/Object.h>


struct vertexuse;
struct edgeuse;
struct loopuse;
struct faceuse;
struct shell;
struct nmgregion;
struct model;


namespace BRLCAD {
    class BRLCAD_COREINTERFACE_EXPORT NonManifoldGeometry : public Object {
    public:
        NonManifoldGeometry(void) throw(bad_alloc);
        NonManifoldGeometry(const NonManifoldGeometry& original) throw(bad_alloc);
        virtual ~NonManifoldGeometry(void) throw();

        const NonManifoldGeometry& operator=(const NonManifoldGeometry& original) throw(bad_alloc);

        // the classes for the subobjects are all non-changing
        // otherwise we would need every subclass in two kinds: constant and mutable
        // the manipulation routines are all in the main class NonManifoldGeometry
        class Edge;
        class Loop;
        class Face;
        class Shell;
        class Region;
        class RegionIterator;

        class BRLCAD_COREINTERFACE_EXPORT Vertex {
        public:
            Vertex(const Vertex& original) throw() : m_vertex(original.m_vertex) {}

            ~Vertex(void) throw() {}

            const Vertex& operator=(const Vertex& original) throw() {
                m_vertex = original.m_vertex;

                return *this;
            }

                          operator const void*(void) const throw() {
                return m_vertex;
            }

            // here we go
            Vector3D      Point(void) const throw();

        private:
            const vertexuse* m_vertex;

            friend class Edge;
            friend class Shell;

            Vertex(void) throw() : m_vertex(0) {}
        };


        class BRLCAD_COREINTERFACE_EXPORT Edge {
        public:
            Edge(const Edge& original) throw() : m_edge(original.m_edge) {}

            ~Edge(void) throw() {}

            const Edge& operator=(const Edge& original) throw() {
                m_edge = original.m_edge;

                return *this;
            }

                        operator const void*(void) const throw() {
                return m_edge;
            }

            // here we go
            Vertex      ForwardVertex(void) const throw();
            Vertex      BackwardVertex(void) const throw();

        private:
            const edgeuse* m_edge;

            friend class Loop;
            friend class Shell;

            Edge(void) throw() : m_edge(0) {}
        };


        class BRLCAD_COREINTERFACE_EXPORT Loop {
        public:
            Loop(const Loop& original) throw() : m_loop(original.m_loop), m_currentEdge(original.m_currentEdge) {}

            ~Loop(void) throw() {}

            const Loop& operator=(const Loop& original) throw() {
                m_loop        = original.m_loop;
                m_currentEdge = original.m_currentEdge;

                return *this;
            }

                        operator const void*(void) const throw() {
                return m_loop;
            }

            // here we go
            bool        IsHole(void) const throw();

            // it's an iterator
            void        GotoFirstEdge(void) throw();
            const Loop& operator++(void) throw();
            Edge        CurrentEdge(void) const throw();

        private:
            const loopuse* m_loop;
            const edgeuse* m_currentEdge;

            friend class Face;
            friend class Shell;

            Loop(void) throw() : m_loop(0), m_currentEdge(0) {}
        };


        class BRLCAD_COREINTERFACE_EXPORT Face {
        public:
            Face(const Face& original) throw() : m_face(original.m_face), m_currentLoop(original.m_currentLoop) {}

            ~Face(void) throw() {}

            const Face& operator=(const Face& original) throw() {
                m_face        = original.m_face;
                m_currentLoop = original.m_currentLoop;

                return *this;
            }

                        operator const void*(void) const throw() {
                return m_face;
            }

            // it's an iterator
            void        GotoFirstLoop(void) throw();
            const Face& operator++(void) throw();
            Loop        CurrentLoop(void) const throw();

        private:
            const faceuse* m_face;
            const loopuse* m_currentLoop;

            friend class Shell;
            friend class NonManifoldGeometry;

            Face(void) throw() : m_face(0), m_currentLoop(0) {}
        };


        class BRLCAD_COREINTERFACE_EXPORT Shell {
        public:
            Shell(const Shell& original) throw() : m_shell(original.m_shell),
                                                   m_currentFace(original.m_currentFace),
                                                   m_currentLoop(original.m_currentLoop),
                                                   m_currentEdge(original.m_currentEdge),
                                                   m_currentVertex(original.m_currentVertex) {}

            ~Shell(void) throw() {}

            const Shell& operator=(const Shell& original) throw() {
                m_shell         = original.m_shell;
                m_currentFace   = original.m_currentFace;
                m_currentLoop   = original.m_currentLoop;
                m_currentEdge   = original.m_currentEdge;
                m_currentVertex = original.m_currentVertex;

                return *this;
            }

                        operator const void*(void) const throw() {
                return m_shell;
            }

            // in fact it's a multiple iterator
            void         GotoFirstFace(void) throw();
            const Shell& GotoNextFace(void) throw();
            Face         CurrentFace(void) const throw();

            void         GotoFirstLoop(void) throw();
            const Shell& GotoNextLoop(void) throw();
            Loop         CurrentLoop(void) const throw();

            void         GotoFirstEdge(void) throw();
            const Shell& GotoNextEdge(void) throw();
            Edge         CurrentEdge(void) const throw();

            void         GotoFirstVertex(void) throw();
            const Shell& GotoNextVertex(void) throw();
            Vertex       CurrentVertex(void) const throw();

        private:
            const shell*     m_shell;
            const faceuse*   m_currentFace;
            const loopuse*   m_currentLoop;
            const edgeuse*   m_currentEdge;
            const vertexuse* m_currentVertex;

            friend class Region;
            friend class NonManifoldGeometry;

            Shell(void) throw() : m_shell(0), m_currentFace(0), m_currentLoop(0), m_currentEdge(0), m_currentVertex(0) {}
        };


        class BRLCAD_COREINTERFACE_EXPORT Region {
        public:
            Region(const Region& original) throw() : m_region(original.m_region), m_currentShell(original.m_currentShell) {}

            ~Region(void) throw() {}

            const Region& operator=(const Region& original) throw() {
                m_region       = original.m_region;
                m_currentShell = original.m_currentShell;

                return *this;
            }

                        operator const void*(void) const throw() {
                return m_region;
            }

            // it's an iterator
            void          GotoFirstShell(void) throw();
            const Region& operator++(void) throw();
            Shell         CurrentShell(void) const throw();

        private:
            const nmgregion* m_region;
            const shell*     m_currentShell;

            friend class RegionIterator;

            Region(void) throw() : m_region(0), m_currentShell(0) {}
        };


        // decuples the iteration over the regions from NonManifoldGeometry
        // i.e. a const NonManifoldGeometry can be used to iterate over the structure
        class BRLCAD_COREINTERFACE_EXPORT RegionIterator {
        public:
            RegionIterator(const RegionIterator& original) throw() : m_model(original.m_model), m_currentRegion(original.m_currentRegion) {}

            ~RegionIterator(void) throw() {}

            const RegionIterator& operator=(const RegionIterator& original) throw() {
                m_model         = original.m_model;
                m_currentRegion = original.m_currentRegion;

                return *this;
            }

            // it's an iterator
            void                  GotoFirstRegion(void) throw();
            const RegionIterator& operator++(void) throw();
            Region                CurrentRegion(void) const throw();

        private:
            const model*     m_model;
            const nmgregion* m_currentRegion;

            friend class NonManifoldGeometry;

            RegionIterator(void) throw() : m_model(0), m_currentRegion(0) {}
        };


        // Basic Operations
        /*
        Region       MakeRegion(void) throw(); // 0

        Shell        MakeShell(Region, Vector3D& firstVertex) throw(bad_alloc); // nmg_msv(region)

        /// creates a new edge by connecting an existing vertex and reusing onother one
        Edge         MakeEdgeVertex(Vetrex& vertexAlreadyInShell,
                                    Vetex&  vertexToAdd) throw(bad_alloc); // mmg_me(vertex, vertex, shell)
        /// creates a new edge by connecting an existing vertex with a new one
        Edge         MakeEdgeVertex(Vetrex& vertexAlreadyInShell,
                                    Vector3D&  vertexToAdd) throw(bad_alloc); // mmg_me(vertex, vertex, shell)

        /// creates a loop existing of one vertex by reusing one
        Loop         MakeVertexLoop(Vetex& vertexToAdd) throw(bad_alloc);
        /// creates a loop existing of one vertex by adding a new one
        Loop         MakeVertexLoop(Vector3D& vertexToAdd) throw(bad_alloc);

        /// creates a loop by connecting two vertices
        Edge         MakeEdgeLoop(Vetrex& firstVertexToConnect,
                                  Vertex& secondVertexToConnect) throw(bad_alloc);

        /// creates a face by using a loop as its boundary
        /// after this the loop isn't more a 1D subset of the manifold
        Face   MakeFaceKillLoop(Loop& loopToConvert) throw(bad_alloc);
        */

        // Extended Operations
        /*
        Vertex       SplitEdgeMakeVertex(EdgeUse&  edgeToSplit,
                                         Vetex&    vertexToInsert) throw(bad_alloc);
        Vertex       SplitEdgeMakeVertex(EdgeUse&  edgeToSplit,
                                         Vector3D& vertexToInsert) throw(bad_alloc);

        /// creates a new face from an existing one by dividing it with an additional edge
        Edge         MakeEdgeFace(Vetrex& firstVertexToConnect,
                              Vertex& secondVertexToConnect) throw(bad_alloc);
        /// connects two faces to form a new common one by removing a common edge
        Face         KillEdgeFace(Edge& edgeToRemove) throw();

        /// creates a new loop from an existing one by removing a "double-edge"
        Loop         KillEdgeMakeLoop(Edge& edgeToRemove) throw();
        /// joins two loops by creating an edge between a point of each loop
        Loop         MakeEdgeKillLoop(Vetrex& firstVertexToConnect,
                                      Vertex& secondVertexToConnect) throw(bad_alloc);
        */
        void         Triangulate(void) throw(bad_alloc);
        void         Triangulate(Shell& shellToTrinagulate) throw(bad_alloc);
        void         Triangulate(Face& faceToTrinagulate) throw(bad_alloc);

        // Destruction Operations
        /*
        void         KillRegion(Region& regionToRemove) throw();
        void         KillShell(Shell& shellToRemove) throw();

        /// if the vertex is unconnected: the objects (vertex, loop with itself, ...) are removed
        /// if the vertex is connected with one other by an edge: the edge well be removed too
        /// if the vertex is connected with two othe vertices by edges: these two other vertices will be connected by an edge
        Edge         KillVertex(Vetrex& vertexToRemove) throw();

        /// in a loop: opens the loop
        /// if the edge is between two faces: the faces  wil be connected to form a new common one
        Face         KillEdge(Edge& edgeToRemove) throw();

        void         KillLoop(Loop* loopToRemove) throw();

        /// makes a loop from the face and removes the face
        Loop         KillFaceMakeLoop(Face& faceToConvert) throw();
        */

        // iterate over the regions
        RegionIterator             Regions(void) const throw();

        // inherited from BRLCAD::Object
        virtual const Object&      operator=(const Object& original) throw(bad_alloc);
        virtual Object*            Clone(void) const throw(bad_alloc, std::bad_alloc);
        static const char*         ClassName(void) throw();
        virtual const char*        Type(void) const throw();
        virtual bool               IsValid(void) const throw();

    protected:
        NonManifoldGeometry(resource*       resp,
                            directory*      pDir,
                            rt_db_internal* ip,
                            db_i*           dbip = 0) throw();

        friend class ConstDatabase;

    private:
        // holds Objects's content if not connected to a database
        model* m_internalp;

        const model* Internal(void) const throw();
        model*       Internal(void) throw();

        friend class Database;
    };
}


#endif // BRLCAD_NONMANIFOLDGEOMETRY_INCLUDED
