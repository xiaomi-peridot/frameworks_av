/*
 * Copyright (C) 2024 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <aidl/android/hardware/audio/core/BnTelephony.h>

using namespace aidl::android::hardware::audio::core;
using namespace aidl::android::media::audio::common;

namespace aidl::android::hardware::audio::core {

class TelephonyMock : public BnTelephony {
  public:
    ndk::ScopedAStatus getSupportedAudioModes(std::vector<AudioMode>*) override {
        return ndk::ScopedAStatus::ok();
    }
    ndk::ScopedAStatus switchAudioMode(AudioMode) override { return ndk::ScopedAStatus::ok(); }
    ndk::ScopedAStatus setTelecomConfig(const ITelephony::TelecomConfig&,
                                        ITelephony::TelecomConfig*) override {
        return ndk::ScopedAStatus::ok();
    }
};

}  // namespace aidl::android::hardware::audio::core
