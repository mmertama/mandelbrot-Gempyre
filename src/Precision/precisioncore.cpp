/*
 *******************************************************************************
 *
 *
 *                       Copyright (c) 2007-2021
 *                       Henrik Vestermark
 *                       Denmark
 *
 *                       All Rights Reserved
 *
 *   This source file is subject to the terms and conditions of the
 *   Henrik Vestermark Software License Agreement which restricts the manner
 *   in which it may be used.
 *   Mail: hve@hvks.com
 *
 *******************************************************************************
*/

/*
 *******************************************************************************
 *
 *
 * Module name     :precisioncore.cpp
 * Module ID Nbr   :   
 * Description     :Arbitrary precision core functions for integer and floating
 *					point precision class
 * -----------------------------------------------------------------------------
 * Change Record   :   
 *
 * Version	Author/Date		Description of changes
 * -------  ---------------	----------------------------------------------------
 * 01.01	HVE/30-Jul-2007	Initial release
 * 01.02	HVE/28-May-2010	Ported to Visual Studio Express 10
 * 01.03	HVE/10-Jul-2010	Fixed several bugs related to the internal base representation
 * 01.04	HVE/10-Jul-2010	Fixed bugs in handeling BASE 2 internal representation
 * 01.05	HVE/12-Jul-2010	Fix a bug in floor() and ceil() to also work for F_RADIX < BASE_10
 * 01.06	HVE/11-Aug-2010	Replace sprintf with ostringstream 
 * 01.07	HVE/AUg 17-2012	modf() loss of precision fixed
 * 01.08	HVE/AUG 22-2012 added _float_precision_ftoainteger(). Also fixed some issue with the cin operator for whitespaces
 * 01.09	HVE/AUG 24-2012 added ipow_modulo() for effecient calculation of a^b%c
 * 01.10	HVE/SEP 02-2012 added iprime() for checkking a number for a prime
 * 01.11	HVE/SEP 03-2012 added _int_precision_fastdiv() and _int_precision_fastrem() for very fast / % integer operations.
 * 01.12	HVE/FEB 10-2013 replace Microsoft _itoa( with itostring() to make the code portable.
 * 01.13	HVE/JUN 21-2013 Exhanced the exp algorithm avoid loss of significant digits when x is small
 * 01.14	HVE/JUN-22-2013	Added Sinh() hyperbolic functions
 * 01.15	HVE/JUN-25-2013	Added Cosh() and Tanh()
 * 01.16	HVE/JUN-26-2013	Added ArcSinh(), ArcCosh(), ArcTanh()
 * 01.17	HVE/AUG-19-2013	Improve speed of log() by dynamic adjustment of the argument reduction. This speed up the
 *							calculation with a factor of 3+ for 10,000 digits, 2+ for 1000 digits and smaller improvement for
 *							100 digits argument.
 * 01.18	HVE/AUG-23-2013	Improve speed of atan() by dynamic adjustment of argument reduction. This speed up calculation of
 *							more than 3+ for a 10,000 digits number and 2.5 for a 1000 digits and smaller iomprovement for
 *							100 digits argument.
 * 01.19	HVE/Aug-27-2013	Improve ArcSin() by replacing the Newton iteration with a Taylor series plus an agressive
 *							Argument reduction. Improve ArcSin() also improve the ArcCos=0.5*PI-ArcSin().
 * 01.20	HVE/Aug-28-2013	Improve performance of Sin() and Cos() by using dynamic argument reduction.
 * 01.21	HVE/Sep-02-2013	Improve the _int_precision_umul_short() with optimization if multiply with the digit 1.
 * 01.22	HVE/Sep-09-2013	float_precision float_precision::epsilon() rewritten to take advantges of integer power () and special
 *							optmize for F_RADIX==BASE_10
 * 01.23	HVE/Sep-09-2013	Fix an issue with Exp() for extreme small values where there where a loss of precision to do sqrt(1+v^2) for
 *							with extreme values close to epsilon() of the precision. Now precision is double to accomodate for v^2
 * 01.24	HVE/Sep-10-2013	Improved and optmized the pow() to take advantages when y is an integer in x^y
 * 01.25	HVE/Nov-18-2013	Fix and issue with exp() for small negative value that resulted in a loss of precision. Now negative value is
 *							handle as exp(-x)= 1/exp(x).
 * 01.26	HVE/Feb-15-2014 Polish the code so it also work with different compilers
 * 01.27	HVE/Jun-17-2014 More polishing
 * 01.28	HVE/JUN-22-2014	Remove the requirements for microsoft stdafx.h precompiled header.
 * 01.29	HVE/OCT-29-2014 Fixed a bug in iprime() test function that incorrect did not report the first ten primes 2,3,5,7,11,13,17,19,23&29.
 * 01.30	HVE/JUL-24-2016	Fixed a bug in ldexp() where the 1<<exp wast treated as signed where it should be 1U<<exp
 * 01.31	HVE/NOV-13-2016	Added two extra guard digits in the pow() function to avoid loss of precision when doing x^y=exp(ln(x)*y). Also improved ipow() by avoiding the last square
 * 01.32	HVE/NOV-14-2016	Improved the sqrt() function by using Newton method with iterative deepening.(3 times faster)
 * 01.33	HVE/NOV-17-2016	Added nroot() for many times faster calculation of x^(1/n) than doing the equivalent pow(x,1/n)
 * 01.34	HVE/NOV-19-2016	Switch calculation of Pi from Borwein algorithm 2.1 to Brent-Salamin method which prove to be approx 3 times faster
 * 01.35	HVE/JAN-28-2017	Replaced calculation of transcendental constant ln(2) & ln(10) to a spigot algorithm increasing the performance 
 *							with a factor 60-100 times
 * 01.36	HVE/JAN-29-2017	Added special constant _EXP to _float_table() for calculation of exp(1) using a spigot algorithm. 
 *							Increase speed with a factor of 70-100+ range
 * 01.37	HVE/Feb-03-2017	Added gcd() Greatest Common Division & lcm() Least Common Multiplier. 
 * 01.38	HVE/Feb-04-2017	Improved exp(x). if x is an integer then power(exp(1),x) is 100 times faster than exp(x)
 * 01.39	HVE/Jun-06-2017	Squashing out some warnings
 * 01.40	HVE/Oct-28-2017	Polish the code
 * 01.41	HVE/Mar-17-2018 Polish the code
 * 01.42	HVE/Jul-09-2019	Added <stdint.h> for added portability and use int64_t,uint64_t instead of the proprietary _int64
 * 01.43	HVE/Jul-08-2019	Fix a bug in istream input << for int_precision
 * 01.44	HVE/Jul-11-2019	Fixed a bug where a domain error was throw for acos(x) x==1||x==-1, same for asin(x). Also Polish the code and remove compiler warnings
 * 01.45	HVE/JUL-20-2019 Fixed an issue in gcd() for int_precision arguments. It can now handle negative parameters
 * 01.46	HVE/AUG-22-2019	Added * std::string _int_precision_umul64(string *src1, string *src2) to utilized binary multiplication for performance improvement
 *							Also added _stringtou64 that above mention function is using.
 *							Also Added std::string _int_precision_udiv64(string *src1, string *src2 ) to utilized binary division for performance improvement
 * 02.01	HVE/16-SEP-2019	The initial guess for 1/v a d sqrt has been speed up particular for large floating points numbers 
 * 02.02	HVE/17-SEP-2019	_int_precision_atoi() has been speed up by avoiding conversion when RADIX == the BASE representation of the number
 * 02.03	HVE/10-Aug-2020	replace the code for modf() with a more efficient version
 * 02.04	HVE/12-Aug-2020	Change precision type from unsinged int to size_t to enable both 32 and 64bit target. 
 * 02.05	HVE/01-Oct-2020	Change uint64_t to unsinged long long and int64_t to long long
 * 02.06	HVE/18-Mar-2021 Replace most string.insert()=> string.push_back() and then a reverse(string.begin(),string.end()) at the end of the function. This gives a significant 
 *							performance improvement for large digits. Bug in _float_precision_ftoainteger() where the sign was missing from the result has been fixed.
 *							Optimized the use of the string.erase() calls
 * 02.07	HVE/22-Mar-2021 Fix a bug in _int_precision_atoi() that was introduce when sign was moving out of the mNumber string. 
 * 02.08	HVE/4-Jul-2021	Fixed an bug in umul_fourier where a variable was not initialized
 * 02.09	HVE/5-Jul-2021	Replaced all deprecreated headers with current ones. Fixed a bug in floor() and ceil() where exponent was handle as unsigned instead of signed
 * 02.10	HVE/29-Jul-2021	minor cosmetics changes making the code more portable to other environments
 *
 * End of Change Record
 * --------------------------------------------------------------------------
*/

/* define version string */
static char _VIP_[] = "@(#)precisioncore.cpp 02.09 -- Copyright (C) Henrik Vestermark";

#include <cstdint>
#include <ctime>
#include <cmath> 
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <omp.h>

using namespace std;

#include "iprecision.h"
#include "fprecision.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Integer Precision Core
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class precision_ctrl precision_ctrl( BASE_10, BASE_10);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Integer Precision Input and output operator
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& strm, const int_precision& d ) 
   { return strm << _int_precision_itoa(const_cast<int_precision *>(&d) ).c_str(); }

