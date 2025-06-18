# Akuma - Discord Desktop Wrapper

> **⚠️ EARLY DEVELOPMENT WARNING**
> 
> This project is in **very early testing stages**. Features may be incomplete, unstable, or subject to major changes without notice. APIs and functionality are not finalized and may be completely rewritten or removed. Use at your own risk and expect breaking changes.

A lightweight, fast Discord desktop client written in C++ using SDL2. Akuma provides a native desktop experience for Discord with improved performance and customization options.

## 🚧 Development Status

**ALPHA** - Core GUI framework only. Discord functionality not yet implemented.

- [x] Basic GUI framework
- [ ] Discord integration
- [ ] Authentication
- [ ] Messaging

**Note**: Things could change, or this project could never finish. Do not make a total switch, nor think about it at this current time.

## Features

- **Native C++ Performance**: Built from the ground up in C++ for optimal speed and resource usage
- **Custom GUI Framework**: Lightweight widget-based interface built on SDL2
- **Cross-platform Support**: Works on Windows, macOS, and Linux
- **Modern Interface**: Clean, responsive UI with customizable themes

### Planned Features (Not Yet Implemented)
- Discord login and authentication
- Real-time messaging
- Voice and video calls
- Server and channel management
- Custom themes and plugins

## Dependencies

- **SDL2**: Core graphics, windowing, and input handling
- **SDL2_image**: Image loading support
- **SDL2_ttf**: Font rendering
- **CMake 3.16+**: Build system
- **C++17**: Modern C++ features

## Building

### Linux/macOS
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev cmake

# Arch dependencies
sudo pacman -Sy sdl2 sdl2_ttf sdl2_image cmake

# Clone and build
git clone https://github.com/tomosfps/akuma
cd akuma
mkdir build && cd build
cmake ..
cmake --build .

# Run
./akuma
```

### Windows
```cmd
# Install vcpkg and dependencies
vcpkg install sdl2 sdl2-image[libjpeg-turbo] sdl2-ttf

# Build with CMake
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

## Architecture

### Core Components

- **`core::Application`**: Main application window and event loop management
- **`gui::Widget`**: Base class for all UI elements with hierarchical rendering
- **`gui::Panel`**: Container widgets for organizing layouts
- **`gui::Button`**: Interactive buttons with click callbacks
- **`gui::Label`**: Text display with font and color customization
- **`gui::TextBox`**: Text input fields for user interaction

### Project Structure
```
akuma/
├── src/
│   ├── core/
│   │   └── application.cpp    # Main application logic
│   └── gui/
│       ├── widget.cpp         # Base widget system
│       ├── panel.cpp          # Container widgets
│       ├── button.cpp         # Interactive buttons
│       ├── label.cpp          # Text rendering
│       └── textbox.cpp        # Text input
├── include/                   # Header files
├── assets/
│   └── fonts/
│       └── arial.ttf          # UI fonts
└── main.cpp                   # Entry point
```

## Running the Demo

Currently, Akuma shows a demo interface with:
- Welcome message
- Interactive button
- Text input field
- Submit button that logs input

```bash
./akuma
```

## Roadmap

### Phase 1: Authentication (In Progress)
- [ ] Discord OAuth2 login
- [ ] Token management and storage
- [ ] User session handling

### Phase 2: Core Discord Features
- [ ] WebSocket connection to Discord Gateway
- [ ] Real-time message receiving
- [ ] Message sending
- [ ] Server/channel listing
- [ ] User presence

### Phase 3: Advanced Features
- [ ] Voice channel support
- [ ] File uploads and embeds
- [ ] Emoji and reaction support
- [ ] Direct messages
- [ ] Notifications

### Phase 4: Customization
- [ ] Custom themes
- [ ] Plugin system
- [ ] Keybind customization
- [ ] Advanced settings

## Why Akuma?

- **Performance**: Native C++ provides better resource usage than Electron-based clients
- **Customization**: Full control over UI and features
- **Privacy**: Open source with transparent data handling
- **Lightweight**: Minimal system resource usage
- **Fast**: Optimized for speed and responsiveness
