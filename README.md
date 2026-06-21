# IMLIB (IMage LIBrary)

A library for basic image processing operations.

## Roadmap

- [x] Tensor Representation for Images
- [x] File I/O

### Geometric Transformations

- [ ] Cropping / Flipping / Rotating
- [ ] Scaling with interpolation

### Pixel Manipulation

- [x] Color space conversion (e.g. RGB to Grayscale)
- [ ] Thersholding
- [ ] Contrast and brightness
- [ ] Histogram Equalization

### Spacial Filtering

- [x] Blurring: Gaussian, Median and Box filters.
- [ ] Edge Detection: Sobel, Perwitt and Canny.
- [ ] Sharpening: Laplacian and Unsharp.
- [ ] Custom convolution i.e. allow user to provide his own kernel.

### Morphological Operations

- [ ] Erosion and Dilation.
- [ ] Opening and Closing.

### Performance Optimization

- [x] Hardware acceleration with CUDA.
- [ ] Parallelize code if running on cpu.

### External Bindings

- [ ] Python Binding.
