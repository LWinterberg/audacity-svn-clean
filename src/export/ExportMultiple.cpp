/**********************************************************************

  Audacity: A Digital Audio Editor

  ExportMultiple.cpp

  Dominic Mazzoni

  2004.06.23 added mp3 export using tompg.exe for multiple export, by ChackoN
  tested under w2k only

*******************************************************************//**

\class ExportMultiple
\brief Presents a dialog box allowing the user to export multiple files
  either by exporting each track as a separate file, or by
  exporting each label as a separate file.

*//********************************************************************/

#include "../Audacity.h"

#include <wx/defs.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/dirdlg.h>
#include <wx/dynarray.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/intl.h>
#include <wx/msgdlg.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>

#include "Export.h"
#include "ExportMultiple.h"

#include "../Internat.h"
#include "../FileFormats.h"
#include "../LabelTrack.h"
#include "../Project.h"
#include "../Prefs.h"
#include "../Tags.h"


/* define our dynamic array of export settings */
#include <wx/arrimpl.cpp>     // much hackery
WX_DEFINE_OBJARRAY( ExportKitArray )

enum {
   FormatID = 10001,
   OptionsID,
   DirID,
   CreateID,
   ChooseID,
   LabelID,
   FirstID,
   FirstFileNameID,
   TrackID,
   ByNameID,
   ByNumberID,
   PrefixID,
   OverwriteID
};

//
// ExportMultiple methods
//

BEGIN_EVENT_TABLE(ExportMultiple, wxDialog)
   EVT_CHOICE(FormatID, ExportMultiple::OnFormat)
   EVT_BUTTON(OptionsID, ExportMultiple::OnOptions)
   EVT_BUTTON(CreateID, ExportMultiple::OnCreate)
   EVT_BUTTON(ChooseID, ExportMultiple::OnChoose)
   EVT_BUTTON(wxID_OK, ExportMultiple::OnExport)
   EVT_BUTTON(wxID_CANCEL, ExportMultiple::OnCancel)
   EVT_RADIOBUTTON(LabelID, ExportMultiple::OnLabel)
   EVT_RADIOBUTTON(TrackID, ExportMultiple::OnTrack)
   EVT_RADIOBUTTON(ByNameID, ExportMultiple::OnByName)
   EVT_RADIOBUTTON(ByNumberID, ExportMultiple::OnByNumber)
   EVT_CHECKBOX(FirstID, ExportMultiple::OnFirst)
   EVT_TEXT(FirstFileNameID, ExportMultiple::OnFirstFileName)
   EVT_TEXT(PrefixID, ExportMultiple::OnPrefix)
END_EVENT_TABLE()

ExportMultiple::ExportMultiple(AudacityProject *project)
: wxDialog(project, wxID_ANY, wxString(_("Export Multiple")))
{
   mProject = project;
   mTracks = project->GetTracks();
   mPlugins = mExporter.GetPlugins();

   // create array of characters not allowed in file names
   wxString forbid = wxFileName::GetForbiddenChars();
   for(unsigned int i=0; i < forbid.Length(); i++)
      exclude.Add( forbid.Mid(i, 1) );

   ShuttleGui S(this, eIsCreatingFromPrefs);

   // Creating some of the widgets cause cause events to fire
   // and we don't want that until after we're completely
   // created.  (Observed on Windows)
   mInitialized = false;
   PopulateOrExchange(S);
   mInitialized = true;

   Layout();
   Fit();
   SetMinSize(GetSize());
   Center();

   EnableControls();
}

ExportMultiple::~ExportMultiple()
{
}

int ExportMultiple::ShowModal()
{
   Track *tr;

   mLabels = NULL;
   mNumLabels = 0;
   mNumTracks = 0;

   // Examine the track list looking for Wave and Label tracks
   for (tr = mIterator.First(mTracks); tr != NULL; tr = mIterator.Next()) {
      switch (tr->GetKind())
      {
         // Only count WaveTracks, and for linked pairs, only count the
         // second one of the pair
         case Track::Wave:
         {
            if (tr->GetLinked() == false) {
               mNumTracks++;
            }

            break;
         }

         // Only support one label track???
         case Track::Label:
         {
            if (mLabels == NULL) {
               mLabels = (LabelTrack *)tr;
               mNumLabels = mLabels->GetNumLabels();
            }

            break;
         }
      }
   }

   if (mNumTracks < 2 && mNumLabels < 1) {
      ::wxMessageBox(_("If you have more than one Audio Track, you can export each track as a separate file,\nor if you have a Label Track, you can export a new file for each label.\n\nThis project does not have multiple tracks or a Label Track, so you cannot export multiple files."),
                     _("Can't export multiple files"),
                     wxOK | wxCENTRE, this);
      return wxID_CANCEL;
   }

   if (mNumLabels < 1) {
      mLabel->Enable(false);
      mTrack->SetValue(true);
      mLabel->SetValue(false);
   }

   if (mNumTracks < 2) {
      mTrack->Enable(false);
      mLabel->SetValue(true);
      mTrack->SetValue(false);
   }

   EnableControls();

   return wxDialog::ShowModal();
}

