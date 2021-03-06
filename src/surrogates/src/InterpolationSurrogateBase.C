//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// QUESO - a library to support the Quantification of Uncertainty
// for Estimation, Simulation and Optimization
//
// Copyright (C) 2008-2017 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-

// This class
#include <queso/InterpolationSurrogateBase.h>

// QUESO
#include <queso/GslVector.h>
#include <queso/GslMatrix.h>
#include <queso/InterpolationSurrogateData.h>

// C++
#include <sstream>
#include <vector>
#include <cmath>

namespace QUESO
{
  template<class V, class M>
  InterpolationSurrogateBase<V,M>::InterpolationSurrogateBase(const InterpolationSurrogateData<V,M>& data)
    : SurrogateBase<V>(),
    m_data(data)
    {}

  template<class V, class M>
  void InterpolationSurrogateBase<V,M>::verify_bounds(const V & domainVector) const
  {
    for (unsigned int i=0; i<domainVector.sizeGlobal(); ++i)
      {
        if ( (std::isnan(domainVector[i])) || (domainVector[i] > this->m_data.x_max(i)) || (domainVector[i] < this->m_data.x_min(i)) )
          {
            std::stringstream ss;
            ss  <<"ERROR: Cannot evaluate surrogate outside bounds for parameter " <<i
                <<", value requested: " <<domainVector[i] <<std::endl;

            queso_error_msg(ss.str());
          }
      }
  }

} // end namespace QUESO

// Instantiate
template class QUESO::InterpolationSurrogateBase<QUESO::GslVector,QUESO::GslMatrix>;
