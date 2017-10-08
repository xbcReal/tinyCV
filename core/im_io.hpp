/*
* @file im_io.hpp
* @brief some io operation for Image
*/
#pragma once
#include "image.hpp"


extern "C" 
{
#include "jpeglib.h"
}
namespace tinycv
{
    void write_raw_data_to_jpeg_file(const char* jpegFileName, unsigned char* inputData,
        int nWidth, int nHeight, int nChannel, int nQuality=100);
	int read_raw_data_from_jpeg_file(const char *filename,
		unsigned char** outputData,
		int& nWidth, int& nHeight, int &nChannel);

	Image_<uchar> imread(const char *filename)
	{
		unsigned char* data;
		int nWidth, nHeight, nChannel;
		if (0!=read_raw_data_from_jpeg_file(filename, &data, nWidth, nHeight, nChannel))
		{
			std::cout << "read error" << std::endl;
			return Image_<uchar>();
		}
		
		Image_<uchar>  dst(nHeight, nWidth, nChannel, 0, data);
		delete[]data;
		return dst;
	}

    int imsave(const Image_<uchar> &image,const char *filename)
    {
        long count =  image.rows()*image.cols()*image.channels(); 
        uchar *raw_data = new uchar[count];
        for(int i=0;i<image.rows();i++)
        {
            for(int j=0;j<image.cols();j++)
            {
                for(int c=0;c<image.channels();c++)
                {
                    raw_data[i*image.cols()*image.channels() + j*image.channels() + c] =
                                              image._dvec[c][i][j];
                }

            }
        }
		
        write_raw_data_to_jpeg_file(filename, raw_data,
            image.cols(), image.rows(),image.channels());

        delete []raw_data;


        return 0;
    }
    
