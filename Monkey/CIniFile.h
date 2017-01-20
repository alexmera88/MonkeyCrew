#ifndef CIniFile_h__
#define CIniFile_h__

#include <string>
#include <deque>
#include <map>

class CIniFile {
public:
	typedef std::deque<std::string> TSectionList;
	typedef std::map<std::string, std::string> TSection;

	bool load( const std::string &vFilePath );
	bool include( const std::string &vFilePath );

	void clear();
	bool empty() const;

	int sectionCount() const;
	void getSectionList( TSectionList &rSections ) const;
	void getSection( const std::string &vSectionName, TSection &rNameValues ) const;	

private:
	typedef std::map<std::string, TSection> TIniSections;

	TIniSections mSections;

	void splitNameValue( const std::string &vLine, std::string &rName, std::string &rValue ) const;
};

#endif // CIniFile_h__
