# Copyright 2018 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

package(default_visibility = ["//visibility:public"])

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
    "envoy_cc_library",
    "envoy_cc_test",
)
load("@io_bazel_rules_docker//container:image.bzl", "container_image")

envoy_cc_binary(
    name = "envoy",
    srcs = ["config.cc"],
    repository = "@envoy",
    deps = [
        ":filter",
        "@envoy//source/exe:envoy_main_entry_lib",
    ],
)

envoy_cc_library(
    name = "filter",
    srcs = ["filter.cc"],
    hdrs = ["filter.h"],
    repository = "@envoy",
    deps = [
        "//base",
        "@com_google_absl//absl/strings",
        "@com_googlesource_code_re2//:re2",
        "@envoy//source/exe:envoy_common_lib",
    ],
)

container_image(
    name = "envoy_image",
    base = "@official_ubuntu//image",
    cmd = [
        "/envoy",
        "-c",
        "config.yaml",
    ],
    files = [
        "config.yaml",
        ":envoy",
    ],
)
