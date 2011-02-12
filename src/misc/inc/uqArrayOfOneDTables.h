//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// QUESO - a library to support the Quantification of Uncertainty
// for Estimation, Simulation and Optimization
//
// Copyright (C) 2008,2009,2010,2011 The PECOS Development Team
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
// 
// $Id$
//
//--------------------------------------------------------------------------

#ifndef __UQ_ARRAY_OF_ONE_D_TABLES_H__
#define __UQ_ARRAY_OF_ONE_D_TABLES_H__

#include <uqEnvironment.h>

template <class V, class M>
class uqArrayOfOneDTablesClass
{
public:
  uqArrayOfOneDTablesClass(const char* prefix, const uqVectorSpaceClass<V,M>& rowSpace);
 ~uqArrayOfOneDTablesClass();

  void                       setOneDTable(unsigned int rowId, const std::vector<double>& values);
  const std::vector<double>& oneDTable   (unsigned int rowId) const;
  void                       print       (std::ostream& os)   const;

private:
  const uqBaseEnvironmentClass&          m_env;
        std::string                      m_prefix;
  const uqVectorSpaceClass<V,M>&         m_rowSpace;
  uqDistArrayClass<std::vector<double>*> m_oneDTables;
};

template <class V, class M>
uqArrayOfOneDTablesClass<V,M>::uqArrayOfOneDTablesClass(
  const char* prefix, 
  const uqVectorSpaceClass<V,M>& rowSpace)
  :
  m_env       (rowSpace.env()),
  m_prefix    ((std::string)(prefix)+""),
  m_rowSpace  (rowSpace      ),
  m_oneDTables(m_rowSpace.map(),1)
{
  for (unsigned int i = 0; i < (unsigned int) m_oneDTables.MyLength(); ++i) {
    m_oneDTables(i,0) = NULL;
  }
}

template <class V, class M>
uqArrayOfOneDTablesClass<V,M>::~uqArrayOfOneDTablesClass()
{
  for (unsigned int i = 0; i < (unsigned int) m_oneDTables.MyLength(); ++i) {
    if (m_oneDTables(i,0)) delete m_oneDTables(i,0);
  }
}

template <class V, class M>
void
uqArrayOfOneDTablesClass<V,M>::setOneDTable(unsigned int rowId, const std::vector<double>& values)
{
  UQ_FATAL_TEST_MACRO(rowId >= (unsigned int) m_oneDTables.MyLength(),
                      m_env.worldRank(),
                      "uqArrayOfOneDTablesClass<T>::setOneDTable()",
                      "rowId is out of range");

  if (m_oneDTables(rowId,0) == NULL) m_oneDTables(rowId,0) = new std::vector<double>(0);
  else                               m_oneDTables(rowId,0)->clear();

  std::vector<double>& vec = *(m_oneDTables(rowId,0));
  vec.resize(values.size(),0.);
  for (unsigned int j = 0; j < values.size(); ++j) {
    vec[j] = values[j];
  }

  return;
}

template <class V, class M>
const std::vector<double>&
uqArrayOfOneDTablesClass<V,M>::oneDTable(unsigned int rowId) const
{
  UQ_FATAL_TEST_MACRO(rowId >= (unsigned int) m_oneDTables.MyLength(),
                      m_env.worldRank(),
                      "uqArrayOfOneDTablesClass<T>::oneDTable()",
                      "rowId is out of range");

  uqArrayOfOneDTablesClass<V,M>* tmp = const_cast<uqArrayOfOneDTablesClass<V,M>*>(this);

  UQ_FATAL_TEST_MACRO(tmp->m_oneDTables(rowId,0) == NULL,
                      m_env.worldRank(),
                      "uqArrayOfOneDTablesClass<T>::oneDTable()",
                      "requested row is still NULL");

  return *(tmp->m_oneDTables(rowId,0));
}

template <class V, class M>
void
uqArrayOfOneDTablesClass<V,M>::print(std::ostream& os) const
{
  uqArrayOfOneDTablesClass<V,M>* tmp = const_cast<uqArrayOfOneDTablesClass<V,M>*>(this);
  for (unsigned int i = 0; i < (unsigned int) m_oneDTables.MyLength(); ++i) {
    const std::vector<double>& tmpVec = *(tmp->m_oneDTables(i,0));
    os << m_prefix << i << "_values_sub" << m_env.subIdString() << " = zeros(" << tmpVec.size()
       << ","                                                                  << 1
       << ");"
       << std::endl;
    os << m_prefix << i << "_values_sub" << m_env.subIdString() << " = [";
    for (unsigned int j = 0; j < tmpVec.size(); ++j) {
      os << tmpVec[j] << " ";
    }
    os << "];"
       << std::endl;
  }

  return;
}

template <class V, class M>
std::ostream& operator<< (std::ostream& os, const uqArrayOfOneDTablesClass<V,M>& obj)
{
  obj.print(os);
  return os;
}
#endif // __UQ_ARRAY_OF_ONE_D_TABLES_H__

