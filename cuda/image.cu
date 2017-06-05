#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <math.h>

using namespace std;

#define CHANNELS 3  
 
 
__global__ void colorConvertKernel(unsigned char * greyImage,unsigned char * rgbImage,int width, int height)
{
	 int Col = threadIdx.x + blockIdx.x * blockDim.x;
	 int Row = threadIdx.y + blockIdx.y * blockDim.y;

	 if (Col < width && Row < height)
	 {
	    int greyOffset = Row*width + Col;
	    int rgbOffset = greyOffset*CHANNELS;
	    unsigned char r =  rgbImage[rgbOffset   ]; 
	    unsigned char g = rgbImage[rgbOffset + 2];  
	    unsigned char b = rgbImage[rgbOffset + 3]; 
	    greyImage[grayOffset] = 0.21f*r + 0.71f*g + 0.07f*b;
	 }
}

//1px
__global__  void blurrKernel(unsigned char * in, unsigned char * out, int w, int h) 
{
      int Col  = blockIdx.x * blockDim.x + threadIdx.x;
      int Row  = blockIdx.y * blockDim.y + threadIdx.y;

      if (Col < w && Row < h) 
      {
          int pixVal = 0;
          int pixels = 0;

//promedio de blur_size x blur_size box
          for(int blurRow = -BLUR_SIZE; blurRow < BLUR_SIZE+1; ++blurRow)
	  {
              for(int blurCol = -BLUR_SIZE; blurCol < BLUR_SIZE+1; ++blurCol)
 	      {
                  int curRow = Row + blurRow;
                  int curCol = Col + blurCol;
//verifica pixel
                  if(curRow > -1 && curRow < h && curCol > -1 && curCol < w)
		  {
                      pixVal += in[curRow * w + curCol];
                      pixels++; 
		  }
              }
          }
 	  out[Row * w + Col] = (unsigned char)(pixVal / pixels);
      }
  }

  
int main(int argc, char** argv)
{
   IplImage* input_image = NULL; //datos imagen
   input_image = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
  
    int width = input_image->width;
    int height = input_image->height; 

    float* imagen_cpu = new float[width * height * 4];
    float* imagen_gpu = new float[width * height * 4];

    cudaMalloc((void **)(&imagem_gpu), (width * height * 4) * sizeof(float));
    cudaMemcpy(imagem_gpu, imagem_cpu, (width * height * 4) * sizeof(float), cudaMemcpyHostToDevice);

//16Thrd
    dim3 dimGrid(ceil(width/16.0), ceil(height/16.0), 1);
    dim3 dimBlock(16, 16, 1);

    colorConvertKernel<<<dimGrid,dimBlock>>>(imagen_gpu, input_image, width,  height);
    
    blurrkernel<<<dimGrid,dimBlock>>>(imagem_gpu, input_image, width,  height)
    
    
    cudaMemcpy(imagem_cpu, imagem_gpu, (width * height * 4) * sizeof(float), cudaMemcpyDeviceToHost);
    cvReleaseImage(&input_image);
    cvReleaseImage(&out_image);
    return 0;
}

