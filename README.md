# pairs_uav_state_estimators

State-estimation plugins for the PAIRS multirotor-UAV autonomy stack. The package fuses
GNSS, RTK, barometer, and rangefinder data into the position, velocity, heading, and
above-ground-level estimates that the rest of the stack flies on. The estimators are loaded
at runtime by the `pairs_uav_managers` estimation manager through pluginlib, so the active
fusion scheme can be switched per flight without rebuilding.

## Contents

Pluginlib estimators built against the `pairs_uav_managers` estimator interfaces:

- `GpsGarmin`, `GpsBaro` — GNSS-based state estimation with Garmin rangefinder or barometer altitude
- `Rtk`, `RtkGarmin` — RTK-GNSS state estimation, optionally combined with a Garmin rangefinder
- `F9P` — state estimation from a u-blox F9P RTK receiver
- `GroundTruth` — simulation-only estimator fed by ground-truth pose
- `Passthrough` — forwards an externally supplied odometry estimate
- `Dummy` — minimal placeholder estimator for bring-up and testing
- `GarminAgl` — above-ground-level (AGL) height estimator from a Garmin rangefinder

The plugins are assembled from altitude, lateral, heading, and AGL sub-estimators (in
`src/estimators/`), with public/private parameter sets under `config/`.

## Branches

- `ros1` — ROS 1 Noetic (catkin)
- `ros2` — ROS 2 Jazzy (ament_cmake)

## Install (ROS 1 Noetic)

```bash
sudo apt install ros-noetic-pairs-uav-state-estimators
```

## License
BSD 3-Clause. Derived from the CTU-MRS `pairs_uav_state_estimators` package; the original
copyright is retained in [LICENSE](LICENSE).
