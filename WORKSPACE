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
# Required by Protobuf & Grpc (Patched them to use @com_github_madler_zlib//:zlib)
http_archive(
    name = "com_github_madler_zlib",
    build_file = "//third_party:zlib.BUILD",
    sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
    strip_prefix = "zlib-1.2.11",
    urls = ["https://zlib.net/zlib-1.2.11.tar.gz"],
)

# abseil, required by Grpc.
http_archive(
    name = "com_google_absl",
    sha256 = "15e1afcb57de3855941927cb0fb1c7cd6194b38a8d2f297e9a8e4e66ff66aa3d",
    strip_prefix = "abseil-cpp-67222ffc4c83d918ce8395aa61769eeb77df4c4d",
    urls = ["https://github.com/abseil/abseil-cpp/archive/67222ffc4c83d918ce8395aa61769eeb77df4c4d.tar.gz"],  # 2019-08-06
)

# proto_library, cc_proto_library, and java_proto_library rules implicitly
# depend on @com_google_protobuf for protoc and proto runtimes.
http_archive(
    name = "com_google_protobuf",
    patch_args = ["-p1"],
    patches = ["//third_party/protobuf:build-with-com_github_madler_zlib.patch"],
    sha256 = "c90d9e13564c0af85fd2912545ee47b57deded6e5a97de80395b6d2d9be64854",
    strip_prefix = "protobuf-3.9.1",
    urls = ["https://github.com/google/protobuf/archive/v3.9.1.zip"],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

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

# GSL repository
http_archive(
    name = "com_github_microsoft_gsl",
    build_file = "//third_party:gsl.BUILD",
    sha256 = "b991e8b347b763f4b0e521b9687cdf8aebd6c5a831a6b6435b33fc11007e2c7f",
    strip_prefix = "GSL-1212beae777dba02c230ece8c0c0ec12790047ea",
    urls = ["https://github.com/microsoft/GSL/archive/1212beae777dba02c230ece8c0c0ec12790047ea.zip"],  # 2019-06-13
)

#####
# Go Dependencies
#####

# Generate BUILD files for Go project.
http_archive(
    name = "bazel_gazelle",
    sha256 = "be9296bfd64882e3c08e3283c58fcb461fa6dd3c171764fcc4cf322f60615a9b",
    urls = [
        "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/bazel-gazelle/releases/download/0.18.1/bazel-gazelle-0.18.1.tar.gz",
        "https://github.com/bazelbuild/bazel-gazelle/releases/download/0.18.1/bazel-gazelle-0.18.1.tar.gz",
    ],
)

load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies")

gazelle_dependencies()

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

http_archive(
    name = "cpplint",
    build_file = "//third_party:cpplint.BUILD",
    sha256 = "05f879aab5a04307e916e32afb547567d8a44149ddc2f91bf846ce2650ce6d7d",
    strip_prefix = "cpplint-1.4.4",
    urls = [
        "https://github.com/cpplint/cpplint/archive/1.4.4.tar.gz",
    ],
)

# Generate compilation database for C++.
# TODO(zhangshuai.ds): Build kythe in a lighter way.
http_archive(
    name = "io_kythe",
    sha256 = "4856b9b974ac771e5d8be9f14e7d98f71760ce85c9f20321a94c7ea94d4ab2ee",
    strip_prefix = "kythe-b366059660304a27ae7775055ca168a4ecb55a72",
    urls = ["https://github.com/kythe/kythe/archive/b366059660304a27ae7775055ca168a4ecb55a72.zip"],
)

load("@io_kythe//:external.bzl", "kythe_dependencies")

kythe_dependencies()
