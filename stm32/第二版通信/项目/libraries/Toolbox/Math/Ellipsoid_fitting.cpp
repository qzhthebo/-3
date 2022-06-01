#include "Ellipsoid_fitting.h"

Ellipsoid_Fit::Ellipsoid_Fit(){
	N = 0;
	isFailed = false;
	for(u8 count=0;count<6;count++)
		param[count]=solve[count]=0.0f;
		
}
double Ellipsoid_Fit::Abs(double a){
	return a < 0 ? -a : a;
}

void Ellipsoid_Fit::ResetMatrix(void){
	for(uint8_t row=0;row<MATRIX_SIZE;row++){
		for(uint8_t column=0;column<MATRIX_SIZE;column++)
			m_matrix[row][column] = 0.0f;
	}
}

void Ellipsoid_Fit::Row1_swap_Row2(uint8_t row1,uint8_t row2){
	double temp = 0.0f;
	for(uint8_t column=0;column<MATRIX_SIZE+1;column++){
		temp = m_matrix[row1][column];
		m_matrix[row1][column] = m_matrix[row2][column];
		m_matrix[row2][column] = temp;
	}
}

void Ellipsoid_Fit::K_muiltply_Row(double k,uint8_t row){
	for(uint8_t column=0;column<MATRIX_SIZE+1;column++)
		m_matrix[row][column] *= k;
}

void Ellipsoid_Fit::Row2_add_kRow1(double k,uint8_t row1,uint8_t row2){
	for(uint8_t column=0;column<MATRIX_SIZE+1;column++)
		m_matrix[row2][column] += k * m_matrix[row1][column];
}

void Ellipsoid_Fit::MoveBiggestElement_to_Top(uint8_t k){
	u8 row=0;
	for(row=k+1;row<MATRIX_SIZE;row++)
		if(Abs(m_matrix[k][k])<Abs(m_matrix[row][k]))
			Row1_swap_Row2(k,row);
}

u8 Ellipsoid_Fit::Matrix_GaussElimination(void){
	double k=0;
	for(uint8_t cnt=0;cnt<MATRIX_SIZE;cnt++){
		MoveBiggestElement_to_Top(cnt);
		if(m_matrix[cnt][cnt]==0)
			return 1;
		for(uint8_t row=cnt+1;row<MATRIX_SIZE;row++){
			k = -m_matrix[row][cnt] / m_matrix[cnt][cnt];
			Row2_add_kRow1(k,cnt,row);
		}	
	}
	return 0;	
}

void Ellipsoid_Fit::Matrix_RowSimplify(void){
	double k=0;
	for(u8 row=0;row<MATRIX_SIZE;row++){
		k = 1/ m_matrix[row][row];
		K_muiltply_Row(k,row);
	}
//	DispMatrix();
}

void Ellipsoid_Fit::Matrix_Solve(double* solve){
	for (short row = MATRIX_SIZE - 1; row >= 0; row--){
		solve[row] = m_matrix[row][MATRIX_SIZE];
		for (u8 column = MATRIX_SIZE - 1; column >= row + 1; column--)
			solve[row] -= m_matrix[row][column] * solve[column];
	}
}

void Ellipsoid_Fit::CalcData_Input(double x,double y,double z){
	double V[MATRIX_SIZE+1];
	u8 row,column;
	N++;
	V[0]=y*y;
	V[1]=z*z;
	V[2]=x;
	V[3]=y;
	V[4]=z;
	V[5]=1.0;
	V[6]=-x*x;
	for(row=0;row<MATRIX_SIZE;row++)
		for(column=0;column<MATRIX_SIZE+1;column++){
			m_matrix[row][column] += V[row] * V[column];
		}
}

void Ellipsoid_Fit::CalcData_Input_average(void){
	for(u8 row=0;row<MATRIX_SIZE;row++)
		for(u8 column=0;column<MATRIX_SIZE+1;column++)
				m_matrix[row][column] /= N;
}
//void Ellipsoid_Fit::DispMatrix(void){
//	for(u8 row=0;row<MATRIX_SIZE;row++){
//		for(u8 column=0;column<MATRIX_SIZE+1;column++){
//			_debug<<m_matrix[row][column]<<" ";
//			if(column == MATRIX_SIZE-1)
//				_debug<<"|";
//		}
//		_debug<<endl;
//	}
//	_debug<<endl;
//}
double* Ellipsoid_Fit::Ellipsoid_fitting_Process(void){
	CalcData_Input_average();
//	DispMatrix();
	if(Matrix_GaussElimination()){
		isFailed = true;
		return 0;
	}
	else{
		Matrix_RowSimplify();
		Matrix_Solve(solve);
		
		double a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
		a = solve[0];
		b = solve[1];
		c = solve[2];
		d = solve[3];
		e = solve[4];
		f = solve[5];

		double X0 = 0, Y0 = 0, Z0 = 0, A = 0, B = 0, C = 0;
		X0 = -c / 2;
		Y0 = -d / (2 * a);
		Z0 = -e / (2 * b);
		A = sqrt(X0*X0 + a*Y0*Y0 + b*Z0*Z0 - f);
		B = A / sqrt(a);
		C = A / sqrt(b);
		param[0]=X0;
		param[1]=Y0;
		param[2]=Z0;
		param[3]=A;
		param[4]=B;
		param[5]=C;
	}
	return param;
}






