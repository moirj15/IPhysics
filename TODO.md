# TODO

### Voxelization

- [ ] Update the `ShaderBufferManager` in focus to allow for updating of buffers (easily)
- [ ] Finish testing voxelization on a few simple cases
- [ ] Implement a simple renderer
- [ ] Implement the debug rendering for voxels (maybe compute shader?)

### Object Management

- [ ] Implement some object manager for the objects being voxelized and run through the simulation
  - [ ] Should have a "master" manager for restoring objects and so voxelization only has to happen once per object
  - [ ] Should have a sub-manager for the actual simulation and positioning of the objects
  
### Physics-sim

- [ ] Should get the previous code working
- [ ] Consider replacing it with something else with better documentation
- [ ] Look into the possibility of getting it running on gpu at some point
  - Most likely only going to happen if the library I end up using supports it out of the box
  