#include "shared.h"

VS_EXTERNAL_API(void) VapourSynthPluginInit2(VSPlugin* plugin, const VSPLUGINAPI* vspapi) {
	vspapi->configPlugin("com.julek.cv_inpaint", "cv_inpaint", "Image Inpainting", VS_MAKE_VERSION(1, 0), VAPOURSYNTH_API_VERSION, 0, plugin);
	vspapi->registerFunction("InpaintTelea", "clip:vnode;mask:vnode;radius:float:opt;", "clip:vnode;", teleaCreate, nullptr, plugin);
	vspapi->registerFunction("InpaintNS", "clip:vnode;mask:vnode;radius:float:opt;", "clip:vnode;", nsCreate, nullptr, plugin);
	vspapi->registerFunction("InpaintFSR", "clip:vnode;mask:vnode;", "clip:vnode;", fsrCreate, nullptr, plugin);
	vspapi->registerFunction("InpaintShiftmap", "clip:vnode;mask:vnode;", "clip:vnode;", shiftmapCreate, nullptr, plugin);
}