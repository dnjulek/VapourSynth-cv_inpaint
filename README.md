## InpaintTelea

Algorithm proposed by Alexandru Telea ([OpenCV doc](https://docs.opencv.org/5.x/df/d3d/tutorial_py_inpainting.html)).

### Usage
```python
cv_inpaint.InpaintTelea(vnode clip, vnode mask[, float radius=3.0])
```
### Parameters:

- clip\
    A clip to process. 8-bit RGB or GRAY only.
- mask\
    8-bit binarized mask, areas with a value of 255 will be processed, and the ones with 0 won't.\
    Only the first plane will be used.
- radius\
    Radius of a circular neighborhood of each point inpainted that is considered by the algorithm.

## InpaintNS

Navier-Stokes based method ([OpenCV doc](https://docs.opencv.org/5.x/df/d3d/tutorial_py_inpainting.html)).

### Usage
```python
cv_inpaint.InpaintNS(vnode clip, vnode mask[, float radius=3.0])
```
### Parameters:

- clip\
    A clip to process. 8-bit RGB or GRAY only.
- mask\
    8-bit binarized mask, areas with a value of 255 will be processed, and the ones with 0 won't.\
    Only the first plane will be used.
- radius\
    Radius of a circular neighborhood of each point inpainted that is considered by the algorithm.

## InpaintFSR

Rapid Frequency Selective Reconstructiom (FSR) algorithm ([OpenCV doc](https://docs.opencv.org/5.x/dc/d2f/tutorial_xphoto_inpainting.html)).

### Usage
```python
cv_inpaint.InpaintFSR(vnode clip, vnode mask)
```
### Parameters:

- clip\
    A clip to process. 8-bit RGB or GRAY only.
- mask\
    8-bit binarized mask, areas with a value of 255 will be processed, and the ones with 0 won't.\
    Only the first plane will be used.

## InpaintShiftmap

This algorithm searches for dominant correspondences (transformations) of image patches and tries to seamlessly fill-in the area to be inpainted using this transformations.

### Usage
```python
cv_inpaint.InpaintShiftmap(vnode clip, vnode mask)
```
### Parameters:

- clip\
    A clip to process. 8-bit RGB or GRAY only.
- mask\
    8-bit binarized mask, areas with a value of 255 will be processed, and the ones with 0 won't.\
    Only the first plane will be used.