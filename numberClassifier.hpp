// #ifndef NUMBER_CLASSIFIER
// #define NUMBER_CLASSIFIER

#include <array>
#include <vector>
#include <iostream>
#include <tensorflow/c/c_api.h>
#include <math.h>

class NumberClassifier {
    TF_Buffer *run_options;
    TF_SessionOptions *session_options;
    TF_Graph *graph;
    TF_Status *status;
    TF_Session *session;
    std::array<TF_Output, 1> input_ops;
    std::array<TF_Output, 1> output_ops;
    std::array<TF_Tensor *, 1> input_values;
    std::array<TF_Tensor *, 1> output_values;

public:
    explicit NumberClassifier()
    {
        this->run_options = TF_NewBufferFromString("", 0);
        this->session_options = TF_NewSessionOptions();
        this->graph = TF_NewGraph();
        this->status = TF_NewStatus();
        std::array<char const *, 1> tags{ "serve" };
        
        // session
        this->session = TF_LoadSessionFromSavedModel(session_options, run_options,
                                                    "./data/", tags.data(), tags.size(),
                                                    graph, nullptr, status);
        if (TF_GetCode(status) != TF_OK) {
            std::cout << TF_Message(status) << "(session)\n";
        }

        // operations
        auto *input_op = TF_GraphOperationByName(graph, "serving_default_flatten_input");
        if (input_op == nullptr) {
            std::cout << "Failed to find graph operation\n";
        }
        auto *output_op = TF_GraphOperationByName(graph, "StatefulPartitionedCall");
        if (output_op == nullptr) {
            std::cout << "Failed to find graph operation\n";
        }
        this->input_ops = { TF_Output{ input_op, 0 } };
        this->output_ops = { TF_Output{ output_op, 0 } };
    }
    NumberClassifier(NumberClassifier& other) = delete;
    ~NumberClassifier()
    {
        // delete resources for model
        TF_DeleteBuffer(run_options);
        TF_DeleteSessionOptions(session_options);
        TF_DeleteSession(session, status);
        TF_DeleteGraph(graph);
        TF_DeleteStatus(status);
    }

    std::array<float, 10> predict(const std::array<float, 28 * 28>& input)
    {

        std::vector<std::array<float, 28 * 28>> inputs{ input };
        std::array<int64_t, 2> const dims{ static_cast<int64_t>(inputs.size()), static_cast<int64_t>(input.size()) };
        void *data = (void *) inputs.data();
        std::size_t const ndata = inputs.size() * input.size() * TF_DataTypeSize(TF_FLOAT);

        auto const deallocator = [](void *, std::size_t, void *) {}; // unused deallocator because of RAII

        auto *input_tensor = TF_NewTensor(TF_FLOAT, dims.data(), dims.size(), data, ndata, deallocator, nullptr);
        this->input_values = { input_tensor };
        this->output_values = { nullptr };

        // predict
        TF_SessionRun(session,
                    run_options,
                    input_ops.data(), input_values.data(), input_ops.size(),
                    output_ops.data(), output_values.data(), output_ops.size(),
                    nullptr, 0,
                    nullptr,
                    status);
        if (TF_GetCode(status) != TF_OK) {
            std::cout << TF_Message(status) << "(predict)\n";
        }
        
        // output
        auto *output_tensor = static_cast<std::array<float, 10> *>(TF_TensorData(output_values[0]));
        std::vector<std::array<float, 10>> outputs { output_tensor, output_tensor + inputs.size() };

        auto ret = outputs[0];
        // delete resources for input/output
        TF_DeleteTensor(input_values[0]);
        TF_DeleteTensor(output_values[0]);

        return ret;
    }

    void printVersion()
    {
        std::cout << "TensorFlow Version: " << TF_Version() << std::endl;
    }

    static std::array<float, 10> softmax(const std::array<float, 10>& output)
    {
        std::array<long double, 10> softmaxOutput;
        long double denominator = 0;
        for (uint64_t idx = 0; idx < output.size(); ++idx)
        {
            long double expValue = expl(output[idx]);
            denominator += expValue;
            softmaxOutput[idx] = expValue;
        }
        for (auto& z : softmaxOutput) z /= denominator;
        std::array<float, 10> ret;
        for (uint64_t idx = 0; idx < output.size(); ++idx)
        {
            ret[idx] = (float)softmaxOutput[idx];
        }
        return ret;
    }

    static int predictionLabel(std::array<float, 10> output)
    {
        float max = output[0];
        uint64_t maxIdx = 0;
        for (uint64_t idx = 0; idx < output.size(); ++idx)
        {
            if (max < output[idx]) { max = output[idx]; maxIdx = idx; }
        }
        return maxIdx;
    }
};


// #endif
