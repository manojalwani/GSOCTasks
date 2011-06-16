/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library

  Copyright (c) 2006-2011 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmStreamImageReader.h"
#include "gdcmStreamImageWriter.h"
#include "gdcmFileMetaInformation.h"
#include "gdcmSystem.h"
#include "gdcmFilename.h"
#include "gdcmByteSwap.h"
#include "gdcmTrace.h"
#include "gdcmTesting.h"
#include "gdcmImageHelper.h"
#include "gdcmImageReader.h"
#include "gdcmImage.h"

int TestStreamImageWrite(const char* filename , const char* outfilename)
{

  gdcm::ImageReader theImageReaderOriginal;
  gdcm::ImageReader theImageReader;
  gdcm::StreamImageWriter theStreamWriter;

  theImageReaderOriginal.SetFileName( filename );

   if ( theImageReaderOriginal.Read() )
    {
    //int res = 0;

    //to test the writer out, we have to read an image and then
    //write it out one line at a time.
    //a _real_ test would include both the streamimagereader and writer.
    //so, this test is:
    //1) read in the image via the stream reader
    //2) write it out line by line
    //3) read it in by a reader
    //4) compare the image from step 1 with step 3
    //5) go to step2, replace step 3 with a regular image reader.
    //for now, we'll do 1-4

    //pull image information prior to messing with the file
    gdcm::Image theOriginalImage = theImageReaderOriginal.GetImage();
    char* theOriginalBuffer = new char[theOriginalImage.GetBufferLength()];
    if (!theOriginalImage.GetBuffer(theOriginalBuffer)){
      std::cerr << "Unable to get original image buffer, stopping." << std::endl;
      delete [] theOriginalBuffer;
      return 1;
    }

    //first, check that the image information can be written
    //theStreamReader.GetFile().GetDataSet().Print( std::cout );

    theStreamWriter.SetFile(theImageReaderOriginal.GetFile());
    theStreamWriter.SetFileName(outfilename);
    if (!theStreamWriter.CanWriteFile()){
      std::cout<< "Not able to write";
      delete [] theOriginalBuffer;
      return 0;//this means that the file was unwritable, period.
      //very similar to a ReadImageInformation failure
    }
    if (!theStreamWriter.WriteImageInformation()){
      std::cerr << "unable to write image information" << std::endl;
      delete [] theOriginalBuffer;
      return 1; //the CanWrite function should prevent getting here, else,
      //that's a test failure∫
    }
    std::vector<unsigned int> extent =
      gdcm::ImageHelper::GetDimensionsValue(theImageReaderOriginal.GetFile());

    unsigned short xmax = extent[0];
    unsigned short ymax = extent[1];
    unsigned short theChunkSize = 1;
    unsigned short ychunk = extent[1]/theChunkSize; //go in chunk sizes of theChunkSize
    unsigned short zmax = extent[2];

    if (xmax == 0 || ymax == 0)
      {
      std::cerr << "Image has no size, unable to write zero-sized image." << std::endl;
      return 0;
      }

    int z, y, nexty;
    unsigned long prevLen = 0; //when going through the char buffer, make sure to grab
    //the bytes sequentially.  So, store how far you got in the buffer with each iteration.
    for (z = 0; z < zmax; ++z){
      for (y = 0; y < ymax; y += ychunk){
        nexty = y + ychunk;
        if (nexty > ymax) nexty = ymax;
        theStreamWriter.DefinePixelExtent(0, xmax, y, nexty, z, z+1);
        unsigned long len = theStreamWriter.DefineProperBufferLength();
        char* finalBuffer = new char[len];
        memcpy(finalBuffer, &(theOriginalBuffer[prevLen]), len);

        if (!theStreamWriter.Write(finalBuffer, len)){
          std::cerr << "writing failure:" << outfilename << " at y = " << y << " and z= " << z << std::endl;
          delete [] theOriginalBuffer;
          delete [] finalBuffer;
          return 1;
        }
        delete [] finalBuffer;
        prevLen += len;
      }
    }
    delete [] theOriginalBuffer;
return 0;
}
}


int main(int argc, char *argv[])
{

  if( argc < 3 )
    {
    std::cerr << argv[0] << " input.dcm output.dcm" << std::endl;
    return 1;
    }

  const char *filename = argv[1];
  const char *outfilename = argv[2];
  // else
  // First of get rid of warning/debug message
  gdcm::Trace::DebugOn();
  gdcm::Trace::WarningOn();
  
   int r = TestStreamImageWrite(filename,outfilename); 
    

  return r;
}
