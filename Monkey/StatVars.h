#ifndef StatVars_h__
#define StatVars_h__

#include <time.h>

#ifndef  _USE_MATH_DEFINES
	#define M_PI 3.14159265358979323846
#endif
#include <math.h>

// Based on: http://www.pamvotis.org/vassis/RandGen.htm

class StatVars {
public:
	enum TStatDist { SD_None, SD_U01, SD_Constant, SD_Uniform, SD_Exponential, SD_Poisson, SD_Pareto, SD_Normal, SD_Geometric, SD_Weibull, SD_Erlang };

	inline static float U01()
	{
		//Generate a random number between 0 and 1.
		//REMEMBER: Always cast the operands of a division to float, or truncation will be performed.
		return (float)rand() / (float)(RAND_MAX+1);
	}

	static float Constant (float mean) 
	{
		return mean;
	}

	static float Uniform(float mean) 
	{
		return ( 2 * mean * U01() );
	}


	static float Exponential(float mean)
	{
		//Put it as a parameter to the inverse distribution function.
		return  -mean * log( U01() );
	}

	static int   Poisson(float mean) //Special technique required: Box-Muller method...
	{
		float sum( 0 );
		int i( -1 );
		
		while( sum <= mean ) {
			sum -= log( U01() );
			i++;
		}

		return i;
	}

	static float Pareto(float alpha)
	{
		return 1.0 / pow( U01(), (float) 1.0 / alpha );
	}


	static float Normal(float mean, float stdev)
	{
		return mean + stdev * cos( 2 * M_PI * U01() ) * sqrt( -log( U01() ) );
	}


	static int   Geometric(float p)
	{
		return log( U01() ) / log( 1 - p ) - 1;
	}


	static float Weibull(float scale, float shape)
	{
		return scale * pow( -log( U01() ), (float) 1.0 / shape );
	}


	static float Erlang(int scale, float shape)          //Special technique required...
	{
		float R( 1 );
		
		for ( int i=1; i<=scale; i++ ) {
			R *= U01();
		}

		return -shape * log( R );
	}

	
	static float getSample( TStatDist vDist, float vParam1, float vParam2 = 0.0 ) {
		switch( vDist ) {
			case SD_None		: return 0;
			case SD_U01			: return U01();
			case SD_Constant	: return Constant( vParam1 );
			case SD_Uniform		: return Uniform( vParam1 );
			case SD_Exponential	: return Exponential( vParam1 );
			case SD_Poisson		: return Poisson( vParam1 );
			case SD_Pareto		: return Pareto( vParam1 );
			case SD_Normal		: return Normal( vParam1, vParam2 );
			case SD_Geometric	: return Geometric( vParam1 );
			case SD_Weibull		: return Weibull( vParam1, vParam2 );
			case SD_Erlang		: return Erlang( (int) vParam1, vParam2 );
		}
	}
	static TStatDist strToStatDist( const std::string &vValue ) 
	{
		#define CASE_DIST( NAME ) if ( !strcmp( vValue.c_str(), #NAME ) ) return SD_##NAME;

		CASE_DIST( U01 ); 
		CASE_DIST( Constant );
		CASE_DIST( Uniform );
		CASE_DIST( Exponential );
		CASE_DIST( Poisson );
		CASE_DIST( Pareto );
		CASE_DIST( Normal );
		CASE_DIST( Geometric );
		CASE_DIST( Weibull );
		CASE_DIST( Erlang );

		#undef CASE_DIST

		return SD_None;
	}
};

#endif // StatVars_h__