void ExportMultiple::PopulateOrExchange(ShuttleGui& S)
{

   wxString name = mProject->GetName();

   mFormatIndex = -1;

   wxString defaultFormat = gPrefs->Read(wxT("/Export/Format"),
      wxT("WAV"));

   mFilterIndex = 0;

   for (size_t i = 0; i < mPlugins.GetCount(); i++) {
      for (size_t j = 0; j < mPlugins[i]->GetFormatCount(); j++)
      {
         if (mPlugins[i]->GetFormat(j) == defaultFormat) {
            mFormatIndex = i;
            mSubFormatIndex = j;
         }
         if (mFormatIndex == -1) mFilterIndex++;
      }
   }
   if (mFormatIndex == -1)
   {
      mFormatIndex = 0;
      mFilterIndex = 0;
      mSubFormatIndex = 0;
   }


   S.SetBorder(5);
   S.StartMultiColumn(4, true);
   {
      wxArrayString formats;
      
      for (size_t i = 0; i < mPlugins.GetCount(); i++) {
         for (size_t j = 0; j < mPlugins[i]->GetFormatCount(); j++)
         {
            formats.Add(mPlugins[i]->GetDescription(j));
         }
      }

      S.AddPrompt(_("Export format:"));
      mFormat = S.Id(FormatID)
         .TieChoice(wxT(""),
                    wxT("/Export/MultipleFormat"),
                    mPlugins[mFormatIndex]->GetFormat(mSubFormatIndex),
                    formats,
                    formats);
      S.Id(OptionsID).AddButton(_("Options..."));
      S.AddVariableText(wxT(""), false);

      S.AddPrompt(_("Export location:"));
      mDir = S.Id(DirID)
         .TieTextBox(wxT(""),
                     wxT("/Export/MultiplePath"),
                     gPrefs->Read(wxT("/Export/Path"), ::wxGetCwd()),
                     64);
      S.Id(ChooseID).AddButton(_("Choose..."));
      S.Id(CreateID).AddButton(_("Create"));
   }
   S.EndMultiColumn();

   S.StartHorizontalLay(wxEXPAND, true);
   {
      S.SetBorder(5);
      S.StartStatic(_("Split files based on:"), true);
      {
         S.StartMultiColumn(2, false);
         S.SetStretchyCol(1);
         {
            // Row 1
            S.SetBorder(1);
            mLabel = S.Id(LabelID)
               .AddRadioButton(wxT(""));
            S.SetBorder(3);
            mLabelLabel = S.AddVariableText(_("Labels"), false);

            // Row 2
            S.AddVariableText(wxT(""), false);
            mFirst = S.Id(FirstID)
               .AddCheckBox(_("Include audio before first label"), wxT("false"));

            // Row 3
            S.AddVariableText(wxT(""), false);
            S.StartHorizontalLay(wxEXPAND, false);
            {
               mFirstFileLabel = S.AddVariableText(_("First file name:"), true);
               mFirstFileName = S.Id(FirstFileNameID)
                  .TieTextBox(wxT(""),
                              name,
                              30);
            }
            S.EndHorizontalLay();

            // Row 4
            S.SetBorder(1);
            mTrack = S.Id(TrackID)
               .AddRadioButtonToGroup(wxT(""));
            S.SetBorder(3);
            mTrackLabel = S.AddVariableText(_("Tracks"), false);
         }
         S.EndMultiColumn();
      }
      S.EndStatic();

      S.SetBorder(5);
      S.StartStatic(_("Name files:"), false);
      {
         S.StartMultiColumn(2, false);
         S.SetStretchyCol(1);
         {
            // Row 1
            S.SetBorder(1);
            mByName = S.Id(ByNameID)
               .AddRadioButton(wxT(""));
            S.SetBorder(3);
            mByNameLabel = S.AddVariableText(_("Using Label/Track Name"), false);

            // Row 2
            S.SetBorder(1);
            mByNumber = S.Id(ByNumberID)
               .AddRadioButtonToGroup(wxT(""));
            S.SetBorder(3);
            mByNumberLabel = S.AddVariableText(_("Numbering consecutively"), false);

            // Row 3
            S.AddVariableText(wxT(""), false);
            S.StartHorizontalLay(wxEXPAND, false);
            {
               mPrefixLabel = S.AddVariableText(_("File name prefix:"), true);
               mPrefix = S.Id(PrefixID)
                  .TieTextBox(wxT(""),
                              name,
                              30);
            }
            S.EndHorizontalLay();
         }
         S.EndMultiColumn();
      }
      S.EndStatic();
   }
   S.EndHorizontalLay();

   S.SetBorder(5);
   S.StartHorizontalLay(wxEXPAND, false);
   {
      mOverwrite = S.Id(OverwriteID)
         .AddCheckBox(_("Overwrite existing files"), wxT("true"));
   }
   S.EndHorizontalLay();

   S.AddStandardButtons();
   mExport = (wxButton *)wxWindow::FindWindowById(wxID_OK, this);
   mExport->SetLabel(_("E&xport"));

}

