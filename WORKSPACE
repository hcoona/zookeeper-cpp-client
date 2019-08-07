load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#####
# Skylark libraries
#####
http_archive(
    name = "bazel_skylib",
    sha256 = "2ea8a5ed2b448baf4a6855d3ce049c4c452a6470b1efd1504fdb7c1c134d220a",
    strip_prefix = "bazel-skylib-0.8.0",
    url = "https://github.com/bazelbuild/bazel-skylib/archive/0.8.0.tar.gz",
)

#####
# Toolchains
#####
http_archive(
    name = "io_bazel_rules_go",
    sha256 = "9fb16af4d4836c8222142e54c9efa0bb5fc562ffc893ce2abeac3e25daead144",
    urls = [
        "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/rules_go/releases/download/0.19.0/rules_go-0.19.0.tar.gz",
        "https://github.com/bazelbuild/rules_go/releases/download/0.19.0/rules_go-0.19.0.tar.gz",
    ],
)

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")

go_rules_dependencies()

go_register_toolchains()

#####
# Protobuf & Grpc
#####
# Required by Protobuf
# TODO: rename to com_github_madler_zlib & Patch Protobuf
http_archive(
    name = "zlib",
    build_file = "//third_party:zlib.BUILD",
    sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
    strip_prefix = "zlib-1.2.11",
    urls = ["https://zlib.net/zlib-1.2.11.tar.gz"],
)

http_archive(
    name = "com_google_protobuf",
    sha256 = "8eb5ca331ab8ca0da2baea7fc0607d86c46c80845deca57109a5d637ccb93bb4",
    strip_prefix = "protobuf-3.9.0",
    urls = ["https://github.com/google/protobuf/archive/v3.9.0.zip"],
)

#####
# Common Dependencies
#####
http_archive(
    name = "org_apache_zookeeper",
    build_file = "//third_party/zookeeper-client-c:zookeeper.BUILD",
    sha256 = "b14f7a0fece8bd34c7fffa46039e563ac5367607c612517aa7bd37306afbd1cd",
    strip_prefix = "zookeeper-3.4.14/zookeeper-client/zookeeper-client-c",
    urls = [
        "https://mirrors.tuna.tsinghua.edu.cn/apache/zookeeper/zookeeper-3.4.14/zookeeper-3.4.14.tar.gz",
        "https://www-eu.apache.org/dist/zookeeper/zookeeper-3.4.14/zookeeper-3.4.14.tar.gz",
        "https://archive.apache.org/dist/zookeeper/zookeeper-3.4.14/zookeeper-3.4.14.tar.gz",
    ],
)

#####
# Commandline Tools
#####
http_archive(
    name = "com_github_bazelbuild_buildtools",
    sha256 = "d8440da64ac15eca922ca0e8c6772bbb04eaaf3d2f4de387e5bfdb87cecbe9d2",
    strip_prefix = "buildtools-0.28.0",
    url = "https://github.com/bazelbuild/buildtools/archive/0.28.0.zip",
)

load("@com_github_bazelbuild_buildtools//buildifier:deps.bzl", "buildifier_dependencies")

buildifier_dependencies()
