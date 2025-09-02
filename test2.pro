TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += sdk

server.subdir = server
server.depends = sdk
exists($$server.subdir/*.pro) {
    SUBDIRS += server
}

client.subdir = client
client.depends = sdk
exists($$client.subdir/*.pro) {
    SUBDIRS += client
} else {
    SUBDIRS += client
}
