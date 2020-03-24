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
  std::unordered_map<u32, Physics::ObjectSettings> mObjectSettings;
  Physics::EngineSettings mPhysicsSettings{};
  bool mLoadObjectClicked{false};
  bool mStartSimulationClicked{false};
  bool mStopSimulationClicked{false};
  bool mResetSimulationClicked{false};
  u32 mCurrentObjectSettingsHandle{0};

public:
  IPhysicsUI() : mObjectPath(256, 0)
  {
  }
  ~IPhysicsUI();

  void Init(Window *window);
  void Update();

  NODISCARD std::optional<std::string> LoadObjectClicked();
  NODISCARD bool StartSimulationClicked();
  NODISCARD bool StopSimulationClicked();
  NODISCARD bool ResetSimulationClicked();
  NODISCARD inline u32 CurrentObject() const
  {
    return mCurrentObjectSettingsHandle;
  }
  inline u32 SetCurrentObject(u32 handle)
  {
    mCurrentObjectSettingsHandle = handle;
  }
  NODISCARD inline Physics::ObjectSettings GetCurrentObjectsSettings()
  {
    return mObjectSettings[mCurrentObjectSettingsHandle];
  }

  NODISCARD inline Physics::EngineSettings GetPhysicsSettings() const
  {
    return mPhysicsSettings;
  }

  NODISCARD inline std::vector<std::pair<u32, Physics::ObjectSettings>> GetAllObjectSettings() const
  {
    return std::vector<std::pair<u32, Physics::ObjectSettings>>(
        mObjectSettings.begin(), mObjectSettings.end());
  }
};

} // namespace IPhysics