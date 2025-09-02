TEMPLATE = subdirs

# Build order: sdk, server, client
SUBDIRS = \
    sdk \
    server \
    client

# Configure subdirectory paths
sdk.file = sdk/cmsdk.pro

# Dependencies
server.depends = sdk
client.depends = sdk

CONFIG += ordered