# CloudMeeting Integration README

This repository integrates the CloudMeeting UI into test2 with a custom SDK and top-level qmake project structure.

## Dependencies

Install the following packages on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install -y qtbase5-dev qt5-qmake qtmultimedia5-dev libqt5sql5 libqt5sql5-sqlite
```

## Build Instructions

1. Open the project in Qt Creator:
   - File → Open File or Project
   - Select `test2.pro` from the repository root

2. Or build from command line:
   ```bash
   qmake test2.pro
   make
   ```

The build order is: sdk → server → client

## Project Structure

- **sdk/**: Static library providing cmsdk::AuthApi and cmsdk::OrderApi classes
- **server/**: TCP+JSON server (unchanged from original)
- **client/**: Qt Widgets application using CloudMeeting UI

## SDK Usage

The SDK provides two main classes:

### cmsdk::AuthApi
```cpp
cmsdk::AuthApi auth;
auth.setServer("127.0.0.1", 5555);  // Default server
bool success = auth.loginUser("username", "password");
```

### cmsdk::OrderApi  
```cpp
cmsdk::OrderApi orders;
orders.setServer("127.0.0.1", 5555);  // Default server
int orderId;
bool success = orders.newOrder(orderData, &orderId);
```

## Running the Application

### 1. Start the Server
The server provides multiple services:

```bash
# From the repository root
./server/server
```

Default ports:
- **Auth/Order service**: 5555 (TCP+JSON)
- **Room hub**: 9000  
- **UDP relay**: 9001

### 2. Run the Client
```bash
# From the repository root  
./client/client
```

The client defaults to connecting to `127.0.0.1:5555`. Use `setServer()` methods in the SDK to override the server address if needed.

## Notes

- The SDK uses synchronous QTcpSocket with JSON lines (newline-terminated)
- CloudMeeting UI files are located in `client/ui_cloudmeeting/`
- The server implementation remains unchanged from the original
- Client automatically links to the SDK via `client/sdk_link.pri`