void ExportMultiple::EnableControls()
{
   bool enable;

   if (!mInitialized) {
      return;
   }

   mFirst->Enable(mLabel->GetValue() && mByName->GetValue());
   
   enable = mLabel->GetValue() &&
            mByName->GetValue() &&
            mFirst->GetValue();
   mFirstFileLabel->Enable(enable);
   mFirstFileName->Enable(enable);

   enable = mByNumber->GetValue();
   mPrefixLabel->Enable(enable);
   mPrefix->Enable(enable);

   bool ok = true;

   if (mLabel->GetValue() && mFirst->GetValue() &&
       mFirstFileName->GetValue() == wxT(""))
      ok = false;

   if (mByNumber->GetValue() &&
       mPrefix->GetValue() == wxT(""))
      ok = false;

   mExport->Enable(ok);
}

void ExportMultiple::OnFormat(wxCommandEvent& event)
{
   EnableControls();
}

void ExportMultiple::OnOptions(wxCommandEvent& event)
{
   int sel = mFormat->GetSelection();
   size_t c = 0;
   for (size_t i = 0; i < mPlugins.GetCount(); i++)
   {
      for (size_t j = 0; j < mPlugins[i]->GetFormatCount(); j++)
      {
         if (sel == c)
         {
            mFormatIndex = i;
            mSubFormatIndex = j;
         }
         c++;
      }
   }

   mPlugins[mFormatIndex]->DisplayOptions(mProject,mSubFormatIndex);
}

void ExportMultiple::OnCreate(wxCommandEvent& event)
{
   wxFileName fn;

   fn.AssignDir(mDir->GetValue());

   bool ok = fn.Mkdir(0777, wxPATH_MKDIR_FULL);

   if (!ok) {
      // Mkdir will produce an error dialog
      return;
   }

   ::wxMessageBox(wxString::Format(_("\"%s\" successfully created."),
                                   fn.GetPath().c_str()),
                  _("Export Multiple"),
                  wxOK | wxCENTRE, this);
}

void ExportMultiple::OnChoose(wxCommandEvent& event)
{
   wxDirDialog dlog(this,
                    _("Choose a location to save the exported files"),
                    mDir->GetValue());
   dlog.ShowModal();
   if (dlog.GetPath() != wxT(""))
      mDir->SetValue(dlog.GetPath());
}

void ExportMultiple::OnLabel(wxCommandEvent& event)
{
   EnableControls();
}

void ExportMultiple::OnFirst(wxCommandEvent& event)
{
   EnableControls();
}

void ExportMultiple::OnFirstFileName(wxCommandEvent& event)
{
   EnableControls();
}

void ExportMultiple::OnTrack(wxCommandEvent& event)
{
   EnableControls();
}

void ExportMultiple::OnByName(wxCommandEvent& event)
{
   EnableControls();
}

void ExportMultiple::OnByNumber(wxCommandEvent& event)
{
   EnableControls();
}

void ExportMultiple::OnPrefix(wxCommandEvent& event)
{
   EnableControls();
}

void ExportMultiple::OnCancel(wxCommandEvent& event)
{
   EndModal(0);
}

