/**********************************************************************

  Audacity: A Digital Audio Editor

  ODTask.h

  Created by Michael Chinen (mchinen)
  Audacity(R) is copyright (c) 1999-2008 Audacity Team.
  License: GPL v2.  See License.txt.

******************************************************************//**

\class ODTask
\brief ODTask is an abstract class that outlines the methods that will be used to
support On-Demand background loading of files.  These ODTasks are generally meant to be run
in a background thread.

*//*******************************************************************/




#ifndef __AUDACITY_ODTASK__
#define __AUDACITY_ODTASK__

#include "ODTaskThread.h"
#include "../BlockFile.h"
#include "../Project.h"

#include <vector>
#include <wx/wx.h>
class WaveTrack;


DECLARE_EXPORTED_EVENT_TYPE(AUDACITY_DLL_API, EVT_ODTASK_COMPLETE, -1)

/// A class representing a modular task to be used with the On-Demand structures.
class ODTask
{
 public:

   // Constructor / Destructor

   /// Constructs an ODTask
   ODTask();
   
   virtual ~ODTask(){};
   
   //clones everything except information about the tracks.
   virtual ODTask* Clone()=0;
   
///Do a modular part of the task.  For example, if the task is to load the entire file, load one BlockFile.
///Relies on DoSomeInternal(), which is the subclasses must implement.
///@param amountWork the percent amount of the total job to do.  1.0 represents the entire job.  the default of 0.0
/// will do the smallest unit of work possible
   void DoSome(float amountWork=0.0);
   
   ///Call DoSome until PercentComplete >= 1.0
   void DoAll();
   
   virtual float PercentComplete();
   
   ///returns whether or not this task and another task can merge together, as when we make two mono tracks stereo.
   ///for Loading/Summarizing, this is not an issue because the entire track is processed
   ///Effects that affect portions of a track will need to check this.
   virtual bool CanMergeWith(ODTask* otherTask){return strcmp(GetTaskName(),otherTask->GetTaskName())==0;}   
            
   virtual void StopUsingWaveTrack(WaveTrack* track){}
   
   ///Replaces all instances to a wavetrack with a new one, effectively transferring the task.
   ///ODTask has no wavetrack, so it does nothing.  But subclasses that do should override this.
   virtual void ReplaceWaveTrack(WaveTrack* oldTrack,WaveTrack* newTrack){}
    
    ///Adds a WaveTrack to do the task for
   void AddWaveTrack(WaveTrack* track);
   virtual int GetNumWaveTracks();
   virtual WaveTrack* GetWaveTrack(int i);
       
   ///changes the tasks associated with this Waveform to process the task from a different point in the track
   virtual void DemandTrackUpdate(WaveTrack* track, double seconds){}
    
    bool IsComplete();
    
    void TerminateAndBlock();
    
    virtual const char* GetTaskName(){return "ODTask";}
   
    virtual sampleCount GetDemandSample();
    
    virtual void SetDemandSample(sampleCount sample);
    
    ///does an od update and then recalculates the data.
    virtual void RecalculatePercentComplete();
   
    ///returns the number of tasks created before this instance.
    int GetTaskNumber(){return mTaskNumber;}
    
    void SetNeedsODUpdate();
    bool GetNeedsODUpdate();
    void ResetNeedsODUpdate();
    
    virtual const wxChar* GetTip()=0;
    
    ///returns true if the task is associated with the project.
    virtual bool IsTaskAssociatedWithProject(AudacityProject* proj);

 protected:
     
   ///calculates the percentage complete from existing data.
   virtual void CalculatePercentComplete() = 0; 
     
   ///pure virtual function that does some part of the task this object represents.   
   ///this function is meant to be called repeatedly until the IsComplete is true. 
   ///Does the smallest unit of work for this task.
   virtual void DoSomeInternal() = 0;
   
   ///virtual method called before DoSomeInternal is used from DoSome.
   virtual void Update(){}
   
   ///virtual method called in DoSome everytime the user has demanded some OD function so that the
   ///ODTask can readjust its computation order.  By default just calls Update(), but subclasses with
   ///special needs can override this
   virtual void ODUpdate();
   

   int   mTaskNumber;
   float mPercentComplete;
   ODLock mPercentCompleteMutex;
   bool  mDoingTask;
   bool  mTaskStarted;
   bool mTerminate;
   ODLock mTerminateMutex;
   //for a function not a member var.
   ODLock mBlockUntilTerminateMutex;
   
   std::vector<WaveTrack*> mWaveTracks;
   ODLock     mWaveTrackMutex;
   
   sampleCount mDemandSample;
   ODLock      mDemandSampleMutex;
   
   private:
   
   bool mNeedsODUpdate;
   ODLock mNeedsODUpdateMutex;
   
   

};

#endif

