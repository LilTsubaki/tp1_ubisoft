//**********************************************************************************************************************
#include "Time.h"
#include <time.h>
#include <sys/timeb.h>
#include <time.h>

//**********************************************************************************************************************
namespace uu //univubi
{

//**********************************************************************************************************************
/*static*/ u64 Time::GetSynchTime()
{
	timeb timebuffer;
	ftime(&timebuffer);

	u64 u64Seconds;
	u64 u64MilliSeconds;

	u64Seconds = timebuffer.time;
	u64MilliSeconds = timebuffer.millitm;

	return ((u64Seconds * MilliSecondsInSecond) + u64MilliSeconds);
}

} //namespace univubi
