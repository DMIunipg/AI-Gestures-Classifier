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
	if (argc <= 1)
	{
		std::cout << "Wrong arguments" << std::endl;
		return -1;
	}
	std::string this_path(argv[0]);
	std::string path(argv[1]);
	//get ext
	std::string ext = path.substr(path.rfind('.'));
	//ref to manager
	MyoClassifierManager* mcmanager{ nullptr };
	//alloc
		 if (ext == ".knn") mcmanager = new MyoClassifierManager(Classifier::CLA_kNN);
	else if (ext == ".svm") mcmanager = new MyoClassifierManager(Classifier::CLA_SVM);
	else if (ext == ".net") mcmanager = new MyoClassifierManager(Classifier::CLA_RBFNETWORK);
	//alloc?
	if (!mcmanager)
	{
		std::cout << "Wrong extention" << std::endl;
		return -1;
	}
	//models
	mcmanager->loadModel(path);
	mcmanager->setProbabilityFilter(0.5);
	mcmanager->classification([](const std::string& cname)
	{
		std::cout << "Current gesture: " << cname << "\n";
	});
    //wait
    for(;;)
    {
        char buffer[255]={0};
        scanf("exit? %s",buffer);
        if(strcmp(buffer, "yes")==0) break;
    }
    
    return 0;
}
