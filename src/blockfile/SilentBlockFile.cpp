/**********************************************************************

  Audacity: A Digital Audio Editor

  SilentBlockFile.cpp

  Joshua Haberman

**********************************************************************/

#include "SilentBlockFile.h"
#include "../FileFormats.h"

SilentBlockFile::SilentBlockFile(sampleCount sampleLen):
   BlockFile(wxFileName(), sampleLen)
{
}

SilentBlockFile::~SilentBlockFile()
{
}

bool SilentBlockFile::ReadSummary(void *data)
{
   memset(data, 0, (size_t)mSummaryInfo.totalSummaryBytes);
   return true;
}

int SilentBlockFile::ReadData(samplePtr data, sampleFormat format,
                              sampleCount start, sampleCount len)
{
   ClearSamples(data, format, 0, len);

   return len;
}

void SilentBlockFile::SaveXML(int depth, wxFFile &xmlFile)
{
   for(int i = 0; i < depth; i++)
      xmlFile.Write("\t");
   xmlFile.Write("<silentblockfile ");
   xmlFile.Write(wxString::Format("len='%d' ", mLen));
   xmlFile.Write("/>\n");
}

/// static
BlockFile *SilentBlockFile::BuildFromXML(wxString projDir, const char **attrs)
{
   sampleCount len = 0;

   while(*attrs)
   {
       const char *attr =  *attrs++;
       const char *value = *attrs++;

       if( !strcmp(attr, "len") )
          len = atoi(value);
   }

   return new SilentBlockFile(len);
}

/// Create a copy of this BlockFile
BlockFile *SilentBlockFile::Copy(wxFileName newFileName)
{
   BlockFile *newBlockFile = new SilentBlockFile(mLen);

   return newBlockFile;
}

int SilentBlockFile::GetSpaceUsage()
{
   return 0;
}


// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: d7d174aa-19f1-4bc8-8bd9-c075dcd1cc1b

