# CloudMeeting UI Integration

This repository integrates the UI from the KirisameMarisa0602/cloudmeeting repository with the existing test2 server functionality, creating a unified project that can be built with QtCreator on Linux.

## Requirements

Install the following packages on a Linux system:
```bash
sudo apt install qtbase5-dev qt5-qmake qtmultimedia5-dev libqt5sql5 libqt5sql5-sqlite
```

## Building

Open the project in QtCreator:
1. Open `test2.pro` in QtCreator
2. Configure the project for your Qt 5.x installation
3. Build the project

Alternatively, build from command line:
```bash
qmake test2.pro
make
```

## Project Structure

The project consists of three sub-projects built in order:

1. **sdk/** - Static library (libcmsdk.a) providing C++ APIs to communicate with the server
   - `cmsdk::AuthApi` - Authentication API (register/login users)
   - `cmsdk::OrderApi` - Order management API (create/get/update/delete orders)

2. **server/** - Backend server providing auth/order services and real-time communication
   - Auth/Order service on port 5555 (TCP+JSON)
   - RoomHub service on port 9000 (WebRTC-style room management)
   - UDP relay service on port 9001

3. **client/** - Qt Widgets application with CloudMeeting UI
   - Uses the integrated CloudMeeting UI from `client/ui_cloudmeeting/`
   - Links to the SDK library for server communication
   - Includes video conferencing, screen sharing, and collaboration features

## Running

1. **Start the server:**
   ```bash
   cd server/
   ./server
   ```
   The server will start with default ports:
   - Auth/Order service: 5555
   - RoomHub: 9000
   - UDP relay: 9001

2. **Start the client:**
   ```bash
   cd client/
   ./client
   ```

## SDK Configuration

The SDK defaults to connecting to `127.0.0.1:5555` for auth/order services. You can override this in your application:

```cpp
#include <cmsdk/auth_api.h>
#include <cmsdk/order_api.h>

cmsdk::AuthApi auth;
auth.setServer("your-server-host", 5555);

cmsdk::OrderApi orders;  
orders.setServer("your-server-host", 5555);
```

## Features Preserved

- All existing server functionality (roomhub, udprelay, auth/order endpoints)
- Original client code is preserved in the repository 
- CloudMeeting UI provides enhanced collaboration features
- Self-contained build - no manual file copying required

## Development

The project uses qmake and is designed to work out-of-the-box after cloning or downloading as ZIP. The CloudMeeting UI files are located under `client/ui_cloudmeeting/` and are automatically included in the build.