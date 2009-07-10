/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------
 *
 * Copyright (C) 2008,2009 The PECOS Development Team
 *
 * Please see http://pecos.ices.utexas.edu for more information.
 *
 * This file is part of the QUESO Library (Quantification of Uncertainty
 * for Estimation, Simulation and Optimization).
 *
 * QUESO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QUESO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QUESO. If not, see <http://www.gnu.org/licenses/>.
 *
 *--------------------------------------------------------------------------
 *
 * $Id$
 *
 * Basic API: Routines for interfacing with QUESO.
 * 
 *--------------------------------------------------------------------------
 *-------------------------------------------------------------------------- */

using namespace std;

#define _QUESO_Basic_API_DEF

#include <basic_classes.h>
#include <basic_int.h>

using namespace QUESO_Basic_API;

char *f2c_char(char *,int);

//-------------
// C Interface 
//-------------

extern "C" void QUESO_init(const char *inputfile)
{
  _QUESO_Basic = new QUESO_Basic_Class();
  _QUESO_Basic->Initialize(inputfile);
  return;
}

extern "C" void QUESO_statistical_inversion(double (*fp)(double *) )
{
  _QUESO_Basic->DefineParameterSpace ();
  _QUESO_Basic->Likelihood_Register  (fp);
  _QUESO_Basic->SolveInverseProblem  ();
  return;
}

extern "C" void QUESO_finalize()
{
  delete _QUESO_Basic;
  printf("\n QUESO: Complete\n");
  return;
}

//-------------------
// Fortran Interface 
//-------------------
    
extern "C" void queso_init_(char *inputfile,int _namelen)
{
  char *name = f2c_char(inputfile,_namelen);
  QUESO_init(name);

  delete[] name;
  return;
}

extern "C" void queso_statistical_inversion_(double (*fp)(double *) )
{
  QUESO_statistical_inversion(fp);
  return;
}

extern "C" void queso_finalize_()
{
  QUESO_finalize();
  return;
}


// f2c_char(): Convert evil Fortran character strings to C                 

char *f2c_char(char*input,int len)
{
  char* name = new char[len+1];

  strncpy(name,input,len);
  name[len]='\0';
  return(name);
}
