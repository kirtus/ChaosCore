/*
* Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _MMAP_TERRAIN_BUILDER_H
#define _MMAP_TERRAIN_BUILDER_H

#include "PathCommon.h"
#include "Map.h"
#include "SharedDefines.h"

#include "G3D/Array.h"
#include "G3D/Vector3.h"
#include "G3D/Matrix3.h"

using namespace Trinity;

namespace Pathfinding
{
   enum Spot
   {
       TOP     = 1,
       RIGHT   = 2,
       LEFT    = 3,
       BOTTOM  = 4,
       ENTIRE  = 5
   };

   enum Grid
   {
       GRID_V8,
       GRID_V9
   };

   static const int V9_SIZE = 129;
   static const int V9_SIZE_SQ = V9_SIZE*V9_SIZE;
   static const int V8_SIZE = 128;
   static const int V8_SIZE_SQ = V8_SIZE*V8_SIZE;
   static const float GRID_SIZE = 533.33333f;
   static const float GRID_PART_SIZE = GRID_SIZE/V8_SIZE;

   // see src/Tools/extractor/system.cpp, CONF_use_minHeight 
   static const float INVALID_MAP_LIQ_HEIGHT = -500.f; 

   struct MeshData
   {
       G3D::Array<float> solidVerts;
       G3D::Array<int> solidTris;

       G3D::Array<float> liquidVerts;
       G3D::Array<int> liquidTris;
       G3D::Array<uint8> liquidType;
   };

   class TerrainBuilder
   {
       public:
           TerrainBuilder(bool skipLiquid);
           ~TerrainBuilder();

           void loadMap(uint32 mapID, uint32 tileX, uint32 tileY, MeshData &meshData);
           bool usesLiquids() { return !m_skipLiquid; }

       private:

           /// Loads a portion of a map's terrain
           bool loadMap(uint32 mapID, uint32 tileX, uint32 tileY, MeshData &meshData, Spot portion);

           /// Sets loop variables for selecting only certain parts of a map's terrain
           void getLoopVars(Spot portion, int &loopStart, int &loopEnd, int &loopInc);

           /// Controls whether liquids are loaded
           bool m_skipLiquid;

           /// Load the map terrain from file
           bool loadHeightMap(uint32 mapID, uint32 tileX, uint32 tileY, G3D::Array<float> &vertices, G3D::Array<int> &triangles, Spot portion);

           /// Get the vector coordinate for a specific position
           void getHeightCoord(int index, Grid grid, float xOffset, float yOffset, float* coord, float* v);

           /// Get the triangle's vector indices for a specific position
           void getHeightTriangle(int square, Spot triangle, int* indices, bool liquid = false);

           /// Determines if the specific position's triangles should be rendered
           bool isHole(int square, const uint16 holes[16][16]);

           /// Get the liquid vector coordinate for a specific position
           void getLiquidCoord(int index, int index2, float xOffset, float yOffset, float* coord, float* v);

           /// Get the liquid type for a specific position
           uint8 getLiquidType(int square, const uint8 liquid_type[16][16]);

           // hide parameterless and copy constructor
           TerrainBuilder();
           TerrainBuilder(const TerrainBuilder &tb);
   };
}

#endif