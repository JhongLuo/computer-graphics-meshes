#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>

// Inputs:
//   V  #V by 3 list of vertex positions
//   F  #F by 3 list of mesh triangle indices into V
//   corner_threshold  threshold in degrees on sharp angles
// Outputs:
//   N  #F*3 by 3 list of mesh vertex 3D normals, where the normal
//     for corner F(i,j) is at N.row(i*3+j)
void per_corner_normals(
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	const double corner_threshold,
	Eigen::MatrixXd& N)
{
	N = Eigen::MatrixXd::Zero(F.rows() * 3, 3);

	double threshold_cos = cos(corner_threshold / 360 * 2 * 3.14159265);
	printf("%lf\n", threshold_cos);

	std::vector<std::vector<int> > VF;
	vertex_triangle_adjacency(F, V.rows(), VF);

	for (int i = 0;i < F.rows();i++) {
		Eigen::RowVector3d face_norm(triangle_area_normal(V.row(F(i, 0)), V.row(F(i, 1)), V.row(F(i, 2))).normalized());
		for (int j = 0;j < 3;j++) {
			std::vector<int>& adj_faces = VF[F(i, j)];
			Eigen::RowVector3d adj_area_norm_sum(0, 0, 0);
			for (int i = 0;i < adj_faces.size();i++) {
				int face_id = adj_faces[i];
				Eigen::RowVector3d adj_area_norm(triangle_area_normal(V.row(F(face_id, 0)), V.row(F(face_id, 1)), V.row(F(face_id, 2))));

				if ((adj_area_norm.dot(face_norm) / adj_area_norm.norm()) < threshold_cos) {//too sharp
					continue;
				}
				else {//add it
					adj_area_norm_sum += adj_area_norm;
				}
			}
			N.row(i * 3 + j) = adj_area_norm_sum.normalized();
		}
	}
}
