#ifndef ESTIMATORS_STATE_F9P_H
#define ESTIMATORS_STATE_F9P_H

/* includes //{ */

#include <ros/ros.h>

#include <nav_msgs/Odometry.h>

#include <sensor_msgs/NavSatFix.h>

#include <pairs_lib/lkf.h>
#include <pairs_lib/profiler.h>
#include <pairs_lib/param_loader.h>
#include <pairs_lib/subscribe_handler.h>
#include <pairs_lib/publisher_handler.h>
#include <pairs_lib/attitude_converter.h>
#include <pairs_lib/transformer.h>
#include <pairs_lib/gps_conversions.h>

#include <pairs_uav_managers/state_estimator.h>

//}

namespace pairs_uav_state_estimators
{

namespace f9p
{
const char name[]         = "f9p";
const char frame_id[]     = "f9p_origin";
const char package_name[] = "pairs_uav_state_estimators";

const bool is_core_plugin = true;

using namespace pairs_uav_managers::estimation_manager;

class F9P : public pairs_uav_managers::StateEstimator {

  using CommonHandlers_t = pairs_uav_managers::estimation_manager::CommonHandlers_t;

private:
  const std::string package_name_ = "pairs_uav_state_estimators";

  const std::string est_lat_name_ = "lat_f9p";

  const std::string est_alt_name_ = "alt_f9p";

  const std::string est_hdg_name_ = "hdg_f9p";

  const bool is_core_plugin_;

  pairs_lib::SubscribeHandler<nav_msgs::Odometry> sh_f9p_odom_;
  void                                          callbackF9POdom(const nav_msgs::Odometry::ConstPtr msg);
  double                                        _critical_timeout_f9p_odom_;
  std::string                                   msg_topic_;

  pairs_lib::SubscribeHandler<sensor_msgs::NavSatFix> sh_gnss_;
  void callbackGnss(const sensor_msgs::NavSatFix::ConstPtr msg);
  double gnss_x_, gnss_y_, gnss_z_;
  bool got_gnss_ = false;
  std::string gnss_topic_;

  ros::Timer       timer_check_f9p_odom_hz_;
  void             timerCheckF9POdomHz(const ros::TimerEvent &event);
  std::atomic<int> counter_odom_msgs_ = 0;
  ros::Time        t_check_hz_last_;
  double           prev_avg_hz_ = 0;
  bool             kickoff_     = false;

  ros::Timer                 timer_update_;
  void                       timerUpdate(const ros::TimerEvent &event);
  nav_msgs::OdometryConstPtr prev_msg_;
  bool                       first_iter_ = true;

  bool isConverged();

  void waitForEstimationInitialization();

  void updateUavState();

public:
  F9P() : StateEstimator(f9p::name, f9p::frame_id, f9p::package_name), is_core_plugin_(is_core_plugin) {
  }

  ~F9P(void) {
  }

  void initialize(ros::NodeHandle &parent_nh, const std::shared_ptr<CommonHandlers_t> &ch, const std::shared_ptr<PrivateHandlers_t> &ph) override;
  bool start(void) override;
  bool pause(void) override;
  bool reset(void) override;

  /* pairs_msgs::UavState  getUavState() override; */
  /* nav_msgs::Odometry  getInnovation() const override; */
  /* std::vector<double> getPoseCovariance() const override; */
  /* std::vector<double> getTwistCovariance() const override; */

  bool setUavState(const pairs_msgs::UavState &uav_state) override;
};

}  // namespace f9p

}  // namespace pairs_uav_state_estimators

#endif  // ESTIMATORS_STATE_f9p_H
