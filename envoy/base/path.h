// Copyright 2018 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <string>

#include "absl/strings/string_view.h"

namespace file {

// Collapse duplicate "/"s, resolve ".." and "." path elements, remove
// trailing "/".
//
// NOTE: This respects relative vs. absolute paths, but does not
// invoke any system calls (getcwd(2)) in order to resolve relative
// paths wrt actual working directory.  That is, this is purely a
// string manipulation, completely independent of process state.
std::string CleanPath(absl::string_view path);

}  // namespace file
