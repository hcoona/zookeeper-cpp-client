cc_library(
    name = "gsl",
    hdrs = [
        "include/gsl/gsl",
        "include/gsl/gsl_algorithm",
        "include/gsl/gsl_assert",
        "include/gsl/gsl_byte",
        "include/gsl/gsl_util",
        "include/gsl/multi_span",
        "include/gsl/pointers",
        "include/gsl/span",
        "include/gsl/string_span",
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++14"],
        "//conditions:default": ["-std=c++14"],
    }),
    strip_include_prefix = "./include",
    visibility = ["//visibility:public"],
)
