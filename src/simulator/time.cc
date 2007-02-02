/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "time.h"
#include "simulator.h"

namespace ns3 {

Time::Time ()
  : TimeUnit<1> ()
{}
Time::Time (TimeUnit<1> time)
  : TimeUnit<1> (time)
{}
Time::Time (HighPrecision const& value)
  : TimeUnit<1> (value)
{}

Time 
Time::Seconds (double seconds)
{
  return Seconds (seconds);
}

double 
Time::GetSeconds (void) const
{
  HighPrecision seconds = GetHighPrecision ();
  seconds.Div (HighPrecision (1000000000, false));
  return seconds.GetDouble ();
}
int32_t 
Time::GetMilliSeconds (void) const
{
  HighPrecision ms = GetHighPrecision ();
  ms.Div (HighPrecision (1000000, false));
  return (int32_t) ms.GetInteger ();
}
int64_t 
Time::GetMicroSeconds (void) const
{
  HighPrecision us = GetHighPrecision ();
  us.Div (HighPrecision (1000, false));
  return us.GetInteger ();
}
int64_t 
Time::GetNanoSeconds (void) const
{
  return GetHighPrecision ().GetInteger ();
}

std::ostream& 
operator<< (std::ostream& os, Time const& time)
{
  os << time.GetNanoSeconds () << "ns";
  return os;
}

Time Seconds (double seconds)
{
  return Time (HighPrecision (seconds * 1000000000.0));
}
Time MilliSeconds (uint32_t ms)
{
  return Time (HighPrecision (ms * 1000000, false));
}
Time MicroSeconds (uint64_t us)
{
  return Time (HighPrecision (us * 1000, false));
}
Time NanoSeconds (uint64_t ns)
{
  return Time (HighPrecision (ns, false));
}
Time Now (void)
{
  return Time (Simulator::Now ());
}

Scalar::Scalar ()
  : TimeUnit<0> ()
{}
Scalar::Scalar (TimeUnit<0> scalar)
  : TimeUnit<0> (scalar)
{}

Scalar::Scalar (double scalar)
  : TimeUnit<0> (HighPrecision (scalar))
{}

double
Scalar::GetDouble (void) const
{
  return GetHighPrecision ().GetDouble ();
}

}; // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
//#include <iostream>

namespace ns3 {

class TimeTests : public Test
{
public:
  TimeTests ();
  virtual ~TimeTests ();
  virtual bool RunTests (void);
};

TimeTests::TimeTests ()
  : Test ("Time")
{}
TimeTests::~TimeTests ()
{}
bool TimeTests::RunTests (void)
{
  bool ok = true;

  Time t0 = Seconds (10.0);
  //std::cout << "t0="<<t0.GetSeconds ()<<" (expected 10.0)"<<std::endl;
  Time t1 = Seconds (11.0);
  //std::cout << "t1="<<t1.GetSeconds ()<<" (expected 11.0)"<<std::endl;
  t0 = Seconds (1.5);
  //std::cout << "t0="<<t0.GetSeconds ()<<" (expected +1.5)"<<std::endl;
  t0 = Seconds (-1.5);
  //std::cout << "t0="<<t0.GetSeconds ()<<" (expected -1.5)"<<std::endl;

  t0 = Seconds (10.0);
  t1 = Seconds (11.0);

  Time t2, t3;

  t2 = t1 - t0;
  if (!t2.IsStrictlyPositive ())
    {
      ok = false;
    }
  t2 = t1 - t1;
  if (!t2.IsZero ())
    {
      ok = false;
    }
  t2 = t0 - t1;
  if (!t2.IsStrictlyNegative ())
    {
      ok = false;
    }

  t2 = t0 - t1;
  t3 = t2 * t0 / t1;
  t3 = t0 * t2 / t1;
  t3 = t0 * t1 / t2;
  t3 = t0 * (t1 / t2);
  t3 = (t0 * t1) / t2;
  t3 = t0 / t1 * t2;
  t3 = (t0 / t1) * t2;
  TimeInvert ti0 = t0 / (t1 * t2);
  t3 = t0 * Scalar (10.0);
  t3 = Scalar (10.0) * t0;
  t3 = Scalar (10.0) * t0 / t2 * t1;
  t3 = (Scalar (10.0) * t0 ) / t2 * t1;
  Scalar s0 = t0 / t1;
  Scalar s1;
  s1 = t0 * t1 / (t2 * t0);
  TimeUnit<0> tu0 = s0;
  TimeUnit<1> tu1 = t0;
  TimeUnit<2> tu2 = t0 * t1;
  TimeUnit<3> tu3 = t0 * tu2;
  TimeUnit<-2> tu4 = t0 / tu3;

  Time tmp = MilliSeconds (0);
  if ((tmp != NanoSeconds (0)) ||
      (tmp > NanoSeconds (0)) ||
      (tmp < NanoSeconds (0)))
    {
      ok = false;
    }

  Time t4 = Seconds (10.0) * Scalar (1.5);
  //std::cout << "10.0s * 1.5 = " << t4.GetSeconds () << "s" << std::endl;
  Time t5 = NanoSeconds (10) * Scalar (1.5);
  //std::cout << "10ns * 1.5 = " << t5.GetNanoSeconds () << "ns" << std::endl;

  return ok;
}

static TimeTests g_time_tests;
  
};

#endif /* RUN_SELF_TESTS */
