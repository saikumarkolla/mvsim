/*+-------------------------------------------------------------------------+
  |                       MultiVehicle simulator (libmvsim)                 |
  |                                                                         |
  | Copyright (C) 2014  Jose Luis Blanco Claraco (University of Almeria)    |
  | Distributed under GNU General Public License version 3                  |
  |   See <http://www.gnu.org/licenses/>                                    |
  +-------------------------------------------------------------------------+  */

#pragma once

#include <mvsim/WorldElements/WorldElementBase.h>
#include <mrpt/slam/COccupancyGridMap2D.h>
#include <mrpt/slam/CSinCosLookUpTableFor2DScans.h>
#include <mrpt/opengl/CSetOfObjects.h>
#include <mrpt/opengl/CPointCloud.h>
#include <mrpt/synch/CCriticalSection.h>
#include <mrpt/poses/CPose2D.h>
#include <mrpt/slam/CObservation2DRangeScan.h>

namespace mvsim
{
	class OccupancyGridMap : public WorldElementBase
	{
		DECLARES_REGISTER_WORLD_ELEMENT(OccupancyGridMap)
	public:

		OccupancyGridMap(World*parent,const rapidxml::xml_node<char> *root);
		virtual ~OccupancyGridMap();

		virtual void loadConfigFrom(const rapidxml::xml_node<char> *root) ; //!< See docs in base class
		virtual void gui_update( mrpt::opengl::COpenGLScene &scene); //!< See docs in base class

		virtual void simul_pre_timestep(const TSimulContext &context); //!< See docs in base class

		const mrpt::slam::COccupancyGridMap2D & getOccGrid() const { return m_grid; }
		mrpt::slam::COccupancyGridMap2D & getOccGrid() { return m_grid; }

	protected:
		mrpt::slam::COccupancyGridMap2D  m_grid;

		bool m_gui_uptodate; //!< Whether m_gl_grid has to be updated upon next call of gui_update()
		mrpt::opengl::CSetOfObjectsPtr m_gl_grid;

		struct TFixturePtr 
		{
			b2Fixture* fixture;
			TFixturePtr() : fixture(NULL) {}
		};

		struct TInfoPerVeh
		{
			mrpt::poses::CPose2D pose;
			mrpt::slam::CObservation2DRangeScanPtr scan;
			b2Body* collide_body;
			std::vector<TFixturePtr> collide_fixtures;

			TInfoPerVeh() : collide_body(NULL) {}
		};

		std::vector<TInfoPerVeh>  m_obstacles_for_each_veh; 
		std::vector<mrpt::opengl::CSetOfObjectsPtr> m_gl_obs_clouds;
		
		mrpt::synch::CCriticalSection m_gl_obs_clouds_buffer_cs;
		std::vector<mrpt::opengl::CPointCloudPtr> m_gl_obs_clouds_buffer;

		mrpt::slam::CSinCosLookUpTableFor2DScans m_sincos_lut;

		bool m_show_grid_collision_points;

	};
}
