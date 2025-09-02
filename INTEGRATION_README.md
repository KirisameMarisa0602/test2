# Cloudmeeting integration for test2

This repo now contains a top-level qmake project (test2.pro) with three sub-projects in order: sdk (static library), server (existing), client (Qt Widgets app).

## Prerequisites (Ubuntu/Debian example)

```bash
sudo apt-get update
sudo apt-get install -y qtbase5-dev qt5-qmake qtmultimedia5-dev libqt5sql5 libqt5sql5-sqlite build-essential
```

## Build in Qt Creator

1. File -> Open File or Project... -> select test2.pro
2. Choose a Qt 5.x kit, run qmake, then Build.
3. Build order: sdk -> server -> client (qmake SUBDIRS enforces this order).

## Build in terminal

```bash
qmake test2.pro
make -j
```

## Run services

- Auth/Order TCP JSON server: 127.0.0.1:5555 (default used by SDK)
- roomhub: 9000
- udprelay: 9001

## Client

The client links against the new sdk (cmsdk). For now a placeholder main window is provided. To integrate the full cloudmeeting UI, copy these directories from KirisameMarisa0602/cloudmeeting into client/ui_cloudmeeting/:

- client/Forms -> client/ui_cloudmeeting/Forms
- client/Headers -> client/ui_cloudmeeting/Headers
- client/Sources -> client/ui_cloudmeeting/Sources
- client/Resources -> client/ui_cloudmeeting/Resources

The project will automatically pick up .h/.cpp/.ui/.qrc files from those folders.

## SDK usage

Default server: 127.0.0.1:5555. You can override at runtime:

```cpp
cmsdk::AuthApi auth; auth.setServer("192.168.1.10", 5555);
cmsdk::OrderApi order; order.setServer("192.168.1.10", 5555);
```

The SDK uses a simple JSON-lines protocol over QTcpSocket. Each request is a single compact JSON object terminated by a newline and expects one newline-terminated JSON reply with `{ "ok": true }` on success or `{ "ok": false, "error": "..." }` on failure.

---

This is the initial real change set. Further commits can copy the full cloudmeeting UI files into client/ui_cloudmeeting and adjust server endpoints if needed.
