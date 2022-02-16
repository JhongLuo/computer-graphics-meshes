#include "catmull_clark.h"
#include <unordered_map>
#include <utility>
#include <functional>
#include "vertex_triangle_adjacency.h"
// Conduct num_iters iterations of Catmull-Clark subdivision on a **pure quad**
// mesh (V,F).
//
// Inputs:
//   V  #V by 3 list of vertex positions
//   F  #F by 4 list of quad mesh indices into V
//   num_iters  number of iterations
// Outputs:
//   SV  #SV by 3 list of vertex positions
//   SF  #SF by 4 list of quad mesh indices into SV
using namespace Eigen;

inline RowVector3d avg_2(RowVector3d a, RowVector3d b)
{
	return (a + b) / 2;
}
inline RowVector3d avg_3(RowVector3d a, RowVector3d b, RowVector3d c, RowVector3d d)
{
	return (a + b + c) / 3;
}
inline RowVector3d avg_4(RowVector3d a, RowVector3d b, RowVector3d c, RowVector3d d)
{
	return (a + b + c + d) / 4;
}

void catmull_clark(
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	const int num_iters,
	Eigen::MatrixXd& SV,
	Eigen::MatrixXi& SF)
{
	SV = V;
	SF = F;
	for (int i = 0;i < F.rows();i++) {
		for (int j = 0;j < 3;j++) {
			printf("%d ", F(i, j));
		}
		printf("\n");
	}
	//for (int iter = 0;iter < num_iters;iter++) {
	//	Eigen::MatrixXi new_SV = Eigen::MatrixXi::Zero(SV.rows() * 2 + SF.rows(), 3);
	//	Eigen::MatrixXi new_SF = Eigen::MatrixXi::Zero(SF.rows() * 4, 4);

	//	for (int i = 0; i < SF.rows(); i++) {
	//		new_SV.row(i) = avg_4(V.row(F(i, 0)), V.row(F(i, 1)), V.row(F(i, 2)), V.row(F(i, 3)));
	//	}

	//	std::vector<std::vector<int> > VF;
	//	vertex_triangle_adjacency(SF, SV.rows(), VF);


	//}
}



