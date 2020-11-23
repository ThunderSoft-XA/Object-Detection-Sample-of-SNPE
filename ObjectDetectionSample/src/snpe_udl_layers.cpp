#include <algorithm>
#include <iostream>
#include <cstring>
#include <iterator>
#include <numeric>

#include "snpe_udl_layers.h"

namespace engine_spne_udl {

    bool UdlShapeIndexPatch::setup(void *cookie, size_t insz, const size_t **indim,
                                   const size_t *indimsz, size_t outsz, const size_t **outdim,
                                   const size_t *outdimsz) {

        if (cookie != (void *) 0xdeadbeaf) {
            return false;
        }
        if (insz != 2 or outsz != 1) {
            return false;
        }
        // parse the params
        const void *blob = m_Context.getBlob();
        if (!blob) {
            return false;
        }

        if (!ParseShapeIndexPatchParams(blob, m_Context.getSize(), m_Params)) {
            return false;
        }

        poitNum = 5;

        origin_patch_h = m_Params.origin_patch_h;
        origin_patch_w = m_Params.origin_patch_w;
        origin_h = m_Params.origin_h;
        origin_w = m_Params.origin_w;
        batchSize = indim[0][0];
        feat_h = indim[0][1];
        feat_w = indim[0][2];
        channels = indim[0][3];

        feat_patch_h = origin_patch_h * feat_h / float(origin_h) + 0.5;
        feat_patch_w = origin_patch_w * feat_w / float(origin_w) + 0.5;

        top_size = outsz;

        return true;
    }

    void UdlShapeIndexPatch::close(void *cookie) noexcept {
        if (cookie != (void *) 0xdeadbeaf) {
            return;
        }
        delete this;
        return;
    }

    bool UdlShapeIndexPatch::execute(void *cookie, const float **input,
                                     float **output) {
        if (cookie != (void *) 0xdeadbeaf) {
            return false;
        }

        const float r_h = (feat_patch_h - 1) / 2.0;
        const float r_w = (feat_patch_w - 1) / 2.0;

        int pointUnit = poitNum * 2;
        int inputUnit = feat_h * feat_w * channels;
        int outputUnit = feat_patch_h * feat_patch_w * poitNum * channels;

        for (int batch = 0; batch < batchSize; batch++) {
            for (int i = 0; i < poitNum; i++) {
                // coordinate of the first patch pixel, scale to the feature map coordinate
                const int y = input[1][batch * pointUnit + 2 * i + 1] * (feat_h - 1) - r_h + 0.5;
                const int x = input[1][batch * pointUnit + 2 * i] * (feat_w - 1) - r_w + 0.5;

                for (int ph = 0; ph < feat_patch_h; ph++) {
                    for (int pw = 0; pw < feat_patch_w; pw++) {
                        const int y_p = y + ph;
                        const int x_p = x + pw;
                        for (int c = 0; c < channels; c++) {
                            // set zero if exceed the img bound
                            if (y_p < 0 || y_p >= feat_h || x_p < 0 || x_p >= feat_w) {
//						output[i][ph * feat_patch_w * channels + pw * channels
//								+ c] = 0;
                                output[0][batch * outputUnit +
                                          ph * poitNum * feat_patch_w * channels
                                          + (i * feat_patch_w + pw) * channels + c] = 0;
                            } else {
//						output[i][ph * feat_patch_w * channels + pw * channels
//								+ c] = input[0][y_p * feat_w * channels
//								+ x_p * channels + c];
                                output[0][batch * outputUnit +
                                          ph * poitNum * feat_patch_w * channels
                                          + (i * feat_patch_w + pw) * channels + c] =
                                        input[0][batch * inputUnit + y_p * feat_w * channels
                                                 + x_p * channels + c];
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    bool UdlShapeIndexPatch::ParseShapeIndexPatchParams(const void *buffer,
                                                        size_t size,
                                                        ShapeIndexPatchParams &params) {
        if (!ParseCommonLayerParams(buffer, size, m_Params.common_params))
            return false;

        size_t r_size = size - sizeof(CommonLayerParams);
        uint8_t *r_buffer = (uint8_t *) buffer;
        r_buffer += sizeof(CommonLayerParams);

        // origin_patch_h
        if (r_size < sizeof(uint32_t))
            return false;
        params.origin_patch_h = *reinterpret_cast<int *>(r_buffer);
        r_size -= sizeof(uint32_t);
        r_buffer += sizeof(uint32_t);

        if (r_size < sizeof(uint32_t))
            return false;
        params.origin_patch_w = *reinterpret_cast<int *>(r_buffer);
        r_size -= sizeof(uint32_t);
        r_buffer += sizeof(uint32_t);

        if (r_size < sizeof(uint32_t))
            return false;
        params.origin_h = *reinterpret_cast<int *>(r_buffer);
        r_size -= sizeof(uint32_t);
        r_buffer += sizeof(uint32_t);

        if (r_size < sizeof(uint32_t))
            return false;
        params.origin_w = *reinterpret_cast<int *>(r_buffer);

        return true;
    }

} // ns batchrun
