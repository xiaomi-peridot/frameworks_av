/*
 * Copyright 2024, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef C2_OMX_NODE_H_
#define C2_OMX_NODE_H_

#include <android/IOMXBufferSource.h>
#include <codec2/hidl/client.h>
#include <media/IOMX.h>
#include <media/OMXBuffer.h>

namespace android {

struct C2NodeImpl;

/**
 * IOmxNode implementation around codec 2.0 component, only to be used in
 * IGraphicBufferSource::configure. Only subset of IOmxNode API is implemented
 * and others are left as stub. As a result, one cannot expect this IOmxNode
 * to work in any other usage than IGraphicBufferSource.
 */
struct C2OMXNode : public BnOMXNode {
    explicit C2OMXNode(const std::shared_ptr<Codec2Client::Component> &comp);
    ~C2OMXNode() override = default;

    // IOMXNode
    status_t freeNode() override;
    status_t sendCommand(OMX_COMMANDTYPE cmd, OMX_S32 param) override;
    status_t getParameter(
            OMX_INDEXTYPE index, void *params, size_t size) override;
    status_t setParameter(
            OMX_INDEXTYPE index, const void *params, size_t size) override;
    status_t getConfig(
            OMX_INDEXTYPE index, void *params, size_t size) override;
    status_t setConfig(
            OMX_INDEXTYPE index, const void *params, size_t size) override;
    status_t setPortMode(OMX_U32 port_index, IOMX::PortMode mode) override;
    status_t prepareForAdaptivePlayback(
            OMX_U32 portIndex, OMX_BOOL enable,
            OMX_U32 maxFrameWidth, OMX_U32 maxFrameHeight) override;
    status_t configureVideoTunnelMode(
            OMX_U32 portIndex, OMX_BOOL tunneled,
            OMX_U32 audioHwSync, native_handle_t **sidebandHandle) override;
    status_t getGraphicBufferUsage(
            OMX_U32 port_index, OMX_U32* usage) override;
    status_t setInputSurface(
            const sp<IOMXBufferSource> &bufferSource) override;
    status_t allocateSecureBuffer(
            OMX_U32 port_index, size_t size, buffer_id *buffer,
            void **buffer_data, sp<NativeHandle> *native_handle) override;
    status_t useBuffer(
            OMX_U32 port_index, const OMXBuffer &omxBuf, buffer_id *buffer) override;
    status_t freeBuffer(
            OMX_U32 port_index, buffer_id buffer) override;
    status_t fillBuffer(
            buffer_id buffer, const OMXBuffer &omxBuf, int fenceFd) override;
    status_t emptyBuffer(
            buffer_id buffer, const OMXBuffer &omxBuf,
            OMX_U32 flags, OMX_TICKS timestamp, int fenceFd) override;
    status_t getExtensionIndex(
            const char *parameter_name,
            OMX_INDEXTYPE *index) override;
    status_t dispatchMessage(const omx_message &msg) override;

    /**
     * Returns underlying IOMXBufferSource object.
     */
    sp<IOMXBufferSource> getSource();

    /**
     * Configure the frame size.
     */
    void setFrameSize(uint32_t width, uint32_t height);

    /**
     * Notify that the input buffer reference is no longer needed by the component.
     * Clean up if necessary.
     *
     * \param index input work index
     */
    void onInputBufferDone(c2_cntr64_t index);

    /**
     * Notify input buffer is emptied.
     */
    void onInputBufferEmptied();

    /**
     * Returns dataspace information from GraphicBufferSource.
     */
    android_dataspace getDataspace();

    /**
     * Returns dataspace information from GraphicBufferSource.
     */
    uint32_t getPixelFormat();

    /**
     * Sets priority of the queue thread.
     */
    void setPriority(int priority);

private:
    std::shared_ptr<C2NodeImpl> mImpl;
};

}  // namespace android

#endif  // C2_OMX_NODE_H_
