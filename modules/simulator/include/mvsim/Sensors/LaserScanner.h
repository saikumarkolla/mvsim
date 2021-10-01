/*+-------------------------------------------------------------------------+
  |                       MultiVehicle simulator (libmvsim)                 |
  |                                                                         |
  | Copyright (C) 2014-2020  Jose Luis Blanco Claraco                       |
  | Copyright (C) 2017  Borys Tymchenko (Odessa Polytechnic University)     |
  | Distributed under 3-clause BSD License                                  |
  |   See COPYING                                                           |
  +-------------------------------------------------------------------------+ */

#pragma once

#include <mrpt/obs/CObservation2DRangeScan.h>
#include <mrpt/opengl/CPlanarLaserScan.h>
#include <mrpt/poses/CPose2D.h>
#include <mvsim/Sensors/SensorBase.h>

#include <mutex>

namespace mvsim
{
class LaserScanner : public SensorBase
{
	DECLARES_REGISTER_SENSOR(LaserScanner)
   public:
	LaserScanner(Simulable& parent, const rapidxml::xml_node<char>* root);
	virtual ~LaserScanner();

	// See docs in base class
	virtual void loadConfigFrom(const rapidxml::xml_node<char>* root) override;

	virtual void simul_pre_timestep(const TSimulContext& context) override;
	virtual void simul_post_timestep(const TSimulContext& context) override;

	void poses_mutex_lock() override {}
	void poses_mutex_unlock() override {}

   protected:
	virtual void internalGuiUpdate(
		mrpt::opengl::COpenGLScene& viz, mrpt::opengl::COpenGLScene& physical,
		bool childrenOnly) override;

	int m_z_order;	//!< to help rendering multiple scans
	mrpt::poses::CPose2D m_sensor_pose_on_veh;
	std::string m_name;	 //!< sensor label/name
	double m_rangeStdNoise;
	double m_angleStdNoise;
	/** Whether all box2d "fixtures" are visible (solid) or not (Default=true)
	 */
	bool m_see_fixtures;

	bool m_viz_visiblePlane = false;
	bool m_viz_visiblePoints = false;
	float m_viz_pointSize = 3.0f;

	// Store here all scan parameters. This obj will be copied as a
	// "pattern" to fill it with actual scan data.
	mrpt::obs::CObservation2DRangeScan m_scan_model;

	std::mutex m_last_scan_cs;
	/** Last simulated scan */
	mrpt::obs::CObservation2DRangeScan::Ptr m_last_scan;
	mrpt::obs::CObservation2DRangeScan::Ptr m_last_scan2gui;

	/** Whether m_gl_scan has to be updated upon next call of
	 * internalGuiUpdate() from m_last_scan2gui */
	bool m_gui_uptodate = false;
	std::mutex m_gui_mtx;
	mrpt::opengl::CPlanarLaserScan::Ptr m_gl_scan;
};
}  // namespace mvsim
