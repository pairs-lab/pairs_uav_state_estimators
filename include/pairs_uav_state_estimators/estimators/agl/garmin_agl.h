#ifndef ESTIMATORS_STATE_GARMIN_AGL_H
#define ESTIMATORS_STATE_GARMIN_AGL_H

/* includes //{ */

#include <rclcpp/rclcpp.hpp>

#include <pairs_msgs/msg/float64_stamped.hpp>

#include <pairs_lib/lkf.h>
#include <pairs_lib/profiler.h>
#include <pairs_lib/param_loader.h>
#include <pairs_lib/subscriber_handler.h>
#include <pairs_lib/publisher_handler.h>
#include <pairs_lib/attitude_converter.h>
#include <pairs_lib/transformer.h>

#include <pairs_uav_managers/agl_estimator.h>

#include <pairs_uav_state_estimators/estimators/altitude/alt_generic.h>

#include <ament_index_cpp/get_package_share_directory.hpp>

//}

/* typedefs //{ */

#if USE_ROS_TIMER == 1
typedef pairs_lib::ROSTimer TimerType;
#else
typedef pairs_lib::ThreadTimer TimerType;
#endif

//}

namespace pairs_uav_state_estimators
{

namespace garmin_agl
{
const char name[]         = "garmin_agl";
const char frame_id[]     = "garmin_agl_origin";
const char package_name[] = "pairs_uav_state_estimators";

const bool is_core_plugin = true;

class GarminAgl : public pairs_uav_managers::AglEstimator {

private:
  rclcpp::CallbackGroup::SharedPtr cbkgrp_timers_;

  std::unique_ptr<AltGeneric> est_agl_garmin_;
  const std::string           est_agl_name_ = "garmin_agl";

  const bool is_core_plugin_;

  std::shared_ptr<TimerType> timer_update_;
  int                        _update_timer_rate_;
  void                       timerUpdate();

  std::shared_ptr<TimerType> timer_check_health_;
  int                        _check_health_timer_rate_;
  void                       timerCheckHealth();

  bool isConverged();

  void waitForEstimationInitialization();

public:
  GarminAgl() : AglEstimator(garmin_agl::name, garmin_agl::frame_id, garmin_agl::package_name), is_core_plugin_(is_core_plugin) {
  }

  void initialize(const rclcpp::Node::SharedPtr &node, const std::shared_ptr<CommonHandlers_t> &ch, const std::shared_ptr<PrivateHandlers_t> &ph) override;
  bool start(void) override;
  bool pause(void) override;
  bool reset(void) override;

  pairs_msgs::msg::Float64Stamped getUavAglHeight() const override;
  std::vector<double>           getHeightCovariance() const override;
};

} // namespace garmin_agl

} // namespace pairs_uav_state_estimators

#endif // ESTIMATORS_STATE_GARMIN_AGL_H
