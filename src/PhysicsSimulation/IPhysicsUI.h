#pragma once

#include "../PhysicsEngine/Settings.h"
#include "../common.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

struct Window;

namespace IPhysics
{

class IPhysicsUI
{
  std::string mObjectPath;
  // TODO: consider making the settings pointers
  std::unordered_map<u32, Physics::ObjectSettings> mObjectSettings;
  Physics::EngineSettings mPhysicsSettings;
  bool mLoadObjectClicked;
  bool mStartSimulationClicked;
  bool mStopSimulationClicked;
  bool mResetSimulationClicked;
  u32 mCurrentObjectSettings;

public:
  IPhysicsUI() :
      mObjectPath(256, 0), mLoadObjectClicked(false), mStartSimulationClicked(false),
      mStopSimulationClicked(false), mResetSimulationClicked(false), mCurrentObjectSettings(0)
  {
  }
  ~IPhysicsUI();

  void Init(Window *window);
  void Update();

  NODISCARD std::optional<std::string> LoadObjectClicked();
  NODISCARD bool StartSimulationClicked();
  NODISCARD bool StopSimulationClicked();
  NODISCARD bool ResetSimulationClicked();
  NODISCARD u32 CurrentObject();
};

} // namespace IPhysics