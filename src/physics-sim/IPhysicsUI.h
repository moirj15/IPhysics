#pragma once

#include <Common.h>
#include "Settings.h"
#include <Context.hpp>

//#include <optional>
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
  bool mSettingsFieldModified{false};
  u32 mCurrentObjectSettingsHandle{0};

public:
  IPhysicsUI() : mObjectPath(256, 0)
  {
  }
  ~IPhysicsUI();

  void Init(const focus::Window &window);
  void Update(const focus::Window &window);

  NODISCARD std::string* LoadObjectClicked();
  NODISCARD bool StartSimulationClicked();
  NODISCARD bool StopSimulationClicked();
  NODISCARD bool ResetSimulationClicked();

  NODISCARD inline bool SettingsFieldModified()
  {
    if (mSettingsFieldModified)
    {
      mSettingsFieldModified = false;
      return true;
    }
    return false;
  }

  NODISCARD inline u32 CurrentObject() const
  {
    return mCurrentObjectSettingsHandle;
  }
  inline void SetCurrentObject(u32 handle)
  {
    mCurrentObjectSettingsHandle = handle;
    if (mObjectSettings.find(handle) == mObjectSettings.end())
    {
      mObjectSettings.emplace(handle, Physics::ObjectSettings());
    }
  }
  NODISCARD inline Physics::ObjectSettings GetCurrentObjectsSettings()
  {
    return mObjectSettings[mCurrentObjectSettingsHandle];
  }

  NODISCARD inline Physics::EngineSettings GetPhysicsSettings() const
  {
    return mPhysicsSettings;
  }

  NODISCARD inline std::unordered_map<u32, Physics::ObjectSettings> GetAllObjectSettings() const
  {
    return mObjectSettings;
  }
};

} // namespace IPhysics