void ExportMultiple::OnExport(wxCommandEvent& event)
{
   ShuttleGui S(this, eIsSavingToPrefs);
   PopulateOrExchange(S);

   // Make sure the output directory is in good shape
   if (!DirOk()) {
      return;
   }

   mFilterIndex = mFormat->GetSelection();

   size_t c = 0;
   for (size_t i = 0; i < mPlugins.GetCount(); i++)
   {
      for (size_t j = 0; j < mPlugins[i]->GetFormatCount(); j++)
      {
         if (mFilterIndex == c)
         {
            mFormatIndex = i;
            mSubFormatIndex = j;
         }
         c++;
      }
   }

   bool overwrite = mOverwrite->GetValue();
   bool ok;

   if (mLabel->GetValue()) {
      ok = ExportMultipleByLabel(mByName->GetValue(),
                                 mPrefix->GetValue());
   }
   else {
      ok = ExportMultipleByTrack(mByName->GetValue(),
                                 mPrefix->GetValue());
   }

   if (!ok) {
      return;
   }

   EndModal(1);
}

bool ExportMultiple::DirOk()
{
   wxFileName fn;

   fn.AssignDir(mDir->GetValue());

   if (fn.DirExists()) {
      return true;
   }

   wxString prompt;

   prompt.Printf(_("\"%s\" doesn't exist.\n\nWould you like to create it?"),
                 fn.GetFullPath().c_str());
            
   int action = wxMessageBox(prompt,
                             wxT("Warning"),
                             wxYES_NO | wxICON_EXCLAMATION);
   if (action != wxYES) {
      return false;
   }

   return fn.Mkdir(0777, wxPATH_MKDIR_FULL);
}

bool ExportMultiple::ExportMultipleByLabel(bool byName, wxString prefix)
{
   AudacityProject *project = GetActiveProject();
   bool tagsPrompt = project->GetShowId3Dialog();
   int numFiles = mNumLabels;
   int l = 0;        // counter for files done
   ExportKitArray exportSettings; // dynamic array we will use to store the
                                  // settings needed to do the exports with in
   exportSettings.Alloc(numFiles);   // allocated some guessed space to use

   // Account for exporting before first label
   if (mFirst->GetValue()) {
      l--;
      numFiles++;
   }

   // Figure out how many channels we should export
   int channels = (mTracks->GetNumExportChannels(false));
   
   wxArrayString otherNames;  // keep track of file names we will use, so we
                              // don't duplicate them
   ExportKit setting;   // the current batch of settings
   setting.destfile.SetPath(mDir->GetValue());
   setting.destfile.SetExt(mPlugins[mFormatIndex]->GetExtension(mSubFormatIndex));

   wxString name;    // used to hold file name whilst we mess with it
   wxString title;   // un-messed-with title of file for tagging with

   const LabelStruct *info = NULL;
   /* Examine all labels a first time, sort out all data but don't do any
    * exporting yet (so this run is quick but interactive) */
   while (l < mNumLabels) {
   
      // Get file name and starting time
      if (l < 0) {
         // create wxFileName for output file 
         name = (mFirstFileName->GetValue());
         setting.t0 = 0.0;
      }
      else {
         info = mLabels->GetLabel(l);
         name = (info->title);
         setting.t0 = info->t;
      }

      // Figure out the ending time
      if (info && info->t < info->t1) {
         setting.t1 = info->t1;
      }
      else if (l < mNumLabels-1) {
         const LabelStruct *info1 = mLabels->GetLabel(l+1);
         setting.t1 = info1->t;
      }
      else {
         setting.t1 = mTracks->GetEndTime();
      }

      // store title of label to use in tags
      title = name;

      // Numbering files...
      if (!byName) {
         name.Printf(wxT("%s-%d"), prefix.c_str(), l+1);
      }

      // store sanitised and user checjed name in object
      setting.destfile.SetName(MakeFileName(name));

      if (setting.destfile.GetName().IsEmpty())
         {  // user cancelled dialogue, or deleted everything in feild.
         // either way, cancel
         return false;
         }
      wxASSERT(setting.destfile.IsOk());     // scream if file name is broke

      // Make sure the (final) file name is unique within the set of exports
      MakeNameUnique(otherNames, setting.destfile);

      /* do the metadata for this file */
      // copy project metadata to start with
      setting.filetags = *(mProject->GetTags());
      // over-ride with values
      setting.filetags.SetTag(TAG_TITLE, title);
      setting.filetags.SetTag(TAG_TRACK, l+1);
      // let the user have a crack at editing it, exit if cancelled
      if (!setting.filetags.ShowEditDialog(mProject,_("Edit metadata"), tagsPrompt))
         return false;

      /* add the settings to the array of settings to be used for export */
      exportSettings.Add(setting);

      l++;  // next label, count up one
   }

   bool ok = true;   // did it work?
   int count = 0; // count the number of sucessful runs
   ExportKit activeSetting;  // pointer to the settings in use for this export
   /* Go round again and do the exporting (so this run is slow but
    * non-interactive) */
   for(count = 0; count < (numFiles); count++) {
      /* get the settings to use for the export from the array */
      activeSetting = exportSettings[count];

      // Export it
      ok = DoExport(channels, activeSetting.destfile, false, activeSetting.t0, activeSetting.t1, activeSetting.filetags);
      if (!ok) {
         break;
      }
   }

   // Give 'em the result
   ::wxMessageBox(wxString::Format(ok
                                   ? _("Successfully exported %d file(s).")
                                   : _("Something went wrong after exporting %d file(s)."),
                                   count),
                  _("Export Multiple"),
                  wxOK | wxCENTRE, this);

   return ok;
}

