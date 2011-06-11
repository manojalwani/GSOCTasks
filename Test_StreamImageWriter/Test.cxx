#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmAttribute.h"
#include <gdcmStreamImageWriter.h>
#include <gdcmStreamImageReader.h>
#include <iostream>
#include "gdcmImageHelper.h"
#include "gdcmRAWCodec.h"
#include "gdcmTag.h"
#include "gdcmMediaStorage.h"
#include <algorithm>
#include "gdcmImageReader.h"

int main(int argc, char *argv[])
{
  if( argc < 3 )
    {
    std::cerr << argv[0] << " input.dcm output.dcm" << std::endl;
    return 1;
    }
  const char *filename = argv[1];
  const char *outfilename = argv[2];

  // Instanciate the reader:
  gdcm::ImageReader reader;
  reader.SetFileName( filename );
  if( !reader.Read() )
    {
    std::cerr << "Could not read: here " << filename << std::endl;
    return 1;
    }
 


 gdcm::Image image = reader.GetImage();
 image.Print( std::cout );
 char * buf = new char[image.GetBufferLength()];
 int len1 = image.GetBufferLength();
 //std::cout<< "\n "<<len1;


 if (!image.GetBuffer(buf)){
      std::cerr << "Unable to get image buffer" << std::endl;
      delete [] buf;
      return 1;
    }

  gdcm::StreamImageWriter writer;
  writer.SetFile( reader.GetFile() );
  writer.SetFileName( outfilename );
  
     
//PixelFormat pixelInfo = ImageHelper::GetPixelFormatValue(mWriter.GetFile());
  if(!writer.CanWriteFile())
     std::cout<< "mistake in writing the file";
  else
     std::cout<<"No Problems in writing the file";
   //writer.SetFileName( outfilename );

  if(!writer.WriteImageInformation())
     std::cout<< "\n mistake is here in information";
  else
     std::cout<<"\n No Problems in information";

std::vector<unsigned int> extent =
      gdcm::ImageHelper::GetDimensionsValue(reader.GetFile());

    unsigned short xmax = extent[0];
    unsigned short ymax = extent[1];
    unsigned short zmax = extent[2];


        writer.DefinePixelExtent(0, xmax,0,ymax,0, zmax);
        unsigned long len = writer.DefineProperBufferLength();
        //std::cout<< "\n"<<len;
        char* finalBuffer = new char[len];
        memcpy(finalBuffer, &(buf[0]), len);

        if (!writer.Write(finalBuffer, len)){
          std::cerr << "writing failure:" << outfilename ;
          delete [] buf;
          delete [] finalBuffer;
          return 1;
        }
       
  return 0;
}


