#include <pairs_uav_state_estimators/estimators/state/state_generic.h>

namespace pairs_uav_state_estimators
{

namespace gps_baro
{

const char estimator_name[] = "gps_baro";
const bool is_core_plugin = true;

class GpsBaro : public StateGeneric {
public:
  GpsBaro() : StateGeneric(estimator_name, is_core_plugin) {
  }

  ~GpsBaro(void) {
  }
};

}  // namespace gps_baro
}  // namespace pairs_uav_state_estimators

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(pairs_uav_state_estimators::gps_baro::GpsBaro, pairs_uav_managers::StateEstimator)