bool ExportMultiple::ExportMultipleByTrack(bool byName,
                                           wxString prefix)
{
   AudacityProject *project = GetActiveProject();
   bool tagsPrompt = project->GetShowId3Dialog();
   Track *tr, *tr2;
   int channels = 0;  // how many channels export?
   int l = 0;     // track counter
   bool ok = true;
   wxArrayString otherNames;
   wxArrayPtrVoid selected;   /**< Array of pointers to the tracks which were
                                selected when we started */
   ExportKitArray exportSettings; // dynamic array we will use to store the
                                  // settings needed to do the exports with in
   exportSettings.Alloc(mNumLabels);   // allocated some guessed space to use
   ExportKit setting;   // the current batch of settings
   setting.destfile.SetPath(mDir->GetValue());
   setting.destfile.SetExt(mPlugins[mFormatIndex]->GetExtension(mSubFormatIndex));

   wxString name;    // used to hold file name whilst we mess with it
   wxString title;   // un-messed-with title of file for tagging with

   /* Remember which tracks were selected, and set them to unselected */
   for (tr = mIterator.First(mTracks); tr != NULL; tr = mIterator.Next()) {
      if (tr->GetKind() != Track::Wave) {
         continue;
      }

      if (tr->GetSelected()) {
         selected.Add(tr);
         tr->SetSelected(false);
      }
   }

   /* Examine all tracks in turn, collecting export information */
   for (tr = mIterator.First(mTracks); tr != NULL; tr = mIterator.Next()) {

      // Only want wave tracks
      if (tr->GetKind() != Track::Wave) {
         continue;
      }

      // Get the times for the track
      setting.t0 = tr->GetStartTime();
      setting.t1 = tr->GetEndTime();

      // Check for a linked track
      tr2 = NULL;
      if (tr->GetLinked()) {
         tr2 = mIterator.Next();
         if (tr2) {

            // Make sure it gets included
            if (tr2->GetStartTime() < setting.t0) {
               setting.t0 = tr2->GetStartTime();
            }

            if (tr2->GetEndTime() > setting.t1) {
               setting.t1 = tr2->GetEndTime();
            }
         }
      }

      // number of export channels?
      if (tr2 == NULL && tr->GetChannel() == WaveTrack::MonoChannel &&
                 ((WaveTrack *)tr)->GetPan() == 0.0)
         channels = 1;
      else
         channels = 2;

      // Get name and title
      title = tr->GetName();
      if (byName) {
         name = title;
      }
      else {
         name = (wxString::Format(wxT("%s-%d"), prefix.c_str(), l+1));
      }

      // store sanitised and user checked name in object
      setting.destfile.SetName(MakeFileName(name));

      if (setting.destfile.GetName().IsEmpty())
         {  // user cancelled dialogue, or deleted everything in feild.
         // either way, cancel
         return false;
         }
      wxASSERT(setting.destfile.IsOk());     // scream if file name is broke

      // Make sure the (final) file name is unique within the set of exports
      MakeNameUnique(otherNames, setting.destfile);

      /* do the metadata for this file */
      // copy project metadata to start with
      setting.filetags = *(mProject->GetTags());
      // over-ride with values
      setting.filetags.SetTag(TAG_TITLE, title);
      setting.filetags.SetTag(TAG_TRACK, l+1);
      // let the user have a crack at editing it, exit if cancelled
      if (!setting.filetags.ShowEditDialog(mProject,_("Edit metadata"), tagsPrompt))
         return false;

      /* add the settings to the array of settings to be used for export */
      exportSettings.Add(setting);

      l++;  // next label, count up one
   }
   // end of user-interactive data gathering loop, start of export processing
   // loop
   int count = 0; // count the number of sucessful runs
   ExportKit activeSetting;  // pointer to the settings in use for this export
   for (tr = mIterator.First(mTracks); tr != NULL; tr = mIterator.Next()) {

      // Only want wave tracks
      if (tr->GetKind() != Track::Wave) {
         continue;
      }

      /* Select the track */
      tr->SetSelected(true);

      // Check for a linked track
      tr2 = NULL;
      if (tr->GetLinked()) {
         tr2 = mIterator.Next();
         if (tr2) {
            // Select it also
            tr2->SetSelected(true);
         }
      }

      /* get the settings to use for the export from the array */
      activeSetting = exportSettings[count];
      // export data
      ok = DoExport(channels, activeSetting.destfile, true, activeSetting.t0, activeSetting.t1, activeSetting.filetags);

      // Reset selection state
      tr->SetSelected(false);
      if (tr2) {
         tr2->SetSelected(false);
      }

      // Stop if an error occurred
      if (!ok) {
         break;
      }
      // increment export counter
      count++;

   }   
   
   // Restore the selection states
   for (size_t i = 0; i < mSelected.GetCount(); i++) {
      ((Track *) selected[i])->SetSelected(true);
   }

   // Give 'em the result
   ::wxMessageBox(wxString::Format(ok
                                   ? _("Successfully exported %d file(s).")
                                   : _("Something went wrong after exporting %d file(s)."),
                                   count),
                  _("Export Multiple"),
                  wxOK | wxCENTRE, this);
   
   return ok;
}

