#include "math_ops.h"
#include "graph.h"
#include "../tensorflow/tensor.h"
#include "../addons/operation.h"
#include "../lib/utils.h"
#include "../lib/conversions.h"

namespace tensorflow {

using namespace v8;
using namespace Nan;
using namespace addons;

// TODO: move to conversions
void Deallocator(void* data, size_t, void* arg) { tensorflow::cpu_allocator()->DeallocateRaw(data); }

TF_Operation* MathOps::add(TF_Graph *graph, TF_Operation* l, TF_Operation* r) {
  TF_Status* s = TF_NewStatus();

  TF_OperationDescription* desc = TF_NewOperation(graph, "Add", lib::uniqueId("Add").c_str());
  TF_SetAttrType(desc, "T", TF_FLOAT);
  TF_Port l_input = {l, 0};
  TF_AddInput(desc, l_input);
  TF_Port r_input = {r, 0};
  TF_AddInput(desc, r_input);

  TF_Operation* result = TF_FinishOperation(desc, s);
  if (TF_OK != TF_GetCode(s)) { std::cout << TF_Message(s); }
  TF_DeleteStatus(s);
  return result;
}

TF_Operation* MathOps::matmul(TF_Graph *graph, TF_Operation* l, TF_Operation* r) {
  TF_Status* s = TF_NewStatus();

  TF_OperationDescription* desc = TF_NewOperation(graph, "MatMul", lib::uniqueId("MatMul").c_str());

  TF_SetAttrType(desc, "T", TF_FLOAT);
  TF_Port l_input = {l, 0};
  TF_AddInput(desc, l_input);
  TF_Port r_input = {r, 0};
  TF_AddInput(desc, r_input);
  TF_SetAttrBool(desc, "transpose_a", false);
  TF_SetAttrBool(desc, "transpose_b", false);

  TF_Operation* result = TF_FinishOperation(desc, s);
  if (TF_OK != TF_GetCode(s)) { std::cout << TF_Message(s); }
  TF_DeleteStatus(s);
  return result;
}

TF_Operation* MathOps::matmul_add(TF_Graph *graph, TF_Operation* l, TF_Operation* r, TF_Operation* a) {
  return MathOps::add(graph, MathOps::matmul(graph, l, r), a);
}

TF_Operation* MathOps::reduce_mean(TF_Graph *graph, TF_Operation* v) {
  TF_Status* s = TF_NewStatus();

  TF_OperationDescription* desc = TF_NewOperation(graph, "Mean", lib::uniqueId("ReduceMean").c_str());
  TF_SetAttrType(desc, "T", TF_FLOAT);
  TF_Port input = {v, 0};
  TF_AddInput(desc, input);

  // reduction_indices
  const int byte_count = 1 * sizeof(int);
  int* values = reinterpret_cast<int*>(tensorflow::cpu_allocator()->AllocateRaw(EIGEN_MAX_ALIGN_BYTES, byte_count));
  values[0] = 1;
  TF_Tensor* reduction_indices = TF_NewTensor(TF_INT32, nullptr, 0, values, byte_count, &Deallocator, nullptr);
  TF_Operation* op = Graph::constant(graph, reduction_indices);
  TF_Port input_ri = {op, 0};
  TF_AddInput(desc, input_ri);

  // keep_dims

  TF_Operation* result = TF_FinishOperation(desc, s);
  if (TF_OK != TF_GetCode(s)) { std::cout << TF_Message(s); }
  TF_DeleteStatus(s);
  return result;
}

TF_Operation* MathOps::equal(TF_Graph *graph, TF_Operation* l, TF_Operation* r) {
  TF_Status* s = TF_NewStatus();

  TF_OperationDescription* desc = TF_NewOperation(graph, "Equal", lib::uniqueId("Equal").c_str());
  TF_SetAttrType(desc, "T", TF_FLOAT);
  TF_Port l_input = {l, 0};
  TF_AddInput(desc, l_input);
  TF_Port r_input = {r, 0};
  TF_AddInput(desc, r_input);

  TF_Operation* result = TF_FinishOperation(desc, s);
  if (TF_OK != TF_GetCode(s)) { std::cout << TF_Message(s); }
  TF_DeleteStatus(s);
  return result;
}

TF_Operation* MathOps::argmax(TF_Graph *graph, TF_Operation* v, int dim) {
  TF_Status* s = TF_NewStatus();

  TF_OperationDescription* desc = TF_NewOperation(graph, "ArgMax", lib::uniqueId("ArgMax").c_str());
  TF_SetAttrType(desc, "T", TF_INT32);
  TF_Port input = {v, 0};
  TF_AddInput(desc, input);
  TF_SetAttrInt(desc, "dim", dim);

  TF_Operation* result = TF_FinishOperation(desc, s);
  if (TF_OK != TF_GetCode(s)) { std::cout << TF_Message(s); }
  TF_DeleteStatus(s);
  return result;
}

TF_Operation* MathOps::cast(TF_Graph *graph, TF_Operation* v, TF_DataType dtype) {
  TF_Status* s = TF_NewStatus();

  TF_OperationDescription* desc = TF_NewOperation(graph, "Cast", lib::uniqueId("Cast").c_str());
  TF_SetAttrType(desc, "T", TF_INT64);
  TF_Port input = {v, 0};
  TF_AddInput(desc, input);
  TF_SetAttrInt(desc, "dtype", dtype);

  TF_Operation* result = TF_FinishOperation(desc, s);
  if (TF_OK != TF_GetCode(s)) { std::cout << TF_Message(s); }
  TF_DeleteStatus(s);
  return result;
}

TF_Operation* MathOps::log(TF_Graph *graph, TF_Operation* v) {
  TF_Status* s = TF_NewStatus();

  TF_OperationDescription* desc = TF_NewOperation(graph, "Log", lib::uniqueId("Log").c_str());
  TF_SetAttrType(desc, "T", TF_FLOAT);
  TF_Port input = {v, 0};
  TF_AddInput(desc, input);

  TF_Operation* result = TF_FinishOperation(desc, s);
  if (TF_OK != TF_GetCode(s)) { std::cout << TF_Message(s); }
  TF_DeleteStatus(s);
  return result;
}

} // namespace tensorflow
