#include <pairs_uav_state_estimators/estimators/state/state_generic.h>

namespace pairs_uav_state_estimators
{

namespace gps_garmin
{

const char estimator_name[] = "gps_garmin";
const bool is_core_plugin   = true;

class GpsGarmin : public StateGeneric {
public:
  GpsGarmin() : StateGeneric(estimator_name, is_core_plugin) {
  }
};

} // namespace gps_garmin

} // namespace pairs_uav_state_estimators


#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(pairs_uav_state_estimators::gps_garmin::GpsGarmin, pairs_uav_managers::StateEstimator)