    //  inputData:  (RGB,RGB,....)
    void write_raw_data_to_jpeg_file(const char* jpegFileName, unsigned char* inputData,
        int nWidth, int nHeight, int nChannel, int nQuality)
    {
        /* This struct contains the JPEG compression parameters and pointers to
        * working space (which is allocated as needed by the JPEG library).
        * It is possible to have several such structures, representing multiple
        * compression/decompression processes, in existence at once.  We refer
        * to any one struct (and its associated working data) as a "JPEG object".
        */
        struct jpeg_compress_struct cinfo;
    
        /* This struct represents a JPEG error handler.  It is declared separately
        * because applications often want to supply a specialized error handler
        * (see the second half of this file for an example).  But here we just
        * take the easy way out and use the standard error handler, which will
        * print a message on stderr and call exit() if compression fails.
        * Note that this struct must live as long as the main JPEG parameter
        * struct, to avoid dangling-pointer problems.
        */
        struct jpeg_error_mgr jerr;
    
        /* More stuff */
        FILE *outfile;                  /* target file */
        JSAMPROW row_pointer[1];        /* pointer to JSAMPLE row[s] */
        int     row_stride;             /* physical row width in image buffer */
    
        /* Step 1: allocate and initialize JPEG compression object */
    
        /* We have to set up the error handler first, in case the initialization
        * step fails.  (Unlikely, but it could happen if you are out of memory.)
        * This routine fills in the contents of struct jerr, and returns jerr's
        * address which we place into the link field in cinfo.
        */
        cinfo.err = jpeg_std_error(&jerr);
    
        /* Now we can initialize the JPEG compression object. */
        jpeg_create_compress(&cinfo);  /* Now we can initialize the JPEG compression object. */
    
        /* Step 2: specify data destination (eg, a file) */
        /* Note: steps 2 and 3 can be done in either order. */
    
        /* Here we use the library-supplied code to send compressed data to a
        * stdio stream.  You can also write your own code to do something else.
        * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
        * requires it in order to write binary files.
        */
        if ((outfile = fopen(jpegFileName, "wb")) == NULL)
        {
            fprintf(stderr, "can't open %s\n", jpegFileName);
            return;
        }
        jpeg_stdio_dest(&cinfo, outfile);
    
        /* Step 3: set parameters for compression */
        /* First we supply a description of the input image.
        * Four fields of the cinfo struct must be filled in:
        */
        cinfo.image_width = nWidth;                /* image width and height, in pixels */
        cinfo.image_height = nHeight;
        cinfo.input_components = nChannel;         /* # of color components per pixel */
    
        if (nChannel == 1)
        {
            cinfo.in_color_space = JCS_GRAYSCALE;  /* colorspace of input image */
        }
        else if (nChannel == 3)
        {
            cinfo.in_color_space = JCS_RGB;        /* colorspace of input image */
        }
        
        /* Now use the library's routine to set default compression parameters.
        * (You must set at least cinfo.in_color_space before calling this,
        * since the defaults depend on the source color space.)
        */
        jpeg_set_defaults(&cinfo);
    
        // Now you can set any non-default parameters you wish to.
        // Here we just illustrate the use of quality (quantization table) scaling:
        jpeg_set_quality(&cinfo, nQuality, TRUE); /* limit to baseline-JPEG values */
    
        /* Step 4: Start compressor */
        /* TRUE ensures that we will write a complete interchange-JPEG file.
        * Pass TRUE unless you are very sure of what you're doing.
        */
        jpeg_start_compress(&cinfo, TRUE);
    
        /* Step 5: while (scan lines remain to be written) */
        /*           jpeg_write_scanlines(...); */
    
        /* Here we use the library's state variable cinfo.next_scanline as the
        * loop counter, so that we don't have to keep track ourselves.
        * To keep things simple, we pass one scanline per call; you can pass
        * more if you wish, though.
        */
        row_stride = nWidth * nChannel; /* JSAMPLEs per row in image_buffer */
        while (cinfo.next_scanline < cinfo.image_height)
        {
            /* jpeg_write_scanlines expects an array of pointers to scanlines.
            * Here the array is only one element long, but you could pass
            * more than one scanline at a time if that's more convenient.
            */
            row_pointer[0] = &inputData[cinfo.next_scanline * row_stride];
            (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }
        /* Step 6: Finish compression */
        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);
        /* After finish_compress, we can close the output file. */
        fclose(outfile);
    }

	int read_raw_data_from_jpeg_file(const char *filename,
		unsigned char** outputData,
		int& nWidth, int& nHeight, int &nChannel)
	{
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		FILE *input_file;
		FILE *output_file;
		JSAMPARRAY buffer;
		int row_width;

		unsigned char *output_buffer;
		unsigned char *tmp = NULL;

		cinfo.err = jpeg_std_error(&jerr);

		if ((input_file = fopen(filename, "rb")) == NULL) 
		{
			fprintf(stderr, "can't open %s\n", filename);
			return -1;
		}

		// Initialization of JPEG compression objects  
		jpeg_create_decompress(&cinfo);

		/* Specify data source for decompression */
		jpeg_stdio_src(&cinfo, input_file);
		(void)jpeg_read_header(&cinfo, TRUE);
		(void)jpeg_start_decompress(&cinfo);

		nWidth = cinfo.output_width;
		nChannel = cinfo.output_components;
		nHeight = cinfo.output_height;

		
		buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr)&cinfo, JPOOL_IMAGE, nWidth*nChannel, 1);

		

		*outputData = (unsigned char *)malloc(nWidth*nChannel * nHeight);
		memset(*outputData, 0, nWidth*nChannel * nHeight);
		tmp = *outputData;

		while (cinfo.output_scanline < cinfo.output_height)
		{
			(void)jpeg_read_scanlines(&cinfo, buffer, 1);

			memcpy(tmp, *buffer, nWidth*nChannel);
			tmp += nWidth*nChannel;
		}
		(void)jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		fclose(input_file);
		return 0;
	}

}//namespace tinycv
