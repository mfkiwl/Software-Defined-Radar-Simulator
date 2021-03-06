// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of Software Defined Radar Simulator (SDRS) Software.
//
// SDRS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// SDRS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SDRS.  If not, see <http://www.gnu.org/licenses/>.
#ifndef SINUSOID_GENERATOR_HPP
#define SINUSOID_GENERATOR_HPP

#include<sdr_simulator/input/SignalGenerator.hpp>
#include<boost/math/constants/constants.hpp>
#include<tr1/math.h>

//Use an unnamed namespace to properly get a const from a computation
namespace 
{
   const static double TWO_PI = 2.0f*boost::math::constants::pi<float>();
};

// Gaussian noise generator with adjustable mean, variance, and amplitude
template<typename DATA_TYPE, typename RESET_TYPE>
class AnalogSinusoidGenerator: public SignalGenerator<DATA_TYPE,RESET_TYPE>
{
   // define constants
   const double AMPLITUDE;
   const double NORMALIZED_FREQUENCY;
   DATA_TYPE data_;
   int time_;

   // compute a new sample on each clock cycle
   virtual void Compute()
   {
         data_ = this->reset.read() ? 
            0 : AMPLITUDE*std::tr1::cos( time_++*TWO_PI* NORMALIZED_FREQUENCY );
         this->output.write( data_ );
   }

   public:

   SC_HAS_PROCESS( AnalogSinusoidGenerator );

   // CTOR
   AnalogSinusoidGenerator( 
         const sc_module_name& nm, 
         const double normalizedFrequency,
         const int sampleBits,
         const double amplitude = 1.0
         ):
      SignalGenerator< DATA_TYPE,RESET_TYPE>( nm , 0), 
      NORMALIZED_FREQUENCY( normalizedFrequency ),  
      AMPLITUDE( amplitude ),
      time_(0)
   { }
};


#endif
