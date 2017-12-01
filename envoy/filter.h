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

#include "envoy/http/filter.h"
#include "envoy/server/filter_config.h"
#include "re2/re2.h"

namespace gke {

using namespace Envoy;
using namespace Envoy::Http;

class MDSConcealmentFilter : public StreamDecoderFilter {
 public:
  MDSConcealmentFilter(){};
  ~MDSConcealmentFilter(){};

  void onDestroy() override{};

  FilterHeadersStatus decodeHeaders(HeaderMap& headers, bool) override;

  FilterDataStatus decodeData(Buffer::Instance&, bool) override;

  FilterTrailersStatus decodeTrailers(HeaderMap&) override;

  void setDecoderFilterCallbacks(
      StreamDecoderFilterCallbacks& callbacks) override;

 private:
  bool deny(const std::string&);
  StreamDecoderFilterCallbacks* callbacks_{};
};

}  // namespace gke
