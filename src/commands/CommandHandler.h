/**********************************************************************

   Audacity - A Digital Audio Editor
   Copyright 1999-2009 Audacity Team
   License: GPL v2 - see LICENSE.txt

   Dan Horgan

******************************************************************//**

\file CommandHandler.h
\brief Contains declarations for the CommandHandler class.

*//******************************************************************/


#ifndef __COMMANDHANDLER__
#define __COMMANDHANDLER__

//#include <wx/event.h>

#include "../AudacityApp.h"
class AudacityProject;
class AppCommandEvent;
class Command;
class CommandExecutionContext;

//class CommandHandler : public wxEvtHandler
class CommandHandler
{
   private:
      CommandExecutionContext *mCurrentContext;

   public:
      CommandHandler(AudacityApp &app);
      ~CommandHandler();

      // This should only be used during initialization
      void SetProject(AudacityProject *proj);

      // Whenever a command is recieved, process it.
      void OnReceiveCommand(AppCommandEvent &event);

/*
   protected:
      DECLARE_EVENT_TABLE()
*/
};

#endif /* End of include guard: __COMMANDHANDLER__ */

// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: TBD
