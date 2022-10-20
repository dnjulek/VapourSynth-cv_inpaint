#pragma once

#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/xphoto/inpainting.hpp>
#include "VapourSynth4.h"
#include "VSHelper4.h"

extern void VS_CC teleaCreate(const VSMap* in, VSMap* out, void* userData, VSCore* core, const VSAPI* vsapi);
extern void VS_CC nsCreate(const VSMap* in, VSMap* out, void* userData, VSCore* core, const VSAPI* vsapi);
extern void VS_CC fsrCreate(const VSMap* in, VSMap* out, void* userData, VSCore* core, const VSAPI* vsapi);
extern void VS_CC shiftmapCreate(const VSMap* in, VSMap* out, void* userData, VSCore* core, const VSAPI* vsapi);