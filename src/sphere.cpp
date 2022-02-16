#include "sphere.h"
#include <iostream>

// Inputs:
//   num_faces_u  number of faces in the longitudinal direction
//   num_faces_v  number of faces in the latitudinal direction
// Outputs:
//   V  #V by 3 list of 3D corner vertex positions
//   F  #F by 4 list of quad face indices into rows of V
//   UV  #UV by 2 list of corner parameterization positions
//   UF  #F by 4 list of quad face indices into rows of UV
//   NV  #NV  by 3 list of 3D unit normal vectors
//   NF  #F by 4 list of quad face indices into rows of NV
void sphere(
	const int num_faces_u,
	const int num_faces_v,
	Eigen::MatrixXd& V,
	Eigen::MatrixXi& F,
	Eigen::MatrixXd& UV,
	Eigen::MatrixXi& UF,
	Eigen::MatrixXd& NV,
	Eigen::MatrixXi& NF)
{
	////////////////////////////////////////////////////////////////////////////
	const int v_total = num_faces_v * (num_faces_u + 1) + 1;
	V.resize(v_total, 3);
	UV.resize(v_total, 2);
	NV.resize(v_total, 3);

	const int f_total = num_faces_u * num_faces_v;
	F.resize(f_total, 4);
	UF.resize(f_total, 4);
	NF.resize(f_total, 4);

	V.row(0) = Eigen::RowVector3d(0, 0, 1);
	NV.row(0) = Eigen::RowVector3d(0, 0, 1).normalized();
	UV.row(0) = Eigen::RowVector2d(0, 1);


	for (int i = 0; i < num_faces_v; i++) {
		for (int j = 0; j <= num_faces_u; j++) {
			unsigned int idx = i * (num_faces_u + 1) + j + 1;


			double lati = ((double)(i + 1) / num_faces_v) * 3.1415926;
			double z = 1 * cos(lati);

			double longti = ((double)(j) / num_faces_u) * 2 * 3.1415926;
			double x = 1 * sin(lati) * cos(longti);
			double y = 1 * sin(lati) * sin(longti);
			//printf("%f %f %f\n", x, y, z);

			V.row(idx) = Eigen::RowVector3d(x, y, z);
			NV.row(idx) = V.row(idx).normalized();
			UV.row(idx) = Eigen::RowVector2d((double)(j) / num_faces_u, (double)(num_faces_v - i - 1) / num_faces_v);

			//all the right down corner of faces
			if (j != 0) {
				int right_down, left_down, left_up, right_up;
				if (i == 0) {
					right_down = idx;
					left_down = idx - 1;
					left_up = 0;
					right_up = 0;
				}
				else {
					right_down = idx;
					left_down = idx - 1;
					left_up = idx - (num_faces_u + 1) - 1;
					right_up = idx - (num_faces_u + 1);
				}
				F.row(i * num_faces_u + (j - 1)) = Eigen::RowVector4i(right_down, left_down, left_up, right_up);
				UF.row(i * num_faces_u + (j - 1)) = Eigen::RowVector4i(right_down, left_down, left_up, right_up);
				NF.row(i * num_faces_u + (j - 1)) = Eigen::RowVector4i(right_down, left_down, left_up, right_up);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////
}
