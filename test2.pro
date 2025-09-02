TEMPLATE = subdirs

# Build order: sdk, server, client
SUBDIRS = \
    sdk \
    server \
    client

# Dependencies
server.depends = sdk
client.depends = sdk

CONFIG += ordered