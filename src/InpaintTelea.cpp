#include "shared.h"

struct TELEAData final {
	VSNode* node;
	VSNode* node2;
	const VSVideoInfo* vi;
	float radius;
};

static void telea_process_gray(const VSFrame* src, const VSFrame* src2, VSFrame* dst, const TELEAData* const VS_RESTRICT d, const VSAPI* vsapi) noexcept {
	const int w = vsapi->getFrameWidth(src, 0);
	const int h = vsapi->getFrameHeight(src, 0);
	ptrdiff_t stride = vsapi->getStride(src, 0);
	const uint8_t* srcp = vsapi->getReadPtr(src, 0);
	const uint8_t* maskp = vsapi->getReadPtr(src2, 0);
	uint8_t* dstp = vsapi->getWritePtr(dst, 0);

	cv::Mat srcImg(cv::Size(w, h), CV_8UC1);
	cv::Mat maskImg(cv::Size(w, h), CV_8UC1);
	cv::Mat dstImg(cv::Size(w, h), CV_8UC1);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			srcImg.at<uint8_t>(y, x) = srcp[x];
		}
		srcp += stride;
	}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			maskImg.at<uint8_t>(y, x) = maskp[x];
		}
		maskp += stride;
	}

	cv::inpaint(srcImg, maskImg, dstImg, d->radius, cv::INPAINT_TELEA);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			dstp[x] = dstImg.at<uint8_t>(y, x);
		}
		dstp += stride;
	}
}

static void telea_process_rgb(const VSFrame* src, const VSFrame* src2, VSFrame* dst, const TELEAData* const VS_RESTRICT d, const VSAPI* vsapi) noexcept {
	const int w = vsapi->getFrameWidth(src, 0);
	const int h = vsapi->getFrameHeight(src, 0);
	ptrdiff_t stride = vsapi->getStride(src, 0);

	const uint8_t* maskp = vsapi->getReadPtr(src2, 0);
	const uint8_t* srcp_r = vsapi->getReadPtr(src, 0);
	const uint8_t* srcp_g = vsapi->getReadPtr(src, 1);
	const uint8_t* srcp_b = vsapi->getReadPtr(src, 2);
	uint8_t* dstp_r = vsapi->getWritePtr(dst, 0);
	uint8_t* dstp_g = vsapi->getWritePtr(dst, 1);
	uint8_t* dstp_b = vsapi->getWritePtr(dst, 2);

	cv::Mat maskImg(cv::Size(w, h), CV_8UC1);
	cv::Mat srcImg_b(cv::Size(w, h), CV_8UC1);
	cv::Mat srcImg_g(cv::Size(w, h), CV_8UC1);
	cv::Mat srcImg_r(cv::Size(w, h), CV_8UC1);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			srcImg_b.at<uint8_t>(y, x) = srcp_b[x];
		}
		srcp_b += stride;
	}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			srcImg_g.at<uint8_t>(y, x) = srcp_g[x];
		}
		srcp_g += stride;
	}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			srcImg_r.at<uint8_t>(y, x) = srcp_r[x];
		}
		srcp_r += stride;
	}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			maskImg.at<uint8_t>(y, x) = maskp[x];
		}
		maskp += stride;
	}

	cv::Mat srcImg_bgr(cv::Size(w, h), CV_8UC3);
	cv::Mat dstImg_bgr(cv::Size(w, h), CV_8UC3);
	std::vector<cv::Mat> src_channels(3);
	std::vector<cv::Mat> dst_channels(3);

	srcImg_b.convertTo(src_channels[0], CV_8U);
	srcImg_g.convertTo(src_channels[1], CV_8U);
	srcImg_r.convertTo(src_channels[2], CV_8U);

	merge(src_channels, srcImg_bgr);
	cv::inpaint(srcImg_bgr, maskImg, dstImg_bgr, d->radius, cv::INPAINT_TELEA);

	split(dstImg_bgr, dst_channels);
	cv::Mat dstImg_b = dst_channels[0];
	cv::Mat dstImg_g = dst_channels[1];
	cv::Mat dstImg_r = dst_channels[2];

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			dstp_b[x] = dstImg_b.at<uint8_t>(y, x);
		}
		dstp_b += stride;
	}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			dstp_g[x] = dstImg_g.at<uint8_t>(y, x);
		}
		dstp_g += stride;
	}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			dstp_r[x] = dstImg_r.at<uint8_t>(y, x);
		}
		dstp_r += stride;
	}
}

