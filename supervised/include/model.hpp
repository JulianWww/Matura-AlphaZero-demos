#pragma once

#include <torch/torch.h>
#include <wx/bitmap.h>
#include <point.hpp>

namespace SL {
  class Model : public torch::nn::Module {
    private: using Optimizer          = torch::optim::SGD;
    private: using OptimizerOptions   = torch::optim::SGDOptions;

    private: torch::nn::Linear lin1, lin2, lin3;
    private: torch::nn::LeakyReLU relu;
    private: torch::nn::Sigmoid sigm;
    private: Optimizer optim;

    public: Model();
    public: torch::Tensor forward(torch::Tensor x);
    public: void fit(torch::Tensor x, torch::Tensor y);
    public: void train();

    public: wxImage getMap();
    private: torch::Tensor getMapTensor();

    private: static std::pair<torch::Tensor, torch::Tensor> getTrainingData();
    private: static double classify(const VQ::Point& point);
  };
}