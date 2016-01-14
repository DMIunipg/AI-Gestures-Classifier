//
//  main.cpp
//  GesturesClassifierExemple
//
//  Created by Gabriele Di Bari on 11/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <MyoClassifierManager.h>
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define MESSAGE( x ) std::cout << x << std::endl;

int main(int argc, const char * args[])
{
    //input types
    Classifier  cType        { Classifier::CLA_kNN };
    std::string pathInput    { ""    };
    std::string pathOuput    { ""    };
    std::string sArguments   { ""    };
    bool        bShowHelp    { false };
    bool        bShowVersion { false };
    //name app
    std::string name(args[0]);
    //C++ args
    std::vector< std::string > v_args;
    //alloc args
    v_args.resize(argc-1);
    //put all arguments into array
    for(size_t arg=0; arg!= v_args.size() ; ++arg) v_args[arg] = args[arg+1];
    //start to parsing
    for(size_t arg=0; arg!= v_args.size() ; ++arg)
    {
        if(v_args[arg] == "-h" || v_args[arg] == "--help")
        {
            bShowHelp = true;
        }
        else if(v_args[arg] == "-v" || v_args[arg] == "--version")
        {
            bShowVersion = true;
        }
        else if( (arg+1)!= v_args.size() )
        {
            if(v_args[arg] == "-i" || v_args[arg] == "--input")
            {
                pathInput = v_args[++arg];
            }
            else if(v_args[arg] == "-o" || v_args[arg] == "--output")
            {
                pathOuput = v_args[++arg];
            }
            else if(v_args[arg] == "-c" || v_args[arg] == "--classifier")
            {
                     if(v_args[arg+1] == "kNN" || v_args[arg+1] == "KNN")        cType = Classifier::CLA_kNN;
                else if(v_args[arg+1] == "SVM"                          )        cType = Classifier::CLA_SVM;
                else if(v_args[arg+1] == "RBF" || v_args[arg+1] == "RBFNetwork") cType = Classifier::CLA_RBFNETWORK;
                else break;
                //append all args:
                for (arg+=2; arg < v_args.size(); ++arg)
                {
                    sArguments += "\t" + v_args[arg];
                }
                //stop loop
                break;
            }
        }
    }
    //show version
    if(bShowVersion)
    {
        MESSAGE( name << ": " << MAJOR_VERSION << "." << MINOR_VERSION );
    }
    //show help
    if(bShowHelp)
    {
        MESSAGE( name << " [options] (-c | -classifier) [classifier args]" );
        MESSAGE( "Options:" );
        MESSAGE( "\t--help/-h help" );
        MESSAGE( "\t--version/-v classifier version" );
        MESSAGE( "\t--input/-i <path> dataset path" );
        MESSAGE( "\t--output/-o <path> model output path" );
    }
    //valid arguments?
    if( !pathInput.size() ||  !pathOuput.size()  )
    {
        MESSAGE("Not valid arguments");
        return -1;
    }
    //start classification
    MyoClassifierManager mcmanager(cType, false);
    //build model
    auto* model = mcmanager.buildModel(pathInput,sArguments);
    //serialize
    if(model) model->serialize(pathOuput);
    //fail to build model
    else
    {
        MESSAGE("Fail to build model");
        return -1;
    }
    //else return success
    return 0;
}