std::istream& operator>>( std::istream& strm, int_precision& d )
         { 
         char ch; std::string s;
         strm.get(ch);// strm >> ch; 
         while( ch == ' ' ) strm.get(ch);  // Ignore leading white space.
         if( ch == '+' || ch == '-' ) { s += ch; strm.get(ch); } else s += '+';  // Parse sign

         if( ch == '0' ) // Octal, Binary or Hexadecimal number
            {
            strm.get( ch );
            if( ch == 'x' || ch == 'X' ) // Parse Hexadecimal
               for( s += "0x"; ch >= '0' && ch <= '9' || ch >='a' && ch <= 'f' || ch >= 'A' && ch <= 'F'; strm.get( ch ) ) s += ch;
            else
               if( ch == 'b' || ch == 'B' )  // Parse Binary
                  for( s += "0b"; ch >= '0' && ch <= '1'; strm.get( ch ) ) s += ch;
               else // Parse Octal
                  for( s += "0"; ch >= '0' && ch <= '7'; strm.get( ch ) ) s += ch;
            }
         else // Parse Decimal number
            for( ; ch >= '0' && ch <= '9'; strm.get(ch) /*strm >> ch*/ ) s += ch;

         strm.putback( ch );  // ch contains the first character not part of the number, so put it back
         if(!strm.fail() && s.length() >= 2 )  // Valid number has at least a length of 2 or higher
            d = int_precision( const_cast<char *>( s.c_str() ) );

         return strm;
         }


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Miscellaneous
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/19/2005
///	@brief 		std::string _int_precision_itoa Convert number to ascii string
///	@return		std::string -	the converted number in ascii string format
///	@param		"a"	-	Number to convert to ascii
///
///	@todo 
///
/// Description:
///   Convert int_precsion to ascii string
///   Based on RADIX conversion from RADIX to BASE_10. Sign is only added if negative
//
std::string _int_precision_itoa( int_precision *a )
   {
   return ( a->sign() < 0 ? "-": "" ) + _int_precision_itoa( a->pointer() );
   }



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Core Functions
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Core functions
// The core functions all perform unsigned arithmetic un elements of the string class!
//    _int_precision_strip_leading_zeros -- Strips non significant leading zeros
//    _int_precision_compare     -- compare two strings for numeric order
//    _int_precision_uneg        -- Negate ones-complement of unsigned integer
//    _int_precision_uadd_short  -- add a short digit [0..RADIX] to the string
//    _int_precision_uadd        -- add two unsigned strings
//    _int_precision_usub_short  -- subtract a short digit [0..RADIX] from the string
//    _int_precision_usub        -- subtract two unsigned strings
//    _int_precision_umul_short  -- multiply a short digiti [0..RADIX] to the string
//    _int_precision_umul        -- multiply two unsigned strings
//    _int_precision_udiv_short  -- Divide a short digit [0..RADIX] to the string
//    _int_precision_udiv        -- divide two unsinged strings
//    _int_precision_urem        -- remainder of dividing two unsinged strings
//    _int_precision_itoa        -- Convert internal precision to BASE_10 string
//    _int_reverse_binary        -- Reverse bit in the data buffer
//    _int_fourier               -- Fourier transformn the data
//    _int_real_fourier          -- Convert n discrete double data into a fourier transform data set
//

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/19/2005
///	@brief 	_int_reverse_binary
///	@return 	void	-	
///	@param   "data[]"	-	array of double complex number to permute
///	@param   "n"	-	number of element in data[]
///
///	@todo  
///
/// Description:
///   Reverse binary permute
///   n must be a power of 2
//
static void _int_reverse_binary( std::complex<double> data[], const size_t n )
   {
   size_t i, j, m;

   if( n <=2 ) return;

   for( j=1, i=1; i < n; i++ )
      {
      if( j > i ) 
         std::swap( data[ j - 1 ], data[ i - 1 ] );
 
      for( m = n >> 1; m >= 2 && j > m; m >>= 1 )
         j -= m;

      j += m;
      }
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/19/2005
///	@brief 	_int_fourier do the fourier transformation
///	@return 	static void	-	
///	@param   "data[]"	-	complex<double> fourie data
///	@param   "n"	-	number of element in data (must be a power of 2)
///	@param   "isign"	-	transform in(1) or out(-1)
///
///	@todo
///
/// Description:
///   Wk=exp(2* PI *i *j )  j=0..n/2-1
///   exp( k * i *j ) => exp( k * i * (j-1) + k * i ) => exp( t + o ) for each new step
///   exp( t + o ) => exp(t)-exp(t)*( 1 - cos(o) -isin(o) ) => exp(t)-exp(t)*(a-ib)
///   => exp(t)+exp(t)*(-a+ib) => exp(t)( 1 + (-a+b) )
///   sin(t+o)=sin(t)+[-a*sin(t)+b*cos(t)]
///   a=2sin^2(o/2), b=sin(o)
///   n must be a power of 2
//
static void _int_fourier( std::complex<double> data[], const size_t n, const int isign )
   {
   double theta;
   std::complex<double> w, wp;
   size_t mh, m, r, j, i;

   _int_reverse_binary( data, n );

   for( m = 2; n >= m; m <<= 1 )
      {
      theta = isign * 2 * 3.14159265358979323846264 / m;
      wp = std::complex<double>( -2.0 * sin( 0.5 * theta ) * sin( 0.5 * theta ), sin( theta ) );
      w = std::complex<double> ( 1, 0 ); // exp(0) == exp( isign*2*PI*i/mmax * m-1 )
      mh = m >> 1;

      for( j = 0; j < mh; j++ )      // m/2 iteration
         {
         for( r = 0; r <= n - m; r += m )
            {
            std::complex<double> tempc;
            i = r + j;
            tempc = w * data[ i + mh ];              // u=data[i]; v=data[j]*w; data[i]=u+v;data[j]=u-v;
            data[ i + mh ] = data[ i ] - tempc;
            data[ i ] += tempc;
            }
      
         w =  w * wp + w;  // w = w(1+wp) ==> w *=1+wp;
         }
      }
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/19/2005
///	@brief 			_int_real_fourier
///	@return 			static void	-	
///	@param   "data[]"	-	
///	@param   "n"	-	number of data element in data. n must be a power of 2)
///	@param   "isign"	-	Converting in(1) or out(-1)
///
///	@todo  
///
/// Description:
///   Convert n discrete double data into a fourier transform data set
///   n must be a power of 2
//
void _int_real_fourier( double data[], const size_t n, const int isign )
   {
   size_t i;
   double theta, c1 = 0.5, c2;
   std::complex<double> w, wp, h1, h2;

   theta = 3.14159265358979323846264 / (double)( n >> 1 );
   if( isign == 1 )
      {
      c2 = -c1;
      _int_fourier( (std::complex<double> *)data, n >> 1, 1 );
      }
   else
      {
      c2 = c1;
      theta = -theta;
      }
   wp = std::complex<double>( -2.0 * sin( 0.5 * theta ) * sin( 0.5 * theta ), sin( theta ) );
   w = std::complex<double> ( 1 + wp.real(), wp.imag() );
   for( i = 1; i < (n>>2); i++ )
      {
      size_t i1, i2, i3, i4;
      std::complex<double> tc;

      i1 = i + i;
      i2 = i1 + 1;
      i3 = n + 1 - i2;
      i4 = i3 + 1;
      h1 = std::complex<double> ( c1 * ( data[i1] + data[i3] ), c1 * ( data[i2]-data[i4]));
      h2 = std::complex<double> ( -c2 * ( data[i2]+data[i4] ), c2 * ( data[i1]-data[i3]));
      tc = w * h2;
      data[i1]=h1.real()+tc.real();
      data[i2]=h1.imag()+tc.imag();
      data[i3]=h1.real() - tc.real();
      data[i4]=-h1.imag() + tc.imag();
      w *= ( std::complex<double>(1) + wp );
      }
   if( isign == 1 )
      {
      double t;
      data[0] = (t=data[0]) + data[1];
      data[1] = t - data[1];
      }
   else
      {
      double t;
      data[0]=c1*((t=data[0])+data[1]);
      data[1]=c1*(t-data[1]);
      _int_fourier( (std::complex<double> *)data, n>>1, -1 );
      }
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/19/2005
///	@brief 	_int_precision_strip_leading_zeros
///	@return 	void	-	
///	@param   "s"	-	pointer to source operand
///
///	@todo
///
/// Description:
///   Remove leading nosignificant zeros
//
void _int_precision_strip_leading_zeros( std::string *s )
   {
   std::string::iterator pos;

   // Strip leading zeros
   for (pos = s->begin(); pos != s->end() && IDIGIT(*pos) == 0; ++pos) ;	// Find first not zero digit
     
   if (s->begin() != pos )
	 s->erase(s->begin(), pos);
   if( s->empty() )
      *s = ICHARACTER(0);

   return;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/19/2005
///	@brief 	_int_precision_compare
///	@return 	int	-	Yhe compare result. o==equal, 1==s1>s2, -1==s1<s2
///	@param   "s1"	-	First operand to compare
///	@param   "s2"	-	Second operand to compare
///
///	@todo  
///
/// Description:
///   Compare two unsigned decimal string 
///   and return 0 is equal, 1 if s1 > s2 otherwise -1
///   Optimized check length first and determine 1 or -1 if equal
///   compare the strengths.
//
int _int_precision_compare( std::string *s1, std::string *s2 )
   {
   int cmp;

   if( s1->length() > s2->length() )
      cmp = 1;
   else
      if( s1->length() < s2->length() )
         cmp = -1;
      else
         cmp = s1->compare( *s2 );

   return cmp;
   }



///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/19/2005
///	@brief 			std::string _int_precision_uneg
///	@return 			std::string	- The negated number	
///	@param         "src"	-	The number to negate
///
///	@todo
///
/// Description:
///   Negate one-complement the unsigned integer src
//
std::string _int_precision_uneg( std::string *src )
   {
   unsigned short ireg = RADIX;
   std::string::reverse_iterator r_pos;
   std::string des;

   des = *src;
   for( r_pos = des.rbegin(); r_pos != des.rend(); ++r_pos )
      {
      ireg = (unsigned short)( RADIX - 1 - IDIGIT( *r_pos ) + ICARRY( ireg ) );
      *r_pos = ICHARACTER( (unsigned char)ISINGLE( ireg ) );
      }

   return des;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 		std::string _int_precision_uadd_short
///	@return 		std::string - 	the result of the add
///	@param      "src1"	-	Source string to add short number
///	@param      "d"	   -	Number to add.   
///
///	@todo
///
/// Description:
///   Short Add: The digit d [0..RADIX] is added to the unsigned decimal string
///   Optimized 0 add or early out add is implemented
///   Please note that uadd_short will throw an exception if d is larger than the 
///   RADIX of the internal representation. e.g. is RADIX is BASE_10 then d can
///   be in the range of 0..10. If base is BASE_2 the d can only be in the range 
///   from 0..2
//
std::string _int_precision_uadd_short( std::string *src1, unsigned int d )
   {
   unsigned short ireg;
   std::string::reverse_iterator r1_pos;
   std::string::reverse_iterator rd_pos;
   std::string des1;

   if( d > (unsigned)RADIX )
      { throw int_precision::out_of_range();      }

   if( d == 0 )   // Zero add
      return *src1;

   ireg = (unsigned short)( RADIX * d );
   des1 = *src1;
   rd_pos = des1.rbegin();
   r1_pos = src1->rbegin();
   
   for(; r1_pos != src1->rend(); ++r1_pos, ++rd_pos )
      {
      ireg = (unsigned short)( IDIGIT( *r1_pos ) + ICARRY( ireg ) ); 
      *rd_pos = ICHARACTER( (unsigned char)ISINGLE( ireg ) );
      if( ICARRY( ireg ) == 0 ) // Early out add
         break;
      }

   if( ICARRY( ireg ) != 0 )  // Insert the carry in the front of the number
      des1.insert( (std::string::size_type)0, 1, ICHARACTER( (unsigned char)ICARRY( ireg ) ) );
   _int_precision_strip_leading_zeros( &des1 );

   return des1;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 	std::string _int_precision_uadd
///	@return 	std::string	-	the result of adding src1 and src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Add two unsigned decimal strings
///   Optimized: Used early out add
//
std::string _int_precision_uadd( std::string *src1, std::string *src2 )
   {
   unsigned short ireg = 0;
   std::string des1;
   std::string::reverse_iterator r_pos, r_end;
   std::string::reverse_iterator rd_pos;

   if( src1->length() >= src2->length() )
      {
      des1 = *src1; 
      r_pos = src2->rbegin();
      r_end = src2->rend();
      }
   else
      {
      des1 = *src2;
      r_pos = src1->rbegin();
      r_end = src1->rend();
      }
   rd_pos = des1.rbegin();
   
   for( ; r_pos != r_end; )
      { // Adding element by element for the two numbers
      ireg = (unsigned char)( IDIGIT( *r_pos ) + IDIGIT( *rd_pos ) + ICARRY( ireg ) );
      *rd_pos = ICHARACTER( (unsigned char)ISINGLE( ireg ) );
      ++r_pos;
      ++rd_pos;
      }

   // Exhaust the smalles of the number, so only the carry can changes the uppper radix digits
   for( ; ICARRY( ireg ) != 0 && rd_pos != des1.rend(); )
      {
      ireg = (unsigned short)( IDIGIT( *rd_pos ) + ICARRY( ireg ) );
      *rd_pos = ICHARACTER( (unsigned char)ISINGLE( ireg ) );
      ++rd_pos;
      }

   // No more carry or end of upper radix number. 
   if( ICARRY( ireg ) != 0 ) // If carry add the carry as a extra radix digit to the front of the number
      des1.insert( (std::string::size_type)0, 1, ICHARACTER( (unsigned char)ICARRY( ireg ) ) );

   _int_precision_strip_leading_zeros( &des1 );

   return des1;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 		std::string _int_precision_uadd_short
///	@return 		std::string - 	the result of the add
///	@param      "src1"	-	Source string to add short number
///	@param      "d"	   -	Number to add.   
///   @param        "result" - Indicated wrap around (1) or not (0)
///
///	@todo
///
/// Description:
///   Short subtract: The digit d [0..RADIX] is subtracted from the unsigned decimal string
///   if src1 < d result is set to -1 (wrap around) otherwise result is set to  0 (no wrap around)
///   Optimized 0 subtract
///   Please note that usub_short will throw an exception if d is larger than the 
///   RADIX of the internal representation. e.g. is RADIX is BASE_10 then d can
///   be in the range of 0..10. If base is BASE_2 the d can only be in the range
///   from 0..2
//
std::string _int_precision_usub_short( int *result, std::string *src1, unsigned int d )
   {
   unsigned short ireg = RADIX;
   std::string::reverse_iterator r1_pos;
   std::string des1;

   if( d > (unsigned)RADIX )
      { throw int_precision::out_of_range();  }

   if( d == 0 ) // Nothing to subtract
      {
      *result = 0;
      return *src1;
      }

   // des1.erase();	// Not needed
   des1.reserve( src1->capacity() );  // Reserver space to avoid time consuming reallocation
   r1_pos = src1->rbegin();

   ireg = (unsigned short)( RADIX - 1 + IDIGIT( *r1_pos ) - d + ICARRY( ireg ) );
   des1.push_back( ICHARACTER( (unsigned char)ISINGLE( ireg ) ) );
   for( ++r1_pos; ICARRY( ireg ) && r1_pos != src1->rend(); ++r1_pos )
      {
      ireg = (unsigned short)( RADIX - 1 + IDIGIT( *r1_pos ) + ICARRY( ireg ) );
      des1.push_back( ICHARACTER( (unsigned char)ISINGLE( ireg ) ) );
      }
   reverse(des1.begin(), des1.end());
   _int_precision_strip_leading_zeros( &des1 );
   
   *result = ICARRY( ireg ) - 1;
   return des1;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 	std::string _int_precision_usub
///	@return 	std::string	-	the result of subtracting src2 from src1
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///   @param   "result" - Return indicate wrap around (-1) otherwise 0
///
///	@todo
///
/// Description:
///   Subtract two unsigned decimal strings
///   if src1 < src2 return -1 (wrap around) otherwise return 0 (no wrap around)
//
std::string _int_precision_usub( int *result, std::string *src1, std::string *src2 )
   {
   unsigned short ireg = RADIX;
   std::string::reverse_iterator r1_pos, r2_pos;
   std::string des1;

   // des1.erase();	// unnecessary
   if( src1->length() > src2->length() )
      des1.reserve( src1->capacity() );  // Reserver space to avoid time consuming reallocation
   else
      des1.reserve( src2->capacity() );  // Reserver space to avoid time consuming reallocation
   r1_pos = src1->rbegin();
   r2_pos = src2->rbegin();

   for(; r1_pos != src1->rend() || r2_pos != src2->rend();)
      {
      if( r1_pos != src1->rend() && r2_pos != src2->rend() )
         { ireg = (unsigned short)( RADIX - 1 + IDIGIT( *r1_pos ) - IDIGIT( *r2_pos ) + ICARRY( ireg ) ); ++r1_pos; ++r2_pos; }
      else
         if( r1_pos != src1->rend() )
            { ireg = (unsigned short)( RADIX - 1 + IDIGIT( *r1_pos ) + ICARRY( ireg ) ); ++r1_pos; }
         else
            { ireg = (unsigned short)( RADIX - 1 - IDIGIT( *r2_pos ) + ICARRY( ireg ) ); ++r2_pos; }
      des1.push_back( ICHARACTER( (unsigned char)ISINGLE( ireg ) ) );
      }
   reverse(des1.begin(), des1.end());
   _int_precision_strip_leading_zeros( &des1 );
   
   *result = ICARRY( ireg ) - 1;
   return des1;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 		std::string _int_precision_umul_short
///	@return 	std::string - 	the result of the short multiplication
///	@param      "src1"	-	Source string to multiply short number
///	@param      "d"	   -	Number to multiply   
///
///	@todo
///
/// Description:
///   Short Add: The digit d [0..RADIX] is multiplied to the unsigned decimal string
///   Optimized Multiply with zero yields zero, Multiply with one return the original or Multiply by RADIX just add a zero to the end.
///   Please note that umul_short will throw an exception if d is larger than the 
///   RADIX of the internal representation. e.g. is RADIX is BASE_10 then d can
///   be in the range of 0..10. If base is BASE_2 the d can only be in the range
///   from 0..2
//
std::string _int_precision_umul_short( std::string *src1, unsigned int d )
   {
   unsigned short ireg = 0;
   std::string::reverse_iterator r1_pos;
   std::string des1;

   if( d > (unsigned)RADIX )
      { throw int_precision::out_of_range(); }

   if( d == 0 )  // Multiply by zero is zero.
      {
      des1.push_back( ( ICHARACTER(0) ) );
      return des1;
      }

   if( d == 1 )  // Multiply by one dont change the src1.
      {
      des1 = *src1;
      _int_precision_strip_leading_zeros( &des1 );
      return des1;
      }

   if( d == RADIX )  // Multiply by RADIX add a zero to the back of the number.
      {
	  des1 = *src1;
	  des1.append( 1, ICHARACTER(0) );
      _int_precision_strip_leading_zeros( &des1 );
      return des1;
      }

  // des1.erase();   // not needed
   des1.reserve( src1->capacity() );  // Reserver space to avoid time consuming reallocation   
   r1_pos = src1->rbegin();
   
   for(; r1_pos != src1->rend(); ++r1_pos )
      {
      ireg = (unsigned short)( IDIGIT( *r1_pos ) * d + ICARRY( ireg ) );
      des1.push_back( ICHARACTER( (unsigned char)ISINGLE( ireg ) ) );
      }
 
   if( ICARRY( ireg ) != 0 )
      des1.push_back( ICHARACTER( (unsigned char)ICARRY( ireg ) ) );
   reverse(des1.begin(), des1.end());
   _int_precision_strip_leading_zeros( &des1 );

   return des1;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 	std::string _int_precision_umul
///	@return 	std::string	-	the result of multiplying src1 and src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Multiply two unsigned decimal strings.
///	Not used anymore since the complexity is o(n^2)
//
std::string _int_precision_umul( std::string *src1, std::string *src2 )
   {
   int disp;
   std::string des1, tmp;
   std::string::reverse_iterator r_pos2;
   
   r_pos2 = src2->rbegin();
   des1 = _int_precision_umul_short( src1, IDIGIT( *r_pos2 ) );
   for( r_pos2++, disp = 1; r_pos2 != src2->rend(); disp++, r_pos2++ )
      {
      if( IDIGIT( *r_pos2 ) != 0 )
         {
         tmp = _int_precision_umul_short( src1, IDIGIT( *r_pos2 ) );
         tmp.append( disp, ICHARACTER( 0 ) );
         des1 = _int_precision_uadd( &des1, &tmp );
         }
      }

   _int_precision_strip_leading_zeros( &des1 );

   return des1;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  22-Aug-2019
///	@brief 	std::string _int_precision_umul64
///	@return 	std::string	-	the result of multiplying src1 and src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Multiply two unsigned decimal strings, by converting it to binary and then do the multiplication and convert it back to string representation 
///	  Notice there is no check for overflow. The calling functions must quarantee that operations can be safely performed.
//
std::string _int_precision_umul64(std::string *src1, std::string *src2)
	{
	unsigned long long a = _stringtou64(src1, RADIX);
	unsigned long long b = _stringtou64(src2, RADIX);
	a *= b;
	return u64to_precision_string(a, RADIX);
	}
	
///	@author Henrik Vestermark (hve@hvks.com)
///	@date  22-Aug-2019
///	@brief 	std::string _int_precision_karatsuba_umul
///	@return 	std::string	-	the result of multiplying src1 and src2
///	@param   "lhs"	-	First unsigned source argument
///	@param   "rhs"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Multiply two unsigned decimal strings, using the karatsuba method
///	  Karatsuba is faster than umul_fourier with operands up to	 19-20 decimal digits whereafter _umul_fourier is faster
///   Notice when operands can fit into a 64bit integer we switch to native multiplications.
//
string _int_precision_karatsuba_umul(const std::string *lhs, const std::string *rhs)
	{
	std::string result, z0, z1, z2, z3;
	std::string lhs0, lhs1, rhs0, rhs1;
	std::string l01, r01, z01;
	int wrap;
	size_t half_length, length, l_length = lhs->size(), r_length = rhs->size(), tot_len;
	length = l_length;
	if (length < r_length)
		length = r_length;
	tot_len = l_length + r_length;
	if (( RADIX == BASE_10 && tot_len <= 18 ) || ( RADIX == BASE_256 && tot_len <= 8 ) || ( RADIX == BASE_8 && tot_len <= 20 ) || ( RADIX == BASE_2 && tot_len <= 64 ) )  // If max digits in lhs & rhs less than fit into a 32 bit integer then do it the binary way
		{
		result = _int_precision_umul64((std::string *)lhs, (std::string *)rhs);
		return result;
		}

	// Splitting
	half_length = length >> 1;
	if (l_length <= half_length)
		{
		lhs0.insert(0, 1, ICHARACTER(0)); lhs1 = lhs->substr();
		}
	else if (l_length < length)
		{
		size_t tmp_length = half_length - (length - l_length);
		if (tmp_length == 0) lhs0.insert(0, 1, ICHARACTER(0)); else lhs0 = lhs->substr(0, tmp_length); lhs1 = lhs->substr(tmp_length);
		}
	else
		{
		lhs0 = lhs->substr(0, half_length); lhs1 = lhs->substr(half_length);
		}
	if (r_length <= half_length)
		{
		rhs0.insert(0, 1, ICHARACTER(0)); rhs1 = rhs->substr();
		}
	else if (r_length < length)
		{
		size_t tmp_length = half_length - (length - r_length);
		if (tmp_length == 0) rhs0.insert(0, 1, ICHARACTER(0)); else rhs0 = rhs->substr(0, tmp_length); rhs1 = rhs->substr(tmp_length);
		}
	else
		{
		rhs0 = rhs->substr(0, half_length); rhs1 = rhs->substr(half_length);
		}

	// Evaluation
//#pragma omp parallel sections
	{
//#pragma omp section
		{z0 = _int_precision_karatsuba_umul(&lhs0, &rhs0); }
//#pragma omp section
		{z1 = _int_precision_karatsuba_umul(&lhs1, &rhs1); }
//#pragma omp section
		{
		l01 = _int_precision_uadd(&lhs0, &lhs1);
		r01 = _int_precision_uadd(&rhs0, &rhs1);
		z2 = _int_precision_karatsuba_umul(&l01, &r01);
		}
	}
	z01 = _int_precision_uadd(&z0, &z1);
	z3 = _int_precision_usub(&wrap, &z2, &z01);

	// Recomposition
	z0.append(2 * (length - half_length), ICHARACTER(0));
	z3.append(length - half_length, ICHARACTER(0));
	z01 = _int_precision_uadd(&z0, &z1);
	result = _int_precision_uadd(&z01, &z3);
	return result;
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  22-Aug-2019
///	@brief 	std::string _int_precision_schonhage_strassen_linear_umul
///	@return  std::string	-	the result of multiplying src1 and src2
///	@param   "lhs"	-	First unsigned source argument
///	@param   "rhs"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Multiply two unsigned decimal strings, using the Schonhage-Strassen (linear convolution) method
///   Notice when operands can fit into a 64bit integer we switch to native multiplications.
//
std::string _int_precision_schonhage_strassen_linear_umul(const std::string *lhs, const std::string *rhs)
	{
	std::string l;
	unsigned int i, j;
	size_t length, l_length = lhs->size(), r_length = rhs->size();
	std::vector<unsigned int> linearconvolution(l_length + r_length - 1,0);  // initialize it with zero
	length = l_length + r_length;

	// Small enough argument to do it directly using native multiplication
	if (( RADIX == BASE_10 && length <= 18 ) || ( RADIX == BASE_256 && length <= 8 ) || ( RADIX == BASE_8 && length <= 20 ) || ( RADIX == BASE_2 && length <= 64 ) )  // If max digits in lhs & rhs less than fit into a 32 bit integer then do it the binary way
		{
		return _int_precision_umul64((std::string *)lhs, (std::string *)rhs);
		}

	//for (i = 0; i < length - 1; ++i)
	//	linearconvolution[i] = 0;

	for (i = 0; i < r_length; ++i)
		for (j = 0; j < l_length; ++j)
			linearconvolution[i + j] += IDIGIT((*rhs)[r_length - 1 - i]) * IDIGIT((*lhs)[l_length - 1 - j]);

	l.reserve(length + 16);
	l = "";

	int nextCarry = 0;
	for (i = 0; i < length - 1; i++)
		{
		linearconvolution[i] += nextCarry;
		l.push_back(ICHARACTER((char)( linearconvolution[i] % RADIX ) ) );
		nextCarry = linearconvolution[i] / RADIX;
		}
	if (nextCarry != 0)
		l.push_back( ICHARACTER((char)nextCarry) );
	reverse(l.begin(), l.end());

	return l;
	}

double xxxtime=0, xxxbest = 10000000, xxxworst = 0, xxxtot = 0, xxxno= 0;

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 	std::string _int_precision_umul_fourier
///	@return 	std::string	-	the result of multplying src1 and src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Multiply two unsigned decimal strings
///   Optimized: Used FFT algorithm to performed the multiplication
//
std::string _int_precision_umul_fourier( std::string *src1, std::string *src2 )
   {
   unsigned short ireg = 0;
   std::string des1;
   std::string::iterator pos;
   size_t n, l, l1, l2, j;
   double *a, *b, cy;
   
   l1 = src1->length();
   l2 = src2->length();
   des1.reserve(l1 + l2 + 16);  // Ensure enough space to hold the Multiplication result to avoid reallocation of des1
   l = l1 < l2 ? l2 : l1;
   for( n = 1; n < l; n <<= 1 ) ;
   n <<= 1;
   a = new double[n];
   b = new double[n];

   // Even that the 4 for loop can been parallelized it is not worth it due to openMP overhead
   for( l=0, pos = src1->begin(); pos != src1->end(); ++pos ) 
	   a[l++] = (double)IDIGIT(*pos);
   for( ; l < n; ) 
	   a[l++] = (double)0;
   for( l=0, pos = src2->begin(); pos != src2->end(); ++pos )
	   b[l++] = (double)IDIGIT(*pos);
   for( ; l < n; ) 
	   b[l++] = (double)0;
   xxxtime = omp_get_wtime();
   // Using parallel sections below speeds up the performance of the two calls to _int_real_Fourier() with a factor of 1.8 
#if true
#pragma omp parallel sections firstprivate(a,b,n)
   {
#pragma omp section 
	   {
	   _int_real_fourier(a, n, 1);
	   }
#pragma omp section
	   {
	   _int_real_fourier(b, n, 1);
	   }
   } 
 #else
   _int_real_fourier(a, n, 1);
   _int_real_fourier(b, n, 1);
#endif

   xxxtime = omp_get_wtime() - xxxtime;
   xxxbest = std::min(xxxtime, xxxbest);
   xxxworst = std::max(xxxtime, xxxworst);
   xxxtot += xxxtime;
   xxxno++;

   b[0] *= a[0];
   b[1] *= a[1];
   for( j = 2; j < n; j += 2 )
      {
      double t;
      b[j]=(t=b[j])*a[j]-b[j+1]*a[j+1];
      b[j+1]=t*a[j+1]+b[j+1]*a[j];
      }
   _int_real_fourier( b, n, -1 );
   for( cy=0, j=0; j <= n-1; ++j )
      {
      double t;
      t=b[n-1-j]/(n>>1)+cy+0.5;
      cy=(unsigned long)( t/RADIX );
      b[n-1-j]=t-cy*RADIX;
      }

   ireg = (unsigned short)cy;
   if( ireg != 0 )
      des1.append( 1, ICHARACTER( (char)ireg ) );
   for( j = 0; j < (int)(l1 + l2 -1); j++ )
      des1.append( 1, ICHARACTER( (char)b[ j ] ) );
   
   _int_precision_strip_leading_zeros( &des1 );

   delete [] a, b;

   return des1;
   }

// Short Division: The digit d [1..RADIX] is divide up into the unsigned decimal string
//
///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 		std::string _int_precision_udiv_short
///	@return 	std::string - 	the result of the short division
///	@param      "src1"		-	Source string to divide with the short number
///	@param      "d"			-	Number to divide
/// @param		"remaind"	- The ramind of the short division
///
///	@todo
///
/// Description:
///   Short divide: The digit d [0..RADIX] is divided up in the unsigned decimal string
///   Divide with zero throw an exception
///   Please note that udiv_short will throw an exception if d is larger than the 
///   RADIX of the internal representation. e.g. is RADIX is BASE_10 then d can
///   be in the range of 0..10. If base is BASE_2 the d can only be in the range 
///   from 0..2
//
std::string _int_precision_udiv_short( unsigned int *remaind, std::string *src1, unsigned int d )
	{
	int i, ir;
	std::string::iterator s1_pos;
	std::string des1;
   
	if (d > (unsigned)RADIX)
		{ throw int_precision::out_of_range(); }

	if( d == 0 )
		{ throw int_precision::divide_by_zero(); }

	if( d == 1 )  // Divide by one dont change the src1.
		{
		des1 = *src1;
		_int_precision_strip_leading_zeros(&des1);
		*remaind = 0;
		return des1;
		}

	if (d == RADIX) // Divide by RADIX remove the last digit of the number
		{
		des1 = *src1;
		*remaind = IDIGIT( des1[ des1.length() - 1 ] );
		des1.erase( des1.length() - 1, 1 );
		return des1;
		}

	//des1.erase();
	des1.reserve( src1->capacity() );  // Reserver space to avoid time consuming reallocation
	s1_pos = src1->begin();
   
	ir = 0;
	for(; s1_pos != src1->end(); ++s1_pos )
		{
		i = RADIX * ir + IDIGIT( *s1_pos );
		des1 += ICHARACTER( (unsigned char)( i / d ) );
		ir = i % d;
		}

	_int_precision_strip_leading_zeros( &des1 );
	*remaind = ir;
	return des1;
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  22-Aug-2019
///	@brief 	std::string _int_precision_udiv64
///	@return 	std::string	-	the result of multiplying src1 and src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Divide two unsigned decimal strings, by converting it to binary then do the division and convert it back to string representation 
///	  Notice there is no check for overflow. The calling functions must quarantee that operations can be safely performed.
//
std::string _int_precision_udiv64(std::string *src1, std::string *src2)
	{
	unsigned long long a = _stringtou64(src1, RADIX);
	unsigned long long b = _stringtou64(src2, RADIX);
	a /= b;
	return u64to_precision_string(a, RADIX);
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 	std::string _int_precision_udiv
///	@return 	std::string	-	the result of disivison
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Divide two unsigned decimal strings
///   Optimized: Used early out add and multiplication w. zero
//
std::string _int_precision_udiv( std::string *src1, std::string *src2 )
   {
   int wrap, plusdigit;
   std::string des, quotient, divisor;
   
   des = ICHARACTER(0);
   divisor = *src1;
   if( src2->length() == 1 ) // Make short div 
      return _int_precision_udiv_short( (unsigned int *)&wrap, &divisor, IDIGIT( (*src2)[0] ) );

   plusdigit = (int)divisor.length() - (int)src2->length();
   for(  ;plusdigit > 1; )
      {
      std::string tmp;

      quotient = (char)ICHARACTER(1);
      quotient.append( plusdigit, ICHARACTER( 0 ) );
      tmp = _int_precision_umul_fourier( src2, &quotient );
      if( _int_precision_compare( &divisor, &tmp ) < 0 )
         { // Too much reduce with one power of radix
         plusdigit--;
         quotient = (char)ICHARACTER(1);
         quotient.append( plusdigit, ICHARACTER( 0 ) );
         tmp = _int_precision_umul_fourier( src2, &quotient );
         }
      divisor = _int_precision_usub( &wrap, &divisor, &tmp );
      des = _int_precision_uadd( &des, &quotient );
      while(_int_precision_compare( &divisor, &tmp ) >= 0 )
        {
        divisor = _int_precision_usub( &wrap, &divisor, &tmp );
        des = _int_precision_uadd( &des, &quotient );
        }
      plusdigit = (int)divisor.length() - (int)src2->length();
      }
   for( wrap = 0; wrap == 0; )
      {
      divisor = _int_precision_usub( &wrap, &divisor, src2 );
      if( wrap == 0 ) // src1 was indeed > src2
         des = _int_precision_uadd_short( &des, 1 );
      }

   _int_precision_strip_leading_zeros( &des );

   return des;
   }


// Short Division: The digit d [1..RADIX] is divide up into the unsigned decimal string
//
///	@author Henrik Vestermark (hve@hvks.com)
///	@date  9-sep-2019
///	@brief 		std::string _int_precision_urem_short
///	@return 	std::string - 	the result of the short remainder
///	@param      "src1"	-	Source string to divide with the short number
///	@param      "d"	   -	Number to divide
///
///	@todo
///
/// Description:
///   Short remainder: The digit d [0..RADIX] is divided up in the unsigned decimal string
///   Divide with zero throw an exception
///   Please note that urem_short will throw an exception if d is larger than the 
///   RADIX of the internal representation. e.g. is RADIX is BASE_10 then d can
///   be in the range of 0..10. If base is BASE_2 the d can only be in the range 
///   from 0..2
///   if d==1 then result == 0, for d==2,4,5,8,10 we only test the last few digits to get the result. This speed up rem for large integers with small rem value
///   since we dont have to run through every digits in src1
//
std::string _int_precision_urem_short(std::string *src1, unsigned int d)
	{
	int i, ir;
	std::string::iterator s1_pos;
	std::string des1;

	if (d > (unsigned)RADIX)
		{ throw int_precision::out_of_range(); }

	if (d == 0)
		{ throw int_precision::divide_by_zero(); }

	if (d == 1)  // Remainer is always 0 for d==1
		{
		des1 = ICHARACTER((unsigned char)(0));
		return des1;
		}

	s1_pos = src1->begin();
	if (src1->length() >= 3 && (BASE_10 == RADIX || BASE_2 ==RADIX || BASE_8 == RADIX || BASE_16 == RADIX || BASE_256 == RADIX ) )
		{// Short cut 
		if (d == 2)
			{ // BASE_2, BASE_8, BASE_10, BASE_16, BASE_256
			des1 = ICHARACTER((unsigned char)(IDIGIT(s1_pos[src1->length() - 1]) % 2)); 
			return des1;
			}
		if (d == 4)
			{ // BASE_8, BASE_10, BASE_256
			des1 = ICHARACTER((unsigned char)((IDIGIT(s1_pos[src1->length() - 2])*RADIX + IDIGIT(s1_pos[src1->length() - 1])) % 4));
			return des1;
			}
		if (d == 5)
			{ // BASE_8, BASE 10, BASE_16, BASE_256
			des1 = ICHARACTER((unsigned char)(IDIGIT(s1_pos[src1->length() - 1]) % 5));
			return des1;
			}
		if (d == 8)
			{ // BASE_8, BASE_10, BASE_16, BASE_256
			des1 = ICHARACTER((unsigned char)((IDIGIT(s1_pos[src1->length() - 3])*RADIX*RADIX + IDIGIT(s1_pos[src1->length() - 2])*RADIX + IDIGIT(s1_pos[src1->length() - 1])) % 8));
			return des1;
			}
		if(d == 10 )
			{ // BASE 10, BASE_16, BASE_256
			des1 = ICHARACTER((unsigned char)(IDIGIT(s1_pos[src1->length() - 1]) % 10 ));
			return des1;
			}
		}

	ir = 0;
	for (; s1_pos != src1->end(); ++s1_pos)
		{
		i = RADIX * ir + IDIGIT(*s1_pos);
		ir = i % d;
		}

	des1 = ICHARACTER((unsigned char)(ir));
	
	return des1;
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  22-Aug-2019
///	@brief 	std::string _int_precision_urem64
///	@return 	std::string	-	the result of multiplying src1 and src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Remainer of Dividing two unsigned decimal strings, by converting it to binary then do the remainer and convert it back to string representation 
///	  Notice there is no check for overflow. The calling functions must quarantee that operations can be safely performed.
//
std::string _int_precision_urem64(std::string *src1, std::string *src2)
	{
	unsigned long long a = _stringtou64(src1, RADIX);
	unsigned long long b = _stringtou64(src2, RADIX);
	a %= b;
	return u64to_precision_string(a, RADIX);
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 	std::string _int_precision_urem
///	@return 	std::string	-	the remaing result of divide src1 with src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Find the remainder when divide two unsigned decimal strings
///   Optimized: Used early out add and multiplication w. zero
//
std::string _int_precision_urem( std::string *src1, std::string *src2 )
   {
   int wrap, plusdigit;
   std::string des, quotient, divisor;
   
   des = ICHARACTER(0);
   divisor = *src1;
   if( src2->length() == 1 ) // Make short rem 
      {
      unsigned int rem;
      _int_precision_udiv_short( &rem, &divisor, IDIGIT( (*src2)[0] ) );
      des = ICHARACTER( (unsigned char)rem );
      return des;
      }

   plusdigit = (int)src1->length() - (int)src2->length();
   for( ; plusdigit > 1; )
      {
      std::string tmp;

      quotient = (char)ICHARACTER(1);
      quotient.append( plusdigit, ICHARACTER( 0 ) );
      tmp = _int_precision_umul_fourier( src2, &quotient );
      if( _int_precision_compare( &divisor, &tmp ) < 0 )
         { // Too much reduce with one power of radix
         plusdigit--;
         quotient = (char)ICHARACTER(1);
         quotient.append( plusdigit, ICHARACTER( 0 ) );
         tmp = _int_precision_umul_fourier( src2, &quotient );
         }
      divisor = _int_precision_usub( &wrap, &divisor, &tmp );
      des = _int_precision_uadd( &des, &quotient );
      while(_int_precision_compare( &divisor, &tmp ) >= 0 )
        {
        divisor = _int_precision_usub( &wrap, &divisor, &tmp );
        des = _int_precision_uadd( &des, &quotient );
        }
      plusdigit = (int)divisor.length() - (int)src2->length();
      }

   for( wrap = 0; wrap == 0; )
      {
      des = divisor;
      divisor = _int_precision_usub( &wrap, &divisor, src2 );
      }

   _int_precision_strip_leading_zeros( &des );

   return des;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  2-sep/2012
///	@brief 	std::string _int_precision_uand
///	@return 	std::string	-	the result of anding src1 and src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   And two unsigned decimal strings
///   Optimized: Used early out and
///  NOT FINISH.
//
std::string _int_precision_uand( std::string *src1, std::string *src2 )
   {
   unsigned short ireg = 0;
   std::string des1;
   std::string::reverse_iterator r_pos, r_end, rd_pos;

   if( src1->length() >= src2->length() ) // Making the sortest operand the result operand since that will be the maxium number of digits
      {
      des1 = *src2; 
      r_pos = src1->rbegin();
      r_end = des1.rend();
      }
   else
      {
      des1 = *src1;
      r_pos = src2->rbegin();
      r_end = des1.rend();
      }
   rd_pos = des1.rbegin();
   
   for( ; rd_pos != r_end; )
      { // Adding element by element for the two numbers
      ireg = IDIGIT( *r_pos ) & IDIGIT( *rd_pos );
      *rd_pos = ICHARACTER( (unsigned char)ISINGLE( ireg ) );
      ++r_pos;
      ++rd_pos;
      }

   _int_precision_strip_leading_zeros( &des1 );

   return des1;
   }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    To and from string conversion
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  2/17/2006
///	@brief 			std::string ito_precision_string
///	@return 			static	-	
///	@param "i"	-	The Integer to convert
///	@param "sg"	-	Treat the integer as signed (true) or unsigned (false)
//	@param "base"	-	Optional Conversion to base, default = RADIX
///
///	@todo  Add to do things	
///
/// Description:
///   This convert a integer number to the internal precisionstring format
///   and return it. the int_precision constructors use this support functions
///   return RADIX <= 10 ? (unsigned char)( x + '0') : (unsigned char)x; 
std::string ito_precision_string( unsigned long i, const bool sg, const int base )
   {
   int sign = 1;
   std::string number;

   if( i == 0 )
      {
      number = "+";
      number.append( 1, base <= 10 ? (unsigned char)( 0 + '0') : (unsigned char)0 );
      return number;
      }

   if( sg== true && (long)i < 0 ) 
      { 
      i = -(long)i;
      sign=-1;
      }

   if( base == BASE_256 )  // Fast BASE_256 conversion
      {
      int j;
      unsigned char *p = (unsigned char *)&i;
      
      for( j = sizeof( int ); j > 0; j-- )
         if( p[j-1] != 0 )  // Strip leading zeros
            break;
      for( ; j > 0; j-- )
         number.append( 1, p[j-1] );
      }
   else
      {// All other Bases
      for( ; i != 0; i /= base )
         number.push_back( base <= 10 ? (unsigned char)( i % base + '0') : (unsigned char)(i % base) );
	  reverse(number.begin(), number.end());
      }

   number = SIGN_STRING( sign ) + number;
   return number;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  11/7/2016
///	@brief 			std::string i64to_precision_string
///	@return 		static	-	
///	@param "i"	-	The 64bit Integer to convert
///	@param "sg"	-	Treat the integer as signed (true) or unsigned (false)
//		@param "base"	-	Optional Conversion to base, default = RADIX
///
///	@todo  Add to do things	
///
/// Description:
///		This convert a 64bit integer number to the internal precisionstring format
///		and return it. the int_precision constructors use this support functions
///		return RADIX <= 10 ? (unsigned char)( x + '0') : (unsigned char)x; 
///		Please note that most conformant C compiler since 1999 will accept the 64bit integer
///		this is equivalent code with ito_precision_string() above
std::string i64to_precision_string( unsigned long long i, const bool sg, const int base)
	{
	int sign = 1;
	std::string number;

	if (i == 0)
		{
		number = "+";
		number.append(1, base <= 10 ? (unsigned char)(0 + '0') : (unsigned char)0);
		return number;
		}

	if (sg == true && (long long)i < 0)
		{
		i = -(long long)i;
		sign = -1;
		}

	if (base == BASE_256)  // Fast BASE_256 conversion
		{
		int j;
		unsigned char *p = (unsigned char *)&i;

		for (j = sizeof(int); j > 0; j--)
			if (p[j - 1] != 0)  // Strip leading zeros
				break;
		for (; j > 0; j--)
			number.append(1, p[j - 1]);
		}
	else
		{// All other Bases
		for (; i != 0; i /= base)
			number.push_back( base <= 10 ? (unsigned char)(i % base + '0') : (unsigned char)(i % base) );
		reverse(number.begin(), number.end());
		}

	number = SIGN_STRING(sign) + number;
	return number;
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  11-Sep-2019
///	@brief 			std::string u64to_precision_string
///	@return 		-	The string representing the unsigned 64bit integer. There is no leading sign	
///	@param "i"		-	The 64bit unsigned Integer to convert
//-	@param "base"	-	Optional Conversion to base, default = RADIX
///
///	@todo  Add to do things	
///
/// Description:
///		This convert a 64bit integer number to the internal precisionstring format
///		and return it. the int_precision constructors use this support functions
///		return RADIX <= 10 ? (unsigned char)( x + '0') : (unsigned char)x; 
///		Please note that most conformant C compiler since 1999 will accept the 64bit integer
///		this is equivalent code with ito_precision_string() above
std::string u64to_precision_string(unsigned long long i, const int base)
	{
	std::string number;

	if (i == 0)
		{
		number = ICHARACTER(0);
		return number;
		}

	if (base == BASE_256)  // Fast BASE_256 conversion
		{
		int j;
		unsigned char *p = (unsigned char *)&i;

		for (j = sizeof(int); j > 0; j--)
			if (p[j - 1] != 0)  // Strip leading zeros
				break;
		for (; j > 0; j--)
			number.append(1, p[j - 1]);
		}
	else
		{// All other Bases
		for (; i != 0; i /= base)
			number.push_back( base <= 10 ? (unsigned char)(i % base + '0') : (unsigned char)(i % base) );
		reverse(number.begin(), number.end());
		}

	return number;
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  2/3/2006
///	@brief 			std::string itostring
///	@return 			static std::string	-	Return the ascii representation of number
///	@param "value"	-	Value to convert to ascii string based on RADIX
///	@param "radix"	-	RADIX value of conversion 
///
///	@todo  Add to do things	
///
/// Description:
///   This function replace Microsoft _itoa() to a generic function that return the
///   string representation of the number in Base Radix. 
///   Radix can be in the range from 2..256 (only 2..36 deliveres a readable string)
///   only if value is < 0 will it return with a leading sign
std::string itostring( int value, const unsigned radix )
   {
   std::string s;
   unsigned digit;
   unsigned uvalue;
   
   if (radix < BASE_2 || radix > BASE_256 )
      return s;  // Conversion not supported

   if( radix == BASE_10 && value < 0 )
      uvalue = -value;
   else
      uvalue = (unsigned)value;

   do 
      {
      digit = (unsigned) (uvalue % radix);
      uvalue /= radix;

      if( radix <= 36 )
         {
         // Convert to ascii and store
         if( digit < 10 )
            s.push_back( (char)ICHARACTER10( (unsigned char)digit ) );      
         else
            s.push_back( (char)( digit - 10 + 'a' ) );      
         }
      else
         { // Keep it 'binary' not readable string
         s.push_back( (unsigned char)digit );      
         }
   } while (uvalue > 0);

   if( radix == BASE_10 && value < 0 )
      s.push_back( '-' );
   reverse(s.begin(), s.end());
   return s;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/14/2005
///	@brief 			std::string _int_precision_itoa
///	@return 		std::string -	Return the inter precision as a string
///	@param         "a"	-	the internal integer precision string
///
///	@todo 
///
/// Description:
///   Convert int_precsion string to ascii string
///   Based on RADIX convertion from RADIX to BASE_10
///   The string has no leading sign
///
std::string _int_precision_itoa( const std::string *a )
   {
   unsigned int rem;
   std::string s, src;
   std::string c0(1, ICHARACTER(0));

   src = *a;
   // s.erase();  //  not needed
   s.reserve( src.capacity() );
   if( RADIX == BASE_10 )       // Nothing to convert
      s += src;
   else
        if( RADIX > BASE_10 )
            {
            for( ; _int_precision_compare( &src, &c0 ) != 0;  )
                {
                src = _int_precision_udiv_short( &rem, &src, BASE_10 );
				s.push_back( (char)ICHARACTER10( (unsigned char)rem ) );
				}
			reverse(s.begin(), s.end());
            }
        else
			{ // Convert RADIX 2..9
            int number;
            std::string base_10, tmp_rem;
            std::string::iterator pos;

            base_10 = itostring( BASE_10, RADIX );
            for( ; _int_precision_compare( &src, &c0 ) != 0;  )
                {
                tmp_rem = _int_precision_urem( &src, &base_10 ); 
                src = _int_precision_udiv( &src, &base_10 );
                // Convert tmp_rem [0..RADIX-1] into text
                number = 0;
                for( pos = tmp_rem.begin(); pos != tmp_rem.end(); ++pos )
                    {
                    number *= RADIX;
                    number += IDIGIT( *pos );
                    }
				s.push_back( (char)ICHARACTER10( (unsigned char)number ) );
				}
			reverse(s.begin(), s.end());
            }

   return s;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  June 01 2010
///	@brief  Build a string repesentation of a number
///	@return	 std::string	- The integer precision string	
///	@param   "digit"		- The next digit to be added to the integer point number to convert. Is always positive
/// @param   "base"			- The base of the digit being added
///
///	@todo 	
///
/// Description:
///   Add a digit to the number being build for the integer precision number
//    
static std::string build_i_number( std::string &number, int digit, int base )
    {
    if( RADIX >= BASE_10)
		{
		number = _int_precision_umul_short( &number, base );
		number = _int_precision_uadd_short( &number, digit );
		}
    else
        {
        std::string nbase;

        nbase=itostring( base, RADIX );
        number = _int_precision_umul( &number, &nbase );
        nbase=itostring( digit, RADIX );
        number = _int_precision_uadd( &number, &nbase );
        }

    return number;
    }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  17-sep-2019
///	@brief 		std::string _int_precision_atoi
///	@return 	string	-	The integer precision string
///	@param		"str"	-	The arbitrary precision string as a regular c-string
/// @param		"sign"	-	Returned the sign as either +1 or -1
///
///	@todo  
///
/// Description:
/// Convert ascii string to string number
/// A leading 0 is intepreted as a octal number
/// a leading 0x is interpreted as a hexadecimal number 
/// a leading 0b is interpreted as a binary number
/// otherwise it's a decimal number.
/// The resulting number is stored in internal BASE RADIX (2,8,10,16 or 256)
//
std::string _int_precision_atoi(const char *str, int *sign)
	{
	std::string s(str);
	std::string::iterator pos;
	size_t startp = 0;
	std::string number;

	*sign = +1;
	pos = s.begin();
	if (*pos == '+' || *pos == '-')
		{
		*sign = CHAR_SIGN(*pos);
		++pos; ++startp;
		if (pos == s.end())
			{
			throw int_precision::bad_int_syntax();
			}
		}
	if (*pos == '0') // Octal, binary or hex representation
		{
		if (pos + 1 != s.end() && tolower(pos[1]) == 'x')
			{
			std::string c16 = itostring(BASE_16, BASE_10); 
			for (pos += 2; pos != s.end(); ++pos)
				if ((*pos < '0' || *pos > '9') && (tolower(*pos) < 'a' || tolower(*pos) > 'f'))
					{
					throw int_precision::bad_int_syntax();
					}
				else
					if (RADIX != BASE_16)
						{
						std::string tmp;

						int hexvalue = IDIGIT10(*pos);
						if (hexvalue > 10)
							hexvalue = tolower(*pos) - 'a' + 10;

						number = _int_precision_umul_fourier(&number, &c16);

						tmp = itostring(hexvalue, BASE_10);
						number = _int_precision_uadd(&number, &tmp);
						}

			if (RADIX == BASE_16)
				number = s.substr(startp+2);
			}
		else
			if (pos + 1 != s.end() && tolower(pos[1]) == 'b')
				{
				for (pos += 2; pos != s.end(); ++pos)
					if (*pos < '0' || *pos > '1')
						{
						throw int_precision::bad_int_syntax();
						}
					else
						if (RADIX != BASE_2)
							number = build_i_number(number, IDIGIT10(*pos), BASE_2);

				if (RADIX == BASE_2)
					number = s.substr(startp+2);
				}
			else
				{ // Collect octal represenation
				for (; pos != s.end(); ++pos)
					if (*pos < '0' || *pos > '7')
						{
						throw int_precision::bad_int_syntax();
						}
					else
						if( RADIX != BASE_8 ) // If decimal representation is different from internal format then build number otherwise the internal representation is the same as the decimal representation
							number = build_i_number(number, IDIGIT10(*pos), BASE_8);

				if (RADIX == BASE_8)
					number = s.substr(startp+1);
				}
		}
	else
		{ // Collect decimal representation
		for (; pos != s.end(); ++pos)
			if (*pos < '0' || *pos > '9')
				{
				throw int_precision::bad_int_syntax();
				}
			else
				if( RADIX != BASE_10 ) // If decimal representation is different from internal format then build number otherwise the internal representation is the same as the decimal representation
					number = build_i_number(number, IDIGIT10(*pos), BASE_10);

		if (RADIX == BASE_10)
			number = s.substr(startp);
		}

	if (number.length() == 1 && number[0] == ICHARACTER(0) && *sign == -1 )
		*sign = +1;

   return number;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  17-sep-2019
///	@brief 		std::string _int_precision_atoi
///	@return 	string	-	The integer precision string
///	@param		"s"	-		The arbitrary precision string as std::string
/// @param		"sign"	-	Returned the sign as either +1 or -1
///
///	@todo  
///
/// Description:
/// Convert ascii string to string number
/// A leading 0 is intepreted as a octal number
/// a leading 0x is interpreted as a hexadecimal number 
/// a leading 0b is interpreted as a binary number
/// otherwise it's a decimal number.
/// The resulting number is stored in internal BASE RADIX (2,8,10,16 or 256)
//
std::string _int_precision_atoi( const std::string &s, int *sign)
	{
	std::string::const_iterator pos;
	std::string number;

	*sign = +1;
	pos = s.begin();
	if (*pos == '+' || *pos == '-')
		{
		*sign = CHAR_SIGN(*pos);
		++pos;
		if (pos == s.end())
			{
			throw int_precision::bad_int_syntax();
			}
		}

	if (*pos == '0') // Octal, binary or hex representation
		{
		if (pos + 1 != s.end() && tolower(pos[1]) == 'x')
			{
			std::string c16 = itostring(BASE_16, BASE_10);
			for (pos += 2; pos != s.end(); ++pos)
				if ((*pos < '0' || *pos > '9') && (tolower(*pos) < 'a' || tolower(*pos) > 'f'))
					{
					throw int_precision::bad_int_syntax();
					}
				else
					if (RADIX != BASE_16)
						{
						std::string tmp;

						int hexvalue = IDIGIT10(*pos);
						if (hexvalue > 10)
							hexvalue = tolower(*pos) - 'a' + 10;

						number = _int_precision_umul_fourier(&number, &c16);

						tmp = itostring(hexvalue, BASE_10);
						number = _int_precision_uadd(&number, &tmp);
						}

			if (RADIX == BASE_16)
				number = s.substr(2);
			}
		else
			if (pos + 1 != s.end() && tolower(pos[1]) == 'b')
				{
				for (pos += 2; pos != s.end(); ++pos)
					if (*pos < '0' || *pos > '1')
						{
						throw int_precision::bad_int_syntax();
						}
					else
						if (RADIX != BASE_2)
							number = build_i_number(number, IDIGIT10(*pos), BASE_2);

				if (RADIX == BASE_2)
					number = s.substr(2);
				}
			else
				{ // Collect octal represenation
				for (; pos != s.end(); ++pos)
					if (*pos < '0' || *pos > '7')
						{
						throw int_precision::bad_int_syntax();
						}
					else
						if (RADIX != BASE_8) // If decimal representation is different from internal format then build number otherwise the internal representation is the same as the decimal representation
							number = build_i_number(number, IDIGIT10(*pos), BASE_8);

				if (RADIX == BASE_8)
					number = s.substr(1);
				}
		}
	else
		{ // Collect decimal representation
		for (; pos != s.end(); ++pos)
			if (*pos < '0' || *pos > '9')
				{
				throw int_precision::bad_int_syntax();
				}
			else
				if (RADIX != BASE_10) // If decimal representation is different from internal format then build number otherwise the internal representation is the same as the decimal representation
					number = build_i_number(number, IDIGIT10(*pos), BASE_10);

		if (RADIX == BASE_10)
			number = s;
		}

	if (number.length() == 1 && number[0] == ICHARACTER(0) && *sign == -1)
		*sign = +1;

	return number;
	}


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  Aug/22/2019
///	@brief 				std::string __stringtou64
///	@return 			string	-	The 64bit integer 
///	@param "str"	-	The arbitrary precision string as a std::string
/// @param "base"	-	Base of the string to convert
///
///	@todo  
///
/// Description:
/// Convert and unsgined ascii string to a 64bit unsigned int. The acsii string is in base RADIX
/// It should be called with a unsigned strings. e.g. no sign
//
unsigned long long _stringtou64( std::string *str, const int base = RADIX)
	{
	std::string::iterator pos = str->begin();
	unsigned long long number = 0;

	switch (base)
		{
		case BASE_2:
		case BASE_8:
		case BASE_10:
			{ // Collect decimal representation
			for (; pos != str->end(); ++pos)
				if (*pos < '0' || *pos > base - 1 + '0')
					{
					throw int_precision::bad_int_syntax();
					}
				else
					{
					number *= base;
					number += IDIGIT10(*pos);
					}
			break;
			}
		case BASE_256:
			{
			for (; pos != str->end(); ++pos)
				{
				number *= base;
				number += IDIGIT(*pos);
				}
			break;
			}
		}

	return number;
	}


///	@date  6/25/2012
///	@brief 		Calculate abs(x)
///	@return 	int_precision -	Return absolute value of x
///	@param      "x"	- The argument
///
///	@todo  
///
/// Description:
///   int precision abs()
///    
//
int_precision abs(const int_precision& x)
	{
	int_precision i;
	
	if (x.sign() < 0)
		{
		i = x; i.change_sign();
		return i;
		}

	return x;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Miscellaneous function
//		ipow()
//		ipow_modulo()
//		iprime()
//		gcd()
//		lcm()
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  8/26/2007
///	@brief 			return the integer power of x^y
///	@return 		int_precision	-	The integer precision power of x^y
///	@param "x"	-	The int precision x
///	@param "y"	-	The int precision y. Max y is 2^32-1
///
///	@todo  
///
/// Description:
/// Return the integer power of x^y. For any pratical purpose the power y is restricted to 2^32-1
//
int_precision ipow( const int_precision& x, const int_precision& y )
   {
   int_precision p(x);
   int_precision r(1);

   for(int n = y; n > 0; n >>= 1) 
        {
        if( ( n & 0x1 ) != 0 ) r *= p;  // Odd
        if( n > 1 ) p *= p;				// Square it				 
        }
   return r;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  8/24/2012
///	@brief 			return the integer power of x^y%z
///	@return 		int_precision	-	The integer precision power of x^y%z
///	@param "x"	-	The int precision x
///	@param "y"	-	The int precision y. Max y is 2^32-1
/// @param "z"	-	The int precision z.
///	@todo  
///
/// Description:
/// Return the integer power of x^y%z. For any pratical purose the power y is restricted to 2^32-1
//
int_precision ipow_modulo( const int_precision& x, const int_precision& y, const int_precision& z )
   {
   int_precision p(x);
   int_precision r(1);

   p%=z;
   for(int n = y; n > 0; n >>= 1) 
        {
        if( ( n & 0x1 ) != 0 ) { r *= p; r %= z; } // Odd
        p *= p;	p %= z;					 
        }
   return r;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  2/Sep/2012
///	@brief 			Check a number for a prime
///	@return 		bool-	true is the integer is a prime number false otherwise
///	@param "prime"	-	The int precision prime
///	@todo  
///
/// Description:
/// Return true if the integer prime is a prime number.
/// All integers are of the form 30k + i for i = 0, 1, 2,...,29 and k an integer from 0..  However, 2 divides 0, 2, 4,...,28 and 3 divides 0, 3, 6,...,27 and 5 divides 0, 5, 10,...,25. 
/// So all prime numbers are of the form 30k + i for i = 1, 7, 11, 13, 17, 19, 23, 29 (i.e. for i < 30 such that gcd(i,30) = 1). 
/// Note that if i and 30 are not coprime, then 30k + i is divisible by a prime divisor of 30, namely 2, 3 or 5, and is therefore not prime.
/// 
///
bool iprime(const int_precision& prime)
	{
	int precheck[11] = { 10, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
	int primes[9] = { 8, 1, 7, 11, 13, 17, 19, 23, 29 };
	int_precision count, kp(30), mod;
	int i;

	for (i = 1; i <= precheck[0]; i++)
	if ((int)(prime % (int_precision)precheck[i]) == 0) return prime==int_precision(precheck[i]);

	for (; kp * kp < prime; kp += 30)   //Loop to divide the number by every number 6*count-1 and 6*count+1 and count < sqrt(i)
		{
		for (i = 1; i <= primes[0]; i++)
			{
			count = kp + primes[i];
			mod = prime % count;
			if (mod == 0)				// Statement to change the variable 'check' to 1 if the number gets divided
				return false;			//meaning its not prime
			}
		}

	return true;						// It is a prime
	}


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  3/Feb/2017
///	@brief 			Greatest Common Divisor 
///	@return 		int_precision - gcd(a,b)
///	@param "a"	-	first number - positive
/// @param "b"	-	second number - positive
///
///	@todo  
///
/// Description:
/// Return the greatest common divisor of the two numbers a & b.
/// It used the Binary gcd method only using shifting and subtraction
/// Change to also handle negative arguments a,b;
///
int_precision gcd( const int_precision& a, const int_precision& b )
	{
	int shift;
	unsigned char s, t;
	int_precision u, v, i0(0), i1(1);

	// GCD(0,v)==v; GCD(u,0)==0; GCD(0,0)==0
	if (a == i0) return b;
	if (b == i0) return a;
	u = a; v = b; if(u < i0) u = -u; if(v < i0) v = -v;
	for (shift = 0; ; ++shift )
		{
		s = IDIGIT( u.pointer()->at(u.size() - 1) );
		t = IDIGIT( v.pointer()->at(v.size() - 1) );
		if ( ( ( s | t ) & 0x1 ) == 0 )  // Both Even
			{
			u >>= i1;
			v >>= i1;
			}
		else
			break;
		}
	for (s = IDIGIT(u.pointer()->at(u.size() - 1)); ( s & 0x1 ) ==0; s = IDIGIT(u.pointer()->at(u.size() - 1)))
		u >>= i1;

	// u is now odd
	do {
		for (t = IDIGIT(v.pointer()->at(v.size() - 1)); ( t & 0x1 ) == 0; t = IDIGIT(v.pointer()->at(v.size() - 1)))
			v >>= i1;
		// u & v is both odd
		if (u > v)
			{// Swap
			int_precision c(v);
			v = u; u = c;
			}
		v -= u;
	} while (v != i0);

	return u <<= shift;		// Restore common factors of 2
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  3/Feb/2017
///	@brief 			Least Common Multiplier
///	@return 		int_precision - lcm(a,b)
///	@param "a"	-	first number
/// @param "b"	-	second number
///
///	@todo  
///
/// Description:
/// Return the least common multiplier of the two numbers a & b.
/// It used the Binary lcm method only using shifting, subtraction and one multiplication and one division
/// 
///
int_precision lcm(const int_precision& a, const int_precision& b)
	{
	int_precision r(a), gcd_ab;

	gcd_ab = gcd(a, b);
	r /= gcd_ab;
	r *= b;
	return r;
	}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    End of Integer Precision Core
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Floating point Precision Core
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class float_precision_ctrl float_precision_ctrl(PRECISION,ROUND_NEAR);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Floating point Precision Input, Output operator
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////


std::ostream& operator<<( std::ostream& strm, const float_precision& d )
    { return strm << _float_precision_ftoa( const_cast<float_precision *>(&d) ).c_str();}

std::istream& operator>>( std::istream& strm, float_precision& d )
         { char ch; std::string s; int cnt, exp_cnt=0;
         strm >> ch;  while( ch == ' ' ) strm.get(ch);  // Ignore leading white space.
         if( ch == '+' || ch == '-' ) { s += ch; strm >> ch; } else s += '+';  // Parse sign
         for( cnt = 0; ch >= '0' && ch <= '9'; cnt++, strm >> ch ) s += ch;  // Parse integer part
         if( ch == '.' )  // Parse fraction
            for( s += '.', strm >> ch; ch >= '0' && ch <= '9'; cnt++, strm >> ch ) s += ch;   // Parse fraction part
         if( ch == 'e' || ch == 'E' )
            {
            s += 'e'; strm >> ch; if( ch == '+' || ch == '-' ) { s += ch; strm >> ch; } else s += '+';  // Parse Expo sign 
            for( exp_cnt =0; ch >= '0' && ch <= '9'; exp_cnt++, strm >> ch ) s += ch;  // Parse expo number
            }

         std::cin.putback( ch );  // ch contains the first character not part of the number, so put it back
         if( !strm.fail() && ( cnt > 0 || exp_cnt > 0 ) )  // Valid number 
            d = float_precision( const_cast<char *>( s.c_str() ), float_precision_ctrl.precision(), float_precision_ctrl.mode() );
         return strm;
         }


//////////////////////////////////////////////////////////////////////////////////////
///
/// CONVERT FLOAT PRECISION to and from ascii representation
///   _float_precision _float_precision_atof()
///   _float_precision_ftoa()
///   _float_precision_dtof()
///   _float_precision 
///
//////////////////////////////////////////////////////////////////////////////////////

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Convert float_precision numbers into string (decimal representation)
///	@return std::string -	The decimal floating point string	
///	@param  "a"			-	float_precision number to convert
///
///	@todo 	
///
/// Description:
///   Convert float_precision numbers into string (decimal representation)
//
std::string _float_precision_ftoa( const float_precision *a )
   {
   int rem;
   std::string s, src;
   float_precision r256;

   r256.precision( a->precision() );
   r256 = *a;
   // s.erase();   // Not needed
   
   if( F_RADIX != BASE_10 )  // 
      {
      float_precision frac, ipart;
      int_precision ip;
      int expo10, expo256;
      bool no_integer;
 
      expo10=0;
      no_integer = false;
      // Convert Integer and fraction part
      frac = modf( r256, &ipart );
      if( ipart == float_precision(0) )
         {
         no_integer = true;
         if( r256.sign() < 0 )
            s = "-0";
         else
            s = "+0";
         }
      else
         {
         std::string::reverse_iterator rpos;
         std::string c0(1, FCHARACTER(0));
         expo256 = ipart.exponent();
         src = ipart.get_mantissa();
         if( (int)src.length() - 1 <= expo256 )
            src.append( (std::string::size_type)( expo256-src.length()+2 ), FCHARACTER( 0 ) );
         
        //c0.insert( (std::string::size_type)0, 1, FCHARACTER( 0 ) );
        // s.append( src, 0, 1 );     // Copy sign
        // src.erase( src.begin() );  // Erase sign
		 if (a->sign() < 0)
			 s = "-";
		 else
			 s = "+";
         if( F_RADIX > BASE_10 )
                {
                for( ; _float_precision_compare( &src, &c0 ) != 0;  )
                    {
                    src = _float_precision_udiv_short( (unsigned int *)&rem, &src, BASE_10 );
                    _float_precision_strip_leading_zeros( &src );
                    s.push_back( (char)FCHARACTER10( (unsigned char)rem ) );  
                    }
				reverse(s.begin()+1, s.end());
                }
            else
                {  // Convert F_RADIX 2..9
                std::string base_10 = itostring( BASE_10, F_RADIX );
                for( ; _float_precision_compare( &src, &c0 ) != 0;  )
                    {
                    std::string tmp_rem;
                    std::string::iterator pos;
                    int number = 0;
                    tmp_rem = _float_precision_urem( &src, &base_10 );
                    src = _float_precision_udiv( &src, &base_10 );
                    // Convert tmp_rem [0..F_RADIX-1] into text
                    for( pos = tmp_rem.begin(); pos != tmp_rem.end(); pos++ )
                        {
                        number *= F_RADIX;
                        number += FDIGIT( *pos );
                        }
                    s.push_back( (char)FCHARACTER10( (unsigned char)number ) ); 
                    }
				reverse(s.begin()+1, s.end());
                }
         }
      
      if( frac != float_precision(0) ) 
         {
         std::string::reverse_iterator rpos;
         std::string::iterator pos;
         int count;
         bool leading_zero = true;

         s.append( "." );
         src = frac.get_mantissa(); 
         for( rem = (int)( (log((double)F_RADIX)/log(10.0)*(r256.precision())+1 ) ); rem > 0; )
            { 
            int digit, expo_base;

            frac *= float_precision( BASE_10 );
            frac = modf( frac, &ipart );
            src = ipart.get_mantissa();
                
            if( F_RADIX > BASE_10 )
                digit= FDIGIT( (unsigned char)src[1] );
            else
                {// Convert [2..9] number and F_RADIX exponent
                expo_base = ipart.exponent()+1;
                for( digit = 0, pos = src.begin(), pos++; pos != src.end(); ++pos, expo_base-- )
                    {
                    digit *= F_RADIX;
                    digit += FDIGIT( *pos );
                    }
                for( ; expo_base > 0; expo_base-- ) digit *= F_RADIX;
                }
            s.append( (std::string::size_type)1, (char)FCHARACTER10( digit % 10 ) );
            if( frac == float_precision( 0 ) )
               break;
            if( leading_zero == true && digit % 10 != 0 )
               leading_zero = false;
            if( leading_zero == false )
               rem--;
            }
      
         // Remove trailing zeros
         // Strip trailing zeros
         for( count = 0, rpos = s.rbegin(); rpos != s.rend() && *rpos == '0'; rpos++ )
            count++;
      
         s.erase( s.length() - count, count );
         if( no_integer == true )
            {
            s.erase( 2, 1 );  // Remove the dot
            // Count leading zeros
            for( expo10 = 0, pos = s.begin(), pos++; pos != s.end() && *pos == '0'; pos++ )
               expo10--;
            if( expo10 < 0 )      
               s.erase( 1, -expo10 );        // Remove leading zeros
            if( s.length() > 2 )
               s.insert( 2, "." );           // Insert dot again
            }
         else
            {
            std::string::size_type nidx;
            
            nidx = s.find_first_of( '.' );
            if( nidx > 2 )
               {
               s.erase( nidx, 1 );     // Erase dot
               s.insert( 2, "." );     // and move it to after the first digit
               expo10 += (int)(nidx - 2);     // increase the exponent by number os positions moved
               }
            }
         }
      else
         {  
         if( no_integer == false )
            {
            if( s.length() > 2 )
               {
               expo10 += (int)(s.length() - 2);
               s.insert( (std::string::size_type)2, "." );
               }
            }
         }

      s += "E";
      s += itostring( expo10, BASE_10 );
      expo256 = 0;
      }
   else
     { // BASE 10
      s = a->get_mantissa();
      if( s.length() >= 2 ) 
          s.insert( (std::string::size_type)1, "." );
	  if (a->sign() < 0)
		  s.insert((std::string::size_type)0, "-");
      s += "E";
      s += itostring( a->exponent(), BASE_10 );
      }

   return s;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  22/8/2012
///	@brief 	Convert float_precision numbers into string (integer representation)
///	@return std::string - The decimal floating point string	
///	@param  "a"			- float_precision number to convert
///
///	@todo 	
///
/// Description:
///   Convert float_precision numbers into string (integer representation)
//
std::string _float_precision_ftoainteger( const float_precision *a )
   {
   int rem;
   std::string s, src;
   float_precision r256, ipart;

   r256.precision( a->precision() );
   ipart.precision( a->precision() );
   r256 = *a;
   // s.erase();	// Not needed

   // Convert Integer and fraction part
   (void)modf( r256, &ipart );
   
   if( F_RADIX != BASE_10 )  // 
      {
      int_precision ip;
      int expo256;
 
      if( ipart == float_precision(0) )
         {
          if( r256.sign() < 0 )
            s = "-0";
         else
            s = "+0";
         }
      else
         {
         std::string::reverse_iterator rpos;
         std::string c0(1, FCHARACTER(0) );
         expo256 = ipart.exponent();
         src = ipart.get_mantissa();
         if( (int)src.length() - 1 <= expo256 )
            src.append( (std::string::size_type)( expo256-src.length()+2 ), FCHARACTER( 0 ) );
		 if (a->sign() < 0)
			 s = "-";
		 else
			 s = "+";
         if( F_RADIX > BASE_10 )
                {
                for( ; _float_precision_compare( &src, &c0 ) != 0;  )
                    {
                    src = _float_precision_udiv_short( (unsigned int *)&rem, &src, BASE_10 );
                    _float_precision_strip_leading_zeros( &src );
                    s.push_back( (char)FCHARACTER10( (unsigned char)rem ) );  
                    }
				reverse(s.begin() + 1, s.end());
                }
            else
                {  // Convert F_RADIX 2..9
                std::string base_10 = itostring( BASE_10, F_RADIX );
                for( ; _float_precision_compare( &src, &c0 ) != 0;  )
                    {
                    std::string tmp_rem;
                    std::string::iterator pos;
                    int number = 0;
                    tmp_rem = _float_precision_urem( &src, &base_10 );
                    src = _float_precision_udiv( &src, &base_10 );
                    // Convert tmp_rem [0..F_RADIX-1] into text
                    for( pos = tmp_rem.begin(); pos != tmp_rem.end(); pos++ )
                        {
                        number *= F_RADIX;
                        number += FDIGIT( *pos );
                        }
                    s.push_back( (char)FCHARACTER10( (unsigned char)number ) ); 
                    }
				reverse(s.begin() + 1, s.end());
                }
         }
      }
   else
     { // BASE 10
     s = ipart.get_mantissa();
     if( (int)(s.length()) <= (int)ipart.exponent() ) 
          s.append( ipart.exponent()-s.length()+1, ICHARACTER(0) ); 
	 if (a->sign() < 0)
		 s.insert((std::string::size_type)0,1,'-');
	 else
		 s.insert((std::string::size_type)0,1,'+');
     }

   return s;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Convert double (IEE754) into a float_precision numbers 
///	@return 	float_precision - The converted float_precision number
///	@param		"d"				- The double IEEE754 number
/// @param		"p"				- The number of significant digits
/// @param		"m"				- Round mode
///
///	@todo 	
///
/// Description:
///   Convert float_precision numbers into string (decimal representation)
//    Constructor for double floating point 
//
float_precision _float_precision_dtof( double d, size_t p, enum round_mode m )
	{
	int expo, cp;
	std::string n, n256;
	char buf[ 32 ];
	float_precision fp(0,p,m);
   
	if( d == 0 )
		return fp;
	expo = 0;

	if( F_RADIX == BASE_10 )
		{
        std::ostringstream sstr;
        sstr << std::setiosflags( ios::scientific ) << std::setprecision( 18 ) << d;
        fp = _float_precision_atof( sstr.str().c_str(), p, m );
        }
	else
		{
		int i;
		double biglittle = 1.0;
		bool little = true;
		float_precision cpower2(2);

		// Big Indian/Little Indian
		if( ((unsigned char *)&biglittle)[0] != 0 )  // Little ?
			little = false;
      
        // Create the Base 256 value of the float
		memcpy( buf, (char *)&d, sizeof( double ) );
		//n256 += "+1"; 
		n256[0] = FCHARACTER( 1 );  // We need to set the implied one from the IEEE754 standard
		if( little == true )
			{
			n256 += ( ( buf[ 6 ] & 0xf ) << 4 ) + ( ( buf[ 5 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 5 ] & 0xf ) << 4 ) + ( ( buf[ 4 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 4 ] & 0xf ) << 4 ) + ( ( buf[ 3 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 3 ] & 0xf ) << 4 ) + ( ( buf[ 2 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 2 ] & 0xf ) << 4 ) + ( ( buf[ 1 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 1 ] & 0xf ) << 4 ) + ( ( buf[ 0 ] & 0xf0 ) >> 4 );
			}
		else
			{
			n256 += ( ( buf[ 1 ] & 0xf ) << 4 ) + ( ( buf[ 2 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 2 ] & 0xf ) << 4 ) + ( ( buf[ 3 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 3 ] & 0xf ) << 4 ) + ( ( buf[ 4 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 4 ] & 0xf ) << 4 ) + ( ( buf[ 5 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 5 ] & 0xf ) << 4 ) + ( ( buf[ 6 ] & 0xf0 ) >> 4 );
			n256 += ( ( buf[ 6 ] & 0xf ) << 4 ) + ( ( buf[ 7 ] & 0xf0 ) >> 4 );
			}
		_float_precision_strip_trailing_zeros( &n256 );  // n is now in the base 256 value of the double
        if( F_RADIX == BASE_2 )
            {
            std::string n2;
            std::string::const_iterator pos;
            std::string const lookup[0x10] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111" };
            pos = n256.begin(); n2+=*pos++; //n2+=*pos++;
            for( ; pos != n256.end(); pos++ )
                {
                n2 += lookup[(*pos >> 4 ) & 0xf];
                n2 += lookup[*pos & 0xf];
                }
            n = n2;
            }
        else 
            if( F_RADIX == BASE_16 )
				{
				std::string n16;
				std::string::const_iterator pos;
				pos = n256.begin(); n16+=*pos++; //n16+=*pos++;
				for( ; pos != n256.end(); pos++ )
					{
					n16 += (*pos >> 4 ) & 0xf;
					n16 += *pos & 0xf;
					}
				n = n16;
				}
			else
				n = n256;
		if( little == true )
			expo = ( ( buf[ 7 ] & 0x7f ) << 4 ) + ( ( buf[ 6 ] & 0xf0 ) >> 4 );
		else
			expo = ( ( buf[ 0 ] & 0x7f ) << 4 ) + ( ( buf[ 1 ] & 0xf0 ) >> 4 );
		expo -= 1023;  // unbiased the double exponent
		if (buf[little == true ? 7 : 0] & 0x80)  // set sign
			fp.sign(-1);
		else
			fp.sign(+1);
		fp.set_n( n );
		// convert exponent in 2^expo to 256^x. exponent modulo 8 is set straight into expo the remainding 
		// is converted by multiplying repeately with 2 or 0.5
        cp = 8; 
        if( F_RADIX == BASE_16 ) cp = 4; else if( F_RADIX == BASE_8 ) cp=3; 
		if( F_RADIX != BASE_2 )
            {
            i = expo % cp;
			expo /= cp;
            } 
        else 
            i=0;
		fp.exponent( expo );
		if( i > 0 )
			{
			// Create 2^i where i [1..7]
			switch( i )
				{
				case 1: (*cpower2.ref_mantissa())[1] = (unsigned char)2; break;
				case 2: (*cpower2.ref_mantissa())[1] = (unsigned char)4; break;
				case 3: (*cpower2.ref_mantissa())[1] = (unsigned char)8; break;
				case 4: (*cpower2.ref_mantissa())[1] = (unsigned char)16; break;
				case 5: (*cpower2.ref_mantissa())[1] = (unsigned char)32; break;
				case 6: (*cpower2.ref_mantissa())[1] = (unsigned char)64; break;
				case 7: (*cpower2.ref_mantissa())[1] = (unsigned char)128; break;
				}
			fp *= cpower2;
			}
		else
			if( i < 0 )
				{
				// std::string s = cpower2.get_mantissa();
				// Create 2^-i where i [-1..-7]
				switch( i+cp )
					{
					case 1: (*cpower2.ref_mantissa())[1] = (unsigned char)0x2; break;
					case 2: (*cpower2.ref_mantissa())[1] = (unsigned char)0x4; break;
					case 3: (*cpower2.ref_mantissa())[1] = (unsigned char)0x8; break;
					case 4: (*cpower2.ref_mantissa())[1] = (unsigned char)0x10; break;
					case 5: (*cpower2.ref_mantissa())[1] = (unsigned char)0x20; break;
					case 6: (*cpower2.ref_mantissa())[1] = (unsigned char)0x40; break;
					case 7: (*cpower2.ref_mantissa())[1] = (unsigned char)0x80; break;
					}
				cpower2.exponent( -1 );
				fp *= cpower2;
				}
		}

   return fp;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Convert a string decimal number into a float_precision number
///	@return 	std::string - The decimal floating point string	
///	@param   "str"	-	ascii string of floating point number to convert
///   @param   "p"   - The precision of the number
///   @param   "m"   - The round mode of the number
///
///	@todo 	
///
/// Description:
///   Convert ascii string into a float_precision numbers 
//    The ascii float format is based on standard C notation
//
static std::string buildnumber( std::string &number, int digit, int base )
{
	bool test = false;
    if(F_RADIX >= BASE_10)
         {
		if (number.capacity() < 100)
			test = true;
         number = _float_precision_umul_short( &number, base );
		 if (number.capacity() < 100)
			 test = true;
         number = _float_precision_uadd_short( &number, digit );
		 if (number.capacity() < 100)
			 test = true;
         }
    else
        {
        std::string nbase;
    
        nbase = itostring( base, F_RADIX );
        number = _float_precision_umul_fourier( &number, &nbase );
        
        nbase = itostring( digit, F_RADIX );
        number = _float_precision_uadd( &number, &nbase );
        }

    return number;
   }
        
///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Convert a string decimal number into a float_precision number
///	@return 	std::string - The decimal floating point string	
///	@param		"str"		-	ascii string of floating point number to convert
/// @param		"p"			- The precision of the number
/// @param		"m"			- The round mode of the number
///
///	@todo 	
///
/// Description:
///   Convert ascii string into a float_precision numbers 
//    The ascii float format is based on standard C notation
//
float_precision _float_precision_atof( const char *str, size_t p, enum round_mode m )
   {
   int sign, sign_expo;
   int expo, expo_radix, expo_e;
   int s_digit, f_digit;
   std::string::size_type i, nidx, idx;
   std::string s(str);
   std::string::iterator pos;
   std::string number, fraction, exponent;
   float_precision fp(0,p,m);
   bool ipart, fpart, epart;
   number.reserve(s.size() + 16);
   expo = 0;
   idx=0;
   ipart=false;
   fpart=false;
   epart=false;
   sign = CHAR_SIGN( '+' );
   // Parse leading sign if any
   pos = s.begin();
   if( *pos == '+' || *pos == '-' )  // 
      {
      sign = CHAR_SIGN( *pos );
      pos++;
      idx=1;
      if( pos == s.end() )
         { throw float_precision::bad_int_syntax(); }
      }

   // Determine any significant, fraction sign or exponent sign
   nidx = s.find_first_of( ".eE", idx );
   if( nidx == std::string::npos ) // Only digits (INTEGER) if any
      {
      if( *pos == '0' ) // Octal or hex representation
         {
         if( pos+1 != s.end() && tolower( pos[1] ) == 'x' )
            {
            for( pos += 2; pos != s.end(); pos++ )
               if( ( *pos < '0' || *pos > '9' ) && ( tolower( *pos ) < 'a' || tolower( *pos ) > 'f' ) )
                  { throw float_precision::bad_int_syntax(); }
               else
                  {
                  std::string tmp;

                  int hexvalue = *pos - '0';
                  if( hexvalue > 10 )
                     hexvalue = tolower( *pos ) - 'a' + 10;
                  tmp = itostring( BASE_16, BASE_10 );
                  number = _float_precision_umul_fourier( &number, &tmp );
                  tmp = itostring( hexvalue, BASE_10 );
                  number = _float_precision_uadd( &number, &tmp );
                  }
            }
         else
            { // Collect octal represenation
			 for (; pos != s.end(); pos++)
				{
				if (*pos < '0' || *pos > '7')
					{ throw float_precision::bad_int_syntax(); }
				else
					if (F_RADIX != BASE_8)
						 number = buildnumber(number, *pos - '0', BASE_8);
				}
			if (F_RADIX == BASE_8)
				number += s.substr(idx+1);  // Dont copy the '0' character for octal designator
            }
         }
      else
         { // Collect decimal representation
		  for (; pos != s.end(); pos++)
			{
			if (*pos < '0' || *pos > '9')
				{ throw float_precision::bad_int_syntax(); }
			else
				if (F_RADIX != BASE_10)
					number = buildnumber(number, *pos - '0', BASE_10);
			}
		  if(F_RADIX == BASE_10)
			  number += s.substr(idx);
         }

      // This is all integers digits, so exponent is the number of digits
      _float_precision_strip_leading_zeros( &number ); // First strip for leading zeros
      if(number.length()==0)	// Speciel check for 0 since this does not accumulate any number
         expo = 0;
      else
         expo =(int)( number.length() -1 );  // Always one digit before the dot
      _float_precision_strip_trailing_zeros( &number ); // Get rid of trailing non-significant zeros
      expo += _float_precision_rounding( &number, sign, p, m );
	  fp.sign(sign);
      fp.set_n( number );
      fp.exponent( expo );

      return fp;
      } // End of Integer
   

   // Floating point number starts here
   s_digit = 0;
   f_digit = 0;
   // Pick up significant beteen idx and nidx 
   if( nidx > idx ) // Number of digits before the . sign or exponent Ee
      {
      ipart=true;
      // Strip leading zeros
      for( i = idx; i != nidx; i++ ) if( s[i] != '0' ) break;
      // Collect significant
	  for (; i != nidx; i++)
		{
		if (s[i] < '0' || s[i] > '9')
		  { throw float_precision::bad_float_syntax(); }
		else
			{	
			if (F_RADIX != BASE_10)
				buildnumber(number, s[i] - '0', BASE_10);
			s_digit++;  // Significant digits. leading space are not counted
			}	
		}
	  if (F_RADIX == BASE_10)
		number += s.substr(idx,s_digit);
      }

   // Floating point representation
   if (s[nidx] == '.') // Any fraction ?
   {
	   idx = nidx + 1;                      // Find start of fraction
	   nidx = s.find_first_of("eE", idx); // Find end of fraction
	   if (nidx == std::string::npos)
		   nidx = s.length();

	   if (idx < nidx)
		   fpart = true;
	   // Remove trailing zero digits
	   for (i = nidx - 1; i >= idx; i--, nidx--) if (s[i] != '0') break;
	   for (i = idx; i < nidx; i++)
		  {
		  if (s[i] < '0' || s[i] > '9')
		   { throw float_precision::bad_float_syntax(); }
		   else
			 {
			 if (F_RADIX != BASE_10)
			   buildnumber(number, s[i] - '0', BASE_10);
			 f_digit++; // fraction digits. trailing zeros are not counted
			}
		}
	  if (F_RADIX == BASE_10)
		  number += s.substr(idx, f_digit);
      nidx = s.find_first_of( "eE", idx );
      }

   expo_e = 0;
   if( nidx != std::string::npos && ( s[ nidx ] == 'e' || s[ nidx ] == 'E' ) )
      {// Parse the exponent 
      idx = nidx + 1;
      nidx = s.length();
      sign_expo = CHAR_SIGN( '+' );;
      if( idx < nidx && ( s[idx] == '+' || s[idx] == '-' ) )  
         {
         sign_expo = CHAR_SIGN( s[idx] );
         idx++;
         if( idx == nidx )
            { throw float_precision::bad_float_syntax(); }  // Sign but no number
         }
      else
         if( idx >= nidx ) 
            { throw float_precision::bad_float_syntax(); }  // E but no number
      
      if( idx < nidx ) 
         epart = true;
      // Collect exponent using base 10
      for( i = idx; i < nidx; i++ )
          if( s[i] < '0' || s[i] > '9' )
             {  throw float_precision::bad_float_syntax(); }
          else
             {
             expo_e *= BASE_10;
             expo_e += s[i] - '0';
             }
      if( sign_expo < 0 )
         expo_e= -expo_e;

      //
	  if (number.length() == 0)
		 {
		  if (ipart == true)
			  number += "0";
		  else
			  { throw float_precision::bad_float_syntax();  }  // no number before a E or no number at all
		}
   }
   
	if(number.length()==0)		// Specielt 0 or 0.0 check which does not accumulate any numbers
		expo_radix = 0;
	else
        expo_radix = (int)(number.length() -1);	// Always one digit before the dot
	if( f_digit > 0 )
		expo_e += -f_digit;  // Adjust for fraction counted as a significant
         
	if( F_RADIX == BASE_10 )
		expo_radix += expo_e;

   // Put it all together
   // Now the number has everything so get rid of trailing non-significant zeros
   expo_radix += _float_precision_normalize( &number );                // Normalize
   expo = _float_precision_rounding( &number, sign, p, m );    // Perform rounding
   expo += expo_radix;
   fp.sign(sign);
   fp.set_n( number );
   fp.exponent( expo_radix );

   if( F_RADIX != BASE_10 && expo_e != 0 )
      {
      if( expo_e > 0 )
         {
         float_precision cpower100(100, p, m ), cpower10(10, p, m );
      
         for( ; expo_e >= 2; expo_e -= 2 )
            fp *= cpower100;
         for( ; expo_e >= 1; expo_e-- )
            fp *= cpower10;
         }
      else
         if( expo_e < 0 )
            {
            float_precision cpower01(10, p+1, m ), cpower001(100, p+1, m );

            cpower01 = _float_precision_inverse( cpower01 );   // Hand craf it to the correct precision
            cpower001 = _float_precision_inverse( cpower001 ); // Hand craf it to the correct precision

            for( ; expo_e < -1; expo_e += 2 )
               fp *= cpower001;
            for( ; expo_e < 0; expo_e++ )
               fp *= cpower01;
            }
      }

   return fp;
   }

  
//////////////////////////////////////////////////////////////////////////////////////
///
/// END CONVERT FLOAT PRECISION to and from ascii representation
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
///
/// FLOATING POINT CORE FUNCTIONS
///
///   _float_precision_strip_leading_zeros
///   _float_precision_strip_trailing_zeros
///   _float_precision_normalize
///   _float_precision_rounding
///   _float_precision_right_shift
///   _float_precision_left_shift
///   _float_precision_compare
///   _float_precision_uadd_short
///   _float_precision_uadd
///   _float_precision_usub_short
///   _float_precision_usub
///   _float_precision_umul_short
///   _float_precision_umul
///   _float_precision_umul_fourier
///   _float_precision_udiv_short
///   _float_precision_udiv
///   _float_precision_urem
///
///   Works Directly on the string class of the float number
///
//////////////////////////////////////////////////////////////////////////////////////


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Remove leading nosignificant zeros from the string
///	@return 	nothing	
///	@param   "s"	-	digital string
///
///	@todo  
///
/// Description:
///   Remove leading nosignificant zeros
//
void _float_precision_strip_leading_zeros( std::string *s )
	{
	std::string::iterator pos;

	// Strip leading zeros
	for (pos = s->begin(); pos != s->end() && FDIGIT(*pos) == 0; ++pos)   ;   // Find end of leading zeros
		//s->erase( pos );
	if (s->begin() != pos)
		s->erase(s->begin(), pos);
      
	if( s->length() == 0 )
		*s = FCHARACTER(0);

	return;
	}


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Remove trailingnosignificant zeros from the string
///	@return 	nothing	
///	@param   "s"	-	digital string
///
///	@todo  
///
/// Description:
///   Remove trailing nosignificant zeros
//
void _float_precision_strip_trailing_zeros( std::string *s )
	{
	std::string::reverse_iterator pos;
	int count;

	// Strip trailing zeros
	for( count = 0, pos = s->rbegin(); pos != s->rend() && FDIGIT( *pos ) == 0; pos++ )
         count++;
      
	s->erase( s->length() - count, count );
	if( s->length() == 0 )
		*s = FCHARACTER(0);

	return;
	}



///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Right shift a string number 
///	@return 	nothing	
///	@param   "s"	-	digital string
///   @param   "shift" - Number of digital shifts
///
///	@todo  
///
/// Description:
///   Right shift number x decimals by inserting 0 in front of the number
//
void _float_precision_right_shift( std::string *s, size_t shift )
	{
	s->insert( (std::string::size_type)0, shift, FCHARACTER( 0 ) );
	}



///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Left shift a string number 
///	@return 	nothing	
///	@param   "s"	-	digital string
///   @param   "shift" - Number of digital shifts
///
///	@todo  
///
/// Description:
///   Left shift number x decimals by appending 0 in the back of the number
//
void _float_precision_left_shift( std::string *s, size_t shift )
	{
	s->append( shift, FCHARACTER( 0 ) );
	}


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Normalize a floating point mantissa
///	@return 	int - Return the exponent adjustment factor due to normalization
///	@param   "m"	-	digital string
///
///	@todo  
///
/// Description:
///   Normalize the mantissa
///   1) If a number does not have a leading digit != 0 then left shift until 
///   it has and adjust the exponent accordingly and return it.
///   2) Then remove trailing zeros
///   3) The mantissa NEVER contain a leading sign
//
int _float_precision_normalize( std::string *m )
	{
	int expo = 0;
	std::string::iterator pos;

	// Left shift until a digit is not 0
	for( pos = m->begin(); pos != m->end() && FDIGIT( *pos ) == 0; ++pos )
		{
		//m->erase( pos );  // dont do single erase
		expo--;
		}
	if (0 != expo)
		m->erase(0, -expo);

	if( m->length() == 0 ) // If all zero the number is zero
		{
		*m = FCHARACTER(0);
		return 0;
		}

	_float_precision_strip_trailing_zeros( m );

	return expo;
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Round the mantisaa to significant digits and rounding control
///	@return 	int - Return the exponent adjustment (0 or 1) 
///	@param   "m"	-	digital string
///   @param   "sign"   - The sign of the number
///   @param   "precision" - The digital precision
///   @param   "mode"   - Rounding mode 
///
///	@todo  
///
/// Description:
///   Rounding control
///   Round the fraction to the number of precision based on the round mode 
///   Note that the mantissa number has ALWAYS been normalize prior to rounding
///   The mantissa NEVER contain a leading sign
///   Rounding Mode Positive numnber   Result    
///   Rounding to nearest              +�   
///   Rounding toward zero (Truncate)  Maximum, positive finite value   
///   Rounding up (toward +�)          +�   
///   Rounding down) (toward -�)       Maximum, positive finite value   
///
///   Rounding Mode Negative number    Result    
///   Rounding to nearest              -�   
///   Rounding toward zero (Truncate)  Maximum, negative finite value   
///   Rounding up (toward +�)          Maximum, negative finite value   
///   Rounding down) (toward -�)       -�   
//
int _float_precision_rounding( std::string *m, int sign, size_t precision, enum round_mode mode )
   {
   enum round_mode rm = mode;

   if( m->length() > precision )  // More digits than we need 
      {
      if( rm == ROUND_NEAR )
         {
         if( 2 * FDIGIT( (*m)[ precision ] ) >= F_RADIX )
            rm = ROUND_UP; //sign < 0 ? ROUND_DOWN : ROUND_UP;
         else
            rm = ROUND_DOWN; // sign < 0 ? ROUND_UP : ROUND_DOWN;
         }
      else
         if( rm == ROUND_UP && sign < 0 )
            rm = ROUND_DOWN;
         else
            if( rm == ROUND_DOWN && sign < 0 )
               rm = ROUND_UP;

      // Chuck excessive digits
      m->erase( (std::string::size_type)precision, m->length() - precision );

      if( rm == ROUND_UP ) 
         {
         size_t before;

         before = m->length();
         *m = _float_precision_uadd_short( m, 1 );
         if( m->length() > before )
            {
            if( m->length() > precision )
               m->erase( (std::string::size_type)precision, m->length() - precision );

            _float_precision_strip_trailing_zeros( m );            
            return 1;
            }
         }
      }

   _float_precision_strip_trailing_zeros( m );            

   return 0;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	Compare to floating point string (mantissa on;y)
///	@return 	int - Return the compared result. 0==same, 1==s1>s2 or -1==s1<s2
///	@param   "s1"	-	First digital string
///   @param   "s2"  - Second digital string
///
///	@todo  
///
/// Description:
///   Compare two unsigned decimal string 
///   and return 0 is equal, 1 if s1 > s2 otherwise -1
///   Optimized check length first and determine 1 or -1 if equal
///   compare the strings
//
int _float_precision_compare( std::string *s1, std::string *s2 )
	{
	int cmp;

	if( s1->length() > s2->length() )
		cmp = 1;
	else
		if( s1->length() < s2->length() )
			cmp = -1;
		else
			cmp = s1->compare( *s2 );

	return cmp;
	}


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	add a short integer to a floating point string (mantissa)
///	@return 	std::string - Return the added string
///	@param   "src1"	-	The source string
///   @param   "d"  - The number to add
///
///	@todo  
///
/// Description:
///   Short float Add: The digit d [0..F_RADIX] is added to the unsigned fraction string
///   Optimized 0 add or early out add is implemented
//
std::string _float_precision_uadd_short( std::string *src1, unsigned int d )
   {
   unsigned short ireg;
   std::string::reverse_iterator r1_pos, rd_pos;
   std::string des1;

   des1.reserve( src1->capacity() );
   des1 = *src1;
   if( d > F_RADIX )
      {
      throw float_precision::out_of_range();
      }

   if( d == 0 )   // Zero add
      return des1;

   ireg = (unsigned short)( F_RADIX * d );
   rd_pos = des1.rbegin();
   r1_pos = src1->rbegin();
   
   for(; r1_pos != src1->rend(); r1_pos++, rd_pos++ )
      {
      ireg = (unsigned short)( FDIGIT( *r1_pos ) + FCARRY( ireg ) ); 
      *rd_pos = FCHARACTER( (unsigned char)FSINGLE( ireg ) );
      if( FCARRY( ireg ) == 0 ) // Early out add
         break;
      }

   if( FCARRY( ireg ) != 0 )  // Insert the carry in the front of the number
      des1.insert( (std::string::size_type)0, 1, FCHARACTER( (unsigned char)FCARRY( ireg ) ) );

   return des1;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	add two floating point string
///	@return 	std::string - Return the added string
///	@param   "src1"	-	The first source string
///   @param   "src2"  - The second source string
///
///	@todo  
///
/// Description:
///   Add two unsigned decimal strings
///   Optimized: Used early out add
//
std::string _float_precision_uadd( std::string *src1, std::string *src2 )
   {
   unsigned short ireg = 0;
   std::string des1;
   std::string::reverse_iterator r_pos, r_end, rd_pos;

   if( src1->length() >= src2->length() )
      {
      des1 = *src1; 
      r_pos = src2->rbegin();
      r_end = src2->rend();
      }
   else
      {
      des1 = *src2;
      r_pos = src1->rbegin();
      r_end = src1->rend();
      }
   rd_pos = des1.rbegin();
   
   for(; r_pos != r_end;)
      { // Adding element by element for the two numbers
      ireg = (unsigned short)( FDIGIT( *r_pos ) + FDIGIT( *rd_pos ) + FCARRY( ireg ) );
      *rd_pos = FCHARACTER( (unsigned char)FSINGLE( ireg ) );
      r_pos++;
      rd_pos++;
      }

   // Exhaust the smalles of the number, so only the carry can changes the uppper radix digits
   for( ; FCARRY( ireg ) != 0 && rd_pos != des1.rend(); )
      {
      ireg = (unsigned short)( FDIGIT( *rd_pos ) + FCARRY( ireg ) );
      *rd_pos = FCHARACTER( (unsigned char)FSINGLE( ireg ) );
      rd_pos++;
      }

   // No more carry or end of upper radix number. 
   if( FCARRY( ireg ) != 0 ) // If carry add the carry as an extra radix digit to the front of the number
      des1.insert( (std::string::size_type)0, 1, FCHARACTER( (unsigned char)FCARRY( ireg ) ) );

   return des1;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	subtract a short integer from a floating point string (mantissa)
///	@return 	std::string - Return the subtracted string
///   @param   "result" -  If the number wraps around (d > src1 ) then result=1 otherwise 0
///	@param   "src1"	-	The source string
///   @param   "d"  - The number to subtract
///
///	@todo  
///
/// Description:
///   Short Subtract: The digit d [0..F_RADIX] is subtracted from a unsigned decimal string
///   if src1 < src2 return -1 (wrap around) otherwise return 0 (no wrap around)
//
std::string _float_precision_usub_short( int *result, std::string *src1, unsigned int d )
   {
   unsigned short ireg = RADIX;
   std::string::reverse_iterator r1_pos;
   std::string des1;

   if( d > F_RADIX )
      { throw float_precision::out_of_range(); }

   if( d == 0 ) // Nothing to subtract
      {
      *result = 0;
      return *src1;
      }
   des1.reserve( src1->capacity() );
   // des1.erase();	// Not needed
   r1_pos = src1->rbegin();

   ireg = (unsigned short)( F_RADIX - 1 + FDIGIT( *r1_pos ) - d + FCARRY( ireg ) );
   des1.push_back( FCHARACTER( (unsigned char)FSINGLE( ireg ) ) );
   for( r1_pos++; FCARRY( ireg ) && r1_pos != src1->rend(); r1_pos++ )
      {
      ireg = (unsigned short)(F_RADIX - 1 + FDIGIT( *r1_pos ) + FCARRY( ireg ) );
      des1.push_back( FCHARACTER( (unsigned char)FSINGLE( ireg ) ) );
      }
   reverse(des1.begin(), des1.end());
   *result = FCARRY( ireg ) - 1;
   return des1;
   }



///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	subtract two floating point string
///	@return 	std::string - Return the subtracted string
///   @param   "result" -  If the number wraps around (d > src1 ) then result=1 otherwise 0
///	@param   "src1"	-	The first source string
///   @param   "src2"  - The second source string
///
///	@todo  
///
/// Description:
///   Subtract two unsigned decimal strings
///   if src1 < src2 return -1 (wrap around) otherwise return 0 (no wrap around)
//
std::string _float_precision_usub( int *result, std::string *src1, std::string *src2 )
   {
   unsigned short ireg = F_RADIX;
   std::string::reverse_iterator r1_pos, r2_pos;
   std::string des1;

   r1_pos = src1->rbegin();
   r2_pos = src2->rbegin();
   des1.reserve(std::max(src1->size(),src2->size()) + 8);

   for(; r1_pos != src1->rend() || r2_pos != src2->rend();)
      {
      if( r1_pos != src1->rend() && r2_pos != src2->rend() )
	  {
		  ireg = (unsigned short)(F_RADIX - 1 + FDIGIT(*r1_pos) - FDIGIT(*r2_pos) + FCARRY(ireg)); ++r1_pos; ++r2_pos;
	  }
      else
         if( r1_pos != src1->rend() )
            { ireg = (unsigned short)( F_RADIX - 1 + FDIGIT( *r1_pos ) + FCARRY( ireg ) ); r1_pos++; }
         else
            { ireg = (unsigned short)( F_RADIX - 1 - FDIGIT( *r2_pos ) + FCARRY( ireg ) ); r2_pos++; }
      des1.push_back( FCHARACTER( (unsigned char)FSINGLE( ireg ) ) );  // Collect in reverse order. Faster than using insert
      }
   reverse( des1.begin(), des1.end() );

   *result = FCARRY( ireg ) - 1;
   return des1;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	multiply a short integer to a floating point string (mantissa)
///	@return std::string - Return the multiplied string
///	@param  "src1"	-	The source string
/// @param   "d"  - The number to multiply
///
///	@todo  
///
/// Description:
///   Short float Multiplication: The unsigned digit d [0..F_RADIX] is multiplied to the unsigned fraction 
///   Optimize: Multiply with zero yields zero, multiply with RADIX and multiply with one.
//
std::string _float_precision_umul_short( std::string *src1, unsigned int d )
   {
   unsigned short ireg = 0;
   std::string::reverse_iterator r1_pos;
   std::string des1;

   des1.reserve( src1->capacity() );
   if( d > F_RADIX )
      { throw float_precision::out_of_range(); }

   if( d == 0 )  // Multiply with zero
      {
      des1.push_back( (char)FCHARACTER(0) );
      return des1;
      }

   if( d == 1 )  // Multiply with 1
      {
      des1 = *src1;
      return des1;
      }

   if( d == F_RADIX )  // Multiply with RADIX
      {
      des1 = *src1;
	  des1.append( 1, FCHARACTER(0) );
      _float_precision_strip_leading_zeros( &des1 );
      return des1;
      }

   // des1.erase();             // Not needed
   r1_pos = src1->rbegin();
   for( ; r1_pos != src1->rend(); r1_pos++ )
      {
      ireg = (unsigned short)( FDIGIT(  *r1_pos ) * d + FCARRY( ireg ) );
      des1.push_back( FCHARACTER( (unsigned char)FSINGLE( ireg ) ) );
      }

   if( FCARRY( ireg ) != 0 )
      des1.push_back( FCHARACTER( (unsigned char)FCARRY( ireg ) ) );
   reverse( des1.begin(), des1.end() );

   return des1;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	multiply two floating point string
///	@return 	std::string - Return the multiplied string
///	@param   "src1"	-	The first source string
///   @param   "src2"  - The second source string
///
///	@todo  
///
/// Description:
///   Multiply two unsigned decimal strings
///   Optimized: Used early out add and multiplication w. zero
///   NO LONGER IN USE. Replaced by _float_precision_umul_fourier
//
std::string _float_precision_umul( std::string *src1, std::string *src2 )
   {
   int disp;
   std::string des1, tmp;
   std::string::reverse_iterator r_pos2;
   
   r_pos2 = src2->rbegin();
   des1.reserve(src1->size() + src2->size() + 8);
   des1 = _float_precision_umul_short( src1, FDIGIT( *r_pos2 ) );
   for( r_pos2++, disp = 1; r_pos2 != src2->rend(); disp++, r_pos2++ )
      {
      if( FDIGIT( *r_pos2 ) != 0 )
         {
         tmp = _float_precision_umul_short( src1, FDIGIT( *r_pos2 ) );
         tmp.append( disp, FCHARACTER( 0 ) );
         des1 = _float_precision_uadd( &des1, &tmp );
         }
      }

   _float_precision_strip_leading_zeros( &des1 ); 

   return des1;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	multiply two floating point string unsing a fourie transformation
///	@return 	std::string - Return the multiplied string
///	@param   "src1"	-	The first source string
///   @param   "src2"  - The second source string
///
///	@todo  
///
/// Description:
///   Multiply two unsigned decimal strings
///   Optimized: Used early out add and multiplication w. zero
///   This is considerable faster than the previous methode and is used now
//
std::string _float_precision_umul_fourier( std::string *src1, std::string *src2 )
   {
   unsigned short ireg = 0;
   std::string des1;
   std::string::iterator pos;
   size_t n, j, l, l1, l2;
   double *a, *b, cy;
   
   l1 = src1->length();
   l2 = src2->length();
   des1.reserve(l1 + l2 + 16);
   l = l1 < l2 ? l2 : l1;
   for( n = 1; n < l; n <<= 1 ) ;
   n <<= 1;
   a = new double [n];
   b = new double[n]; 
 
   for (l = 0, pos = src1->begin(); pos != src1->end(); ++l, ++pos )
	   a[l] = (double)FDIGIT(*pos);
   for (; l < n; ++l) a[l] = (double)0;
   for (l = 0, pos = src2->begin(); pos != src2->end(); ++l, ++pos)
	   b[l] = (double)FDIGIT(*pos);
   for (; l < n; ++l) b[l] = (double)0;
#if true
#pragma omp parallel sections firstprivate(a,b,n)
   {
#pragma omp section 
	   {
	   _int_real_fourier(a, n, 1);
	   }
#pragma omp section
	   {
	   _int_real_fourier(b, n, 1);
	   }
   }
#else
   _int_real_fourier(a, n, 1);
   _int_real_fourier(b, n, 1);
#endif


   b[0] *= a[0];
   b[1] *= a[1];
   for( j = 2; j < n; j += 2 )
      {
      double t;
      b[j]=(t=b[j])*a[j]-b[j+1]*a[j+1];
      b[j+1]=t*a[j+1]+b[j+1]*a[j];
      }
   _int_real_fourier( b, n, -1 );
   for( cy=0, j=0; j <= n-1; ++j )
      {
      double t;
      t=b[n-1-j]/(n>>1)+cy+0.5;
      cy=(unsigned long)( t/ F_RADIX );
      b[n-1-j]=t-cy*F_RADIX;
      }

   ireg = (unsigned short)cy;
   if( ireg != 0 )
      des1.append( 1, FCHARACTER( (char)ireg ) );
   for( j = 0; j < (int)(l1 + l2 -1); j++ )
      des1.append( 1, FCHARACTER( (char)b[ j ] ) );
   
   _float_precision_strip_leading_zeros( &des1 );
   delete [] a, b;

   return des1;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 	divide a short integer into a floating point string (mantissa)
///	@return 	std::string - Return the divided floating point string
///   @param   "remaind" -  Any remainding portion of the division
///	@param   "src1"	-	The source string
///   @param   "d"  - The number to divide
///
///	@todo  
///
/// Description:
///   Short Division: The digit d [1..F_RADIX] is divide up into the unsigned decimal string
//
std::string _float_precision_udiv_short( unsigned int *remaind, std::string *src1, unsigned int d )
	{
	int i, ir;
	std::string::iterator s1_pos;
	std::string des1;
   
	if( d > F_RADIX )
		{ throw float_precision::out_of_range(); }

	if( d == 0 )
		{ throw float_precision::divide_by_zero(); }

	if (d == 1)
		{
		des1 = *src1;
		*remaind = 0;
		return des1;
		}

	// des1.erase();  // Not needed
	s1_pos = src1->begin();
	des1.reserve(src1->size() + 8);
	ir = 0;
	for(; s1_pos != src1->end(); s1_pos++ )
		{
		i = F_RADIX * ir + FDIGIT( *s1_pos );
		des1 += FCHARACTER( (unsigned char)( i / d ) );
		ir = i % d;
		}

	*remaind = ir;
	return des1;
	}

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  July 11 2010
///	@brief 	std::string _float_precision_udiv
///	@return 	std::string	-	the result of disivison
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Divide two unsigned decimal strings
///   Optimized: Used early out add and multiplication w. zero
//
std::string _float_precision_udiv( std::string *src1, std::string *src2 )
   {
   int wrap, plusdigit;
   std::string des, quotient, divisor;
   
   des = FCHARACTER(0);
   divisor = *src1;
   if( src2->length() == 1 ) // Make short div 
      return _float_precision_udiv_short( (unsigned int *)&wrap, &divisor, FDIGIT( (*src2)[0] ) );

   plusdigit = (int)divisor.length() - (int)src2->length();
   for(  ;plusdigit > 1; )
      {
      std::string tmp;

      quotient = (char)FCHARACTER(1);
      quotient.append( plusdigit, FCHARACTER( 0 ) );
      tmp = _float_precision_umul_fourier( src2, &quotient );
      if( _float_precision_compare( &divisor, &tmp ) < 0 )
         { // Too much reduce with one power of radix
         plusdigit--;
         quotient = (char)FCHARACTER(1);
         quotient.append( plusdigit, FCHARACTER( 0 ) );
         tmp = _float_precision_umul_fourier( src2, &quotient );
         }
      divisor = _float_precision_usub( &wrap, &divisor, &tmp );
        _float_precision_strip_leading_zeros( &divisor );
      des = _float_precision_uadd( &des, &quotient );
      plusdigit = (int)divisor.length() - (int)src2->length();
      }
   for( wrap = 0; wrap == 0; )
      {
      divisor = _float_precision_usub( &wrap, &divisor, src2 );
      if( wrap == 0 ) // src1 was indeed > src2
         des = _float_precision_uadd_short( &des, 1 );
      }

   _float_precision_strip_leading_zeros( &des );

   return des;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  Jul 11 2010
///	@brief 	std::string _float_precision_urem
///	@return 	std::string	-	the remaing result of divide src1 with src2
///	@param   "src1"	-	First unsigned source argument
///	@param   "src2"	-	Second unsigned source argument
///
///	@todo
///
/// Description:
///   Find the remainder when divide two unsigned decimal strings
///   Optimized: Used early out add and multiplication w. zero
//
std::string _float_precision_urem( std::string *src1, std::string *src2 )
   {
   int wrap, plusdigit;
   std::string des, quotient, divisor;
   des.reserve(src2->size() + 8);
   des = FCHARACTER(0);
   divisor = *src1;

   if( src2->length() == 1 ) // Make short rem 
      {
      unsigned int rem;
      _float_precision_udiv_short( &rem, &divisor, FDIGIT( (*src2)[0] ) );
      des = FCHARACTER( (unsigned char)rem );
      return des;
      }

   plusdigit = (int)src1->length() - (int)src2->length();
   for( ; plusdigit > 1; )
      {
      std::string tmp;

      quotient = (char)FCHARACTER(1);
      quotient.append( plusdigit, FCHARACTER( 0 ) );
      tmp = _float_precision_umul_fourier( src2, &quotient );
      if( _float_precision_compare( &divisor, &tmp ) < 0 )
         { // Too much reduce with one power of radix
         plusdigit--;
         quotient = (char)FCHARACTER(1);
         quotient.append( plusdigit, FCHARACTER( 0 ) );
         tmp = _float_precision_umul_fourier( src2, &quotient );
         }
      divisor = _float_precision_usub( &wrap, &divisor, &tmp );
      _float_precision_strip_leading_zeros( &divisor );
      des = _float_precision_uadd( &des, &quotient );
      plusdigit = (int)divisor.length() - (int)src2->length();
      }

   for( wrap = 0; wrap == 0; )
      {
      des = divisor;
      divisor = _float_precision_usub( &wrap, &divisor, src2 );
      }

   _float_precision_strip_leading_zeros( &des );

   return des;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  8/26/2007
///	@brief 	return the epsilon such that 1.0+epsilon!=1.0
///	@return		float_precision	Return the epsilon for the given Radix and precision
/// @param   "-" -  None
///
///	@todo  
///
/// Description:
///   Return the epsilon: The number where 1.0+epsilon!=1.0
///	  This function was rewritten to use B^1-<precision> code with floating arguments instead of below standard pow()
///   that was way to time comsuing to execute in favor of this algorithm
///   res = pow(float_precision(F_RADIX, mPrec), float_precision(1-(int)mPrec));  // beta^1-t
///
float_precision float_precision::epsilon()
    {
	float_precision res( 1 );

	// Optimize for F_RADIX==BASE_10)
	if( F_RADIX == BASE_10 )
		{
		res.exponent( 1-(int)mPrec );
		}
	else
		{ // All other bases
		float_precision p( F_RADIX, mPrec );

	    // beta^1-t
		for(size_t n = mPrec-1; n > 0; n >>= 1) 
           {
           if( ( n & 0x1 ) != 0 ) res *= p;  // Odd
           p *= p;						 
           }
        res = _float_precision_inverse( res );
		}

	return res;
    }


//////////////////////////////////////////////////////////////////////////////////////
///
/// END FLOATING POINT CORE FUNCTIONS
///
///
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
///
/// FLOAT PRECISION FUNCTIONS
///
//////////////////////////////////////////////////////////////////////////////////////


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate the inverse of a 
///	@return 	   float_precision -	Return 1/a
///	@param      "a"	-	The float_precision number to inverse
///
///	@todo  
///
/// Description:
///   Inverse of V
///   Using a Newton iterations Un = U(2-UV)
///   Always return the result with 2 digits higher precision that argument
///   _float_precision_inverse() return a interim result for a basic operation like /
//
float_precision _float_precision_inverse( const float_precision& a )
   {
   size_t precision;
   size_t i, imax;
   int expo;
   double fv, dv, fu;
   float_precision r, u, v, c2;
   std::string::reverse_iterator rpos;
   std::string::iterator pos;
   std::string *p;

   precision = a.precision();  
   v.precision( precision + 2 );
   v = a;
   p= v.ref_mantissa();
   if( p->length() == 1 && FDIGIT( (*p)[0] ) == 0 )
      { throw float_precision::divide_by_zero(); }

   expo = v.exponent();
   v.exponent( 0 );
   r.precision( precision + 3 ); // Do iteration using 3 digits higher precision
   u.precision( precision + 3 );
   c2 = float_precision( 2, precision + 3 );

 	// New. Get a initial guess using ordinary floating point
	pos = v.ref_mantissa()->begin();
	fv = FDIGIT(*pos);
	imax = digits_to_check();
	for (i = 1, dv=1.0, ++pos;  i < imax && pos != v.ref_mantissa()->end(); ++pos, ++i)
		{
		dv /= (double)F_RADIX;
		fv += FDIGIT(*pos) * dv;
		}
   if( v.sign() < 0 )
      fv = -fv;
   fu = 1 / fv;
   u = float_precision( fu );
   
   // Now iterate using Netwon Un=U(2-UV)
   for(;;)
      {
      r = u * v;                 // UV
      r = c2-r;                  // 2-UV
      u *= r;                    // Un=U(2-UV)
      for( pos = r.ref_mantissa()->begin(), pos++, i = 0; pos != r.ref_mantissa()->end(); i++, pos++ )
         if( FDIGIT( *pos ) )
            break;

      if( pos == r.ref_mantissa()->end() || i >= precision )
         break;
      }

   u.exponent( u.exponent() - expo );
   u.mode( a.mode() );

   return u;
   }

// Float Precision support functions

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  12/Nov/2015
///	@brief 		Calculate sqrt(x)
///	@return 	   float_precision -	Return sqrt(a)
///	@param      "x"	-	The sqrt argument
///
///	@todo  
///
/// Description:
///   sqrt(V)
///   Equivalent with the same standard C function call
///   Seperate exponent. e.g. sqrt(V*10^x)=10^x/2*sqrt(V)
///   Un=0.5U(3-VU^2)
///   Then Un == 1/Sqrt(V). and sqrt(V) = VUn
/// The functionhas been improved using Newton with iterative deepening creating
///	a speed up with a factor of 3 over the classic Newton method.
//
float_precision sqrt(const float_precision& x)
	{
	const unsigned int extra = 2;
	size_t precision;
	size_t digits;
	int expo, expo_sq, i, imax;
	double fv, dv;
	float_precision r, u, v, tmp;
	const float_precision c0(0), c1(1), c3(3);
	const float_precision c05(0.5);
	std::string::reverse_iterator rpos;
	std::string::iterator pos;

	if (x == c0 || x == c1)
		return x;
	if (x.sign() < 0)
		{ throw float_precision::domain_error(); }
	precision = x.precision();
	v.precision(precision + extra);
	v = x;
	expo = v.exponent();
	expo_sq = expo / 2;
	v.exponent(expo - 2 * expo_sq);
	r.precision(precision + extra); // Do iteration using 2 digits higher precision
	u.precision(precision + extra);

	// Get a initial guess using ordinary floating point
	// New. Get a initial guess using ordinary floating point
	pos = v.ref_mantissa()->begin();
	fv = FDIGIT(*pos);
	imax = digits_to_check();
	for (i = 1, dv = 1.0, ++pos; i < imax && pos != v.ref_mantissa()->end(); ++pos, ++i)
		{
		dv /= (double)F_RADIX;
		fv += FDIGIT(*pos) * dv;
		}
//	rpos = v.ref_mantissa()->rbegin();
//	dv = FDIGIT(*rpos);
//	for (++rpos; rpos != v.ref_mantissa()->rend(); ++rpos )
//		{
//		dv *= (double)1 / (double)F_RADIX;
//		dv += FDIGIT(*rpos);
//		}
	
	if (expo - 2 * expo_sq > 0)
		fv *= (double)F_RADIX;
	else
		if (expo - 2 * expo_sq < 0)
			fv /= (double)F_RADIX;
	fv = 1 / sqrt(fv);  // set the initial guess with at approx 16 correct digits

	tmp.precision(precision+1);
	u = float_precision(fv);
	// Now iterate using Netwon Un=0.5U(3-VU^2)
	for (digits = min((size_t)32, precision); ; digits = min(precision + 2, digits * 2))
		{
		// Increase precision by a factor of two for the working variable s r & u. 
		r.precision(digits);
		u.precision(digits);
		// Notice V is the original number to squareroot which has the full precision 
		// so we start by assigning it to r, rounding it to the precision of r
		r = v;						// V
		r *= u * u;					// VU^2
		r = c3 - r;					// 3-VU^2
		r *= c05;					// (3-VU^2)/2
		u *= r;						// U=U(3-VU^2)/2
		if (digits == precision + 2) // Reach final iteration step in regards to precision
			{
			tmp = r;		// round to final precision
			if (tmp == c1)	// break if no improvement
				break;
			}
		}

	u *= v;
	u.exponent(u.exponent() + expo_sq);
	// Round to same precision as argument and mrounding mode
	u.mode(x.mode());
	u.precision(precision);

	return u;
	}


//////////////////////////////////////////////////////////////////////////////////////
///
/// FLOAT PRECISION
///    Universal Constants LN2, LN10, e and PI
///
//////////////////////////////////////////////////////////////////////////////////////

/// Spigot function for internal calculation of transcendental constants

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  29/Jan/2017
///	@brief 	Calculate transcendetal constant of pi
///	@return std::string -	return the constant as a standard std::string
///	@param   "digits"	-	Number of digits
/// @param   "no_dig"	-	Number of digits calculated per loop
///
///	@todo
///
/// Description:
///
// 64bit version of the spigot algorithm.
// Notice acc, a, g needs to be unsigned 64bit. 
// Emperisk for pi to 2^n digits, acc need to hold approx 2^(n+17) numbers. while a[] and g needs approx 2^(n+3) numbers
// a[] & g could potential be unsigned long (32bit) going to a max of 2^29 digit or 536millions digit of PI. but with 
// unsigned 64bit you can do "unlimited"
static std::string spigot_pi_64(const int digits, int no_dig = 4)
	{
	static unsigned long f_table[] = { 0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
	static unsigned long f2_table[] = { 0,  2,  20,  200,  2000,  20000,  200000,  2000000,  20000000 };
	const int TERMS = (10 * no_dig / 3 + 1);
	bool first_time = true;									// First time in loop flag
	bool overflow_flag = false;								// Overflow flag
	char buffer[32];
	std::string ss;											// The String that hold the calculated PI											// Timer
	long b, c;												// Loop counters
	int carry, no_carry = 0;								// Outer loop carrier, plus no of carroer adjustment counts
	unsigned long f, f2;									// New base 1 decimal digits at a time
	unsigned long dig_n = 0;								// dig_n holds the next no_dig digit to add
	unsigned long e = 0;									// Save previous 4 digits
	unsigned long long acc = 0, g = 0, tmp64;
	ss.reserve(digits + 16);								// Pre reserve the string size to be able to accumulate all digits plus 8
	if (no_dig > 8) no_dig = 8;								// ensure no_dig<=8
	if (no_dig < 1) no_dig = 1;								// Ensure no_dig>0
	c = (digits / no_dig + 1) * no_dig;						// Since we do collect PI in trunks of no_dig digit at a time we need to ensure digits is divisble by no_dig.
	if (no_dig == 1) c++;									// Extra guard digit for 1 digit at a time.
	c = (c / no_dig + 1) * TERMS;							// c ensure that the digits we seek is divisble by no_dig 
	f = f_table[no_dig];									// Load the initial f
	f2 = f2_table[no_dig];									// Load the initial f2

	unsigned long long  *a = new unsigned long long[c];		// Array of 4 digits decimals
															// b is the nominator previous base; c is the index
	for (; (b = c -= TERMS) > 0 && overflow_flag == false; first_time = false)
		{
		for (; --b > 0 && overflow_flag == false;)
			{
			if (acc > ULLONG_MAX / b) overflow_flag = true;		// Check for overflow
			acc *= b;											// Accumulator *= nom previous base
			tmp64 = f;
			if (first_time == true)								// Test for first run in the main loop
				tmp64 *= f2;									// First outer loop. a[b] is not yet initialized
			else
				tmp64 *= a[b];									// Non first outer loop. a[b] is initialized in the first loop
			if (acc > ULLONG_MAX - tmp64) overflow_flag = true;	// Check for overflow
			acc += tmp64;										// add it to accumulator
			g = b + b - 1;										// denominated previous base
			a[b] = acc % g;										// Update the accumulator
			acc /= g;											// save carry
			}
		dig_n = (unsigned long)(e + acc / f);					// Get previous no_dig digits. Could occasinaly be no_dig+1 digits in which case we have to propagate back the extra digit.
		carry = (unsigned)(dig_n / f);							// Check for extra carry that we need to propagate back into the current sum of PI digits
		dig_n %= f;												// Eliminate the extra carrier so now l contains no_dig digits to add to the string
																// Add the carrier to the existing number for PI calculate so far.
		if (carry > 0)
			{
			++no_carry;											// Keep count of how many carrier detect
			for (size_t i = ss.length(); carry > 0 && i > 0; --i)	// Loop and propagate back the extra carrier to the existing PI digits found so far
				{												// Never seen more than one loop here but it can handle multiple carry back propagation 
				int new_digit;
				new_digit = (ss[i - 1] - '0') + carry;			// Calculate new digit
				carry = new_digit / 10;							// Calculate new carry if any
				ss[i - 1] = new_digit % 10 + '0';				// Put the adjusted digit back in our PI digit list
				}
			}

		(void)sprintf_s(buffer, sizeof(buffer), "%0*lu", no_dig, dig_n);		// Print previous no_dig digits to buffer
		ss += std::string(buffer);								// Add it to PI string
		if (first_time == true)
			ss.insert(1, ".");									// add the decimal pointafter the first digit to create 3.14...
		acc = acc % f;											// save current no_dig digits and repeat loop
		e = (unsigned long)acc;
		}

	ss.erase(digits + 1);											// Remove the extra digits that we didnt requested but used as guard digits
	if (overflow_flag == true)
		ss = std::string("Overflow:") + ss;						// Set overflow in the return string
	delete [] a;												// Delete the a[];	
	return ss;													// Return Pi with the number of digits
	}

/// End Spigot PI

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  29/Jan/2017
///	@brief 	Calculate transcendetal constant of exp(1)
///	@return std::string -	return the constant as a standard std::string
///	@param   "digits"	-	Number of digits
///
///	@todo
///
/// Description:
///
/// Spigot algorithm for e
/// From The computer Journal 1968 (A H J Sale) written in Algo 60 and ported with some modification 
/// to c++
static std::string spigot_e( const size_t digits)
	{
	unsigned int m;
	unsigned int tmp, carry;
	double test = (digits+1) * log(10);
	bool first_time = true;
	unsigned int *coef;
	std::string ss("2.");
	ss.reserve(digits + 16);
	double xnew, xold;

	// Stirling approximation of m!~Sqrt(2*pi*digits)(digits/e)^digits.
	// Taken ln on both side you get: m*(Math.log((m)-1)+0.5*Math.log(2*Math.pi*m);
	// Use Newton method to find in less that 4-5 iteration
	for (xold = 5, xnew = 0; ; xold = xnew)
		{
		double  f = xold*(log(xold) - 1) + 0.5*log(2 * 3.141592653589793 * xold);
		double f1 = 0.5 / xold + log(xold);
		xnew = xold - (f - test) / f1;
		if ((int)ceil(xnew) == (int)ceil(xold))
			break;
		}
	m = (unsigned int)ceil(xnew);
	if (m < 5)
		m = 5;
	coef = new unsigned int[m + 1];

	for (size_t i = 1; i < digits; ++i, first_time = false)
		{
		carry = 0;
		for (unsigned int j = m; j >= 2; j--)
			{
			if (first_time == true)
				tmp = 10;
			else
				tmp = coef[j] * 10;
			tmp += carry;
			carry = tmp / (j);
			coef[j] = tmp % (j);
			}
		ss.append(1, (char)(carry + '0'));
		}
	delete [] coef;
	return ss;
	}

/// End Spigot e

/// Spigot LN(X/Y) where x and y are integers and x>0 && x>y as conditions

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  28/Jan/2017
///	@brief 	Calculate transcendetal constant of ln(x/y)
///	@return std::string -	return the constant as a standard std::string
///	@param   "x"	-	The nominator of the number x
/// @param	 "y"	-	The Denominator of the number x
///	@param   "digits"	-	Number of digits
/// @param   "no_dig"	-	Number of digits calculated per loop
///
///	@todo
///
/// Description:
///
/// 64 bit version of spigot algorithm for LN(x/y) fraction 
/// It has automatic 64bit integer overflow detection in which case the result start with the string "Overflow...."
/// A Column: x-1,x-1,x-1,...,x-1
/// B Column: x,x,x,x,x,...,x
/// Initialization values: (x-1)/(x(n+1))...
/// The function is declare static since it only serve as a sub function for the function _float_table()
///
static std::string spigot_lnxy_64(const unsigned int x, const unsigned int y, const size_t digits, int no_dig = 1)
	{
	static unsigned long f_table[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
	bool first_time = true;				// First iteration of the algorithm
	bool overflow_flag = false;			// 64bit integer overflow flag
	char buffer[32];
	std::string ss;						// The std::string that holds the ln(x)
	size_t dig;
	unsigned int car;
	size_t no_terms;				// No of terms to complete as a function of digits
	unsigned long f;					// New base 1 decimal digits at a time
	unsigned long dig_n;				// dig_n holds the next no_dig digit to add
	unsigned long long carry;
	unsigned long long tmp_n, tmp_dn;
	ss.reserve(digits + 16);
	int factor;

	if (x < y || x < 1)
		{ throw float_precision::domain_error(); }

	if (no_dig > 8) no_dig = 8;			// Ensure no_dig<=8
	if (no_dig < 1) no_dig = 1;			// Ensure no_dig>0
	// Since we do it in trunks of no_dig digits at a time we need to ensure digits is divisble with no_dig.
	dig = (digits / no_dig + (digits%no_dig>0 ? 1 : 0)) * no_dig;
	dig += no_dig;						// Extra guard digits
										// Calculate the number of terms needed
	factor = (int)ceil(10 * log(0.5) / log((double)(x - y) / (double)x));
	no_terms = (unsigned int)(factor * dig / 3 + 3);
	// Allocate the needed accumulators
	unsigned long long *acc_n = new unsigned long long[no_terms + 1];
	unsigned long long *acc_dn = new unsigned long long[no_terms + 1];
	f = f_table[no_dig];				// Load the initial f
	carry = 0;							// Set carry to 0
	//Loop for each no_dig
	for (size_t i = 0; i <= dig && overflow_flag == false; i += first_time == true ? 1 : no_dig, first_time = false)
		{
		// Calculate new number of terms needed
		no_terms = (unsigned int)(factor * (dig-i) / 3 + 3);
		// Loop for each no_terms
		for (size_t j = no_terms; j>0 && overflow_flag == false; --j)
			{
			if (first_time == true)
				{// Calculate the initialize value
				tmp_dn = (j + 1) * x;
				tmp_n = (x - y);
				}
			else
				{
				tmp_n = acc_n[j];
				tmp_dn = acc_dn[j];
				}
			if (tmp_n > (ULLONG_MAX) / f)
				overflow_flag = true;
			tmp_n *= f;		// Scale it
			// Check for 64bit overflow. Not very likely 
			if (carry > 0 && tmp_dn > (ULLONG_MAX - tmp_n) / carry)
				overflow_flag = true;
			tmp_n += carry * tmp_dn;
			carry = (tmp_n / (x * tmp_dn));
			carry *= (x - y);
			acc_n[j] = tmp_n % (tmp_dn * x);
			acc_dn[j] = tmp_dn;
			}

		if (first_time == true)
			{
			tmp_n = (x - y) * f;
			if (carry > 0 && tmp_n > (ULLONG_MAX - carry * x))
				overflow_flag = true;

			acc_n[0] = (tmp_n + carry*x);
			acc_dn[0] = x;
			dig_n = (unsigned)(acc_n[0] / (f*acc_dn[0]));
			}
		else
			{
			if (acc_n[0] > (ULLONG_MAX - carry * acc_dn[0]) / f)
				overflow_flag = true;
			dig_n = (unsigned)((acc_n[0] * f + carry * acc_dn[0]) / (f*acc_dn[0]));
			}
		car = (unsigned)(dig_n / f);
		dig_n %= f;
		// Add the carry to the existing number for digits calculate so far.
		if (car > 0)
			{
			for (size_t j = ss.length(); car > 0 && j > 0; --j)
				{
				unsigned int dd;
				dd = (ss[j - 1] - '0') + car;
				car = dd / 10;
				ss[j - 1] = dd % 10 + '0';
				}
			}
		(void)sprintf_s(buffer, sizeof(buffer), "%0*lu", first_time == true ? 1 : no_dig, dig_n);
		ss += std::string(buffer);
		if (first_time == true)
			acc_n[0] %= f*acc_dn[0];
		else
			{
			acc_n[0] = acc_n[0] * f + carry *acc_dn[0];
			acc_n[0] %= f  * acc_dn[0];
			}
		carry = 0;
		}

	ss.insert(1, ".");// add a . come after the first digit to create 2.30...
	if (overflow_flag == false)
		ss.erase(digits + 1); // Remove the extra digits that we didnt requested.
	else
		ss = std::string("Overflow:") + ss;

	delete[] acc_n;
	delete[] acc_dn;
	return ss;
	}

/// End Spigot LN(X/Y)

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/24/2005
///	@brief 	Lookup or generate "fixed" constant ln2, PI log10 etc
///	@return 	float_precision	-	return the new table lookup value
///	@param   "tt"	-	Which table type lookup is needed
///	@param    "precision"	-	Number of significant digits
///
///	@todo
///
/// Description:
///   Dynamic tables for "fixed" constant like ln(2), ln(10), e and PI
///   If a higher precision is requested we create it and return otherwise 
///   we just the "constant" at a higher precision which eventually will be
///   rounded to the destination variables precision 
//
float_precision _float_table( enum table_type tt, size_t precision )
   {
   static float_precision ln2( 0, 0, ROUND_NEAR );
   static float_precision ln10( 0, 0, ROUND_NEAR );
   static float_precision pi( 0, 0, ROUND_NEAR );
   static float_precision e( 0, 0, ROUND_NEAR);
   const float_precision c1(1);
   float_precision res(0, precision, ROUND_NEAR);

   switch( tt )
      {
	  case _EXP1:
	      if (e.precision() >= precision)
		     res = e;
	      else
		     {// Using Spigot algorithm for exp(1) Calculation
		     std::string ss;
		     size_t prec = std::max((size_t)20U, precision + 2);
		     e.precision(prec);
		     ss = spigot_e( prec );				// The result as a string in BASE_10
		     e = float_precision(ss, prec);		// Convert to float_precision
		     e.precision(std::max((size_t)20U, precision));
		     res = e;
	         }
		  break;
      case _LN2:
		  if (ln2.precision() >= precision)
			  res = ln2;
		  else
			 { // Using Spigot algorithm for LN2 Calculation
			 std::string ss;
			 size_t prec = std::max((size_t)20U, precision+2);
			 ln2.precision( prec );
			 ss = spigot_lnxy_64(2, 1, prec, 4);	// The result as a string in BASE_10
			 ln2 = float_precision(ss, prec);		// Convert to float_precision
			 ln2.precision(std::max((size_t)20U, precision));
			 res = ln2;								// Save the result
			 }
		  break;
      case _LN10:
         if( ln10.precision() >= precision )
            res = ln10;
         else
			{ // Using Spigot Algorithm for LN10. LN(10)=3*ln(2)+ln(10/8)
			std::string ss;
			size_t prec=std::max((size_t)20U, precision + 2);
			ln10.precision(prec);
			ss = spigot_lnxy_64(2, 1, prec, 4);		// The result as a string in BASE_10
			ln10 = float_precision(ss, prec);		// Convert to float_precision
			ln10 *= float_precision(3);
			ss = spigot_lnxy_64(10, 8, prec, 4);	// The result as a string in BASE_10
			ln10 += float_precision(ss, prec);		// Convert and add to float_precision ln10
			ln10.precision(std::max((size_t)20U, precision));
			res = ln10;								// Save the result
			}
		 break;
      case _PI: 
         if( pi.precision() > precision )
			res = pi;
		 else
			{  // Using Brent-Salamin method
			size_t min_precision = precision + 5 + (F_RADIX == BASE_2 ? 5 : 0);
			const int limit = -(int)(precision + 2);
			const float_precision c0(0), c2(2), c05(0.5);
			float_precision a(1, min_precision), b(2, min_precision), sum(0.5, min_precision);
			float_precision ak(0, min_precision), bk(0, min_precision), ck(1, min_precision);
			float_precision ab(0, min_precision), asq(0, min_precision );
			float_precision pow2(1, precision);

			if (F_RADIX < BASE_10 && min_precision < PRECISION) min_precision = PRECISION;
			pi.precision(min_precision);
			b = c1 / sqrt(b);
			for (; ck != c0 && ck.exponent()>limit; )
				{
				ak = c05*(a + b);
				ab = a * b;
				bk = sqrt(ab);
				asq = ak * ak;
				ck = asq - ab;
				pow2 *= c2;
				sum -= pow2*ck;
				a = ak; b = bk;
				}
			pi = c2 * asq / sum;
			res = pi;
			// Round and store it
			pi.precision(precision);
			}	
		 break;
      }

   return res;
   }

//////////////////////////////////////////////////////////////////////////////////////
///
/// FLOAT PRECISION FUNCTIONS
///    Exp(), Log(), Log10(), Nroot()
///
//////////////////////////////////////////////////////////////////////////////////////




/// Experimental new exp() using sinh() and sqrt()
///	@author Henrik Vestermark (hve@hvks.com)
///	@date  8/1/2013
///	@brief 		Calculate exp(x)
///	@return 	   float_precision -	Return exp(x)
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
///   Use a the identity that exp(x)=sinh(x)+sqrt(1+sinh(x)^2)
///	  This has proven to be faster than the standard taylor series for exp()
///   exp(x) == 1 + x + x^2/2!+x^3/3!+....
//	  A test i smade for x is an integer in which case we do pow(e,x) which is more than 400 times faster
//    sine e is calculated using the spigot algorithm using pure 64bit integer arithmetic
//
float_precision exp( const float_precision& x )
   {
   size_t precision;
   float_precision v;
   const float_precision c1(1);

   precision = x.precision()+2;  
   v.precision( precision );
   v = x;
   if( v.sign() < 0 )
      v.change_sign();

   if(floor(v)==v)  // v is an Integer
	  {// use the 100 times faster shortcut exp(v)=exp(1)^v
	  v = _float_table(_EXP1, precision );
	  v = pow( v, abs(x) );
	  }
   else
	  {
      v=sinh(v);
      v.precision( 2 * precision );  // Double the precision to avoid loss of significant when performaing 1+v*v
      v=v+sqrt(c1+v*v);
	  v.precision( precision );
      }

   if( x.sign() < 0 )
      v = _float_precision_inverse( v );
   // Round to same precision as argument and rounding mode
   v.mode( x.mode() );v.mode( x.mode() );
   v.precision( x.precision() );  

   return v;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate log(x)
///	@return 	   float_precision -	Return log(x)
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
///   Use a taylor series until their is no more change in the result
///   Equivalent with the same standard C function call
///   ln(x) == 2( z + z^3/3 + z^5/5 ...
///   z = (x-1)/(x+1)
//
float_precision log( const float_precision& x )
   {
   size_t precision;
   int j, k;
   int expo;
   double zd, dlimit;
   float_precision res, r, z, z2;
   const float_precision c1(1);

   if( x <= float_precision(0) ) 
      { throw float_precision::domain_error(); }

   precision = x.precision() + 2;  
   z.precision( precision ); // Do calc at 2 higher precision to allow correct rounding of result
   z = x;
   expo = z.exponent();
   z.exponent( 0 );
   // Check for augument reduction and increase precision if necessary
   zd=PLOG10( precision );
   zd *= zd;
   j=(int)zd; if(j>16) j=16;
   dlimit=1<<(j-1);
   dlimit=pow( 1.2, 1.0/dlimit);
   zd=(double)z;
   for( k = 0; zd > dlimit; k++ )  
       zd = sqrt(zd);
   if(k>0)
       precision=precision + PADJUST( k/4 );
   z.precision( precision ); // adjust precision to allow correct rounding of result
   r.precision( precision ); 
   z2.precision( precision );
   res.precision( precision );
   
   // In order to get a fast Taylor series result we need to get the fraction closer to one
   // The fraction part is [1.xxx-9.999] (base 10) OR [1.xxx-255.xxx] (base 256) at this point
   // Repeat a series of square root until z < dlimit
   for( k = 0; z > float_precision( dlimit ); k++ )
       z = sqrt(z);

   // Calculate the fraction part now at [1.xxx-1.1999]
   z = ( z - c1 ) / ( z + c1 );
   z2 = z * z;
   res = z;
   // Iterate using taylor series ln(x) == 2( z + z^3/3 + z^5/5 ... )
   for( j=3;;j+=2 )
      {
      z *= z2;
      r = z/float_precision(j);
      if( res + r == res )
         break;
      res += r;
      }

   res *= float_precision( pow( 2.0, (double)( k + 1 ) ) );
   if( expo != 0 )
      {
        switch( F_RADIX )
            {
            case BASE_2:
                // Ln(x^y) = Ln(x) + Ln(2^y) = Ln(x) + y * ln(2) 
                res += float_precision( expo ) * _float_table( _LN2, precision + 1 );
                break;
            case BASE_8:
                // Ln(x^y) = Ln(x) + Ln(2^y) = Ln(x) + y * ln(2) = Ln(x) + y * ln(2^3) = Ln(x) + y * 3 * ln(2)
                res += float_precision( expo ) * float_precision( 3 ) * _float_table( _LN2, precision + 1 );
                break;
            case BASE_10:
                // Ln(x^y) = Ln(x) + Ln(10^y) = Ln(x) + y * ln(10)
                res += float_precision( expo ) * _float_table( _LN10, precision + 1 );
                break;
            case BASE_16:
                // Ln(x^y) = Ln(x) + Ln(16^y) = Ln(x) + y * ln(16) = Ln(x) + y * ln(2^4) = Ln(x) + y * 4 * ln(2)
                res += float_precision( expo ) * float_precision( 4 ) * _float_table( _LN2, precision + 1 );
                break;
            case BASE_256:
                // Ln(x^y) = Ln(x) + Ln(256^y) = Ln(x) + y * ln(256) = Ln(x) + y * ln(2^8) = Ln(x) + y * 8 * ln(2)
                res += float_precision( expo ) * float_precision( 8 ) * _float_table( _LN2, precision + 1 );
                break;
            default:  // Base not supported
                { throw float_precision::base_error(); }
                break;
            }
        }

   // Round to same precision as argument and rounding mode
   res.mode( x.mode() );
   res.precision( x.precision() );  

   return res;
   }



///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate log10(x)
///	@return 	   float_precision -	Return log10(x)
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
///   Log10. Use the equation log10(x)=log(x)/log(10)
///   Equivalent with the same standard C function call
//
float_precision log10( const float_precision& x )
   {
   size_t precision = x.precision();  
   float_precision res( 0, precision + 1 );

   if( x <= float_precision(0) ) 
      { throw float_precision::domain_error(); }

   res = x;
   res = log( res ) / _float_table( _LN10, precision + 1 );
   
   // Round to same precision as argument and rounding mode
   res.mode( x.mode() );
   res.precision( x.precision() );  

   return res;
   }

//////////////////////////////////////////////////////////////////////////////////////
///
/// FLOAT PRECISION FUNCTIONS
///    Special functions: pow(), fmod(), floor(), ceil(), modf(), fabs(), ldexp(), frexp()
///
//////////////////////////////////////////////////////////////////////////////////////



///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate pow(x,y)
///	@return 	float_precision -	Return pow(x,y)
///	@param      "x"	- The argument
/// @param      "y" - The power argument
///
///	@todo  
///
/// Description:
///   x^y == exp( y * ln( x ) ) ); in general, however if y is an integer then we use the ipow() algorithm instead.
///   
// 
float_precision pow( const float_precision& x, const float_precision& y )
   {
   float_precision i, res(1);
   int expo;
   bool yinteger=false;

   // add two extra guard digits to avoid loss of precision when performing  exp( y * ln(x) ) )
   res.precision( x.precision()+2 );  

   expo = y.exponent();
   if( expo >= 0 )
      {
      i.precision( y.precision() );
      i = y;
      i.mode( ROUND_ZERO);
      i.precision( 1 + expo );
      i.mode( ROUND_NEAR ); // now i is the integer part of y. 
	  // Check that y is a true integer, with a max range of a 32 bit integer
	  if( y == i && i <= float_precision( INT_MAX ) )
		  yinteger = true;
      }
   
   if( yinteger == false ) // y is not an integer so do x^y= exp^(y*log(x)) the regular way
      {
	  res = x;
      res = log( res ) * y;
      res= exp( res );
      }
   else
      { // raise to the power of y when y is an integer. Use optimized method.
	  int sign = i.sign();
	  if( sign < 0 )
		  i.change_sign();

	  int ie = (int)i;  // raise to the power of ie which is at max a standard 32bit integer
 
	  if( x == float_precision( F_RADIX ) )  // If x=F_RADIX then we can just adjust the exponent directly
		{
		if( sign < 0 )
				ie = -ie;
		res.exponent( ie );
		}
	 else
		{ // All other cases
		float_precision p( x );

		for( int n = ie; n > 0; n >>= 1 ) 
           {
           if( ( n & 0x1 ) != 0 ) res *= p;  // Odd
           p *= p;						 
           }
		if( sign < 0 )
	        res = _float_precision_inverse( res );
		}
      }
   
   res.precision(x.precision());
   return res;
   }
 
///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate fmod(x,y)
///	@return 	float_precision -	Return fmod(x,y)
///	@param      "x"	- The argument
/// @param      "y"   - The argument
///
///	@todo  
///
/// Description:
///   float precision. fmod remainder of x/y
///   Equivalent with the standard C function fmod
///   x = i * y + f or f = x - i * y; and i = integer(x/y)
//
float_precision fmod( const float_precision& x, const float_precision& y )
   {
   float_precision i, f;
   int expo;
   
   f.precision( x.precision() );
   i.precision( x.precision() );
   i = x / y;
   expo = i.exponent();
   if( expo < 0 )
      f = x;
   else
      {
      i.mode( ROUND_ZERO);
      i.precision( 1 + expo );
      i.mode( ROUND_NEAR );
      f = x - i * y;
      }

   return f;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate floor(x)
///	@return 	float_precision -	Return floor(x)
///	@param      "x"	- The argument
///
///	@todo  
///
/// Description:
///   Float Precision floor
///   Equivalent with the same standard C function floor
///	  Rounds x downward, returning the largest integral value that is not greater than x.
//
float_precision floor( const float_precision& x )
   {
   float_precision f;
   int exponent = 1 + x.exponent();

    if( F_RADIX < BASE_10 ) 
        { 
        float_precision fpart, ipart;
        fpart=modf( x, &ipart );
        if( x.sign() < 0 && fpart != float_precision(0) ) 
            ipart -= float_precision(1);
        return ipart;
        }

   if( exponent <= 0 ) // is number less than |1|
      {
      if( x.sign() < 0 )
         f = float_precision( -1, x.precision() );
      else
         f = float_precision( 0, x.precision() );
      }
   else
      {
      f.mode( ROUND_DOWN );
      f.precision( exponent );
      f = x;
      f.precision( x.precision() );
      f.mode( ROUND_NEAR );
      }
 
   return f;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate ceil(x)
///	@return 	float_precision -	Return ceil(x)
///	@param      "x"	- The argument
///
///	@todo  
///
/// Description:
///   Float Precision ceil
///   Equivalent with the same standard C function ceil
///   Rounds x upward, returning the smallest integral value that is not less than x.
//
float_precision ceil( const float_precision& x )
   {
   float_precision f;
   int exponent = 1 + x.exponent();

    if( F_RADIX < BASE_10 ) 
        { 
        float_precision fpart, ipart;
        fpart=modf( x, &ipart );
        if( x.sign() > 0 && fpart != float_precision(0) ) 
            ipart += float_precision(1);
        return ipart;
        }

   if( exponent <= 0 ) // is number less than |1|
      {
      if( x.sign() < 0 )
         f = float_precision( 0, x.precision() );
      else
         f = float_precision( 1, x.precision() );
      }
   else
      {
      f.mode( ROUND_UP );
      f.precision( 1 + x.exponent() );
      f = x;
      f.precision( x.precision() );
      f.mode( ROUND_NEAR );
      }

   return f;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  8/10/2020
///	@brief 		Split a float number into integer part and fraction
///	@return 	float_precision -	Fraction part of x
///	@param      "x"	- The argument
/// @param      "intptr" - Float_precision pointer to integer part of x
///
///	@todo  
///
/// Description:
///   Float Precision fmod
///   Split a Floating point number into integer part and fraction
///   Equivalent with the same standard C function call
///	  Use a modified version of the function fmod(x,c1)
//
float_precision modf( const float_precision& x, float_precision *intptr )
   {/*
   float_precision f(0, x.precision() );
   float_precision c1( 1, x.precision() );

   intptr->precision( x.precision() );

   f = fmod( x, c1 );
   *intptr = x - f;

   return f;*/
 
	float_precision i, f;
	int expo;

	f.precision(x.precision());
	i.precision(x.precision());
	intptr->precision(x.precision());
	i = x;
	expo = i.exponent();
	if (expo < 0)
		{
		f = x; 
		i = float_precision(0);
		}
	else
		{
		i.mode(ROUND_ZERO);
		i.precision(1 + expo);
		i.mode(ROUND_NEAR);
		f = x - i;
		}
	*intptr = i;
	return f;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  6/25/2012
///	@brief 		Calculate abs(x)
///	@return 	float_precision -	Return absolute value of x
///	@param      "x"	- The argument
///
///	@todo  
///
/// Description:
///   Float Precision abs()
///   Equivalent with the same standard C function call fabs()
//
float_precision abs( const float_precision& x )
   {
   float_precision f(0, x.precision() );

   f = x;
   if( f.sign() < 0 )
      f.change_sign();

   return f;
   }



///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate ldexp((x)
///	@return 	float_precision -	Return ldexp(x)
///	@param      "x"	- The argument
/// @param      "exp" - exponent argument
///
///	@todo  
///
/// Description:
///   The ldexp function returns the value of x * 2^exp
//
float_precision ldexp( const float_precision& x, int exp )
   {
   if( exp == 0 )
      return x;
   if( exp > 0 && exp <= 31 )
      return x * float_precision( 1U << exp );

   return x * pow( float_precision( 2 ), float_precision( exp ) );
   }



///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		Calculate frexp(x,expptr)
///	@return 	float_precision -	Return mantissa part of number x
///	@param      "x"	- The argument
/// @param      "expptr" - Pointer to the exponent part of number
///
///	@todo  
///
/// Description:
///   The frexp()
///   The frexp function breaks down the floating-point value (x) into a mantissa (m) and an exponent (n), 
///   such that the absolute value of m is greater than or equal to 1/RADIX and less than RADIX, and x = m*Radix^n. 
///   The integer exponent n is stored at the location pointed to by expptr. 
//
float_precision frexp( float_precision& x, int *expptr )
   {
   if( x == float_precision( 0 ) )
      *expptr = 0;

   *expptr = x.exponent();
   x.exponent( 0 );

   return x;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  12/Nov/2015
///	@brief 		Calculate n root of x  (x^(1/n)
///	@return 	   float_precision -	Return nroot(a)
///	@param      "x"	-	The nroot argument
///
///	@todo  
///
/// Description:
///   nroot(V)
///   The nth root of x^(1/n) No Equivalent standard C function call
///   Seperate exponent. e.g. nroot(V*10^x)=10^x/2*nroot(V)
///   Un=U(1/n)(n+1-VU^n)
///   Then Un == 1/nroot(V). and nroot(V) = 1/Un
/// The function has been improved using Newton with iterative deepening creating
///	a speed up with a factor of 3 over the classic Newton method.
/// This is a much much faster option instead of the traditional pow() function x^y
/// and that is why it has been added as a separate function
//
float_precision nroot(const float_precision& x, unsigned int n)
	{
	const size_t extra = 2;
	size_t precision;
	size_t digits;
	int expo, expo_sq;
	double fv;
	float_precision r, u, v, tmp, fn(n);
	const float_precision c0(0), c1(1);
	std::string::reverse_iterator rpos;

	if (x == c0 || x == c1 || n == 1)
		return x;
	if (x.sign() < 0)
		{ throw float_precision::domain_error(); }
	precision = x.precision();
	v.precision(precision + extra);
	v = x;
	expo = v.exponent();
	expo_sq = expo / 2;
	v.exponent(expo - 2 * expo_sq);
	r.precision(precision + extra); // Do iteration using 2 digits higher precision
	u.precision(precision + extra);
	fn.precision(precision + extra);

	// Get a initial guess using ordinary floating point
	rpos = v.ref_mantissa()->rbegin();
	fv = FDIGIT(*rpos);
	for (rpos++; rpos + 1 != v.ref_mantissa()->rend(); rpos++)
		{
		fv *= (double)1 / (double)F_RADIX;
		fv += FDIGIT(*rpos);
		}
	if (expo - 2 * expo_sq > 0)
		fv *= (double)F_RADIX;
	else
		if (expo - 2 * expo_sq < 0)
			fv /= (double)F_RADIX;
	fv = pow(fv, 1.0 / n);  // set the initial guess with at approx 16 correct digits
	fv = 1 / fv;

	tmp.precision(precision+1);
	u = float_precision(fv);
	fn = 1 / fn;
	// Now iterate using Netwon  Un=U*(-VU^n+(n+1))/n
	for (digits = min((size_t)32, precision); ; digits = min(precision + extra, digits * 2))
		{
		// Increase precision by a factor of two
		r.precision(digits);
		u.precision(digits);
		float_precision p(u);
		float_precision res(1, digits);
		// DO U^N
		for (int i = n; i > 0; i >>= 1)
			{
			if ((i & 0x1) != 0)
				res *= p;  // Odd
			if (i>1)
				p *= p;
			}
		// Notice V is the original number to nroot which has the full precision 
		// so we start by assigning it to r, rounding it to the precision of r
		r = v;							// V
		r *= res;						// VU^n
		r = float_precision(n + 1) - r; // (n+1)-VU^n
		r *= fn;						// (-VU^n+(n+1))/n
		u *= r;							// U=U*(-VU^n+(n+1))/n
		if (digits == precision + extra) // Reach final iteration step in regards to precision
			{
			tmp = r;			// round to final precision
			if (tmp == c1)		// break if no improvement
				break;
			}
		}

	u = 1 / u;			// n root of u is now 1/u;
	u.exponent(u.exponent() + expo_sq);
	// Round to same precision as argument and rounding mode
	u.mode(x.mode());
	u.precision(precision);

	return u;
	}


//////////////////////////////////////////////////////////////////////////////////////
///
/// TRIGONOMETRIC FUNCTIONS
///   atan()
///   atan2()
///   asin()
///   acos()
///   sin()
///   cos()
///   tan()
///
//////////////////////////////////////////////////////////////////////////////////////


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		atan
///	@return		float_precision	-	return atan(x)
///	@param      "x"	-	float_precision argument
///
///	@todo 
///
/// Description:
///   Use the taylot series. ArcTan(x) = x - x^3/3 + x^5/5 ...
///   However first reduce x to abs(x)< 0.5 to improve taylor series
///   using the identity. ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
///   We actually dynamically adjust the argument reduction factor by applying
///   more with higher precision numbers.
//
float_precision atan( const float_precision& x )
   {
   size_t precision;
   int j, k;
   double zd, dlimit;
   float_precision r, u, v, v2;
   const float_precision c1(1), c05(0.5), c2(2);

   precision = x.precision()+2;  
   v.precision( precision );
   v = x;

   // Check for augument reduction and increase precision if necessary
   zd=PLOG10( precision );
   zd *= zd;
   j=(int)zd; if(j>32) j=32;
   
   // Lets just do one reduction because that quarantee us that it is less than 1
   // and we can then use standard IEEE754 to calculate the needed argument reduction.
   zd = (double)abs( v / ( c1 + sqrt( c1 + v * v ) ) );
   // Calculate the number of reduction needed 
   dlimit=0.5/pow(2.0,j); // Only estimated target reduction limit based on x/(1+sqrt(1+x*x)->x/2 for small x
   for( j=1; zd > dlimit; j++ )
       zd=zd/(1.0+sqrt(1.0+zd*zd));

   // Adjust the precision
   if(j>0)
       precision += PADJUST( j/4 );
   r.precision( precision );
   u.precision( precision );
   v.precision( precision );
   v2.precision( precision );

   // Transform the solution to ArcTan(x)=2*ArcTan(x/(1+sqrt(1+x^2)))
   for( k=1; j>0; k *= 2, j-- )
        v = v / ( c1 + sqrt( c1 + v * v ) );

   v2 = v * v;
   r = v;
   u = v;
   // Now iterate using taylor expansion
   for( j=3;; j+=2 )
      {
      v *= v2;
      v.change_sign();
      r = v / float_precision(j);;
      if( u + r == u )
         break;
      u += r;
      }

   u *= float_precision( k );

   // Round to same precision as argument and rounding mode
   u.mode( x.mode() );
   u.precision( x.precision() );  

   return u;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		atan2
///	@return 	float_precision	-	return the angle (in radians) from the X axis to a point (y,x).
/// @param		"y"   -  float_precision y-axis
///	@param      "x"	-	float_precision x-axis
///
///	@todo 
///
/// Description:
///   use atan() to calculate atan2()
//
float_precision atan2( const float_precision& y, const float_precision& x )
   {
   size_t precision;
   float_precision u;
   const float_precision c0(0), c05(0.5);

   if( x == c0 && y == c0 )
      return c0;

   precision = x.precision()+2;  
   u.precision( precision );
   if( x == c0 )
      {
      u = _float_table( _PI, precision );
      if( y < c0 )
         u *= -c05;
      else
         u *= c05;
      }
   else
      if( y == c0 )
         {
         if( x < c0 )
            u = _float_table( _PI, precision );
         else
            u = c0;
         }
      else
         {
         u = atan( y / x );
         if( x < c0  && y < c0 )
            u -= _float_table( _PI, precision );

         if( x < c0 && y >= c0 )
            u += _float_table( _PI, precision );
         }

// Round to same precision as argument and rounding mode
   u.mode( x.mode() );
   u.precision( x.precision() );  

   return u;
   }



   
///	@author Henrik Vestermark (hve@hvks.com)
///	@date  8/26/2013
///	@brief 		Calculate asin(x)
///	@return 	float_precision -	Return asin(x)
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
///   Use a taylor series until their is no more change in the result
///   asin(x) == x + x^3/(2*3)+(1*3)x^5/(2*4*5)+(1*3*5)x^7/(2*4*6*7)....
///   Use argument reduction via the identity arcsin(x)=2arcsin(x)/(sqrt(2)+sqrt(1-x*x))
//	  This function replace the other function using newton iteration. Taylor series is significant
//    faster e.g 50% for 10 digits, 3x for 100 digits and 5x for 1000 digits.
//
float_precision asin( const float_precision& x )
   {
   size_t precision;
   int k, j, sign;
   double zd, dlimit;
   float_precision r, u, v, v2, sqrt2, lc, uc;
   const float_precision c1(1), c2(2);

    if( x > c1 || x < -c1 )
      { throw float_precision::domain_error(); }

   precision = x.precision() + 2;  
   v.precision( precision );
   v = x;

   sign = v.sign();
   if( sign < 0 )
      v.change_sign();

   // Check for augument reduction and increase precision if necessary
   zd=PLOG10( precision );
   zd *= zd;
   j=(int)zd; if(j>16) j=16;
   zd=v;
   // Find the argument reduction factor
   for( dlimit=1.0; j > 0; j-- )
       {
       dlimit/=sqrt(2.0)* sqrt( 1.0 + sqrt( 1.0 - dlimit * dlimit ) );
       if( dlimit < zd ) break;
       }
   // j is the number of argument reduction
    // Adjust the precision
   if(j>0)
       precision += PADJUST( j/4 );
   r.precision( precision );
   u.precision( precision );
   v.precision( precision );
   v2.precision( precision );
   sqrt2.precision( precision );
   lc.precision( precision );
   uc.precision( precision );
  
   // Now use the identity arcsin(x)=2arcsin(x)/(sqrt(2)+sqrt(1-x*x))
  // until argument is less than dlimit
  // Reduce the argument to below 0.5 to make the newton run faster
   sqrt2=c2;				// Ensure correct number of digits
   sqrt2=sqrt( sqrt2 );	// Now calculate sqrt2 with precision digits
   for( k=0; j > 0; k++, j-- )
      v /= sqrt2 * sqrt( c1 + sqrt( c1 - v * v ) );
  
   v2 = v * v;
   r = v;
   u = v;
   // Now iterate using taylor expansion
   for( unsigned int m=3;; m+=2 )
      {
      if( j < 65536 ) 
          {
            uc = float_precision ( ( m - 2 ) * ( m - 2 ) );
            lc = float_precision( m * m - m ); 
        }
      else 
          {
          uc = float_precision( m - 2 ) * float_precision( m - 2 );
          lc = float_precision( m - 1 ) * float_precision( m );
        }
      v = uc * v2 / lc;
      r *= v;
      if( u + r == u )
         break;
      u += r;
      }

   if( k > 0 )
       u *= float_precision( 1 << k );

   // Round to same precision as argument and rounding mode
   u.mode( x.mode() );
   u.precision( x.precision() );  

   if( sign < 0 )
      u.change_sign();

   return u;
   }





///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		acos
///	@return 	float_precision	-	return acos(x)
///	@param      "x"	-	float_precision argument
///
///	@todo 
///
/// Description:
///    Use Arccos(x)=PI/2 - Arcsin(x) or ArcCos(x)=0.5*PI-ArcSin(x)
//
float_precision acos( const float_precision& x )
   {
   size_t precision;
   float_precision y;
   const float_precision c1(1);
   
      if( x > c1 || x < -c1 )
      { throw float_precision::domain_error(); }
      
   precision = x.precision();  
   y = _float_table( _PI, precision );
   y *= float_precision( 0.5 );
   y -= asin( x );

   // Round to same precision as argument and rounding mode
   y.mode( x.mode() );
   y.precision( precision );  

   return y;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		sin
///	@return 	float_precision	-	return sin(x)
///	@param      "x"	-	float_precision argument
///
///	@todo 
///
/// Description:
///   Use the taylor series. Sin(x) = x - x^3/3! + x^5/5! ...
///   1) However first reduce x to between 0..2*PI 
///   2) Then reduced further to between 0..PI using sin(x+PI)=-Sin(x)
///   3) Finally reduced it to below 0.5/3^reduction factor, using the trisection identity
///         sin(3x)=3*sin(x)-4*sin(x)^3
///   4) Then do the taylor. 
///   The argument reduction is used to reduced the number of taylor iteration 
///   and to minimize round off erros and calculation time
//
float_precision sin( const float_precision& x )
   {
   size_t precision;
   int k, sign, j;
   double zd;
   float_precision r, u, v, v2, de(0);
   const float_precision c1(1), c2(2), c3(3), c4(4);

   precision = x.precision() + 2;  
   // Check for augument reduction and increase precision if necessary
   zd=PLOG10( precision );
   zd *= 2.0;
   j=(int)zd; if(j>1 && j<5) j--; if(j>8) j=8;
   // Adjust the precision
   if(j>0)
       precision += PADJUST( j/4 );
   r.precision( precision );
   u.precision( precision );
   v.precision( precision );
   v2.precision( precision );

   v = x;
   sign = v.sign();
   if( sign < 0 )
      v.change_sign();
   
   // Check that argument is larger than 2*PI and reduce it if needed. 
   // No need for high perecision. we just need to figure out if we need to Calculate PI with a higher precision
   if( v > float_precision( 2*3.14159265 ) )
      {
      // Reduce argument to between 0..2PI
      u = _float_table( _PI, precision );
      u *= c2;
      if( abs( v ) > u )
         {
         r = v / u; 
         (void)modf( r, &r ); 
         v -= r * u;
         }
      if( v < float_precision( 0 ) )
         v += u;
	  }   
   
   // Reduced it further to between 0..PI
   // However avoid calculating PI is not needed.
   // No need for high perecision. we just need to figure out if we need to Calculate PI with a higher precision
   if( v > float_precision( 3.14159265 ) )
      {
	  u = _float_table( _PI, precision ); // We dont need to worry that we called it a second time since it will be cached
	  if( v > u )
		  { v -= u; sign *= -1; }
      }

   // Now use the trisection identity sin(3x)=3*sin(x)-4*sin(x)^3
   // until argument is less than 0.5/3^j  Where J is the number of reduction factor based on the needed precision of the argument.
   v2= v * float_precision( 2 * pow( 3.0, j ) );
   for( k = 0, r = c1; v2 > r; k++ )
      r *= c3;
   v /= r;

   v2 = v * v;
   r = v;
   u = v;

   // Now iterate using taylor expansion
   for( unsigned int m=3;; m+=2 )
      {
      de += float_precision( 4 * m - 6 ); // Avoid the multiplication in float_precision. 
      v = v2 / de;  // de is only 20 digits standard preecision but since v2 is precision it will be extended to v2 precision //( j<USHRT_MAX? float_precision( m * (m-1) ) : float_precision(m) * float_precision(m-1) );
      r *= v;
      r.change_sign();
      if( u + r == u )
         break;
      u += r;
      }

   for( ; k > 0 ; k-- )
      u *= ( c3 - c4 * u * u );

   // Round to same precision as argument and rounding mode
   u.mode( x.mode() );
   u.precision( x.precision() );  

   if( sign < 0 )
      u.change_sign();

   return u;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		cos
///	@return 	float_precision	-	return cos(x)
///	@param      "x"	-	float_precision argument
///
///	@todo 
///
/// Description:
///   Use the taylor series. Cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! ...
///   1) However first reduce x to between 0..2*PI
///   2) Then reduced it further to between 0..PI using cos(x)=Cos(2PI-x) for x >= PI
///   3) Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
///      until argument is less than 0.5/3^argument reduction
///   4) Finally use Taylor 
//
float_precision cos( const float_precision& x )
   {
   size_t precision;
   int k, j;
   double zd;
   float_precision r, u, v, v2, de(0);
   const float_precision c05(0.5), c1(1), c2(2), c3(3), c4(4);

   precision = x.precision() + 2;  
   // Check for augument reduction and increase precision if necessary
   zd=PLOG10( precision );
   zd *= 2.0;
   j=(int)zd; if(j>1 && j<5) j--; if(j>8) j=8;
   // Adjust the precision
   if(j>0)
       precision += PADJUST( j/4 );
   r.precision( precision );
   u.precision( precision );
   v.precision( precision );
   v2.precision( precision );

   v = x;
 
   // Check that argument is larger than 2*PI and reduce it if needed. 
   // No need for high perecision. we just need to figure out if we need to Calculate PI with a higher precision
   if( abs( v ) > float_precision( 2*3.14159265 ) )
      {  // Reduce argument to between 0..2P
	  u = _float_table( _PI, precision );
      u *= c2;
      if( abs( v ) > u )
         {
         r = v / u; 
         (void)modf( r, &r ); 
         v -= r * u;
         }
      if( v < float_precision( 0 ) )
         v += u;
      }

   // Reduced it further to between 0..PI. 
   // However avoid calculating PI is not needed.
   // No need for high perecision. we just need to figure out if we need to Calculate PI with a higher precision
   if( abs( v ) > float_precision( 3.14159265 ) )
      {
      r = _float_table( _PI, precision );
      if( v > r )
         v = r * c2 - v;
      }

   // Now use the trisection identity cos(3x)=-3*cos(x)+4*cos(x)^3
   // until argument is less than 0.5/3^j  Where J is the number of reduction factor based on the needed precision of the argument.
   v2= abs( v * float_precision( 2 * pow( 3.0, j ) ) );
   for( k = 0, r = c1; v2 > r; k++ )
      r *= c3;
   v /= r;

   v2 = v * v;
   r = c1;
   u = r;
   // Now iterate using taylor expansion
   for( unsigned int m=2;; m+=2 )
      {
      de += float_precision( 4 * m - 6 ); // Avoid the multiplication in float_precision. 
      v = v2 / de;  // de is only 20 digits standard preecision but since v2 is precision is will be extended to v2 precision //( m<USHRT_MAX? float_precision( m * (m-1) ) : float_precision(m) * float_precision(m-1) );
      r *= v;
      r.change_sign();
      if( u + r == u )
         break;
      u += r;
      }

   for( ; k > 0 ; k-- )
      u *= ( c4 * u * u - c3 );
 
   // Round to same precision as argument and rounding mode
   u.mode( x.mode() );
   u.precision( x.precision() );  

   return u;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  1/21/2005
///	@brief 		tan
///	@return 	float_precision	-	return tan(x)
///	@param      "x"	-	float_precision argument
///
///	@todo 
///
/// Description:
///   Use the identity tan(x)=Sin(x)/Sqrt(1-Sin(x)^2)
///   1) However first reduce x to between 0..2*PI
///   2) Use taylor
//
float_precision tan( const float_precision& x )
   {
   size_t precision;
   float_precision u, r, v, p;
   const float_precision c1(1), c2(2), c3(3), c05(0.5);

   precision = x.precision() + 2;  
   u.precision( precision );
   v.precision( precision );
   p.precision( precision );
   v = x;
  
   // Check that argument is larger than 2*PI and reduce it if needed. 
   p = _float_table( _PI, precision );
   u = c2 * p;
   if( abs( v ) > u )
      {
      r = v / u; 
      (void)modf( r, &r ); 
      v -= r * u;
      }
   if( v < float_precision( 0 ) )
      v += u;
    
   p *= c05;
   if( v == p || v ==  p * c3 )
      { throw float_precision::domain_error(); }

   u = sin( v ); 
   if( v < p || v > p * c3 ) 
      u /= sqrt( c1 - u * u );
   else
      u /= -sqrt( c1 - u * u );
   
   // Round to same precision as argument and rounding mode
   u.mode( x.mode() );
   u.precision( x.precision() );  

   return u;
   }


//////////////////////////////////////////////////////////////////////////////////////
///
/// END TRIGONOMETRIC FUNCTIONS
///
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
///
/// Hyperbolic FUNCTIONS
///   sinh()
///   cosh()
///   tanh()
///	  arcsinh()
///	  arccosh()
///	  arctanh()
///
//////////////////////////////////////////////////////////////////////////////////////

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  6/21/2013
///	@brief 		Calculate Sinh(x)
///	@return 	float_precision -	Return Sinh(x)
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
///   Use a taylor series until their is no more change in the result
///   sinh(x) == x + x^3/3!+x^5/5!+....
///   Use argument reduction via sinh(3x)=sinh(x)(3+4sinh^2(x))	
//
float_precision sinh( const float_precision& x )
   {
   size_t precision;
   int k, j, sign;
   double zd, dlimit;
   float_precision r, u, v, v2, de(0);
   const float_precision c1(1), c2(2), c3(3), c4(4);

   precision = x.precision() + 2;  
   v.precision( precision );
   v = x;
   r.precision( precision ); 
   u.precision( precision );
   v2.precision( precision );

   sign = v.sign();
   if( sign < 0 )
      v.change_sign();

   // Check for augument reduction and increase precision if necessary
   zd=PLOG10( precision );
   zd *= zd;
   j=(int)zd; j -= 1; if(j<0) j=0;  if(j>16) j=16;
   dlimit=pow( 3.0, j );
   // Now use the trisection identity sinh(3x)=sinh(x)(3+4Sinh^2(x))
   // until argument is less than 0.5 * (1/3)^j
   j = int( 2.0 * dlimit );
   v2= v * float_precision( j );
   for( k = 0, r = c1; v2 > r; k++ )
      r *= c3;
   // Adjust the precision
   if(k>0)
       precision += PADJUST( k/4 );
   r.precision( precision );
   u.precision( precision );
   v.precision( precision );
   v2.precision( precision );
  
   v /= r;
   v2 = v * v;
   r = v;
   u = v;
   // Now iterate using taylor expansion
   for( unsigned int m=3;; m+=2 )
      {
      de += float_precision( 4 * m - 6 ); // Avoid the multiplication in float_precision. 
      v = v2 / de;  // de is only 20 digits standard preecision but since v2 is precision is will be extended to v2 precision //( j<USHRT_MAX? float_precision( m * (m-1) ) : float_precision(m) * float_precision(m-1) );
      r *= v;
      if( u + r == u )
         break;
      u += r;
      }

   for( ; k > 0 ; k-- )
      u *= ( c3 + c4 * u * u );

   // Round to same precision as argument and rounding mode
   u.mode( x.mode() );
   u.precision( x.precision() );  

   if( sign < 0 )
      u.change_sign();

   return u;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  6/21/2013
///	@brief 		Calculate Cosh(x)
///	@return 	float_precision -	Return Cosh()
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
///   Use a taylor series until their is no more change in the result
///   cosh(x) == 1 + x^2/2!+x^4/4!+....
///   Use argument reduction via cosh(3x)=cosh(x)(4cosh^2(x)-3)	
//
float_precision cosh( const float_precision& x )
   {
   size_t precision;
   int k, j, sign;
   double zd, dlimit;
   float_precision r, u, v, v2, de(0);
   const float_precision c1(1), c2(2), c3(3), c4(4);

   precision = x.precision() + 2;  
   v.precision( precision );
   v = x;
   r.precision( precision ); 
   u.precision( precision );
   v2.precision( precision );

   sign = v.sign();
   if( sign < 0 )
      v.change_sign();  // cosh(-x) = cosh(x)

    // Check for augument reduction and increase precision if necessary
   zd=PLOG10( precision );
   zd *= zd;
   j=(int)zd; j -= 1; if(j<0) j=0;  if(j>16) j=16;
   dlimit=pow( 3.0, j );
   // Now use the trisection identity cosh(3x)=cosh(x)(4cosh^2(xx)-3)
   // until argument is less than 0.5 * (1/3)^j
   j = (int)(  2.0 * dlimit );
   v2= v * float_precision( j );
   for( k = 0, r = c1; v2 > r; k++ )
      r *= c3;
   // Adjust the precision
   if(k>0)
       precision += PADJUST( k/4 );
   r.precision( precision );
   u.precision( precision );
   v.precision( precision );
   v2.precision( precision );
   
   v /= r;
   v2 = v * v;
   r = c1;
   u = r;
   // Now iterate using taylor expansion
   for( unsigned int m=2;; m+=2 )
      {
      de += float_precision( 4 * m - 6 ); // Avoid the multiplication of float_precision.  
      v = v2 / de;  // de is only 20 digits standard preecision but since v2 is precision is will be extended to v2 precision //( j<USHRT_MAX? float_precision(m * (m-1) ) : float_precision(m) * float_precision(m-1) );
      r *= v;
      if( u + r == u )
         break;
      u += r;
      }

   for( ; k > 0 ; k-- )
      u *= ( c4 * u * u -c3 );
      
   // Round to same precision as argument and rounding mode
   u.mode( x.mode() );
   u.precision( x.precision() );  

   return u;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  6/21/2013
///	@brief 		Calculate Tanh(x)
///	@return 	float_precision -	Return Tanh()
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
//	tanh = ( exp(x) - exp(-x) ) / ( exp( x) + exp(-x) )=(e^(2x)-1/(e^(2x)+1)
// 
//
float_precision tanh( const float_precision& x )
   {
   float_precision v, v2;
   const float_precision c1(1);

   v.precision( x.precision() + 1 );
   v2.precision( x.precision() + 1 );
   v = x;
   v = exp( v );
   v2= v * v;
   v = (v2-c1)/(v2+c1);

   // Round to same precision as argument and rounding mode
   v.mode( x.mode() );
   v.precision( x.precision() );  

   return v;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  6/25/2013
///	@brief 		Calculate ArcSinh(x)
///	@return 	float_precision -	Return ArcSinh()
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
//	ArcSinh=Ln(x+Sqrt(x^2+1))
// 
//
float_precision asinh( const float_precision& x )
   {
   float_precision v;
   const float_precision c1(1);

   v.precision( x.precision() + 1 );
   v = x;
   v = log(v+sqrt(v*v+c1));
   
   // Round to same precision as argument and rounding mode
   v.mode( x.mode() );
   v.precision( x.precision() );  

   return v;
   }


///	@author Henrik Vestermark (hve@hvks.com)
///	@date  6/25/2013
///	@brief 		Calculate ArcCosh(x)
///	@return 	float_precision -	Return ArcCosh()
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
//	ArcCosh=Ln(x+Sqrt(x^2-1))
// 
//
float_precision acosh( const float_precision& x )
   {
   float_precision v;
   const float_precision c1(1);

   if( x < c1 )
      { throw float_precision::domain_error(); }
   
   v.precision( x.precision() + 1 );
   v = x;
   v = log(v+sqrt(v*v-c1));
   
   // Round to same precision as argument and rounding mode
   v.mode( x.mode() );
   v.precision( x.precision() );  

   return v;
   }

///	@author Henrik Vestermark (hve@hvks.com)
///	@date  6/25/2013
///	@brief 		Calculate ArcTanh(x)
///	@return 	float_precision -	Return ArcTanh()
///	@param      "x"	-	   The argument
///
///	@todo  
///
/// Description:
//	ArcTanh=0.5*Ln((1+x)/(1-x))
// 
//
float_precision atanh( const float_precision& x )
   {
   float_precision v;
   const float_precision c05(0.5), c1(1);

   if( x >= c1 || x <= -c1 )
      { throw float_precision::domain_error(); }

   v.precision( x.precision() + 1 );
   v = x;
   v = c05*log((c1+v)/(c1-v));
   
   // Round to same precision as argument and rounding mode
   v.mode( x.mode() );
   v.precision( x.precision() );  

   return v;
   }

//////////////////////////////////////////////////////////////////////////////////////
///
/// END TRIGONOMETRIC FUNCTIONS
///
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
///
/// FAST integer division and remaining using Floating point arithmetic
///
//////////////////////////////////////////////////////////////////////////////////////

int_precision _int_precision_fastdiv( const int_precision &s1, const int_precision &s2 )
	{
	size_t ss;
	int_precision r2;
	float_precision f1, f2, rf;

	ss=s1.size(); 
	if(s2.size() > ss ) 
		ss = s2.size();
	f1.precision( ss+2);
	f2.precision( ss+2);
	rf.precision( ss+2 );
	f1=float_precision(s1, ss+ 2);
	f2=float_precision( s2, ss+ 2);
	rf=f1/f2;
	r2=rf.to_int_precision();
	return r2;
	}


int_precision _int_precision_fastrem( const int_precision &s1, const int_precision &s2 )
	{
	size_t ss;
	int_precision r2;
	float_precision f1, f2, rf;

	ss=s1.size(); 
	if(s2.size() > ss )
		ss = s2.size();
	f1.precision( ss+2);
	f2.precision( ss+2);
	rf.precision( ss+2 );
	f1=float_precision(s1, ss+ 2);
	f2=float_precision( s2, ss+ 2);
	rf=f1/f2;
	r2=rf.to_int_precision();
	r2=s1-s2*r2;
	return r2;
	}

//////////////////////////////////////////////////////////////////////////////////////
///
/// FLOATING POINT FUNCTIONS
///
//////////////////////////////////////////////////////////////////////////////////////