#include "vertex_triangle_adjacency.h"
// Inputs:
//   F  #F by 3 list of mesh triangle indices
//   num_vertices  number of vertices (i.e., V.rows(); usually ==F.maxCoeff()+1)
// Outputs:
//   VF  num_verticess-long list of lists so that f=VF[i][j] means that face f
//     is the jth face among those incident on vertex i. Adjacency faces are
//     listed in no particular order (but no duplicates).
void vertex_triangle_adjacency(
	const Eigen::MatrixXi& F,
	const int num_vertices,
	std::vector<std::vector<int> >& VF)
{
	VF.resize(num_vertices);
	for (int i = 0;i < num_vertices;i++) {
		for (int j = 0;j < F.rows();j++) {
			if (F(j, 0) == i || F(j, 1) == i || F(j, 2) == i) {
				VF[i].push_back(j);
			}
		}
	}
}

