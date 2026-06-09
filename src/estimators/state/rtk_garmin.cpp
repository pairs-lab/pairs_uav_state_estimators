#include <pairs_uav_state_estimators/estimators/state/state_generic.h>

namespace pairs_uav_state_estimators
{

namespace rtk_garmin
{

const char estimator_name[] = "rtk_garmin";
const bool is_core_plugin = true;

class RtkGarmin : public StateGeneric {
public:
  RtkGarmin() : StateGeneric(estimator_name, is_core_plugin) {
  }

  ~RtkGarmin(void) {
  }
};

}  // namespace rtk_garmin
}  // namespace pairs_uav_state_estimators

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(pairs_uav_state_estimators::rtk_garmin::RtkGarmin, pairs_uav_managers::StateEstimator)
