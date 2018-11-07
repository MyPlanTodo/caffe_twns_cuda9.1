#include <boost/thread.hpp>
#include "caffe/layer.hpp"

namespace caffe {
 

// Serialize LayerParameter to protocol buffer
template <typename Dtype>
// 注意 void Layer<Dtype>::ToProto(LayerParameter* param, bool write_diff) 已经在hpp文件里写了，就不在这写了
// new added start
/**********************************************************************************************************/
void Layer<Dtype>::BinaryToProto(LayerParameter* param) {
  param->Clear();
  param->CopyFrom(layer_param_);
  param->clear_blobs();
  for (int i = 0; i < blobs_.size(); ++i) {
    blobs_[i]->BinaryToProto(param->add_blobs());
  }
}

// Serialize LayerParameter to protocol buffer
template <typename Dtype>
void Layer<Dtype>::TernaryToProto(LayerParameter* param, string layer_name) {
  param->Clear();
  param->CopyFrom(layer_param_);
  param->clear_blobs();

string layer_type = parse_layer_type(layer_name);
if(layer_type == "loss") return;

const int n = blobs_.size();
for(int i=0; i<n; i++){
  if(i==0 && (layer_type == "convolution" || layer_type == "inner_product")){
    if(DEBUG) LOG(INFO) << "saving " << layer_name << " (blob " << i << " , ternary)";
    blobs_[i]->TernaryToProto(param->add_blobs());
  }else{
     if(DEBUG) LOG(INFO) << "saving " << layer_name << " (blob " << i << " , single)";
     blobs_[i]->ToProto(param->add_blobs());
  }
}

}
/**********************************************************************************************************/


INSTANTIATE_CLASS(Layer);

}  // namespace caffe
