#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
	const std::string& filename,
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	const Eigen::MatrixXd& UV,
	const Eigen::MatrixXi& UF,
	const Eigen::MatrixXd& NV,
	const Eigen::MatrixXi& NF)
{
	assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");
	////////////////////////////////////////////////////////////////////////////
	std::ofstream obj_out(filename);
	if (!obj_out.is_open()) {
		return false;
	}

	for (int i = 0; i < V.rows(); i++) {
		obj_out << "v " << V(i, 0) << " " << V(i, 1) << " " << V(i, 2) << "\n";
	}

	for (int i = 0; i < UV.rows(); i++) {
		obj_out << "vt " << UV(i, 0) << " " << UV(i, 1) << "\n";
	}

	for (int i = 0; i < NV.rows(); i++) {
		obj_out << "vn " << NV(i, 0) << " " << NV(i, 1) << " " << NV(i, 2) << "\n";
	}


	for (int i = 0; (i < F.rows()) || (i < UF.rows()) || (i < NF.rows()); i++) {
		obj_out << "f";
		for (int j = 0; j < F.cols(); j++) {
			obj_out << " ";
			if (i < F.rows())
				obj_out << F(i, j) + 1;
			obj_out << "/";
			if (i < UF.rows())
				obj_out << UF(i, j) + 1;
			obj_out << "/";
			if (i < NF.rows())
				obj_out << NF(i, j) + 1;
			obj_out << "/";
		}
		obj_out << "\n";
	}

	obj_out.close();
	return true;

}

