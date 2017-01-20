#ifndef CActionQueue_h__
#define CActionQueue_h__

#include <fstream>
#include <windows.h>
#include <time.h>
#include <map>

#include "StdOutUtils.h"
#include "CActionList.h"


class CActionQueue {
public:
	CActionQueue();
	CActionQueue( CActionList &vActions );
	virtual ~CActionQueue();

	// Information
	bool empty() const;

	// Methods-------------------------------------------

	// Load events from a given set of available actions
	void load( CActionList &vActions );

	// Begin action queue processing
	void start();

	// Finish action queue processing
	void stop();

	void cancelTimer();

	bool verbose() const;
	void verbose( bool vValue );	

	void output( const std::string &vOutputFile );
	void replay( const std::string &vReplayFile );

private:
	void doEcho( const std::string &vText, TAnsiColor vColor = CL_LIGHT_GRAY ) const;
	void logCommand( const std::string &vCommand, const std::string &vParams = "" ) const;

	void schedule( CAction *vAction, time_t vWhen = -1 );
	bool pop( CAction *&rAction, time_t &rWhen );
	
	void timerStart( HANDLE &vTimer );
	bool timerWait( HANDLE &vTimer, size_t vSeconds );	
	void timerStop( HANDLE &vTimer );

	typedef std::multimap<time_t, CAction *> TActionQueue;
	TActionQueue mQueue;
	bool		 mTerminate;
	HANDLE		 mTimer;
	bool		 mCancelTimer;
	bool		 mVerbose;
	std::fstream *mOutput;
	std::fstream *mReplay;
};

#endif // CActionQueue_h__
