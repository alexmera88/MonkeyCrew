#ifndef _STD_OUT_UTILS_H 
#define _STD_OUT_UTILS_H 

	// https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
	// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
	enum TAnsiColor {
		CL_BLACK, CL_BLUE, CL_GREEN, CL_CYAN, CL_RED, CL_MAGENTA, CL_BROWN, CL_LIGHT_GRAY, 
		CL_DARK_GRAY, CL_LIGHT_BLUE, CL_LIGHT_GREEN, CL_LIGHT_CYAN, CL_LIGHT_RED, CL_LIGHT_MAGENTA, CL_YELLOW, CL_WHITE
	};

	void setForeColor( TAnsiColor color );
	void setBackColor( TAnsiColor color );

#endif	// _STD_OUT_UTILS_H 
