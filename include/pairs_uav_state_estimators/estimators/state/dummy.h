#ifndef ESTIMATORS_STATE_DUMMY_H
#define ESTIMATORS_STATE_DUMMY_H

/* includes //{ */

#include <rclcpp/rclcpp.hpp>

#include <nav_msgs/msg/odometry.hpp>

#include <pairs_lib/lkf.h>
#include <pairs_lib/profiler.h>
#include <pairs_lib/param_loader.h>
#include <pairs_lib/subscriber_handler.h>
#include <pairs_lib/publisher_handler.h>
#include <pairs_lib/attitude_converter.h>
#include <pairs_lib/transformer.h>

#include <pairs_uav_managers/state_estimator.h>

#include <pairs_lib/errorgraph/error_publisher.h>

#include <pairs_uav_state_estimators/estimators/lateral/lat_generic.h>
#include <pairs_uav_state_estimators/estimators/altitude/alt_generic.h>
#include <pairs_uav_state_estimators/estimators/heading/hdg_passthrough.h>

//}

/* using //{ */

using namespace std::chrono_literals;

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

namespace dummy
{
const char name[]         = "dummy";
const char frame_id[]     = "dummy_origin";
const char package_name[] = "pairs_uav_state_estimators";

const bool is_core_plugin = true;

using CommonHandlers_t = pairs_uav_managers::estimation_manager::CommonHandlers_t;

class Dummy : public pairs_uav_managers::StateEstimator {

private:
  rclcpp::CallbackGroup::SharedPtr cbkgrp_timers_;

  const std::string package_name_ = "pairs_uav_state_estimators";

  const std::string est_lat_name_ = "lat_dummy";

  const std::string est_alt_name_ = "alt_dummy";

  const std::string est_hdg_name_ = "hdg_dummy";

  const bool is_core_plugin_;

  std::unique_ptr<pairs_lib::errorgraph::ErrorPublisher> error_publisher_;

  std::shared_ptr<TimerType> timer_update_;
  void                       timerUpdate();
  bool                       first_iter_ = true;

  std::shared_ptr<TimerType> timer_check_health_;
  void                       timerCheckHealth();

  bool isConverged();

  void waitForEstimationInitialization();

  void updateUavState() override;

  std::mutex mutex_update_uav_state_;

public:
  Dummy() : StateEstimator(dummy::name, dummy::frame_id, dummy::package_name), is_core_plugin_(is_core_plugin) {
  }

  void initialize(const rclcpp::Node::SharedPtr &node, const std::shared_ptr<CommonHandlers_t> &ch, const std::shared_ptr<PrivateHandlers_t> &ph) override;
  bool start(void) override;
  bool pause(void) override;
  bool reset(void) override;

  bool setUavState(const pairs_msgs::msg::UavState &uav_state) override;
};

} // namespace dummy

} // namespace pairs_uav_state_estimators

#endif // ESTIMATORS_STATE_DUMMY_H
