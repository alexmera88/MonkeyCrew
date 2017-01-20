#ifndef CAction_h__
#define CAction_h__

#include <string>
#include <iosfwd>
#include "CIniFile.h"
#include "StatVars.h"

class CAction {
public:
	CAction( );
	CAction( const CAction &vOther );
	CAction( const std::string &vName, const CIniFile &vSource );

	// Property setters
	void name( const std::string &vValue );
	void performCommand( const std::string &vValue );
	void revertCommand( const std::string &vValue );	
	void enabled( bool vValue );	

	// Property getters
	std::string name() const;
	std::string performCommand() const;
	std::string revertCommand() const;	
	bool		enabled() const;

	// Information
	bool isReversible() const;

	// Methods	
	void load( const CIniFile &vSource );

	int exec( std::ostream *vOutput );
	std::string execCommand() const;
	std::string execStatus() const;

	time_t nextExecTime() const;

private:
	std::string mName;
	std::string mPerformCommand;
	std::string mRevertCommand;
	bool		mEnabled;	
	bool		mPerformed;

	typedef struct TStatVar {
		TStatVar();
		void loadValue( const std::string &vName, const std::string &vValue );

		StatVars::TStatDist Distrib;
		time_t Min;
		time_t Avg;
	};

	TStatVar	mTBA;	// Time Between Actions
	TStatVar	mTTR;	// Time To Revert

	// Alerts
	std::string mAlertOk;
	std::string mAlertError;
	
	int exec( const std::string &vCommand, std::ostream *vOutput );
	void alert( int vResult );
};

#endif // CAction_h__
