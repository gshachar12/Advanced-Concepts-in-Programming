# Project 3 - Dynamic Plugin Architecture Implementation

## 🚀 What's New: Dynamic Loading System

Project 3 now includes **complete dynamic plugin architecture** as specified in the assignment requirements:

### ✅ Implemented Features

#### 🔌 **Dynamic Library Loading**
- **dlopen/dlclose support** for Windows (LoadLibrary/FreeLibrary) and Unix (dlopen/dlclose)
- **Runtime plugin discovery** - automatically finds .dll/.so files in plugins/ directory
- **Shared library compilation** - algorithms can be built as separate libraries
- **Plugin validation** - ensures loaded plugins have proper registration

#### 📋 **Assignment-Specific Interfaces**
- **AlgorithmRegistrar** class with exact interface from assignment:
  - `createAlgorithmFactoryEntry()`
  - `addPlayerFactoryToLastEntry()` / `addTankAlgorithmFactoryToLastEntry()`
  - `validateLastRegistration()`
  - `BadRegistrationException` handling
- **Registration Macros**: `REGISTER_PLAYER()`, `REGISTER_TANK_ALGORITHM()`, `REGISTER_GAME_MANAGER()`
- **Simulator Class** for centralized plugin management

#### 🏗️ **Plugin Development System**
- **Plugin Templates** - example SimplePlugin shows how to create plugins
- **Factory System** - proper integration with game engine
- **Namespace Support** - clean plugin organization
- **Build Integration** - Makefile supports plugin compilation

---

## 🎮 Quick Start with Dynamic Plugins

### 1. Build Everything
```bash
# Build main game + plugins
mingw32-make all-with-plugins

# Or build separately
mingw32-make all      # Main executable
mingw32-make plugins  # Plugin libraries
```

### 2. Test Dynamic Loading
```bash
# Test the dynamic loading system
mingw32-make test_dynamic

# Test plugin architecture  
mingw32-make test_plugins
```

### 3. Run with Plugins
```bash
# List all algorithms (built-in + plugins)
tanks_game.exe -list

# Run game (automatically loads plugins)
tanks_game.exe -algo1 SimplePlugin -algo2 BFS -verbose
```

---

## 🔧 Plugin Development Guide

### Creating a New Plugin

1. **Create Plugin Directory**:
   ```
   plugins/MyPlugin/
   ├── MyTankAlgorithm.h
   ├── MyTankAlgorithm.cpp
   ├── MyPlayer.h
   └── MyPlayer.cpp
   ```

2. **Implement TankAlgorithm**:
   ```cpp
   #include "../../common/TankAlgorithm.h"
   #include "../../common/TankAlgorithmRegistration.h"
   
   namespace MyPlugin {
   
   class MyTankAlgorithm : public TankAlgorithm {
   public:
       MyTankAlgorithm(int player_index, int tank_index);
       ActionRequest getAction() override;
       void updateBattleInfo(BattleInfo& info) override;
   };
   
   } // namespace MyPlugin
   
   using namespace MyPlugin;
   REGISTER_TANK_ALGORITHM(MyTankAlgorithm);
   ```

3. **Implement Player**:
   ```cpp
   #include "../../common/Player.h"
   #include "../../common/PlayerRegistration.h"
   
   namespace MyPlugin {
   
   class MyPlayer : public Player {
   public:
       MyPlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells);
       void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
   };
   
   } // namespace MyPlugin
   
   using namespace MyPlugin;
   REGISTER_PLAYER(MyPlayer);
   ```

4. **Add to Makefile**:
   ```makefile
   MY_PLUGIN_SOURCES = plugins/MyPlugin/MyTankAlgorithm.cpp plugins/MyPlugin/MyPlayer.cpp
   PLUGINS += $(PLUGINS_DIR)/MyPlugin.dll
   
   $(PLUGINS_DIR)/MyPlugin.dll: $(MY_PLUGIN_SOURCES) $(SIMULATOR_OBJECTS)
       $(CXX) $(PLUGIN_CXXFLAGS) $(MY_PLUGIN_SOURCES) $(SIMULATOR_OBJECTS) -o $@ $(PLUGIN_LDFLAGS)
   ```

