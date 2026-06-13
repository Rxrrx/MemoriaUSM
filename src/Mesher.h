/*
 <Mix-mesher: region type. This program generates a mixed-elements mesh>
 
 Copyright (C) <2013,2017>  <Claudio Lobos>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/gpl.txt>
 */

#ifndef Mesher_h
#define Mesher_h 1

#include "TriMesh.h"
#include "FEMesh.h"
#include "GridMesher.h"
#include "Octant.h"
#include "OctreeEdge.h"
#include "Services.h"
#include "RefinementRegion.h"
#include "RefinementCubeRegion.h"

#include "Visitors/SplitVisitor.h"
#include "Visitors/IntersectionsVisitor.h"
#include "Visitors/TransitionPatternVisitor.h"
#include "Visitors/SurfaceTemplatesVisitor.h"
#include "Visitors/RemoveSubElementsVisitor.h"
#include "Visitors/OneIrregularVisitor.h"

#include <list>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <chrono>

using std::vector;
using std::list;
using std::set;
using Clobscode::OctreeEdge;
using Clobscode::TriMesh;
using Clobscode::RefinementRegion;

namespace Clobscode
{
	
	class Mesher{
		
	public:
		
		Mesher();
		
		virtual ~Mesher();
				
		virtual FEMesh generateMesh(TriMesh &input, const unsigned short &rl,
                                    const string &name, list<RefinementRegion *> &all_reg);
		
        virtual FEMesh refineMesh(TriMesh &input, const unsigned short &rl,
                                  const string &name, list<unsigned int> &roctli,
                                  list<RefinementRegion *> &all_reg,
                                  GeometricTransform &gt, const unsigned short &minrl,
                                  const unsigned short &omaxrl);

        
        virtual void setInitialState(vector<MeshPoint> &epts, vector<Octant> &eocts,
                                     map<OctreeEdge, EdgeInfo> &edge_map);

        void setSkipBalance(bool v) { skipBalance = v; }

	protected:
        
        virtual void splitOctants(const unsigned short &rl, TriMesh &input,
                                  list<unsigned int> &roctli,
                                  list<RefinementRegion *> &all_reg, const string &name,
                                  const unsigned short &minrl, const unsigned short &omaxrl);
		
		virtual void generateOctreeMesh(const unsigned short &rl, TriMesh &input,
                                        list<RefinementRegion *> &all_reg, const string &name,
                                        const unsigned short &minrl,
                                        const unsigned short &givenmaxrl=0);

		virtual bool isItIn(TriMesh &mesh, list<unsigned int> &faces, vector<Point3D> &coords);

		virtual bool rotateGridMesh(TriMesh &input,
									list<RefinementRegion *> &all_reg,
									GeometricTransform &gt);
		
		/*virtual void generateGridFromOctree(const unsigned short &rl, 
                                              TriMesh &input,
                                              const string &name);*/
		
		virtual void generateGridMesh(TriMesh &input);
		
		virtual void linkElementsToNodes();

		virtual void detectInsideNodes(TriMesh &input);

		virtual void removeOnSurface();
		
		virtual void applySurfacePatterns(TriMesh &input);

		virtual void shrinkToBoundary(TriMesh &input);

		virtual unsigned int saveOutputMesh(FEMesh &mesh);
        
        virtual unsigned int saveOutputMesh(const shared_ptr<FEMesh> &mesh);
		
		virtual unsigned int saveOutputMesh(FEMesh &mesh, vector<MeshPoint> &points,
                                            list<Octant> &elements);
        
        virtual unsigned int saveOutputMesh(const shared_ptr<FEMesh> &mesh,
                                            vector<MeshPoint> &tmp_points,
                                            list<Octant> &tmp_octants, vector<Octant> &rest_octs);
        
        virtual unsigned int saveOutputMesh(const shared_ptr<FEMesh> &mesh,
                                            vector<MeshPoint> &tmp_points,
                                            list<Octant> &tmp_octants, list<Octant> &rest_octs);
        
        virtual void projectCloseToBoundaryNodes(TriMesh &input);

        virtual vector<vector<unsigned int>> buildPolyFaces(
            const vector<unsigned int> &pts,
            const vector<unsigned int> &mid) const;

        virtual vector<vector<unsigned int>> buildPolyFacesK(Octant& oct);

        virtual bool hasHangingNodes(Octant &o) const;

        virtual std::vector<unsigned int> getLeafDescendants(unsigned int octId) const;

        virtual void collectFacePoints(unsigned int octId, int faceIdx,
                                       std::vector<unsigned int>& pointIds,
                                       std::unordered_set<unsigned int>& seen) const;

        virtual void buildOctantPos();

	protected:

		vector<MeshPoint> points;
		vector<Octant> octants;
		map<OctreeEdge, EdgeInfo> MapEdges;
		std::unordered_map<unsigned int, std::vector<unsigned int>> childrenMap;
		std::unordered_map<unsigned int, unsigned int> octantPos;
		list<RefinementRegion *> regions;
        list<unsigned int> deb;
        bool skipBalance;
        unsigned int polyKCount;



	};
    
    /*inline void Mesher::setInitialState(vector<MeshPoint> &epts, vector<Octant> &eocts,
                                        map<OctreeEdge, EdgeInfo> &edge_map) {
        octants = eocts;
        points = epts;
        MapEdges = edge_map;
    }*/
	
	
}
#endif
