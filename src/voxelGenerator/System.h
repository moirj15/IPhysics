//
// Created by Jimmy on 2/9/2020.
//

#pragma once
#include <memory>

struct Window;

namespace VoxGen
{
class VoxelizerUI;

class System
{
  std::unique_ptr<Window> mWindow;
  std::unique_ptr<VoxelizerUI> mUI;

public:
  System();
  ~System();

  void Run();
};

} // namespace VoxGen
