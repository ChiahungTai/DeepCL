//#include "OpenCLHelper.h"
//#include "ClConvolve.h"

#include <iostream>

#include "gtest/gtest.h"

#include "Timer.h"
#include "NeuralNet.h"
#include "AccuracyHelper.h"
#include "test/myasserts.h"

using namespace std;

TEST( testsimpleconvolvenet, boardsize1_planes2_filters2_tanh ) {
    Timer timer;
    float *data = new float[2];
    data[0] = 0.5;
    data[1] = -0.5;
    int *labels = new int[2];
    labels[0] = 0;
    labels[1] = 1;
    float *expectedResults = new float[4];
    expectedResults[0] = 0.5;
    expectedResults[1] = -0.5;
    expectedResults[2] = -0.5;
    expectedResults[3] = 0.5;
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(1)->instance();
    net->convolutionalMaker()->numFilters(2)->filterSize(1)->biased()->insert();
    float weights1[] = {0.382147, -1.77522};
    float biasweights1[] = {-1.00181, 0.891056};
    net->initWeights(1, weights1);
    net->initBiasWeights(1, biasweights1);
    for( int epoch = 0; epoch < 30; epoch++ ) {
        net->epochMaker()
            ->learningRate(1)
            ->batchSize(2)
            ->numExamples(2)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
//        net->printWeightsAsCode();
//        net->printBiasWeightsAsCode();
        if( epoch % 10 == 0 ) {
            cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
    //        net->print();
            float const*results = net->getResults();
            AccuracyHelper::printAccuracy( 2, 2, labels, results );
        }
    }
//    net->print();

    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    float const*results = net->getResults();
    AccuracyHelper::printAccuracy( 2, 2, labels, results );

    int numCorrect = AccuracyHelper::calcNumRight( 2, 2, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << 2 << endl;
    assertEquals( numCorrect, 2 );
    assertLessThan( 0.01, loss );

    delete net;
}

TEST( testsimpleconvolvenet, boardsize3_n4_filtersize3_tanh ) {
    Timer timer;
    float data[] = { 0.5, 0.5, 0.5,
                    -0.5, 0.5, 0.5,
                    0.5, 0.5, 0.5,
    
                   0.5, 0.5, 0.5,
                   0.5, -0.5, 0.5,
                   0.5, 0.5, 0.5,

                    -0.5, -0.5, -0.5,
                    -0.5, 0.5, -0.5,
                    -0.5, -0.5, -0.5,
    
                   -0.5, -0.5, -0.5,
                   0.5, -0.5, -0.5,
                   -0.5, -0.5, -0.5
 };

    int *labels = new int[4];
    labels[0] = 0;
    labels[1] = 1;
    labels[2] = 0;
    labels[3] = 1;
    float *expectedResults = new float[8];
    expectedResults[0] = 0.5;
    expectedResults[1] = -0.5;
    expectedResults[2] = -0.5;
    expectedResults[3] = 0.5;
    expectedResults[4] = 0.5;
    expectedResults[5] = -0.5;
    expectedResults[6] = -0.5;
    expectedResults[7] = 0.5;
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(3)->instance();
    net->convolutionalMaker()->numFilters(2)->filterSize(3)->biased()->insert();
    float weights1[] = {-0.171115, 0.28369, 0.201354, -0.496124, 0.391512, 0.120458, 0.396952, -0.1356, -0.319595, 0.251043, 0.318859, 0.220892, -0.480651, -0.51708, 0.2173, 0.365935, 0.304687, -0.712624};
    float biasWeights1[] = {0.375101, 0.00130748};
    net->initWeights(1, weights1);
    net->initBiasWeights(1, biasWeights1 );
    float const*results = 0;
    for( int epoch = 0; epoch < 15; epoch++ ) {
        net->epochMaker()
            ->learningRate(1)
            ->batchSize(4)
            ->numExamples(4)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
//        net->printWeightsAsCode();
//        net->printBiasWeightsAsCode();
        if( epoch % 5 == 0 ) {
            cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
            results = net->getResults();
            AccuracyHelper::printAccuracy( 4, 2, labels, results );
        }
    }
//    net->print();
    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    AccuracyHelper::printAccuracy( 4, 2, labels, results );
    int numCorrect = AccuracyHelper::calcNumRight( 4, 2, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << 4 << endl;
    assertEquals( numCorrect, 4 );
    assertLessThan( 0.0001, loss );

    delete net;
}

TEST( testsimpleconvolvenet, boardsize1_2planes_filtersize1_relu ) {
    Timer timer;
    float *data = new float[2];
    data[0] = 0.5;
    data[1] = -0.5;
    int *labels = new int[2];
    labels[0] = 0;
    labels[1] = 1;
    float *expectedResults = new float[4];
    expectedResults[0] = 1;
    expectedResults[1] = 0;
    expectedResults[2] = 0;
    expectedResults[3] = 1;
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(1)->instance();
    net->convolutionalMaker()->numFilters(2)->filterSize(1)->biased()->relu()->insert();
    float weights1[] = {-0.380177, -1.5738};
    float biasWeights1[] = {0.5, 0.0606055};
    net->initWeights( 1, weights1, biasWeights1 );
    for( int epoch = 0; epoch < 15; epoch++ ) {
        net->epochMaker()
            ->learningRate(1)
            ->batchSize(2)
            ->numExamples(2)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
        if( epoch % 5 == 0 ) {
            cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
    //        net->print();
    //        net->printWeightsAsCode();
    //        net->printBiasWeightsAsCode();
            float const*results = net->getResults();
            AccuracyHelper::printAccuracy( 2, 2, labels, results );
        }
    }
//    net->print();
    float const*results = net->getResults();
    AccuracyHelper::printAccuracy( 2, 2, labels, results );

    int numCorrect = AccuracyHelper::calcNumRight( 2, 2, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << 2 << endl;
    assertEquals( numCorrect, 2 );

    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    assertLessThan( 0.001, loss );

    delete net;
}

TEST( testsimpleconvolvenet, boardsize3_n4_filtersize3_relu ) {
    Timer timer;
    float data[] = { 0.5, 0.5, 0.5,
                    -0.5, 0.5, 0.5,
                    0.5, 0.5, 0.5,
    
                   0.5, 0.5, 0.5,
                   0.5, -0.5, 0.5,
                   0.5, 0.5, 0.5,

                    -0.5, -0.5, -0.5,
                    -0.5, 0.5, -0.5,
                    -0.5, -0.5, -0.5,
    
                   -0.5, -0.5, -0.5,
                   0.5, -0.5, -0.5,
                   -0.5, -0.5, -0.5
 };

    int *labels = new int[4];
    labels[0] = 0;
    labels[1] = 1;
    labels[2] = 0;
    labels[3] = 1;
    float *expectedResults = new float[8];
    expectedResults[0] = 1;
    expectedResults[1] = 0;
    expectedResults[2] = 0;
    expectedResults[3] = 1;
    expectedResults[4] = 1;
    expectedResults[5] = 0;
    expectedResults[6] = 0;
    expectedResults[7] = 1;
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(3)->instance();
    net->convolutionalMaker()->numFilters(2)->filterSize(3)->biased()->relu()->insert();
    float const*results = 0;
    float weights1[] = {0.0113327, 0.280063, -0.0584702, -0.503431, -0.37286, -0.457257, 0.29226, -0.360089, -0.273977, 0.530185, -0.460167, 0.489126, 0.141883, 0.179525, -0.18084, 0.412117, 0.0866731, -0.247958};
    float biasWeights1[] = {0.0418723, 0.158733};
    net->initWeights( 1, weights1, biasWeights1 );
    for( int epoch = 0; epoch < 20; epoch++ ) {
        net->epochMaker()
            ->learningRate(1)
            ->batchSize(4)
            ->numExamples(4)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
        if( epoch % 5 == 0 ) {
            results = net->getResults();
    //        net->printWeightsAsCode();
    //        net->printBiasWeightsAsCode();
            cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
            AccuracyHelper::printAccuracy( 4, 2, labels, results );
        }
    }
//    net->print();
    cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
    AccuracyHelper::printAccuracy( 4, 2, labels, results );
    int numCorrect = AccuracyHelper::calcNumRight( 4, 2, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << 4 << endl;
    assertEquals( numCorrect, 4 );

    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    assertLessThan( 0.000001, loss );

    delete net;
}

TEST( testsimpleconvolvenet, boardsize3_n4_filtersize3_linear ) {
    Timer timer;
    float data[] = { 0.5, 0.5, 0.5,
                    -0.5, 0.5, 0.5,
                    0.5, 0.5, 0.5,
    
                   0.5, 0.5, 0.5,
                   0.5, -0.5, 0.5,
                   0.5, 0.5, 0.5,

                    -0.5, -0.5, -0.5,
                    -0.5, 0.5, -0.5,
                    -0.5, -0.5, -0.5,
    
                   -0.5, -0.5, -0.5,
                   0.5, -0.5, -0.5,
                   -0.5, -0.5, -0.5
 };

    int *labels = new int[4];
    labels[0] = 0;
    labels[1] = 1;
    labels[2] = 0;
    labels[3] = 1;
    float *expectedResults = new float[8];
    expectedResults[0] = 1;
    expectedResults[1] = 0;
    expectedResults[2] = 0;
    expectedResults[3] = 1;
    expectedResults[4] = 1;
    expectedResults[5] = 0;
    expectedResults[6] = 0;
    expectedResults[7] = 1;
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(3)->instance();
    net->convolutionalMaker()->numFilters(2)->filterSize(3)->biased()->linear()->insert();
    float const*results = 0;
    float weights1[] = {0.715867, -0.428623, -0.281465, -0.736675, -0.224507, 0.335028, -0.384762, -0.213304, 0.679177, -0.170055, 0.335075, -0.572057, -0.175718, -0.410962, -0.175277, 0.536131, -0.0568329, -0.00297278};
    float biasWeights1[] = {0.5, 0.5};
    net->initWeights( 1, weights1, biasWeights1 );
    for( int epoch = 0; epoch < 20; epoch++ ) {
        net->epochMaker()
            ->learningRate(1)
            ->batchSize(4)
            ->numExamples(4)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
//        net->printWeightsAsCode();
//        net->printBiasWeightsAsCode();
        if( epoch % 5 == 0 ) {
            cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
            results = net->getResults();
            AccuracyHelper::printAccuracy( 4, 2, labels, results );
        }
    }
//    net->print();
    cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
    AccuracyHelper::printAccuracy( 4, 2, labels, results );
    int numCorrect = AccuracyHelper::calcNumRight( 4, 2, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << 4 << endl;
    assertEquals( numCorrect, 4 );

    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    assertLessThan( 0.00001, loss );

    delete net;
}

TEST( testsimpleconvolvenet, boardsize1_n2_2layers_unbiased ) {
    Timer timer;
    float *data = new float[2];
    data[0] = 0.5;
    data[1] = -0.5;
    int *labels = new int[2];
    labels[0] = 0;
    labels[1] = 1;
    float *expectedResults = new float[4];
    expectedResults[0] = 0.5;
    expectedResults[1] = -0.5;
    expectedResults[2] = -0.5;
    expectedResults[3] = 0.5;
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(1)->instance();
    net->convolutionalMaker()->numFilters(2)->filterSize(1)->biased(0)->insert();
    net->convolutionalMaker()->numFilters(2)->filterSize(1)->biased(0)->insert();
    for( int epoch = 0; epoch < 30; epoch++ ) {
        net->epochMaker()
            ->learningRate(1)
            ->batchSize(2)
            ->numExamples(2)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
//        cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
//        net->print();
        float const*results = net->getResults();
//        AccuracyHelper::printAccuracy( 2, 2, labels, results );
    }
//    net->print();
    cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
    float const*results = net->getResults();
    AccuracyHelper::printAccuracy( 2, 2, labels, results );

    int numCorrect = AccuracyHelper::calcNumRight( 2, 2, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << 2 << endl;
    assertEquals( numCorrect, 2 );

    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    assertLessThan( 0.0001, loss );

    delete net;
}

TEST( testsimpleconvolvenet, boardsize1_n2_2layers_biased ) {
    Timer timer;
    float *data = new float[2];
    data[0] = 0.5;
    data[1] = -0.5;
    int *labels = new int[2];
    labels[0] = 0;
    labels[1] = 1;
    float *expectedResults = new float[4];
    expectedResults[0] = 0.5;
    expectedResults[1] = -0.5;
    expectedResults[2] = -0.5;
    expectedResults[3] = 0.5;
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(1)->instance();
    net->convolutionalMaker()->numFilters(2)->filterSize(1)->biased()->insert();
    net->convolutionalMaker()->numFilters(2)->filterSize(1)->biased()->insert();
    for( int epoch = 0; epoch < 30; epoch++ ) {
        net->epochMaker()
            ->learningRate(1)
            ->batchSize(2)
            ->numExamples(2)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
//        cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
//        net->print();
        float const*results = net->getResults();
//        AccuracyHelper::printAccuracy( 2, 2, labels, results );
    }
//    net->print();
    cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
    float const*results = net->getResults();
    AccuracyHelper::printAccuracy( 2, 2, labels, results );

    int numCorrect = AccuracyHelper::calcNumRight( 2, 2, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << 2 << endl;
    assertEquals( numCorrect, 2 );

    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    assertLessThan( 0.0001, loss );

    delete net;
}

TEST( testsimpleconvolvenet, boardsize_5_3_2layers_filtersize_2_2_biased_n3 ) {
    Timer timer;
    int boardSize = 5;
    int N = 3;
    int numInPlanes = 1;
    int numOutPlanes = 3;
    float data[] = {
                    1,0,1,0,1,
                    0,1,0,1,0,
                    1,0,1,0,1,
                    0,1,0,1,0,    
                    1,0,1,0,1,

                    1,0,1,0,1,
                    1,0,1,0,1,
                    1,0,1,0,1,
                    1,0,1,0,1,
                    1,0,1,0,1,

                    1,1,1,1,1,
                    0,0,0,0,0,
                    1,1,1,1,1,
                    0,0,0,0,0,
                    1,1,1,1,1,
};
    int inputSize = boardSize * boardSize * numInPlanes * N;
    for( int i = 0; i < inputSize; i++ ) {
        data[i] -= 0.5f;
    }
    int labels[] = { 0, 1, 2 };
    int resultsSize = numOutPlanes * N;
    float *expectedResults = new float[resultsSize];
    for( int n = 0; n < N; n++ ) {
        for( int plane = 0; plane < numOutPlanes; plane++ ) {
            expectedResults[ n * numOutPlanes + plane] = -0.5;
        }
        expectedResults[ n * numOutPlanes + labels[n]] = +0.5;
    }
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(5)->instance();
    net->convolutionalMaker()->numFilters(3)->filterSize(2)->biased()->insert();
    net->convolutionalMaker()->numFilters(3)->filterSize(4)->biased()->insert();
    net->print();
    for( int epoch = 0; epoch < 300; epoch++ ) {
        net->epochMaker()
            ->learningRate(1)
            ->batchSize(N)
            ->numExamples(N)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
        cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
//        net->print();
        float const*results = net->getResults();
//        AccuracyHelper::printAccuracy( 2, 2, labels, results );
    }
    net->print();
    cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
    float const*results = net->getResults();
    AccuracyHelper::printAccuracy( N, numOutPlanes, labels, results );

    int numCorrect = AccuracyHelper::calcNumRight( N, numOutPlanes, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << N << endl;
    assertEquals( numCorrect, N );

    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    assertLessThan( 0.01, loss );

    delete net;
}

TEST( testsimpleconvolvenet, boardsize_5_3_2layers_filtersize_2_2_biased_n6 ) {
    Timer timer;
    int boardSize = 5;
    int N = 6;
    int numInPlanes = 1;
    int numOutPlanes = 3;
    float data[] = {
                    1,0,1,0,1,
                    0,1,0,1,0,
                    1,0,1,0,1,
                    0,1,0,1,0,    
                    1,0,1,0,1,

                    1,0,1,0,1,
                    1,0,1,0,1,
                    1,0,1,0,1,
                    1,0,1,0,1,
                    1,0,1,0,1,

                    1,1,1,1,1,
                    0,0,0,0,0,
                    1,1,1,1,1,
                    0,0,0,0,0,
                    1,1,1,1,1,

                    0,1,0,1,0,
                    1,0,1,0,1,
                    0,1,0,1,0,    
                    1,0,1,0,1,
                    0,1,0,1,0,    

                    0,1,0,1,0,
                    0,1,0,1,0,
                    0,1,0,1,0,
                    0,1,0,1,0,
                    0,1,0,1,0,

                    0,0,0,0,0,
                    1,1,1,1,1,
                    0,0,0,0,0,
                    1,1,1,1,1,
                    0,0,0,0,0,
};
    int inputSize = boardSize * boardSize * numInPlanes * N;
    for( int i = 0; i < inputSize; i++ ) {
        data[i] -= 0.5f;
    }
    int labels[] = { 0, 1, 2, 0, 1, 2 };
    int resultsSize = numOutPlanes * N;
    float *expectedResults = new float[resultsSize];
    for( int n = 0; n < N; n++ ) {
        for( int plane = 0; plane < numOutPlanes; plane++ ) {
            expectedResults[ n * numOutPlanes + plane] = -0.5;
        }
        expectedResults[ n * numOutPlanes + labels[n]] = +0.5;
    }
    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(5)->instance();
    net->convolutionalMaker()->numFilters(3)->filterSize(2)->biased()->insert();
    net->convolutionalMaker()->numFilters(3)->filterSize(4)->biased()->insert();
//    net->print();
float weights1[] = {-0.69664, 0.58017, 0.140447, -0.205859, 0.0198638, 0.0110593, -0.388923, -0.844424, -0.472903, 0.453888, -0.616155, -0.454998};
float weights2[] = {0.207138, -0.106497, -0.1228, -0.162173, 0.1822, -0.100027, 0.0447708, 0.165723, -0.0147989, 0.109204, -0.0334504, 0.00452646, 0.198443, -0.23725, 0.105671, 0.192242, -0.0268933, 0.150674, 0.160054, -0.116846, 0.222009, 
0.226935, 0.113873, -0.153742, 0.0273874, -0.216493, 0.177896, 0.155068, -0.0809009, 0.0305763, 0.198926, -0.115796, -0.179839, -0.133567, -0.0386595, -0.166771, -0.11833, -0.219205, -0.0115777, 0.122457, 0.0984342, 
0.0616336, 0.130647, 0.192949, 0.143467, -0.130633, -0.221122, -0.154317, 0.11901, 0.00502961, 0.213079, -0.0373076, -0.0461127, -0.156646, -0.148074, -0.105763, -0.140191, 0.136911, -0.217382, 0.17574, -0.0312263, 
0.0931478, 0.0789604, -0.00794073, -0.218235, 0.0418423, 0.234828, 0.225359, -0.191966, 0.241517, 0.182442, -0.216337, -0.228462, -0.140195, 0.0493267, 0.0383108, -0.0124946, -0.093023, 0.0322872, 0.0855678, -0.0466207, 
-0.025329, -0.198314, -0.0189797, 0.147109, -0.200046, 0.20127, 0.169828, -0.173335, -0.100567, -0.195165, -0.0657755, -0.224493, -0.208405, 0.154131, 0.12547, -0.161635, -0.248707, 0.13305, -0.00289013, 0.228017, 
0.0528438, 0.0157539, 0.161637, -0.199882, 0.171727, 0.171146, -0.237469, -0.226088, 0.2026, -0.131614, 0.0631847, -0.0949208, -0.137853, -0.177839, -0.237589, -0.229862, 0.202094, 0.0531539, -0.0467284, 0.125544, 
-0.0750956, 0.225228, 0.255915, 0.076901, -0.0596187, 0.16937, -0.104811, -0.0815879, -0.196806, 0.0526821, 0.136622, -0.12163, 0.170657, -0.0956968, -0.00985565, 0.0455411, 0.0242914, 0.107953, -0.0594324, 0.124928, 
0.0875922, -0.100952, 0.155045};
float biasWeights1[] = {0.0998941, -0.365008, 0.188937};
float biasWeights2[] = {0.232961, 0.141537, 0.159074};
    net->initWeights(1, weights1, biasWeights1 );
    net->initWeights(2, weights2, biasWeights2 );
    for( int epoch = 0; epoch < 500; epoch++ ) {
        net->epochMaker()
            ->learningRate(0.1)
            ->batchSize(N)
            ->numExamples(N)
            ->inputData(data)
            ->expectedOutputs(expectedResults)
            ->run();
        if( epoch % 100 == 0 ) {
            cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
//        net->print();
//           net->printWeightsAsCode();
//            net->printBiasWeightsAsCode();
        }
//        float const*results = net->getResults();
//        AccuracyHelper::printAccuracy( 2, 2, labels, results );
    }
//    net->print();
    cout << "loss, E, " << net->calcLoss(expectedResults) << endl;
    float const*results = net->getResults();
    AccuracyHelper::printAccuracy( N, numOutPlanes, labels, results );

    int numCorrect = AccuracyHelper::calcNumRight( N, numOutPlanes, labels, net->getResults() );
    cout << "accuracy: " << numCorrect << "/" << N << endl;
    assertEquals( numCorrect, N );

    float loss = net->calcLoss(expectedResults);
    cout << "loss, E, " << loss << endl;
    assertLessThan( 0.00001, loss );

    delete net;
}