void ExportMultiple::MakeNameUnique(wxArrayString &otherNames, wxFileName &newName)
{
   if (otherNames.Index(newName.GetFullName(), false) >= 0) {
      int i=2;
      wxString orig = newName.GetName();
      do {
         newName.SetName(wxString::Format(wxT("%s-%d"), orig.c_str(), i));
         i++;
      } while (otherNames.Index(newName.GetFullName(), false) >= 0);
   }
   otherNames.Add(newName.GetFullName());
}

bool ExportMultiple::DoExport(int channels,
                              wxFileName name,
                              bool selectedOnly,
                              double t0,
                              double t1,
                              Tags tags)
{


   // Generate a unique name if we're not allowed to overwrite
   if (!mOverwrite->GetValue()) {
      int i = 2;
      wxString base(name.GetName());
      while (name.FileExists()) {
         name.SetName(wxString::Format(wxT("%s-%d"), base.c_str(), i++));
      }
   }

   // Call the format export routine
   bool rc = mPlugins[mFormatIndex]->Export(mProject,
                                            channels,
                                            name.GetFullPath(),
                                            selectedOnly,
                                            t0,
                                            t1,
                                            NULL,
                                            &tags);
   return rc;
}

wxString ExportMultiple::MakeFileName(wxString input)
{
   wxString newname; // name we are generating

   // strip out anything that isn't allowed in file names on this platform
   newname = Internat::SanitiseFilename(input, wxT("_"));

   if(!newname.IsSameAs(input))
   {  // need to get user to fix file name
      // build the dialog
      wxString msg;
      msg.Printf(_("Label %s is not a legal file name. You cannot use any of:   %s\nUse..."), input.c_str(), wxFileName::GetForbiddenChars().c_str());
      wxTextEntryDialog dlg( this, msg, _("Save As..."), newname );

      // And tell the validator about excluded chars
      dlg.SetTextValidator( wxFILTER_EXCLUDE_CHAR_LIST );
      wxTextValidator *tv = dlg.GetTextValidator();
      tv->SetExcludes(exclude);

      // Show the dialog and bail if the user cancels
      if( dlg.ShowModal() == wxID_CANCEL )
      {
          return wxEmptyString;
      }
      // Extract the name from the dialog
      newname = dlg.GetValue();
   }  // phew - end of file name sanitisation procedure
   return newname;
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
// arch-tag: d6904b91-a320-4194-8d60-caa9175b6bb4
