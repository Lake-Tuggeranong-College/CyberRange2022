#include "Loop_Diff.h"

unsigned long loop_diff(unsigned long s,unsigned long e,int t)
{
  unsigned long ret;
  // Is our start after our end?
  if (s > e)
  { // It is, so we'll need to do some jig-higgery
    // Let's get our difference
    ret = s-e;
    // What type of variable are we using?
    if (t)
    { // Unsigned long
      // Correct with our U_LONG_MAX
      ret = U_LONG_MAX - ret;
    } else
    { // Unsigned int
      // Correct with our U_INT_MAX
      ret = U_INT_MAX - ret;
    }
  } else
  { // It isn't so we can just subtract
    ret = e-s;
  }
  // Return our difference
  return ret;
}
