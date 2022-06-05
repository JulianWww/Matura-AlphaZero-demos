#include <model.hpp>
#include <config.hpp>
#include <colormap/palettes.hpp>

#define COLORMAP "jet"

const static VQ::Point center(HEIGHT/2, HEIGHT/2);

SL::Model::Model() :
  lin1(register_module("lin1", torch::nn::Linear(2,256))),
  lin2(register_module("lin2", torch::nn::Linear(256, 16))),
  lin3(register_module("lin3", torch::nn::Linear(16, 1))),
  relu(),
  sigm(),
  optim(this->parameters(), OptimizerOptions(LR).momentum(Momentum))
{}

torch::Tensor SL::Model::forward(torch::Tensor x) {
  x = relu(lin1(x));
  x = relu(lin2(x));
  return sigm(lin3(x));
}

void SL::Model::fit(torch::Tensor x, torch::Tensor y_true) {
  torch::Tensor y_pred = this->forward(x);
  auto loss = LOSS(y_true, y_pred);
  loss.backward(loss);
  optim.step();
  optim.zero_grad();
}

void SL::Model::train() {
  auto data = this->getTrainingData();
  for (size_t idx=0; idx < EPOCHS; idx++)
    this->fit(data.first, data.second);
}

wxImage SL::Model::getMap() {
  torch::Tensor y = this->forward(this->getMapTensor());
  wxImage img(RES_HEIGHT, RES_WIDTH);
  auto pal = colormap::palettes.at(COLORMAP).rescale(0, 1);
  for (size_t idx_x=0; idx_x < RES_HEIGHT; idx_x++) {
    for (size_t idx_y=0; idx_y < RES_WIDTH; idx_y++) {
      auto pix = pal(y[idx_x*RES_WIDTH + idx_y][0].item<float>());
      img.SetRGB(idx_x, idx_y, 
        pix.getRed().getValue(), 
        pix.getGreen().getValue(), 
        pix.getBlue().getValue()
      );
    }
  }
  return img;
}

torch::Tensor SL::Model::getMapTensor() {
  torch::Tensor out = torch::zeros({RES_HEIGHT * RES_WIDTH, 2});
  for (size_t idx_x=0; idx_x < RES_HEIGHT; idx_x++) {
    for (size_t idx_y=0; idx_y < RES_WIDTH; idx_y++) {
      out[idx_x*RES_WIDTH + idx_y][0] = (double)idx_x/((double)RES_HEIGHT);
      out[idx_x*RES_WIDTH + idx_y][1] = (double)idx_y/((double)RES_WIDTH);
    }
  }
  return out;
}

std::pair<torch::Tensor, torch::Tensor> SL::Model::getTrainingData() {
  torch::Tensor x = torch::ones({BATCH_SIZE, 2});
  torch::Tensor y = torch::ones({BATCH_SIZE, 1});

  for (size_t idx=0; idx < BATCH_SIZE; idx++) {
    const VQ::Point p = VQ::randomPointInRange(WIDTH, HEIGHT);
    y[idx][0] = classify(p);
    x[idx][0] = p.getPos().first  / (double)HEIGHT;
    x[idx][1] = p.getPos().second / (double)WIDTH;
  }
  return {x, y};
}
double SL::Model::classify(const VQ::Point& point) {
  if ((point - center).abs() > CIRC_RADIUS) {
    return 1.0;
  }
  return 0.0;
}