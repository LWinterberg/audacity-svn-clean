/**********************************************************************

  Audacity: A Digital Audio Editor

  Filter.h

  Dominic Mazzoni

**********************************************************************/

#ifndef __AUDACITY_EFFECT_FILTER__
#define __AUDACITY_EFFECT_FILTER__

#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

class wxString;

#include "Effect.h"

class Envelope;
class WaveTrack;

class EffectFilter: public Effect {
   
public:
   
   EffectFilter();
   
   virtual wxString GetEffectName() { return wxString("Filter..."); }
   
   virtual bool Begin(wxWindow *parent);
   virtual bool DoIt(WaveTrack *t,
                     sampleCount start,
                     sampleCount len);
   
private:
   
   Envelope *mEnvelope;

   int windowSize;
   float *filterFunc;
   
   void Filter(sampleCount len,
               sampleType *buffer);
};

class FilterPanel: public wxPanel
{
public:
   FilterPanel( wxWindow *parent, wxWindowID id, 
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize);

   void OnMouseEvent(wxMouseEvent & event);
   void OnPaint(wxPaintEvent & event);

   Envelope *mEnvelope;

private:

   wxBitmap *mBitmap;
   int mWidth;
   int mHeight;

   DECLARE_EVENT_TABLE()
};

// WDR: class declarations

//----------------------------------------------------------------------------
// FilterDialog
//----------------------------------------------------------------------------

// Declare window functions

#define ID_TEXT 10000
#define ID_FILTERPANEL 10001
#define ID_CLEAR 10002

wxSizer *MakeFilterDialog( wxPanel *parent, bool call_fit = TRUE,
                           bool set_sizer = TRUE );

class FilterDialog: public wxDialog
{
public:
   // constructors and destructors
   FilterDialog( wxWindow *parent, wxWindowID id, const wxString &title,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_DIALOG_STYLE );
   
   // WDR: method declarations for FilterDialog
   virtual bool Validate();
   virtual bool TransferDataToWindow();
   virtual bool TransferDataFromWindow();
   
   void SetEnvelope(Envelope *env);
   
private:
   // WDR: member variable declarations for FilterDialog
   
private:
   // WDR: handler declarations for FilterDialog
   void OnClear( wxCommandEvent &event );
   void OnOk( wxCommandEvent &event );
   void OnCancel( wxCommandEvent &event );
   void OnSize( wxSizeEvent &event );
   
private:
   DECLARE_EVENT_TABLE()
};

#endif
