#ifndef _ELLIPSOID_FITTING_H_
#define _ELLIPSOID_FITTING_H_

#include "stm32f10x.h"
#include <math.h>

#define MATRIX_SIZE 6



class Ellipsoid_Fit{
protected:
	int N;
	double param[6];
	double solve[6];
	double m_matrix[MATRIX_SIZE][MATRIX_SIZE+1];
private:
	double Abs(double a);
	//void DispMatrix(void);

	void CalcData_Input_average(void);		
	void Row1_swap_Row2(uint8_t row1,uint8_t row2);
	void K_muiltply_Row(double k,uint8_t row);
	void Row2_add_kRow1(double k,uint8_t row1,uint8_t row2);
	void MoveBiggestElement_to_Top(uint8_t k);
	u8 Matrix_GaussElimination(void);
	void Matrix_RowSimplify(void);
	void Matrix_Solve(double* solve);

public:
	bool isFailed;
	Ellipsoid_Fit();
	void CalcData_Input(double x,double y,double z);	
	double* Ellipsoid_fitting_Process(void);
	void ResetMatrix(void);
};












#endif