---

## 🧪 Testing Framework

### Available Tests
```bash
mingw32-make test_interfaces    # Interface compilation
mingw32-make test_components    # Core components  
mingw32-make test_plugins       # Plugin architecture
mingw32-make test_dynamic       # Dynamic loading system
mingw32-make test_tournament    # Tournament system
```

### Test Results
- **test_dynamic** - Tests plugin discovery, loading, validation, and cleanup
- **test_plugins** - Tests factory system and algorithm registration
- **test_tournament** - Tests competition system with multiple algorithms

---

## 📁 Updated Project Structure

```
Project3/
├── common/                     # Interface definitions
│   ├── PlayerRegistration.h   # 🆕 Player registration macros
│   ├── TankAlgorithmRegistration.h # 🆕 Algorithm registration macros  
│   ├── GameManagerRegistration.h   # 🆕 Game manager registration macros
│   └── ...                    # Existing interfaces
├── src/
│   ├── simulator/             # 🆕 Dynamic loading system
│   │   ├── AlgorithmRegistrar.h/cpp  # Assignment-specific registrar
│   │   ├── Simulator.h/cpp           # Plugin management
│   │   ├── PlayerRegistration.cpp    # Registration implementation
│   │   └── TankAlgorithmRegistration.cpp
│   ├── algorithms/            # Built-in algorithms  
│   └── ...                    # Game engine
├── plugins/                   # 🆕 Plugin libraries
│   ├── SimplePlugin/          # Example plugin
│   │   ├── SimpleTankAlgorithm.h/cpp
│   │   └── SimplePlayer.h/cpp
│   └── SimplePlugin.dll       # Compiled plugin
├── test_dynamic_loading.cpp   # 🆕 Dynamic loading tests
└── Makefile                   # Updated with plugin support
```

---

## 🎯 Assignment Compliance

### ✅ Required Features Implemented

| Feature | Status | Implementation |
|---------|--------|----------------|
| **Dynamic Library Loading** | ✅ | dlopen/LoadLibrary with error handling |
| **AlgorithmRegistrar Class** | ✅ | Exact interface from assignment |
| **Registration Macros** | ✅ | REGISTER_PLAYER, REGISTER_TANK_ALGORITHM |
| **BadRegistrationException** | ✅ | Proper validation and error reporting |
| **Simulator Class** | ✅ | Plugin discovery and management |
| **Factory Pattern** | ✅ | Lambda-based factory functions |
| **Plugin Validation** | ✅ | Registration completeness checking |

### 🔄 Backwards Compatibility

- **Built-in algorithms** still work without plugins
- **Unified algorithm lookup** searches both built-in and dynamic algorithms  
- **Command line interface** unchanged - automatically detects plugin types
- **Tournament system** supports mixed built-in/plugin competitions

---

## 🚀 Performance & Reliability

### Plugin Management
- **Safe loading/unloading** with proper cleanup
- **Error isolation** - bad plugins don't crash the system
- **Memory management** - smart pointers throughout
- **Platform abstraction** - works on Windows and Unix

### Algorithm Discovery
- **Automatic scanning** of plugin directories
- **File validation** - only loads .dll/.so files
- **Name extraction** - automatic algorithm naming
- **Graceful fallback** - uses built-in algorithms if no plugins found

---

## 🎮 Ready to Play!

The system now fully implements the assignment's dynamic plugin architecture while maintaining all existing functionality. You can:

1. **Use built-in algorithms** (Simple, BFS, Random)
2. **Load dynamic plugins** automatically
3. **Mix and match** built-in and plugin algorithms
4. **Develop custom plugins** using the provided framework
5. **Run tournaments** with any combination of algorithms

**Try it now:**
```bash
mingw32-make all-with-plugins
tanks_game.exe -list
tanks_game.exe -visualize -verbose
```
