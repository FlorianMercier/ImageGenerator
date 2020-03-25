# View Generator <!-- omit in toc -->

## Pitch <!-- omit in toc -->

The goal of this project is to randomly generate landscape images.

## Table of Contents <!-- omit in toc -->

- [Project environment](#project-environment)
- [Generate Bitmap Images](#generate-bitmap-images)
  - [Little-endian format](#little-endian-format)
  - [Bitmap file header](#bitmap-file-header)
  - [Bitmap information header](#bitmap-information-header)
  - [Pixel structure](#pixel-structure)
- [Multi-Dimensional Perlin Noise Grid](#multi-dimensional-perlin-noise-grid)
  - [Perlin Noise](#perlin-noise)
  - [Grids definitions](#grids-definitions)
  - [Dot Product](#dot-product)
  - [Interpolation in n dimensions](#interpolation-in-n-dimensions)
- [Authors](#authors)

## Project environment

| Purpose     | Software     |
| :---------- | :----------- |
| Versioning  | Git          |
| Indentation | Clang-format |
| Developing  | VSCode       |
| Language    | C++          |
| Compiling   | MinGW        |
| Figures     | SVG          |

## Generate Bitmap Images

Bitmap images are used as output for the generator. Their structure consists of two fixed-size headers followed by a variable-sized structure of pixels.
All of the integer values are stored in little-endian format.

- [More about BMP File Format](https://en.wikipedia.org/wiki/BMP_file_format)

### Little-endian format

The least significant byte value is at the lowest address. The other bytes follow in increasing order of significance.

| Decimal value | Hexa little-endian | Hexa big-endian |
| :-----------: | :----------------: | :-------------: |
|      24       |       18 00        |      00 18      |

### Bitmap file header

The first header contains general information about the file.

| Offset | Size (Bytes) | Purpose                                | Value (decimal) |
| :----: | :----------: | :------------------------------------- | :-------------: |
|   00   |      2       | Header field used to identify the file |      'BM'       |
|   02   |      4       | Size of the BMP file in bytes          |        *        |
|   06   |      4       | Reserved (creator information)         |        0        |
|   10   |      4       | Starting byte of the pixel array       |       54        |

### Bitmap information header

The second header contains information about the image.

| Offset | Size (Bytes) | Purpose                                        | Value (decimal) |
| :----: | :----------: | :--------------------------------------------- | :-------------: |
|   14   |      4       | Size of this header in bytes                   |       40        |
|   18   |      4       | Bitmap width in pixels                         |        *        |
|   22   |      4       | Bitmap height in pixels                        |        *        |
|   26   |      2       | Number of color planes                         |        1        |
|   28   |      2       | Number of bits per pixel (rgb 3x8)             |       24        |
|   30   |      4       | Compression method being used                  |        0        |
|   34   |      4       | Image size                                     |        0        |
|   38   |      4       | Image horizontal resolution in pixel per meter |        0        |
|   42   |      4       | Image vertical resolution in pixel per meter   |        0        |
|   46   |      4       | Number of colors                               |        0        |
|   50   |      4       | Number of important colors                     |        0        |

### Pixel structure

The data structure contains all the image pixels. They are stored from the bottom left to the top right.
A pixel is composed of 1 Byte for each color. Here the RGB format is used, pixels are 3 Bytes long.
The padding is an array of Bytes set to 0. It is added at the end of each row so that the total width of the image is a multiple of 4 Bytes.

| Offset | Size (Bytes) | Purpose      | Value (decimal) |
| :----: | :----------: | :----------- | :-------------: |
|   54   |      3       | Pixel\[0,2\] |        *        |
|   57   |      3       | Pixel\[1,2\] |        *        |
|   60   |      3       | Pixel\[2,2\] |        *        |
|   63   |      3       | Padding      |        0        |
|   66   |      3       | Pixel\[0,1\] |        *        |
|   69   |      3       | Pixel\[1,1\] |        *        |
|   72   |      3       | Pixel\[2,1\] |        *        |
|   75   |      3       | Padding      |        0        |
|   78   |      3       | Pixel\[0,0\] |        *        |
|   81   |      3       | Pixel\[1,0\] |        *        |
|   84   |      3       | Pixel\[2,0\] |        *        |
|   87   |      3       | Padding      |        0        |

## Multi-Dimensional Perlin Noise Grid

### Perlin Noise

A Perlin Noise is a gradient noise developed by Ken Perlin in 1983. A list of random numbers could seem chaotic and produce a discontinuous aspect. This algorithm uses randomly generated gradients to create a continuous yet random evolution. The main goal of this technic is to create harmonic randomness closer to what nature looks like.

- [More about Perlin Noise](https://en.wikipedia.org/wiki/Perlin_noise)

### Grids definitions

This project uses two grids composed of hypercubes as base elements, but any type of grid could be used. Hypercubes are squares in 2D and cubes in 3D. A grid is represented here as a multi-dimensional array.

Let be n, a strictly positive natural number, the chosen dimension of the noise.

- The first grid is a n-dimensional array. Each element of the array is a real number between 0 and 1. This number is the value of the noise at the local corresponding point. In two dimensions it could be seen as a pixel.
- The second grid is a n-dimensional array where every element is a n-dimensional vector. Therefore, this grid can also be seen as a 2n-dimensional array of real numbers. It represents the gradient grid. Every cell contains a random gradient vector, normalized and of the same dimension as the noise. If it is considered as an array of vectors, the gradient grid is smaller than the noise grid by a factor called the period. If one dimension of the noise grid were h, the corresponding dimension of the gradient grid would be h/p +1, where p is the period.

A good representation for those two grids is to picture the noise grid as the structure, on which there is, every p noise cells, a gradient cell, containing its vector. Therefore, every noise cell has 2 to the n-th power surrounding gradient cells.

![Grid Definition](./Images/grids_definition.svg)
>Figure 1: 2D Noise grid and Gradient grid.\
>The current point is in grey in its current cell in dark blue. The gradient cells are in red. The blue vectors are the distance vectors between the point and the surrounding gradient cells. The purples vectors are the gradient vectors of each gradient cell. The blue square is the local grid which is inside a lighter blue square which is a part of the noise grid.

### Dot Product

For each point of the noise grid there are two kinds of vectors:

- The distance vectors, calculated between the current point and all the surrounding gradient cells.
- The gradient vectors of each cell of the gradient grid.

To compute the distance vectors the grid has to be graduated.
Let c be the number of the current noise cell and p the period of the gradient cells.
The distance vectors coordinates are represented along a dimension by (c+(1/2))/p.

![Local Grid Distance](Images/local_grid_distance.svg)
>Figure 2: 2D Local Grid coordinates of each point.\
>The coordinates do not start from 0 and do not reach 1 to give only non zero distance values to the 4 corner noise cells.

The distance vectors coordinates are the result of the point coordinates subtracted by the gradient cell coordinates. An easy way to get every gradient cell coordinates in the right order for n dimensions is through the gray binary code. Indeed, going from a gradient cell to another is moving along only 1 dimension.

![Local Grid Calculation](Images/local_grid_calculation.svg)
>Figure 3: 2D Local Grid calculation of distance vectors.\
>d1 = [0.625,0.375] - [0,0] = [0.625,0.375]\
>d2 = [0.625,0.375] - [1,0] = [-0.375,0.375]\
>d3 = [0.625,0.375] - [1,1] = [-0.375,-0.625]\
>d4 = [0.625,0.375] - [0,1] = [0.625,-0.625]\
>Here it can be seen that the coordinates of the gradient cells are [00], [10], [11], [01] and the Gray Binary Code is 00, 01, 11, 10. Therefore an inverted Gray Binary Code, with the least important values first, can be used to automate this calculation algorithm.

The dot products of distance vectors and their corresponding gradient vectors have to be calculated. The results can be stored in a temporary array, they will be interpolated to give the noise point its final value.

![Local Gird Dot Product](Images/local_grid_dot_product.svg)
>Figure 4: A local grid and the results of the dot products.\
>This is a fragment of the noise grid starting at coordinates [X,Y]. The local grid is in blue. The dot products have been calculated and stored as A, B, C and D.

### Interpolation in n dimensions

Those 2 to the n-th power dot products have to be interpolated. That means they are summed up with a certain weight depending on how far they are from the current point. The trick to make this algorithm is through a recursive function. Indeed, a n-dimensional interpolation is a 1D interpolation between the two n-1-dimensional interpolations of each half of the dot products. The dot values have to be in the correct order because every time the interpolation is split into 2 smaller interpolations the first one has its dot product values in the right order but the second one has them inverted.

![Linear Interpolation](Images/linear_interpolation.svg)
>Figure 5: 2D bilinear interpolation.\
>It is a barycentric system. Note that values C and D have been inverted. It is because the interpolation wanted is DC and not CD, so to keep turning in a trigonometric way and still get the right interpolation using the same algorithm, the Gray binary code can be used. The k-th value is switched the k'-th one, where k' is the corresponding Gray value of k.

![Trilinear Interpolation](Images/trilinear_interpolation.svg)
>Figure 6: 3D Gradient grid with a current noise point being calculated.\
>Values C and D are not in the right order, C has a weight of 1-X and not X. So, to apply this algorithm the values have to be reorganized. Here again, the Gray binary code can be used. The k-th value is switched the k'-th vertex of the gradient grid where k' is the corresponding Gray value of k.

For Perlin Noise it is not a linear interpolation. A Smoothstep function is used so that the weight is not proportional to the distance between the gradient cell and the current point. This is a trick to avoid artifact of the noise.

![Smoothstep Function](./Images/smoothstep.svg)
>Figure 7: Plot of a Smoothstep function.\
>The effect is making the weight of a dot product value less important if the point is far from it.

The non-linear interpolation can be seen as a basic interpolation where coordinates have been moved before the calculation.

![Non Linear Interpolation Grid](Images/non_linear.svg)
>Figure 8: Deformed Grid during a non-linear interpolation.\
>The gray points are the points before the Smoothstep function and the blue point are the new points after. A basic interpolation is made with the coordinates of those new points

With all those steps a Perlin Noise grid is created. A harmonic random variation through multiple dimensions. The finale noise value can be normalized between 0 and 1. A way to get more vibrant values is to use a sigmoid function on every output values, increasing the noise contrast.

![Perlin Noise 3D](Images/perlin_noise_3d.gif)
>Figure 9: 3D Perlin Noise represented as a 2D slice moving along the 3rd dimension through time.

## Authors

Florian Mercier
