/*******************************************************************************************************
*  Copyright 2017 Alliance for Sustainable Energy, LLC
*
*  NOTICE: This software was developed at least in part by Alliance for Sustainable Energy, LLC
*  (�Alliance�) under Contract No. DE-AC36-08GO28308 with the U.S. Department of Energy and the U.S.
*  The Government retains for itself and others acting on its behalf a nonexclusive, paid-up,
*  irrevocable worldwide license in the software to reproduce, prepare derivative works, distribute
*  copies to the public, perform publicly and display publicly, and to permit others to do so.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted
*  provided that the following conditions are met:
*
*  1. Redistributions of source code must retain the above copyright notice, the above government
*  rights notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright notice, the above government
*  rights notice, this list of conditions and the following disclaimer in the documentation and/or
*  other materials provided with the distribution.
*
*  3. The entire corresponding source code of any redistribution, with or without modification, by a
*  research entity, including but not limited to any contracting manager/operator of a United States
*  National Laboratory, any institution of higher learning, and any non-profit organization, must be
*  made publicly available under this license for as long as the redistribution is made available by
*  the research entity.
*
*  4. Redistribution of this software, without modification, must refer to the software by the same
*  designation. Redistribution of a modified version of this software (i) may not refer to the modified
*  version by the same designation, or by any confusingly similar designation, and (ii) must refer to
*  the underlying software originally provided by Alliance as �System Advisor Model� or �SAM�. Except
*  to comply with the foregoing, the terms �System Advisor Model�, �SAM�, or any confusingly similar
*  designation may not be used to refer to any modified version of this software or any modified
*  version of the underlying software originally provided by Alliance without the prior written consent
*  of Alliance.
*
*  5. The name of the copyright holder, contributors, the United States Government, the United States
*  Department of Energy, or any of their employees may not be used to endorse or promote products
*  derived from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
*  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER,
*  CONTRIBUTORS, UNITED STATES GOVERNMENT OR UNITED STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR
*  EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
*  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************************************/

#define _TCSTYPEINTERFACE_
#include "tcstype.h"

enum {
	I_IN1,
	I_IN2,
	I_IN3,
	I_IN4,
	I_SCALE,
	I_VEC,
	I_MAT,

	O_SUM,
	O_VEC,
	O_MAT,
	O_STR,

	N_MAX };

tcsvarinfo datatest_variables[] = {
	// vartype    datatype    index   name     label    units   meta   group   default_value
	{ TCS_INPUT,  TCS_NUMBER, I_IN1, "input1", "Data 1", "",      "",      "",     "" },
	{ TCS_INPUT,  TCS_NUMBER, I_IN2, "input2", "Data 2", "",      "",      "",     "" },
	{ TCS_INPUT,  TCS_NUMBER, I_IN3, "input3", "Data 3", "",      "",      "",     "" },
	{ TCS_INPUT,  TCS_NUMBER, I_IN4, "input4", "Data 4", "",      "",      "",     "" },
	{ TCS_INPUT,  TCS_NUMBER, I_SCALE, "scale", "Scale", "",      "",      "",     "" },
	{ TCS_INPUT,  TCS_ARRAY,  I_VEC,  "vec_in", "ArrayI", "",     "",      "",     "" },
	{ TCS_INPUT,  TCS_MATRIX, I_MAT,  "mat_in", "Matrix", "",     "",      "",     "" },

	{ TCS_OUTPUT, TCS_NUMBER, O_SUM, "sum",     "Sum",    "",     "",      "",     "" },
	{ TCS_OUTPUT, TCS_ARRAY,  O_VEC, "vec_out", "Array",  "",     "",      "",     "" },
	{ TCS_OUTPUT, TCS_MATRIX, O_MAT, "mat_out", "Matrix", "",     "",      "",     "" },
	{ TCS_OUTPUT, TCS_STRING, O_STR, "str_out", "String", "",     "",      "",     "" },
	
	{ TCS_INVALID, TCS_INVALID,  N_MAX,       0,            0, 0, 0, 0, 0 }
};


class datatest : public tcstypeinterface
{
private:
public:
	datatest( tcscontext *cxt, tcstypeinfo *ti )
		: tcstypeinterface( cxt, ti )
	{
	}

	virtual ~datatest()
	{
	}

	virtual int init()
	{
		int len;
		double *vec = value( I_VEC, &len );
		
		allocate( O_VEC, 4 );


		int nrows, ncols;
		double *mat = value( I_MAT, &nrows, &ncols );		
		// make the output matrix same size as the input matrix
		if (mat && nrows > 0 && ncols > 0 )
			allocate( O_MAT, nrows, ncols );

		return 0;
	}

	virtual int call( double time, double step, int ncall )
	{
		double v[4], scale = value( I_SCALE );
		v[0] = value( I_IN1 );
		v[1] = value( I_IN2 );
		v[2] = value( I_IN3 );
		v[3] = value( I_IN4 );

		double sum = 0;
		for (int i=0;i<4;i++) sum += v[i];
		sum *= scale;

		value( O_SUM, sum );
		
		int len;
		double *vec = value(O_VEC, &len);
		if (vec && len == 4)
			for (int i=0;i<4;i++)
				vec[i] = v[3-i];


		int inr, inc, onr, onc;
		value( I_MAT, &inr, &inc ); // get sizes
		value( O_MAT, &onr, &onc ); 
		tcsvalue *imat = var( I_MAT ); // get matrix variables
		tcsvalue *omat = var( O_MAT );
		double matsum = 0;
		if (omat && inr == onr && inc == onc && imat != 0)
		{
			for (int r=0;r<inr;r++)
			{
				for (int c=0;c<inc;c++)
				{
					matsum += TCS_MATRIX_INDEX(imat, r, c);
					TCS_MATRIX_INDEX( omat, r, c) =  TCS_MATRIX_INDEX( imat, r, c ) * scale;
				}
			}
		}

		char buf[256];
		sprintf(buf, " %.2lf : %.1lf, %.1lf, %.1lf,%.1lf", matsum, v[0], v[1], v[2], v[3]);
		value_str( O_STR, buf );

		return 0;
	}
};

TCS_IMPLEMENT_TYPE( datatest, "Data test", "Aron Dobos", 1, datatest_variables, NULL, 0 )

