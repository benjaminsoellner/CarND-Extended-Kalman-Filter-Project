#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
	const vector<VectorXd> &ground_truth) {
	
	VectorXd rmse(4);
	rmse << 0, 0, 0, 0;

	// check the validity of the following inputs:
	//  * the estimation vector size should not be zero
	//  * the estimation vector size should equal ground truth vector size
	if (estimations.size() != ground_truth.size()
		|| estimations.size() == 0) {
		cout << "Invalid estimation or ground_truth data" << endl;
		return rmse;
	}

	//accumulate squared residuals
	for (unsigned int i = 0; i < estimations.size(); ++i) {

		VectorXd residual = estimations[i] - ground_truth[i];

		//coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
	}

	//calculate the mean
	rmse = rmse / estimations.size();

	//calculate the squared root
	rmse = rmse.array().sqrt();

	//return the result
	return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
	MatrixXd Hj(3, 4);
	//recover state parameters
	float px = x_state(0);
	float px2 = pow(px, 2);
	float py = x_state(1);
	float py2 = pow(py, 2);
	float vx = x_state(2);
	float vy = x_state(3);

	// DONE

	if (vx == 0 || vy == 0) {
		//check division by zero
		cout << "CalculateJacobian() - Error - Division by Zero";
		Hj << 0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0;
	}
	else {
		Hj << px / pow(px2 + py2, .5), py / pow(px2 + py2, .5), 0, 0,
			-py / (px2 + py2), px / (px2 + py2), 0, 0,
			py*(vx*py - vy*px) / pow(px2 + py2, 3. / 2), px*(vy*px - vx*py) / pow(px2 + py2, 3. / 2),
			px / pow(px2 + py2, .5), py / pow(px2 + py2, .5);
	}
	//compute the Jacobian matrix

	return Hj;
}