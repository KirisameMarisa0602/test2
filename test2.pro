TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    sdk \
    server \
    client

# Build order dependencies
server.depends = sdk
client.depends = sdk