static const VSFrame* VS_CC teleaGetFrame(int n, int activationReason, void* instanceData, void** frameData, VSFrameContext* frameCtx, VSCore* core, const VSAPI* vsapi) {
	auto d{ static_cast<TELEAData*>(instanceData) };

	if (activationReason == arInitial) {
		vsapi->requestFrameFilter(n, d->node, frameCtx);
		vsapi->requestFrameFilter(n, d->node2, frameCtx);
	}
	else if (activationReason == arAllFramesReady) {
		const VSFrame* src = vsapi->getFrameFilter(n, d->node, frameCtx);
		const VSFrame* src2 = vsapi->getFrameFilter(n, d->node2, frameCtx);

		const VSVideoFormat* fi = vsapi->getVideoFrameFormat(src);
		int height = vsapi->getFrameHeight(src, 0);
		int width = vsapi->getFrameWidth(src, 0);
		VSFrame* dst = vsapi->newVideoFrame(fi, width, height, src, core);

		if (d->vi->format.colorFamily == cfGray) {
			telea_process_gray(src, src2, dst, d, vsapi);
		}
		else {
			telea_process_rgb(src, src2, dst, d, vsapi);
		}

		vsapi->freeFrame(src);
		vsapi->freeFrame(src2);
		return dst;
	}
	return nullptr;
}

static void VS_CC teleaFree(void* instanceData, VSCore* core, const VSAPI* vsapi) {
	auto d{ static_cast<TELEAData*>(instanceData) };
	vsapi->freeNode(d->node);
	vsapi->freeNode(d->node2);
	delete d;
}

void VS_CC teleaCreate(const VSMap* in, VSMap* out, void* userData, VSCore* core, const VSAPI* vsapi) {
	auto d{ std::make_unique<TELEAData>() };
	int err{ 0 };

	d->node = vsapi->mapGetNode(in, "clip", 0, nullptr);
	d->node2 = vsapi->mapGetNode(in, "mask", 0, nullptr);
	d->vi = vsapi->getVideoInfo(d->node);
	const VSVideoInfo* vi2 = vsapi->getVideoInfo(d->node2);

	d->radius = vsapi->mapGetFloatSaturated(in, "radius", 0, &err);
	if (err)
		d->radius = 3.0f;

	if (d->vi->format.bytesPerSample != 1 ||
		(d->vi->format.colorFamily != cfGray && d->vi->format.colorFamily != cfRGB)) {
		vsapi->mapSetError(out, "InpaintTelea: only Gray8 and RGB24 formats supported.");
		vsapi->freeNode(d->node);
		vsapi->freeNode(d->node2);
		return;
	}

	if (vi2->format.bytesPerSample != 1) {
		vsapi->mapSetError(out, "InpaintTelea: mask clip should be in 8-bit format.");
		vsapi->freeNode(d->node);
		vsapi->freeNode(d->node2);
		return;
	}

	if (d->vi->width != vi2->width ||
		d->vi->height != vi2->height) {
		vsapi->mapSetError(out, "InpaintTelea: main clip and mask clip must have the same size.");
		vsapi->freeNode(d->node);
		vsapi->freeNode(d->node2);
		return;
	}

	if (d->vi->numFrames != vi2->numFrames) {
		vsapi->mapSetError(out, "InpaintTelea: main clip and mask clip must have the same number of frames.");
		vsapi->freeNode(d->node);
		vsapi->freeNode(d->node2);
		return;
	}

	VSFilterDependency deps[]{ {d->node, rpGeneral}, {d->node2, rpGeneral} };
	vsapi->createVideoFilter(out, "InpaintTelea", d->vi, teleaGetFrame, teleaFree, fmParallel, deps, 2, d.get(), core);
	d.release();
}