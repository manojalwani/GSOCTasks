/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library

  Copyright (c) 2006-2011 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmImage.h"
#include "gdcmImageWriter.h"
#include "gdcmFileDerivation.h"
#include "gdcmUIDGenerator.h"
//#include "gdcmImageChangePhotometricInterpretation.h"


int main(int, char *[])
{
  //Fake Image
  gdcm::SmartPointer<gdcm::Image> im = new gdcm::Image;

  char * buffer = new char[ 256 * 256 * 3 * 3];
  char * p = buffer;
  


  int ybr[3];
  int ybr2[3];
  int rgb[3];

 for(int i=0; i< 3 ;++i)
  for(int row = 0; row < 256; ++row)
    for(int col = 0; col < 256; ++col)
      //for(int b = 0; b < 256; ++b)
      {
      
     if(i==0)
     {
      ybr2[0] = 255;
      ybr2[1] = 0;
      ybr2[2] = 0;
     }
     else if(i==1)
     {
      ybr2[0] = 0;
      ybr2[1] = 255;
      ybr2[2] = 0;
     }

    else
     {
      ybr2[0] = 0;
      ybr2[1] = 0;
      ybr2[2] = 255;
     }
 
       //gdcm::ImageChangePhotometricInterpretation::YBR2RGB(rgb, ybr);
      //gdcm::ImageChangePhotometricInterpretation::RGB2YBR(ybr2, rgb);
      *p++ = ybr2[0];
      *p++ = ybr2[1];
      *p++ = ybr2[2];

      }

  im->SetNumberOfDimensions( 3 );
  im->SetDimension(0, 256 );
  im->SetDimension(1, 256 );
  im->SetDimension(2, 3 );

  im->GetPixelFormat().SetSamplesPerPixel(3);
  //im->SetPhotometricInterpretation( gdcm::PhotometricInterpretation::RGB );
  im->SetPhotometricInterpretation( gdcm::PhotometricInterpretation::RGB);

  unsigned long l = im->GetBufferLength();
  if( l != 256 * 256 * 3 * 3 )
    {
    return 1;
    }
  gdcm::DataElement pixeldata( gdcm::Tag(0x7fe0,0x0010) );
  pixeldata.SetByteValue( buffer, l );
  delete[] buffer;
  im->SetDataElement( pixeldata );
 
  
  gdcm::ImageWriter w;
  w.SetImage( *im );

  // Set the filename:
  w.SetFileName( "output.dcm" );
  if( !w.Write() )
    {
    return 1;
    }

  return 0;
}
