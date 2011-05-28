#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmImageReader.h"
#include "gdcmSequenceOfFragments.h"
#include "gdcmFile.h"

int main(int argc, char *argv[])
{
  if( argc < 3 )
    {
    std::cerr << argv[0] << " input.dcm output.dcm " << std::endl;
    return 1;
    }
  const char *filename = argv[1];
  const char *outfilename = argv[2];

  gdcm::Reader reader;
  reader.SetFileName( filename );
  if( !reader.Read() )
    {
    return 1;
    }

  gdcm::File &file = reader.GetFile();
  const gdcm::DataElement &pixeldata = file.GetDataSet().GetDataElement( gdcm::Tag(0x7fe0,0x0010) );
  const gdcm::SequenceOfFragments *sqf = pixeldata.GetSequenceOfFragments();
  int Resolution = sqf->GetNumberOfFragments();
  std::cout << "\n" << "Number of resolutions are : " << Resolution << "\n";
  
  const gdcm::Fragment &frag = sqf->GetFragment(0);
  const gdcm::ByteValue *bv = frag.GetByteValue();
  const char *ptr = bv->GetPointer();
  size_t len = bv->GetLength();

      gdcm::SmartPointer<gdcm::SequenceOfFragments> sq = new gdcm::SequenceOfFragments;
      gdcm::DataElement pixeldata1( gdcm::Tag(0x7fe0,0x0010) );

      gdcm::Fragment frag1;
      frag1.SetByteValue( ptr, len);
      sq->AddFragment( frag1 );
      pixeldata1.SetValue( *sq );
      file.GetDataSet().Replace( pixeldata1 );

  gdcm::Writer writer;
  writer.SetFile( reader.GetFile() );
  writer.SetFileName( outfilename );
  writer.CheckFileMetaInformationOff();

  if( !writer.Write() )
    {
    std::cerr << "Could not write" << std::endl;
    }



  return 0;
}