//void catmull_clark2(
//	const Eigen::MatrixXd& V,
//	const Eigen::MatrixXi& F,
//	const int num_iters,
//	Eigen::MatrixXd& SV,
//	Eigen::MatrixXi& SF)
//{
//	if (num_iters <= 0) return;
//
//	const int V_rows = V.rows();
//	const int F_rows = F.rows();
//
//	int SV_size = V_rows + F_rows;
//	const int SF_size = F_rows * 4;
//
//	// For each face, add face point.
//	Eigen::MatrixXd face_points = Eigen::MatrixXd::Zero(F_rows, 3);
//	for (int i = 0; i < F_rows; i++) {
//		face_points.row(i) = (V.row(F(i, 0)) + V.row(F(i, 1)) + V.row(F(i, 2)) + V.row(F(i, 3))) / 4.0;
//	}
//
//	std::unordered_map<int, std::vector<int>> new_edges;  // The key is a vertex represented by its index in V. The value is a list of vertex represented by their indices in V. Each new edge is stored only once. 
//	std::unordered_map<int, std::vector<Eigen::RowVector3d>> edge_points;  // The key is a vertex represented by its index in V. The value is a list of edge points. edge_points[i].at(j) is the edge point between the vertex V.row(i) and the vertex V.row(new_edges[i].at(j)). Each edge point is stored only once.
//	std::unordered_map<std::pair<int, int>, std::vector<int>, hash_pair> edge_to_faces; // The key represents an edge. The value is a list of faces, represented by their indices in F. An edge may be stored more than once.
//	int current_vertex_index, another_vertex_index;
//	std::pair<int, int> pair, reversed_pair;
//
//	// Populate edge_to_faces.
//	for (int i = 0; i < F_rows; i++) {
//		for (int j = 0; j < 4; j++) {
//			current_vertex_index = F(i, j);
//			another_vertex_index = F(i, (j + 1) % 4);
//			pair = std::make_pair(current_vertex_index, another_vertex_index);
//			reversed_pair = std::make_pair(another_vertex_index, current_vertex_index);
//
//			if (edge_to_faces.find(pair) == edge_to_faces.end() && edge_to_faces.find(reversed_pair) == edge_to_faces.end()) {
//				std::vector<int> entry;
//				edge_to_faces[pair] = entry;
//				edge_to_faces[pair].push_back(i);
//			}
//			else if (edge_to_faces.find(pair) != edge_to_faces.end()) {
//				edge_to_faces[pair].push_back(i);
//			}
//			else {
//				edge_to_faces[reversed_pair].push_back(i);
//			}
//		}
//	}
//
//	int num_vertex;
//	Eigen::RowVector3d edge_point_value(0, 0, 0);
//	// Populate new_edges and edge_points.
//	for (int i = 0; i < F_rows; i++) {
//		for (int j = 0; j < 4; j++) {
//			current_vertex_index = F(i, j);
//			another_vertex_index = F(i, (j + 1) % 4);
//			pair = std::make_pair(current_vertex_index, another_vertex_index);
//			reversed_pair = std::make_pair(another_vertex_index, current_vertex_index);
//
//			// If key not exist.
//			if (new_edges.find(current_vertex_index) == new_edges.end()) {
//				std::vector<int> entry;
//				new_edges[current_vertex_index] = entry;
//				std::vector<Eigen::RowVector3d> edge_point_entry;
//				edge_points[current_vertex_index] = edge_point_entry;
//			}
//
//			// If the edge between vertex_index and target_index_1 is not already stored.
//			if (std::find(new_edges[current_vertex_index].begin(), new_edges[current_vertex_index].end(), another_vertex_index) == new_edges[current_vertex_index].end() && (new_edges.find(another_vertex_index) == new_edges.end() || std::find(new_edges[another_vertex_index].begin(), new_edges[another_vertex_index].end(), current_vertex_index) == new_edges[another_vertex_index].end())) {
//				new_edges[current_vertex_index].push_back(another_vertex_index);
//				edge_point_value.setZero();
//				if (edge_to_faces.find(pair) != edge_to_faces.end()) {
//					num_vertex = edge_to_faces[pair].size();
//					for (int m = 0; m < num_vertex; m++) {
//						edge_point_value += face_points.row(edge_to_faces[pair].at(m));
//					}
//				}
//				else {
//					num_vertex = edge_to_faces[reversed_pair].size();
//					for (int m = 0; m < num_vertex; m++) {
//						edge_point_value += face_points.row(edge_to_faces[reversed_pair].at(m));
//					}
//				}
//				num_vertex += 2;
//				edge_points[current_vertex_index].push_back((V.row(another_vertex_index) + V.row(current_vertex_index) + edge_point_value) / (double)num_vertex);
//				SV_size++;
//			}
//		}
//	}
//
//	SV = Eigen::MatrixXd::Zero(SV_size, 3);
//	SF = Eigen::MatrixXi::Zero(SF_size, 4);
//
//	int num_edges;
//	std::vector<std::vector<int> > VF;
//	std::vector<int> face_indces;
//	vertex_triangle_adjacency(F, V_rows, VF);
//
//	Eigen::RowVector3d f(0, 0, 0), r(0, 0, 0);
//	Eigen::RowVector3d barycenter(0, 0, 0);
//	int num_points;
//	for (int i = 0; i < V_rows; i++) {
//		// Calculate f, the average of all face points of faces adjacent to vertex at V.row(i).
//		face_indces = VF[i];
//		f.setZero();
//		for (const int face_index : face_indces) {
//			f += face_points.row(face_index);
//		}
//		f /= face_indces.size();
//
//		num_edges = 0;
//		// Calculate r, the average of all edge points of edges touching vertex at V.row(i).
//		r.setZero();
//		for (auto entry : edge_to_faces) {
//			if (entry.first.first == i) {
//				r += (V.row(entry.first.second) + V.row(i)) / 2.0;
//				num_edges++;
//			}
//			else if (entry.first.second == i) {
//				r += (V.row(entry.first.first) + V.row(i)) / 2.0;
//				num_edges++;
//			}
//		}
//		if (num_edges != 0) r /= num_edges;
//
//		barycenter = (f + 2 * r + (num_edges - 3) * V.row(i)) / num_edges;
//
//		SV.row(i) = barycenter;
//	}
//
//	// Put each face point into SV.
//	for (int i = 0; i < F_rows; i++) {
//		SV.row(V_rows + i) = face_points.row(i);
//	}
//
//	std::unordered_map<std::pair<int, int>, int, hash_pair> edge_to_SV_row;  // The key is an edge. The value is the row number of the edge point in SV.
//	int index = V_rows + F_rows;
//	// Put each edge point into SV and opulate edge_to_SV_row.
//	for (int i = 0; i < V_rows; i++) {
//		for (int j = 0; j < edge_points[i].size(); j++) {
//			SV.row(index) = edge_points[i].at(j);
//			std::pair<int, int> pair = std::make_pair(i, new_edges[i].at(j));
//			edge_to_SV_row[pair] = index;
//			index++;
//		}
//	}
//
//	// Populate SF.
//	int point_index_1, point_index_2, point_index_3, point_index_4, another_index_1, another_index_2;
//	std::pair<int, int> pair_1, pair_1_reversed, pair_2, pair_2_reversed;
//	for (int i = 0; i < F_rows; i++) {
//		for (int j = 0; j < 4; j++) {
//			point_index_1 = F(i, j);
//			point_index_3 = V_rows + i;
//
//			another_index_1 = F(i, (j + 1) % 4);
//			another_index_2 = F(i, (j - 1 + 4) % 4);
//
//			pair_1 = std::make_pair(point_index_1, another_index_1);
//			pair_1_reversed = std::make_pair(another_index_1, point_index_1);
//			pair_2 = std::make_pair(point_index_1, another_index_2);
//			pair_2_reversed = std::make_pair(another_index_2, point_index_1);
//			if (edge_to_SV_row.find(pair_1) != edge_to_SV_row.end()) {
//				point_index_2 = edge_to_SV_row[pair_1];
//			}
//			else if (edge_to_SV_row.find(pair_1_reversed) != edge_to_SV_row.end()) {
//				point_index_2 = edge_to_SV_row[pair_1_reversed];
//			}
//
//
//			if (edge_to_SV_row.find(pair_2) != edge_to_SV_row.end()) {
//				point_index_4 = edge_to_SV_row[pair_2];
//			}
//			else if (edge_to_SV_row.find(pair_2_reversed) != edge_to_SV_row.end()) {
//				point_index_4 = edge_to_SV_row[pair_2_reversed];
//			}
//
//
//			SF.row(i * 4 + j) = Eigen::RowVector4i(point_index_1, point_index_2, point_index_3, point_index_4);
//		}
//	}
//
//	catmull_clark(Eigen::MatrixXd(SV), Eigen::MatrixXi(SF), num_iters - 1, SV, SF);
//	////////////////////////////////////////////////////////////////////////////
//}

