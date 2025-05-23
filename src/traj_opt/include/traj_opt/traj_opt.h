#pragma once
#include <ros/ros.h>

#include <chrono>
#include <thread>
#include <vis_utils/vis_utils.hpp>

#include "minco.hpp"
// #include "target_prediction/bezier_predict.h"

namespace traj_opt {

class TrajOpt {
 public:
    ros::NodeHandle nh_;
    std::shared_ptr<vis_utils::VisUtils> visPtr_;
    bool pause_debug_ = false;
    // # pieces and # key points
    int N_, K_, dim_t_, dim_p_;
    // weight for time regularization term
    double rhoT_, rhoVt_;
    double rhoTf_;
    // collision avoiding and dynamics paramters
    double vmax_, amax_;
    double rhoPf_, rhoPt_, rhoV_, rhoA_, rhoVZ_, rhoVland_;
    double landpBound_;
    double rhoVisibleDomain_;
    double rhoThrust_, rhoOmega_;
    double rhoPerchingCollision_;
    // landing parameters
    double robot_l_, robot_r_, platform_r_, platform_l_;
    // SE3 dynamic limitation parameters
    double thrust_max_, thrust_min_;
    double omega_max_, omega_yaw_max_;
    // MINCO Optimizer
    minco::MINCO_S4_Uniform mincoOpt_;
    Eigen::MatrixXd initS_;
    // duration of each piece of the trajectory
    Eigen::VectorXd t_;
    double* x_;

   enum plan_s
   {
      HOVER = 1,
      FOLLOW,
      LAND
   };

    std::vector<Eigen::Vector3d> tracking_ps_;
    std::vector<Eigen::Vector3d> tracking_visible_ps_;
    std::vector<double> tracking_thetas_;

 public:
    TrajOpt(ros::NodeHandle& nh);
    ~TrajOpt() {}

    int optimize(const double& delta = 1e-4);
    bool generate_traj(const Eigen::MatrixXd& iniState,
                        const Eigen::Vector3d& car_p,
                        const Eigen::Vector3d& car_v,
                        const Eigen::Quaterniond& car_q,
                        const Eigen::Quaterniond& uav_q,
                        const bool& predict_flag,
                        const int& N,
                        Trajectory& traj, 
                        plan_s *plan_state,
                        const double& t_replan = -1.0);

    bool generate_test_traj(const std::vector<Eigen::Vector3d> route,
                            Trajectory& traj, 
                            const double& t_replan = -1.0);

    bool feasibleCheck(Trajectory& traj);

    void addTimeIntPenalty(double& cost);

    bool grad_cost_v(const Eigen::Vector3d& v,
                    Eigen::Vector3d& gradv,
                    double& costv);
    
    bool grad_cost_v_z(const Eigen::Vector3d& v,
                    Eigen::Vector3d& gradv,
                    double& costv);

    bool grad_cost_v_land(const Eigen::Vector3d& v,
                          Eigen::Vector3d& gradv,
                          double& costvland);

    bool grad_cost_thrust(const Eigen::Vector3d& a,
                            Eigen::Vector3d& grada,
                            double& costa);
    bool grad_cost_omega(const Eigen::Vector3d& a,
                        const Eigen::Vector3d& j,
                        Eigen::Vector3d& grada,
                        Eigen::Vector3d& gradj,
                        double& cost);
    bool grad_cost_omega_yaw(const Eigen::Vector3d& a,
                            const Eigen::Vector3d& j,
                            Eigen::Vector3d& grada,
                            Eigen::Vector3d& gradj,
                            double& cost);
    bool grad_cost_floor(const Eigen::Vector3d& p,
                        Eigen::Vector3d& gradp,
                        double& costp);

    bool grad_cost_top(const Eigen::Vector3d& p,
                        Eigen::Vector3d& gradp,
                        double& costp);

    bool grad_cost_perching_collision(const Eigen::Vector3d& pos,
                                        const Eigen::Vector3d& acc,
                                        const Eigen::Vector3d& car_p,
                                        Eigen::Vector3d& gradp,
                                        Eigen::Vector3d& grada,
                                        Eigen::Vector3d& grad_car_p,
                                        double& cost);
                                        

      bool grad_cost_visible_domain(const Eigen::Vector3d& pos,
                                             const Eigen::Vector3d& acc,
                                             const Eigen::Vector3d& car_p,
                                             Eigen::Vector3d& gradp,
                                             Eigen::Vector3d& grada,
                                             Eigen::Vector3d& grad_car_p,
                                             double& cost);

    bool check_collilsion(const Eigen::Vector3d& pos,
                            const Eigen::Vector3d& acc,
                            const Eigen::Vector3d& car_p);
};

}  // namespace traj_opt