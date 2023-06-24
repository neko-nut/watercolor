## Abstract
Watercolor rendering is a kind of non-photorealistic rendering method. It uses a specific way to render the object and product a watercolor appearance. One of the key steps in watercolor rendering is wet-in-wet and wet-to-dry painting. It simulates the wet color blocks painted on the wet paper or the dry paper. In this assignment, I have implemented a wet-in-wet and wet-to-dry simulation on the edge of the color blocks. I use the convolution function to detect the edge, and use Gaussian blur to extract the edge. Then use interpolation to smooth the color change and apply the darker edge layer to the original object. The result looks like the drawing of the color block on the wet paper and the edge is blending with the paper.

## Background

Dr Thomas Luft and Oliver Deussen had created a high-quality real-time watercolor illustration of plants with watercolor rendering and blurred depth tests. In general, this illustration has three steps. First, segment the scene. Then implement the watercolor rendering. The third step is calculating the diffuse lighting, and finally, generating the shadow. 

The first step is to define the segmentation of the scene. It assigns an ID to the object, separates the color channels, and creates the ID image for each channel. This ID image is important in the following steps.

In this approach, the way they implement watercolor rendering is the methods they described in the article Interactive Watercolor Animations. In this article, they use a Gaussian filter to create an abstract smooth edge for each ID image. Then implement the wet-in-wet and wet-to-dry algorithms. It simulates the appearance of the color and water blending with the paper. Then darker the edge and use an intensity texture to simulate the paper.

The following step is adding diffuse lighting. The algorithm for diffuse lighting in this approach is Gooch. It uses warm colors and cool colors to adjust the surface. The warm and cool color is defined by the programmer and be the lighter and darker color of the object normally. 

The last step is the soft shadow rendering. The blurred depth test has been used in this step. This algorithm is mainly used to smooth the change of the depth, and soft shadow generate. It adds a semi-visible layer to define the continuous visibility between the traditional visible and invisible layers. To calculate this layer, both the basic binary visibility and the smooth step function are needed. The binary visibility should be stored as an intermediary result. Then use the smooth step function to calculate the depth for each fragment. The result has been stored in an RGBA texture and blended with the original objects.

## Implementation details

 I use three cubes to display the rendering result. The color of these cubes is red, blue, and green. The front part of the cube is lighter, and the side part is darker.

 ### Framebuffer

The framebuffer is a space in the GPU memory. It can be used to store some useful information like the color value or the depth information. OpenGL has a default framebuffer to store the rendering result when we create the window. In this approach, we need to apply some image processing algorithms to the rendering, so we have to create our own framebuffer. This framebuffer contains the object with its color and depth and the way to render it. 

Then we attach the data in the framebuffer to a 2D texture. We use OpenGL to generate a virtual texture in the buffer. Then attach the color value in the framebuffer to this 2D texture. We receive a texture that contains the RGBA value on the screen pixel by pixel. We can use it as a normal texture whenever we want. In this approach, all the objects should be generated into the texture and preprocessed before display. So, the size of the texture is equal to the size of the screen and will be displayed directly on the screen after preprocessing.

### Edge Detection

The edge detection algorithm I use is convolution with an edge detection kernel. The convolution is an algorithm that processes the input data with a specific kernel. In image processing, the kernel is always a 3*3 matrix that contains the weight of the pixel and its neighbor. For each pixel in the image, the output is the sum of the pixel and these neighbors multiply by the kernel.

The kernel I used for edge detection is:

It is obvious that, the convolution result will be 0 if the color of the pixel is equal to the color of all its neighbors. Otherwise, the value is larger or lower than 0. So, inside the color block, the result is always 0, and the result will be varied on the edge. 

I use a step function to distinguish the color block and the edge. First, I calculate the absolute value for the RGB channels in the convolution result. If any of this is larger than the threshold, the pixel will be marked as edge. All the edge pixels will be displayed as white. The edge detection result has been stored in another texture.

### Extend Edge

The width of the edge in edge detection is always 1 or 2 pixels. It is not enough to do the blending. Therefore, we have to extend the edge.

The function I used to extend the edge is 2D Gaussian blur. It use the Gaussian distribution to process the pixel and the pixel around it. In Gaussian blur, we also process the image pixel by pixel. The value of each pixel is equal to the sum of itself and its neighbors multiply by the weight. The weight of its neighbor is calculated by the Gaussian distribution. The 2D Gaussian distribution equation is:

The x and y value are depends on the distance between the pixel and its neighbor, and other values are define by myself.

Based on the equation, we can easily figure out that the value is higher if the pixel is close to the edge. Then I define the number of relevant neighbor and the threshold. They are used to define the width of the edges we generate. The number of relevant neighbors define the maximum distance we care about, and the threshold define the value of the edge. If the value of the pixel is larger than the threshold, it will be defined as the edge.

### Smooth Change 

The color should be darker at edge and change to the original color gradient. Therefore, an interpolation function is needed to smooth the change. In this approach, I use the ease-out function.

The ease-out function is a kind of interpolation function that changes quicker in the beginning and slower in the end.

### Translucent Layer

In this approach, instead of modifying the original texture directly, I use the translucent layer to darker the edge. The color of this layer is darker than the original object, and the transparency depends on the value we calculated before. The transparency is higher when the pixel is close to the edge, and lower when it is far from the edge. Therefore, at the edge, we can only see the darker color, and it gradually blends with the original color. I use the basic blending function in OpenGL to blend the translucent layer and the original layer.

This design is open to improvement and adding new things. For instance, if we want to add the shadow, we can easily add a new translucent layer without modifying the original one. So different layers will not affect each other, which provides the freedom to add and modify any appearance we want.
