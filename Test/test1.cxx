//#include "gdcmJPEG2000Codec.h"
#include "gdcmTransferSyntax.h"
#include "gdcmTrace.h"
#include "gdcmDataElement.h"
#include "gdcmSequenceOfFragments.h"

#include <cstring>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "openjpeg.h"
#include "getopt.h"
#include "gdcm_j2k.h"
#include "gdcm_jp2.h"
//#include "opj_config.h"
//#include "opj_includes.h"
//#include "../libopenjpeg/j2k.h"
//#include "../libopenjpeg/jp2.h"


int main(int argc, char *argv[])
{
  if( argc < 3 )
    {
    std::cerr << argv[0] << " input.jp2 output.dcm" << std::endl;
    return 1;
    }

 const char *filename = argv[1];
 const char *outfilename = argv[2];


  std::ifstream is(filename);
  gdcm::TransferSyntax ts;
  
   opj_dparameters_t parameters;
   parameters.decod_format = 2;

   opj_set_default_decoder_parameters(&parameters);
  // std::cout << parameters.cp_layer;
   //std::cout << parameters.decod_format;
   opj_image_t *image = NULL;
   FILE *fsrc = NULL;
   unsigned char *src = NULL;
   int file_length;
   int num_images;
   int i,imageno;
  // dircnt_t *dirptr = NULL;
   opj_dinfo_t* dinfo = NULL;	/* handle to a decompressor */
   opj_cio_t *cio = NULL;
   opj_codestream_info_t cstr_info;  /* Codestream information structure */
   char indexfilename[OPJ_PATH_LEN];	/* index file name */
 
   *indexfilename = 0;
    strncpy(parameters.infile, filename, sizeof(parameters.infile)-1);
    std::cout << "\n ..." <<parameters.infile;


              fsrc = fopen(parameters.infile, "rb");
                if (!fsrc) {
                        fprintf(stderr, "ERROR -> failed to open %s for reading\n", parameters.infile);
                        return 1;
                }
		fseek(fsrc, 0, SEEK_END);
                file_length = ftell(fsrc);
                fseek(fsrc, 0, SEEK_SET);
                src = (unsigned char *) malloc(file_length);
                fread(src, 1, file_length, fsrc);
                fclose(fsrc);

             dinfo = opj_create_decompress(CODEC_JP2);
             opj_setup_decoder(dinfo, &parameters);

			/* open a byte stream */
			cio = opj_cio_open((opj_common_ptr)dinfo, src, file_length);
                        
			/* decode the stream and fill the image structure */
			if (*indexfilename)				// If need to extract codestream information
				image = opj_decode_with_info(dinfo, cio, &cstr_info);
			else
				image = opj_decode(dinfo, cio);			
			if(!image) {
                                std::cout << "here is the error";
				fprintf(stderr, "ERROR -> j2k_to_image: failed to decode image!\n");
				opj_destroy_decompress(dinfo);
				opj_cio_close(cio);
				return 1;
			}







 




  

}
