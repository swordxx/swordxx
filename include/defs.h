// ----------------------------------------------------------------------------
// Making or using sword as a Windows DLL
// ----------------------------------------------------------------------------

#if defined(__SWMSW__)

// _declspec works in BC++ 5 and later, as well as VC++
#if defined(__VISUALC__)

#  ifdef SWMAKINGDLL
#    define SWDLLEXPORT _declspec( dllexport )
#    define SWDLLEXPORT_DATA(type) _declspec( dllexport ) type
#    define SWDLLEXPORT_CTORFN
#  elif defined(SWUSINGDLL)
#    define SWDLLEXPORT _declspec( dllimport )
#    define SWDLLEXPORT_DATA(type) _declspec( dllimport ) type
#    define SWDLLEXPORT_CTORFN
#  else
#    define SWDLLEXPORT
#    define SWDLLEXPORT_DATA(type) type
#    define SWDLLEXPORT_CTORFN
#  endif

#elif defined(__SWPM__)

#  ifdef SWMAKINGDLL
#    define SWDLLEXPORT _Export
#    define SWDLLEXPORT_DATA(type) _Export type
#    define SWDLLEXPORT_CTORFN
#  elif defined(SWUSINGDLL)
#    define SWDLLEXPORT _Export
#    define SWDLLEXPORT_DATA(type) _Export type
#    define SWDLLEXPORT_CTORFN
#  else
#    define SWDLLEXPORT
#    define SWDLLEXPORT_DATA(type) type
#    define SWDLLEXPORT_CTORFN
#  endif

#elif defined(__GNUWIN32__) || defined(__BORLANDC__)

#  ifdef SWMAKINGDLL
#    define SWDLLEXPORT __declspec( dllexport )
#    define SWDLLEXPORT_DATA(type) __declspec( dllexport ) type
#    define SWDLLEXPORT_CTORFN
#  elif defined(SWUSINGDLL)
#    define SWDLLEXPORT __declspec( dllimport )
#    define SWDLLEXPORT_DATA(type) __declspec( dllimport ) type
#    define SWDLLEXPORT_CTORFN
#  else
#    define SWDLLEXPORT
#    define SWDLLEXPORT_DATA(type) type
#    define SWDLLEXPORT_CTORFN
#  endif


#else
#  define SWDLLEXPORT
#  define SWDLLEXPORT_DATA(type) type
#  define SWDLLEXPORT_CTORFN
#endif

#else // !Windows
#  define SWDLLEXPORT
#  define SWDLLEXPORT_DATA(type) type
#  define SWDLLEXPORT_CTORFN
#endif // Win/!Win

// For ostream, istream ofstream
#if defined(__BORLANDC__) && defined( _RTLDLL )
#  define SWDLLIMPORT __import
#else
#  define SWDLLIMPORT
#endif

