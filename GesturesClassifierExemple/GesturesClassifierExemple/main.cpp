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

int main(int argc, const char * argv[])
{ 
    MyoClassifierManager mcmanager(Classifier::CLA_SVM);
    mcmanager.buildModel("datas/MARY/data.ds");
    mcmanager.setProbabilityFilter(0.2);
    mcmanager.classification([](const std::string& cname)
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
