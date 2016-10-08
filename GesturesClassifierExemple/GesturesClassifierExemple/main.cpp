//
//  main.cpp
//  GesturesClassifierExemple
//
//  Created by Gabriele Di Bari on 11/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <iostream>
#include <string>
#include <MyoClassifierManager.h>
#include <MyoModelInterface.h>

int main(int argc, const char * argv[])
{
#if 0
	//"fft true type MANHATTAN_DISTANCE weight DEMOCRATIC"
    MyoClassifierManager mcmanager(Classifier::CLA_kNN);
	//mcmanager.buildModel("C:\\Users\\gabri\\Desktop\\campioni_braccio\\ds2\\gab02.ds", "fft true type MANHATTAN_DISTANCE");
	mcmanager.buildModel("C:\\Users\\gabri\\Desktop\\campioni_braccio\\ds2\\gab02.ds", "fft false");
	//mcmanager.buildModel("C:\\Users\\gabri\\Desktop\\campioni_braccio\\dataset_2\\gab01.ds", "fft true type MANHATTAN_DISTANCE");
	//mcmanager.buildModel("C:\\Users\\gabri\\Desktop\\campioni_braccio\\dataset_2\\gab01.ds", "fft false rtoc 1");
	//mcmanager.getModel()->serialize("C:\\Users\\gabri\\Desktop\\campioni_braccio\\dataset_2\\gab01@3.knn");
	//mcmanager.loadModel("C:\\Users\\gabri\\Desktop\\campioni_braccio\\tave\\tave3.knn");
    mcmanager.setProbabilityFilter(0.0);
    mcmanager.classification([](const std::string& cname)
                             {
                                 std::cout << "Current gesture: " << cname << "\n";
                             });
#else 
	MyoClassifierManager mcmanager(Classifier::CLA_SVM);
#if 0
	mcmanager.buildModel("C:\\Users\\gabri\\Desktop\\campioni_braccio\\ds3\\gab03_norm.ds",
					     "type C_SVC " 
						 "kernel LINEAR "
						 "cache 2000 "
						 "coef0 0.01 "
						 "degree 3 "
						 "eps 0.01 "
						 "gamma 0.1 "
						 "nu 0.1 "
						 "p 0.1 "
						 "const 2 "
						 "probability true "
						 "shrinking true "
						 "fft true"
	);
#else
	mcmanager.loadModel("C:\\Users\\gabri\\Desktop\\campioni_braccio\\ds3\\gab03_norm.svm");

#endif

	mcmanager.setProbabilityFilter(0.5);
	mcmanager.classification([](const std::string& cname)
	{
		std::cout << "Current gesture: " << cname << "\n";
	});
#endif 
    //wait
    for(;;)
    {
        char buffer[255]={0};
        scanf("exit? %s",buffer);
        if(strcmp(buffer, "yes")==0) break;
    }
    
    return 0;
}
