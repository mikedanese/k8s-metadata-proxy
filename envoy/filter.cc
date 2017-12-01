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

#include "filter.h"
#include "absl/strings/match.h"
#include "base/path.h"
#include "common/http/headers.h"
#include "common/http/utility.h"
#include "envoy/http/filter.h"
#include "envoy/request_info/request_info.h"

namespace gke {

using namespace Envoy;
using namespace Envoy::Http;
using namespace Envoy::Server::Configuration;

namespace {

std::vector<const re2::RE2*> concealed_regexps_ = {
    new re2::RE2("/0.1/meta-data/service-accounts/.+/identity"),
    new re2::RE2(
        "/computeMetadata/v1beta1/instance/service-accounts/.+/identity"),
    new re2::RE2("/computeMetadata/v1/instance/service-accounts/.+/identity"),
    // disallow recursive queries
    new re2::RE2("\\?(.*&)?recurse"),
};

std::vector<std::string> concealed_paths_ = {
    "/0.1/meta-data/attributes/kube-env",
    "/computeMetadata/v1beta1/instance/attributes/kube-env",
    "/computeMetadata/v1/instance/attributes/kube-env",
};

std::vector<std::string> known_prefixes_ = {
    "/0.1/meta-data/",
    "/computeMetadata/v1beta1/",
    "/computeMetadata/v1/",
};

std::vector<std::string> discovery_paths_ = {
    "",
    "/",
    "/0.1",
    "/0.1/",
    "/0.1/meta-data",
    "/computeMetadata",
    "/computeMetadata/",
    "/computeMetadata/v1beta1",
    "/computeMetadata/v1",
};

}  // namespace

FilterDataStatus MDSConcealmentFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
};

FilterTrailersStatus MDSConcealmentFilter::decodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
};

void MDSConcealmentFilter::setDecoderFilterCallbacks(
    StreamDecoderFilterCallbacks& callbacks) {
  callbacks_ = &callbacks;
};

bool MDSConcealmentFilter::deny(const std::string& dirtypath) {
  auto path = file::CleanPath(dirtypath);
  for (auto& discovery_path : discovery_paths_) {
    if (path == discovery_path) return false;
  }
  for (auto& concealed_path : concealed_paths_) {
    if (path == concealed_path) return true;
  }
  for (auto& re : concealed_regexps_) {
    if (re2::RE2::PartialMatch(path, *re)) return true;
  }
  for (auto& known_prefix : known_prefixes_) {
    if (absl::StartsWith(path, known_prefix)) return false;
  }
  return true;
}

FilterHeadersStatus MDSConcealmentFilter::decodeHeaders(HeaderMap& headers,
                                                        bool) {
  if (deny(headers.get(Http::Headers::get().Path)->value().c_str())) {
    callbacks_->requestInfo().setResponseFlag(
        RequestInfo::ResponseFlag::FaultInjected);
    Http::Utility::sendLocalReply(*callbacks_, false, Http::Code::Forbidden,
                                  "");
    return FilterHeadersStatus::StopIteration;
  }
  return FilterHeadersStatus::Continue;
};

}  // namespace gke
