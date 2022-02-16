#include "cube.h"

void cube(
	Eigen::MatrixXd& V,
	Eigen::MatrixXi& F,
	Eigen::MatrixXd& UV,
	Eigen::MatrixXi& UF,
	Eigen::MatrixXd& NV,
	Eigen::MatrixXi& NF)
{
	V.resize(8, 3);
	V <<
		0, 0, 0,
		0, 0, 1,
		0, 1, 0,
		1, 0, 0,
		0, 1, 1,
		1, 0, 1,
		1, 1, 0,
		1, 1, 1;

	F.resize(6, 4);
	F <<
		0, 2, 6, 3,
		0, 3, 5, 1,
		0, 1, 4, 2,
		7, 6, 2, 4,
		7, 4, 1, 5,
		7, 5, 3, 6;

	UV.resize(14, 2);
	UV <<
		1, 3,
		2, 3,

		0, 2,
		1, 2,
		2, 2,
		3, 2,
		4, 2,

		0, 1,
		1, 1,
		2, 1,
		3, 1,
		4, 1,

		1, 0,
		2, 0;
	UV = UV / 4;

	UF.resize(6, 4);
	UF << 
		8, 12, 13, 9,
		2, 7, 8, 3,
		10, 5, 6, 11,
		9, 4, 5, 10,
		0, 1, 4, 3,
		3, 4, 9, 8;

	NV.resize(6, 3);
	NV << 
		0, 0, -1,
		0, -1, 0,
		-1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		1, 0, 0; 

	NF.resize(6, 4);
	NF <<
		0, 0, 0, 0,
		1, 1, 1, 1,
		2, 2, 2, 2,
		3, 3, 3, 3,
		4, 4, 4, 4,
		5, 5, 5, 5;
}
